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
#define N 1024
 
#ifndef  MAJOR
  #define MINOR_BITS 0
  #define MAJOR(dev) ((unsigned) dev >> MINOR_BITS)
  #define MINOR(dev) (dev & MINOR_BITS)
#endif
 
// показать тип файла в первой позиции выходной строки 
void display_file_type ( int st_mode ) 
{                                   
    switch ( st_mode & S_IFMT )
    {
        case S_IFDIR:  putchar ( 'd' ); return;
        case S_IFCHR:  putchar ( 'c' ); return;
        case S_IFBLK:  putchar ( 'b' ); return;
        case S_IFREG:  putchar ( '-' ); return;
        case S_IFLNK:  putchar ( 'l' ); return;
        case S_IFSOCK: putchar ( 's' ); return;
    }
} 
 
// показать права доступа для владельца, группы и прочих пользователей, а также все спец.флаги 
void display_permission ( int st_mode )
{
  static const char xtbl[10] = "rwxrwxrwx";
  char     amode[10];
  int      i, j;
 
  for ( i = 0, j = ( 1 << 8 ); i < 9; i++, j >>= 1 )
    amode[i] = ( st_mode&j ) ? xtbl[i]: '-';
  if ( st_mode & S_ISUID )   amode[2]= 's';
  if ( st_mode & S_ISGID )   amode[5]= 's';
  if ( st_mode & S_ISVTX )   amode[8]= 't';
  amode[9]='\0';
  printf ( "%s ",amode );
}
 
// перечислить атрибуты одного файла
void long_list ( char * path_name )
{
  struct stat     statv;
  struct passwd  *pw_d;
 
  if ( lstat ( path_name, &statv ) )
  { 
    perror ( path_name ); 
    return;
  }
  display_file_type ( statv.st_mode );
  display_permission ( statv.st_mode );
  printf ( "%d ",statv.st_nlink );  // значение счетчика жестких связей
  pw_d = getpwuid ( statv.st_uid ); // преобразовать UID в имя пользователя
  printf ( "%s ",pw_d->pw_name );   // и напечатать его
 
  if (
      ( statv.st_mode & S_IFMT) == S_IFCHR  ||
      ( statv.st_mode & S_IFMT) == S_IFBLK
     )
    // показать старший и младший номера устройства
    printf ( "%d, %d",MAJOR(statv.st_rdev), MINOR(statv.st_rdev) );
  else
    // или размер файла
    printf ( "%lld", statv.st_size );
  //  показать имя файла
  printf ( "     %s\n", path_name );
}


void ls_R(char *dr)
{
    DIR *d = opendir(dr);
    struct dirent *dir;
    char s[N];
    getcwd(s, N);
    while((dir = readdir(d))!= NULL){
        if (dir->d_type == DT_DIR && dir->d_name[0] != '.') {
            strcat(s,"/");
            strcat(s,dir->d_name);
            ls_R(s);
        }
        if (dir->d_name[0] != '.')
            printf("%s\t",dir->d_name);
    }
    closedir(d);
}


int main(int argc, char **argv) {
    if (argc == 1) {
        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if ((dir->d_name)[0] != '.')
                    printf("%s\t", dir->d_name);
            }
            closedir(d);
        }
        printf("\n");
    }
    else if (strcmp(argv[1], "-g") == 0) {
        struct stat     statv;
        struct group  *pw_d;
        char *path_name = ".";
 
        if ( lstat ( path_name, &statv ) ) { 
            perror ( path_name ); 
            return 1;
        }
        pw_d = getgrgid(statv.st_gid);
        printf("%s\n",pw_d->gr_name);
    }
    else if (strcmp(argv[1], "-R") == 0) {
        ls_R(".");
        printf("\n");
    }
    else if (strcmp(argv[1], "-l") == 0){
        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if ((dir->d_name)[0] != '.')
                    long_list(dir->d_name);
            }
            closedir(d);
        }
    }
    return 0;
}
