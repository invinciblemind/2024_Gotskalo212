#ifndef __CLEARLIST
#define __CLEARLIST

void clearlist();

void null_list();

void termlist();

void nullbuf();

void addsym(int c);

void addword();

void printlist();

void heshlist();

void sortlist();

int symset(int c);

void del2list();

int ret_sizelist();

char ** ret_lst();

char **task3();

struct cmd_inf {
    char **argv; // список из имени команды и аргументов
    char *infile; // переназначенный файл стандартного ввода
    char *outfile; // переназначенный файл стандартного вывода
    int outfile_index; // =0, если указан > (запись в начало файла), 
                        //=1, если указан >> (запись в конец файла)
    int backgrnd; // =1, если команда подлежит выполнению в фоновом режиме
    int next_exit_mode; // =0, если не было || и &&, =1, если был ||, =2, 
                                                            //если был &&
    struct cmd_inf *psubcmd; // команды для запуска в дочернем shell
    struct cmd_inf *pipe; // следующая команда после “|”
    struct cmd_inf *next; // следующая после “;” (или после “&”)
};

void print_tree(struct cmd_inf *pointer, int level);

void launch_tree(struct cmd_inf *tree);

int exit_status(void);

#endif
