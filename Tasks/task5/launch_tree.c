#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include "task3.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SIZE 1

int exit_stat = 0;

void launch_tree(struct cmd_inf *tree);

void cd(char *path){
    char r_path[1024];
    if (path == NULL) {
        chdir(getenv("HOME"));
    }
    else {
        if (realpath(path, r_path) != NULL) {
            chdir(r_path);
        }
        else
            printf("Error: incorrect path\n");
    }
}

int chck_status(int status)
{
    if (WIFEXITED(status))
        if (WEXITSTATUS(status) == 0)
            return 0;
    return 1;
}

void simple_command(char *file_name, 
                    char **args, 
                    char *in_file, 
                    char *out_file, 
                    int outfile_index, int conv_index, int *status)
{
    if ((strcmp(file_name, "cd") == 0)) {
        if (conv_index == 0) {
            cd(args[1]);
        }
    }
    else if ((strcmp(file_name, "exit") == 0)) {
        if (conv_index == 0)
            exit_stat = 1;
    }
    else if (fork() == 0) {
        if (in_file != NULL) {
            int inf = open(in_file, O_RDONLY, 0666);
            dup2(inf, 0);
            close(inf);
        }
        if (out_file != NULL) {
            int outf;
            if (outfile_index == 1) {
                outf = open(out_file, O_CREAT|O_WRONLY, 0666);
                lseek(outf, 0, SEEK_END);
            }
            else {
                outf = open(out_file, O_CREAT|O_WRONLY|O_TRUNC, 0666);
                lseek(outf, 0, SEEK_SET);
            }
            dup2(outf, 1);
            close(outf);
        }
        if ((strcmp(file_name, "pwd") == 0)) {
            if (conv_index == 0) {
                char path[1024];
                getcwd(path, 1024);
                printf("%s\n", path);
            }
        }
        else
            if (execvp(file_name, args) == -1)
                printf("Error: incorrect command\n");
        exit(1);
    }
    else {
        wait(status);
    }
}

void command(struct cmd_inf *tree_part, int conv_index, int *status)
{
    if (tree_part->argv != NULL) {
        simple_command(tree_part->argv[0], 
                        tree_part->argv, 
                        tree_part->infile, 
                        tree_part->outfile, 
                        tree_part->outfile_index, conv_index, status);
    }
    else if (fork() == 0) {
        if (tree_part->infile != NULL) {
            int inf = open(tree_part->infile, O_RDONLY, 0666);
            dup2(inf, 0);
            close(inf);
        }
        if (tree_part->outfile != NULL) {
            int outf;
            if (tree_part->outfile_index == 1) {
                outf = open(tree_part->outfile, O_CREAT|O_WRONLY, 0666);
                lseek(outf, 0, SEEK_END);
            }
            else {
                outf = open(tree_part->outfile, O_CREAT|O_WRONLY|O_TRUNC, 0666);
                lseek(outf, 0, SEEK_SET);
            }
            dup2(outf, 1);
            close(outf);
        }
        launch_tree(tree_part->psubcmd);
        exit(0);
    }
    else {
        wait(status);
    }
}

void conveyer(struct cmd_inf *tree_part, int *status)
{
    if (tree_part->pipe == NULL) {
        if (tree_part->backgrnd == 0) {
            command(tree_part, 0, status);
        }
        else {
            if (fork() == 0) {
                command(tree_part, 0, status);
                exit(0);
            }
        }
    }
    else {
        if (tree_part->backgrnd == 0) {
            int fd[2], in, out, next_in, i, pipe_number = 0;
            pipe(fd);
            out=fd[1];
            next_in=fd[0];
            struct cmd_inf *tree_pointer = tree_part;
            pipe_number++;
            if (fork()==0) {
                close(next_in);
                dup2(out,1);
                close(out);
                command(tree_pointer, 1, status);
                exit(0);
            }
            tree_pointer = tree_pointer->pipe;
            in=next_in;
            while (tree_pointer->pipe != NULL) {
                close(out);
                pipe(fd);
                out=fd[1];
                next_in=fd[0];
                pipe_number++;
                if(fork() == 0){
                    close(next_in);
                    dup2(in,0);
                    close(in);
                    dup2(out,1);
                    close(out);
                    command(tree_pointer, 1, status);
                    exit(0);
                }
                tree_pointer = tree_pointer->pipe;
                close(in);
                in=next_in;
            }
            close(out);
            pipe_number++;
            if (fork()==0) {
                dup2(in,0);
                close(in);
                command(tree_pointer, 1, status);
                exit(0);
            }
            close(in);
            for (i=1; i <= pipe_number; i++)
                wait(status);
            if (tree_pointer->next != NULL) {
                launch_tree(tree_pointer->next);
            }
        }
        else {
            struct cmd_inf *tree_pointer0 = tree_part;
            while (tree_pointer0->pipe != NULL)
                tree_pointer0 = tree_pointer0->pipe;
            if (fork() == 0) {
                int fd[2], in, out, next_in, i, pipe_number = 0;
                pipe(fd);
                out=fd[1];
                next_in=fd[0];
                struct cmd_inf *tree_pointer = tree_part;
                pipe_number++;
                if (fork()==0) {
                    close(next_in);
                    dup2(out,1);
                    close(out);
                    command(tree_pointer, 1, status);
                    exit(0);
                }
                tree_pointer = tree_pointer->pipe;
                in=next_in;
                while (tree_pointer->pipe != NULL) {
                    close(out);
                    pipe(fd);
                    out=fd[1];
                    next_in=fd[0];
                    pipe_number++;
                    if(fork() == 0){
                        close(next_in);
                        dup2(in,0);
                        close(in);
                        dup2(out,1);
                        close(out);
                        command(tree_pointer, 1, status);
                        exit(0);
                    }
                    tree_pointer = tree_pointer->pipe;
                    close(in);
                    in=next_in;
                }
                close(out);
                pipe_number++;
                if (fork()==0) {
                    dup2(in,0);
                    close(in);
                    command(tree_pointer, 1, status);
                    exit(0);
                }
                close(in);
                for (i=1; i <= pipe_number; i++)
                    wait(status);
                exit(0);
            }
            if (tree_pointer0->next != NULL) {
                launch_tree(tree_pointer0->next);
            }
        }
    }
}

void launch_tree(struct cmd_inf *tree) {
    int status = 0;
    if (tree->next_exit_mode == 0) {
        conveyer(tree, &status);
    }
    else if (tree->next_exit_mode == 1) {
        if (chck_status(status) != 0) {
            conveyer(tree, &status);
        }
    }
    else if (chck_status(status) == 0) {
        conveyer(tree, &status);
    }
    if (tree->next != NULL) {
        launch_tree(tree->next);
    }
    if (tree->backgrnd == 1) {
        wait(&status);
    }
}

int exit_status(void)
{
    return exit_stat;
}
