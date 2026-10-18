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
#define SIZE 1


int curlist_of_args; /* индекс текущего символа в буфере*/
int sizelist_of_args;
int pipe_deep = 1;

char **lst_of_args;

char ** lst_copy, **argp;
int i, size_l, outfile_ind = 0;
int index_of_shell = 0;

struct cmd_inf *tree;
struct cmd_inf *tree_pipe_pointer;
struct cmd_inf **tree_pointer;
int cur_tree_p_ind = 0;

void clearlist_of_args(){ // очистить список слов (освободить все указатели на слова и указатель на список)
    int i;
    sizelist_of_args=0;
    curlist_of_args=0;
    if (lst_of_args==NULL) return;
    for (i=0; lst_of_args[i]!=NULL; i++)
        free(lst_of_args[i]);
    free(lst_of_args);
    lst_of_args=NULL;
}

void null_list_of_args(){
    sizelist_of_args=0;
    curlist_of_args=0;
    lst_of_args=NULL;
}

void printlist_of_args(){
    int i;
    if (lst_of_args==NULL) return;
    for (i=0; i<sizelist_of_args-1; i++)
        printf("%s\n",lst_of_args[i]);
}

void copy_list(int flag)
{
    int i;
    if (tree_pointer[cur_tree_p_ind]->argv == NULL) {
        tree_pointer[cur_tree_p_ind]->argv = realloc(tree_pointer[cur_tree_p_ind]->argv, (sizelist_of_args+flag)*sizeof(*lst_of_args));
        tree_pointer[cur_tree_p_ind]->argv[0] = NULL;
    }
    else
        tree_pointer[cur_tree_p_ind]->argv = realloc(tree_pointer[cur_tree_p_ind]->argv, (sizelist_of_args+flag)*sizeof(*lst_of_args));
    tree_pointer[cur_tree_p_ind]->argv[sizelist_of_args-1+flag] = NULL;
    for (i = 0; i < sizelist_of_args-1+flag; i++) {
        if (tree_pointer[cur_tree_p_ind]->argv[i] != NULL)
            free(tree_pointer[cur_tree_p_ind]->argv[i]);
        tree_pointer[cur_tree_p_ind]->argv[i] = NULL;
        tree_pointer[cur_tree_p_ind]->argv[i] = realloc(tree_pointer[cur_tree_p_ind]->argv[i], strlen(lst_of_args[i]) + 1);
        strcpy(tree_pointer[cur_tree_p_ind]->argv[i], lst_of_args[i]);
    }
}

void termlist_of_args(){ // завершение списка и точное выравнивание памяти
    if (lst_of_args==NULL) return;
    if (curlist_of_args>sizelist_of_args-1)
        lst_of_args=realloc(lst_of_args,(sizelist_of_args+1)*sizeof(*lst_of_args));
    lst_of_args[curlist_of_args]=NULL;
    /*выравниваем используемую под список память точно по размеру списка*/
    lst_of_args=realloc(lst_of_args,(sizelist_of_args=curlist_of_args+1)*sizeof(*lst_of_args));
    copy_list(0);
}

void addword_of_args(char *str){ // завершение текущего слова и добавление слова в список (с преобразованием памяти)
    int euid, i, del;
    char *string = NULL, *str2, *shell_path;
    if (curlist_of_args>sizelist_of_args-1)
        lst_of_args=realloc(lst_of_args, (sizelist_of_args+=SIZE)*sizeof(*lst_of_args)); /* увеличиваем массив под список при необходимости */
    if (strcmp(str, "$HOME") == 0) {
        string = realloc(string, strlen(getenv("HOME")) + 1);
        strcpy(string, getenv("HOME"));
        lst_of_args[curlist_of_args++] = string;
    }
    else if (strcmp(str, "$SHELL") == 0) {
        shell_path = argp[index_of_shell];
        string = realloc(string, strlen(shell_path) - 6 + 1);
        for (i = 6; shell_path[i] != '\0'; i++)
            string[i - 6] = shell_path[i];
        string[i - 6] = shell_path[i];
        lst_of_args[curlist_of_args++] = string;
    }
    else if (strcmp(str, "$USER") == 0) {
        str2 = getpwuid(getuid())->pw_name;
        string = realloc(string, strlen(str2) + 1);
        strcpy(string, str2);
        lst_of_args[curlist_of_args++] = string;
    }
    else if (strcmp(str, "$EUID") == 0) {
        euid = geteuid();
        del = 1000000000;
        while (euid / del == 0 && del > 1)
            del /= 10;
        for (i = 0; del > 1; i++) {
            string = realloc(string, i + 2);
            string[i] = '0' + euid / del;
            euid %= del;
            del /= 10;
        }
        string[i] = '\0';
        lst_of_args[curlist_of_args++] = string;
    }
    else {
        string = realloc(string, strlen(str) + 1);
        strcpy(string, str);
        lst_of_args[curlist_of_args++] = string;
    }
}


void tree_writer(char *name, int i, int outfile_ind)
{
    char *string=NULL, *shell_path;
    int del, j, euid;
    if (strcmp(name, "argv") == 0) {
        copy_list(1);
    }
    else if (strcmp(name, "infile") == 0) {
        if (strcmp(lst_copy[i], "$HOME") == 0) {
            string = realloc(string, strlen(getenv("HOME")) + 1);
            strcpy(string, getenv("HOME"));
            tree_pointer[cur_tree_p_ind]->infile = string;
        }
        else if (strcmp(lst_copy[i], "$SHELL") == 0) {
            shell_path = argp[index_of_shell];
            string = realloc(string, strlen(shell_path) - 6 + 1);
            for (j = 6; shell_path[j] != '\0'; j++)
                string[j - 6] = shell_path[j];
            string[j - 6] = shell_path[j];
            tree_pointer[cur_tree_p_ind]->infile = string;
        }
        else if (strcmp(lst_copy[i], "$USER") == 0) {
            string = realloc(string, strlen(getpwuid(getuid())->pw_name) + 1);
            strcpy(string, getpwuid(getuid())->pw_name);
            tree_pointer[cur_tree_p_ind]->infile = string;
        }
        else if (strcmp(lst_copy[i], "$EUID") == 0) {
            euid = geteuid();
            del = 1000000000;
            while (euid / del == 0 && del > 1)
                del /= 10;
            for (j = 0; del > 1; j++) {
                string = realloc(string, j + 2);
                string[j] = '0' + euid / del;
                euid %= del;
                del /= 10;
            }
            string[j] = '\0';
            tree_pointer[cur_tree_p_ind]->infile = string;
        }
        else {
            tree_pointer[cur_tree_p_ind]->infile = realloc(tree_pointer[cur_tree_p_ind]->infile, strlen(lst_copy[i])+1);
            strcpy(tree_pointer[cur_tree_p_ind]->infile, lst_copy[i]);
        }
    }
    else if (strcmp(name, "outfile") == 0) {
        if (strcmp(lst_copy[i], "$HOME") == 0) {
            string = realloc(string, strlen(getenv("HOME")) + 1);
            strcpy(string, getenv("HOME"));
            tree_pointer[cur_tree_p_ind]->outfile = string;
        }
        else if (strcmp(lst_copy[i], "$SHELL") == 0) {
            shell_path = argp[index_of_shell];
            string = realloc(string, strlen(shell_path) - 6 + 1);
            for (j = 6; shell_path[j] != '\0'; j++)
                string[j - 6] = shell_path[j];
            string[j - 6] = shell_path[j];
            tree_pointer[cur_tree_p_ind]->outfile = string;
        }
        else if (strcmp(lst_copy[i], "$USER") == 0) {
            string = realloc(string, strlen(getpwuid(getuid())->pw_name) + 1);
            strcpy(string, getpwuid(getuid())->pw_name);
            tree_pointer[cur_tree_p_ind]->outfile = string;
        }
        else if (strcmp(lst_copy[i], "$EUID") == 0) {
            euid = geteuid();
            del = 1000000000;
            while (euid / del == 0 && del > 1)
                del /= 10;
            for (j = 0; del > 1; j++) {
                string = realloc(string, j + 2);
                string[j] = '0' + euid / del;
                euid %= del;
                del /= 10;
            }
            string[j] = '\0';
            tree_pointer[cur_tree_p_ind]->outfile = string;
        }
        else {
            tree_pointer[cur_tree_p_ind]->outfile = realloc(tree_pointer[cur_tree_p_ind]->outfile, strlen(lst_copy[i])+1);
            strcpy(tree_pointer[cur_tree_p_ind]->outfile, lst_copy[i]);
        }
        tree_pointer[cur_tree_p_ind]->outfile_index = outfile_ind;
    }
}

void tree_free(struct cmd_inf *pointer)
{
    int i;
    if (pointer != NULL) {
        tree_free(pointer->next);
        tree_free(pointer->pipe);
        tree_free(pointer->psubcmd);
        if (pointer->argv!=NULL) {
            for (i=0; pointer->argv[i] != NULL; i++)
                free(pointer->argv[i]);
            free(pointer->argv);
        }
        if (pointer->infile!=NULL) {
            free(pointer->infile);
        }
        if (pointer->outfile!=NULL) {
            free(pointer->outfile);
        }
        free(pointer);
    }
}

void print_tab(int level) {
    int i;
    for(i = 1; i <= level; i++)
        printf("    ");
}

void print_tree(struct cmd_inf *pointer, int level)
{
        print_tab(level);
        if (pointer->argv!=NULL) {
            printf("argv:\n");
            for (i=0; pointer->argv[i] != NULL; i++) {
                print_tab(level);
                printf("    %s\n", pointer->argv[i]);
            }
        }
        else {
            printf("argv = NULL\n");
        }
        print_tab(level);
        if (pointer->infile!=NULL)
            printf("infile = %s\n", pointer->infile);
        else
            printf("infile = NULL\n");
        print_tab(level);
        if (pointer->outfile!=NULL)
            printf("outfile = %s\n", pointer->outfile);
        else
            printf("outfile = NULL\n");
        print_tab(level);
        printf("outfile index = %d\n", pointer->outfile_index);
        print_tab(level);
        printf("backgrnd = %d\n", pointer->backgrnd);
        print_tab(level);
        printf("next exit mode = %d\n", pointer->next_exit_mode);
        print_tab(level);
        if (pointer->psubcmd!=NULL) {
            printf("psubcmd:\n");
            print_tree(pointer->psubcmd, level+1);
        }
        else
            printf("psubcmd = NULL\n");
        print_tab(level);
        if (pointer->pipe!=NULL) {
            printf("pipe:\n");
            print_tree(pointer->pipe, level+1);
        }
        else
            printf("pipe = NULL\n");
        print_tab(level);
        if (pointer->next!=NULL) {
            printf("next:\n");
            print_tree(pointer->next, level+1);
        }
        else
            printf("next = NULL\n");
}

int max_pipe_deep(struct cmd_inf *pointer)
{
    int max_pipe_d;
    if (pointer->pipe == NULL)
        return 1;
    else {
        max_pipe_d = 1 + max_pipe_deep(pointer->pipe);
        if (max_pipe_d > pipe_deep) {
            pipe_deep = max_pipe_d;
        }
        return max_pipe_d;
    }
}

void search_deep(struct cmd_inf *pointer)
{
    max_pipe_deep(pointer);
    if (pointer->next != NULL)
        search_deep(pointer->next);
    if (pointer->pipe != NULL)
        search_deep(pointer->pipe);
    if (pointer->psubcmd != NULL)
        search_deep(pointer->psubcmd);
}

int main(int argc, char **argv, char ** argp0) {
    argp = argp0;
    while (argp[index_of_shell][0] != 'S' || argp[index_of_shell][1] != 'H' || argp[index_of_shell][2] != 'E' || argp[index_of_shell][3] != 'L' || argp[index_of_shell][4] != 'L') {
        index_of_shell++;
    }
    char *name = "argv";
    null_list_of_args();
    
    while(1) {
        lst_copy = task3();
        tree_pointer = (struct cmd_inf**)malloc(sizeof(struct cmd_inf*));
        //printlist();
        //printf("\n");
        //printf("%d\n", ret_sizelist());
        size_l = ret_sizelist() - 1;
        tree_pointer[cur_tree_p_ind] = (struct cmd_inf*)malloc(sizeof(struct cmd_inf));
        // [cur_tree_p_ind]
        tree = tree_pointer[cur_tree_p_ind];
        tree_pointer[cur_tree_p_ind]->argv = NULL;
        tree_pointer[cur_tree_p_ind]->infile = NULL;
        tree_pointer[cur_tree_p_ind]->outfile = NULL;
        tree_pointer[cur_tree_p_ind]->outfile_index = 0;
        tree_pointer[cur_tree_p_ind]->backgrnd = 0;
        tree_pointer[cur_tree_p_ind]->next_exit_mode = 0;
        tree_pointer[cur_tree_p_ind]->psubcmd = NULL;
        tree_pointer[cur_tree_p_ind]->pipe = NULL;
        tree_pointer[cur_tree_p_ind]->next = NULL;
        tree_pipe_pointer = tree_pointer[cur_tree_p_ind];
        printf("\n");
        for(i = 0; i <= size_l - 1; i++) {
            if (strcmp(lst_copy[i], "&") == 0) {
                tree_pipe_pointer->backgrnd = 1;
                termlist_of_args();
                clearlist_of_args();
                tree_pointer[cur_tree_p_ind]->next = (struct cmd_inf*)malloc(sizeof(struct cmd_inf));
                tree_pointer[cur_tree_p_ind]->next->argv = NULL;
                tree_pointer[cur_tree_p_ind]->next->infile = NULL;
                tree_pointer[cur_tree_p_ind]->next->outfile = NULL;
                tree_pointer[cur_tree_p_ind]->next->backgrnd = 0;
                tree_pointer[cur_tree_p_ind]->next->outfile_index = 0;
                tree_pointer[cur_tree_p_ind]->next->next_exit_mode = 0;
                tree_pointer[cur_tree_p_ind]->next->psubcmd = NULL;
                tree_pointer[cur_tree_p_ind]->next->pipe = NULL;
                tree_pointer[cur_tree_p_ind]->next->next = NULL;
                tree_pointer[cur_tree_p_ind] = tree_pointer[cur_tree_p_ind]->next;
                tree_pipe_pointer = tree_pointer[cur_tree_p_ind];
            }
            else if (strcmp(lst_copy[i], "&&") == 0) {
                termlist_of_args();
                clearlist_of_args();
                tree_pointer[cur_tree_p_ind]->next = (struct cmd_inf*)malloc(sizeof(struct cmd_inf));
                tree_pointer[cur_tree_p_ind]->next->argv = NULL;
                tree_pointer[cur_tree_p_ind]->next->infile = NULL;
                tree_pointer[cur_tree_p_ind]->next->outfile = NULL;
                tree_pointer[cur_tree_p_ind]->next->backgrnd = 0;
                tree_pointer[cur_tree_p_ind]->next->outfile_index = 0;
                tree_pointer[cur_tree_p_ind]->next->next_exit_mode = 2;
                tree_pointer[cur_tree_p_ind]->next->psubcmd = NULL;
                tree_pointer[cur_tree_p_ind]->next->pipe = NULL;
                tree_pointer[cur_tree_p_ind]->next->next = NULL;
                tree_pointer[cur_tree_p_ind] = tree_pointer[cur_tree_p_ind]->next;
                tree_pipe_pointer = tree_pointer[cur_tree_p_ind];
            }
            else if (strcmp(lst_copy[i], "||") == 0) {
                termlist_of_args();
                clearlist_of_args();
                tree_pointer[cur_tree_p_ind]->next = (struct cmd_inf*)malloc(sizeof(struct cmd_inf));
                tree_pointer[cur_tree_p_ind]->next->argv = NULL;
                tree_pointer[cur_tree_p_ind]->next->infile = NULL;
                tree_pointer[cur_tree_p_ind]->next->outfile = NULL;
                tree_pointer[cur_tree_p_ind]->next->backgrnd = 0;
                tree_pointer[cur_tree_p_ind]->next->outfile_index = 0;
                tree_pointer[cur_tree_p_ind]->next->next_exit_mode = 1;
                tree_pointer[cur_tree_p_ind]->next->psubcmd = NULL;
                tree_pointer[cur_tree_p_ind]->next->pipe = NULL;
                tree_pointer[cur_tree_p_ind]->next->next = NULL;
                tree_pointer[cur_tree_p_ind] = tree_pointer[cur_tree_p_ind]->next;
                tree_pipe_pointer = tree_pointer[cur_tree_p_ind];
            }
            else if (strcmp(lst_copy[i], ";") == 0) {
                termlist_of_args();
                clearlist_of_args();
                tree_pointer[cur_tree_p_ind]->next = (struct cmd_inf*)malloc(sizeof(struct cmd_inf));
                tree_pointer[cur_tree_p_ind]->next->argv = NULL;
                tree_pointer[cur_tree_p_ind]->next->infile = NULL;
                tree_pointer[cur_tree_p_ind]->next->outfile = NULL;
                tree_pointer[cur_tree_p_ind]->next->backgrnd = 0;
                tree_pointer[cur_tree_p_ind]->next->outfile_index = 0;
                tree_pointer[cur_tree_p_ind]->next->next_exit_mode = 0;
                tree_pointer[cur_tree_p_ind]->next->psubcmd = NULL;
                tree_pointer[cur_tree_p_ind]->next->pipe = NULL;
                tree_pointer[cur_tree_p_ind]->next->next = NULL;
                tree_pointer[cur_tree_p_ind] = tree_pointer[cur_tree_p_ind]->next;
                tree_pipe_pointer = tree_pointer[cur_tree_p_ind];
            }
            else if (strcmp(lst_copy[i], "|") == 0) {
                termlist_of_args();
                clearlist_of_args();
                tree_pointer[cur_tree_p_ind]->pipe = (struct cmd_inf*)malloc(sizeof(struct cmd_inf));
                tree_pointer[cur_tree_p_ind]->pipe->argv = NULL;
                tree_pointer[cur_tree_p_ind]->pipe->infile = NULL;
                tree_pointer[cur_tree_p_ind]->pipe->outfile = NULL;
                tree_pointer[cur_tree_p_ind]->pipe->backgrnd = 0;
                tree_pointer[cur_tree_p_ind]->pipe->outfile_index = 0;
                tree_pointer[cur_tree_p_ind]->pipe->next_exit_mode = 0;
                tree_pointer[cur_tree_p_ind]->pipe->psubcmd = NULL;
                tree_pointer[cur_tree_p_ind]->pipe->pipe = NULL;
                tree_pointer[cur_tree_p_ind]->pipe->next = NULL;
                tree_pointer[cur_tree_p_ind] = tree_pointer[cur_tree_p_ind]->pipe;
            }
            else if (strcmp(lst_copy[i], "(") == 0) {
                termlist_of_args();
                clearlist_of_args();
                cur_tree_p_ind++;
                tree_pointer = realloc(tree_pointer, (cur_tree_p_ind + 1) * sizeof(struct cmd_inf*));
                tree_pointer[cur_tree_p_ind] = (struct cmd_inf*)malloc(sizeof(struct cmd_inf));
                tree_pointer[cur_tree_p_ind]->argv = NULL;
                tree_pointer[cur_tree_p_ind]->infile = NULL;
                tree_pointer[cur_tree_p_ind]->outfile = NULL;
                tree_pointer[cur_tree_p_ind]->backgrnd = 0;
                tree_pointer[cur_tree_p_ind]->outfile_index = 0;
                tree_pointer[cur_tree_p_ind]->next_exit_mode = 0;
                tree_pointer[cur_tree_p_ind]->psubcmd = NULL;
                tree_pointer[cur_tree_p_ind]->pipe = NULL;
                tree_pointer[cur_tree_p_ind]->next = NULL;
                tree_pointer[cur_tree_p_ind-1]->psubcmd = tree_pointer[cur_tree_p_ind];
            }
            else if (strcmp(lst_copy[i], ")") == 0) {
                termlist_of_args();
                clearlist_of_args();
                tree_pointer[cur_tree_p_ind] = NULL;
                cur_tree_p_ind--;
            }
            else if (strcmp(lst_copy[i], "<") == 0) {
                name = "infile";
            }
            else if (strcmp(lst_copy[i], ">") == 0) {
                name = "outfile";
                outfile_ind = 0;
            }
            else if (strcmp(lst_copy[i], ">>") == 0) {
                name = "outfile";
                outfile_ind = 1;
            }
            else {
                if (strcmp(name, "argv") == 0)
                    addword_of_args(lst_copy[i]);
                tree_writer(name, i, outfile_ind);
                name = "argv";
            }
        }
        termlist_of_args();
        //printlist_of_args();
        //print_tree(tree, 0);
        //pipe_deep = 1;
        //search_deep(tree);
        //printf("max pipe deep = %d\n", pipe_deep);

        launch_tree(tree);
        
        tree_free(tree);
        free(tree_pointer);
        clearlist();
        clearlist_of_args();
        if (exit_status() == 1)
            break;
    }
    return 0;
}
