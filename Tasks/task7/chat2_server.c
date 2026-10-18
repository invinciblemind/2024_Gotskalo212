#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#define BASE_STRING_LEN 80
#define MAX_NUM_OF_CLIENTS 20
#define QUESIZE 1000

int own_sockfd, client_sockfds[MAX_NUM_OF_CLIENTS], last_client_num = 0, pids[MAX_NUM_OF_CLIENTS];
FILE *clients, *left_clients;

// правильная обработка сигнала SIGINT на клиентских процессах с закрытием сокетов
void sig_hndlr(int s)
{
    int i;
    fclose(clients);
    fclose(left_clients);
    for (i = 0; i < last_client_num; i++)
        close(client_sockfds[i]);
    kill(getpid(), SIGINT);
}

// правильная обработка сигнала SIGINT на сервере с закрытием сокетов
void sig_hndlr2(int s)
{
    signal(SIGINT, SIG_DFL);
    int i;
    for (i = 0; i < last_client_num; i++) {
        shutdown(client_sockfds[i], 2);
        kill(pids[i], SIGUSR1);
    }
    shutdown(own_sockfd, 2);
    fclose(clients);
    fclose(left_clients);
    kill(getpid(), SIGINT);
}

int main(int argc, char * argv[])
{
    char nickname[BASE_STRING_LEN], nicknames[MAX_NUM_OF_CLIENTS][BASE_STRING_LEN], buf_str[BASE_STRING_LEN], 
    buf_str3[BASE_STRING_LEN], buf_str2[BASE_STRING_LEN], buf, *p, private_clients[MAX_NUM_OF_CLIENTS][BASE_STRING_LEN];
    struct sockaddr_in own_addr, client_addrs[MAX_NUM_OF_CLIENTS];
    int len, fd[2], i, j, mas_of_left_clients[MAX_NUM_OF_CLIENTS], last_i, k, x, portnum, last_private;
    
    clients = fopen("clients.txt", "w+");
    left_clients = fopen("left_clients.txt", "w+");
    
    // считывание из командной строки номера порта
    strcpy(buf_str, argv[1]);
    portnum = 0;
    i = 0;
    while (buf_str[i] != '\0') {
        portnum = portnum * 10 + buf_str[i++] - '0';
    }
    
    // работа с серверным сокетом
    own_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&own_addr, 0, sizeof(own_addr));
    own_addr.sin_family = AF_INET;
    own_addr.sin_addr.s_addr = INADDR_ANY;
    own_addr.sin_port = htons(portnum);
    bind(own_sockfd, (struct sockaddr *) &own_addr, sizeof(own_addr));
    listen(own_sockfd, QUESIZE);
    
    // открытие(создание) дополнительный файлов для хранение общей информации о клиентах,
    // а также организация канала для взаимного исключения
    pipe(fd);
    write(fd[1], "1", 1);
    
    while (1) {
        signal(SIGINT, sig_hndlr2);
        
        // подключение нового клиента
        memset(&client_addrs[last_client_num], 0, sizeof(client_addrs[last_client_num]));
        len = sizeof(client_addrs[last_client_num]);
        client_sockfds[last_client_num] = accept(own_sockfd, (struct sockaddr *)&client_addrs[last_client_num], &len);
        
        // завершение работы старых клиентов (вся основная информация сохраняется)
        for (i = 0; i < last_client_num; i++)
            kill(pids[i], SIGUSR1);
        memset(&pids, 0, sizeof(pids));
        
        // считывание имен клиентов
        memset(&nicknames, 0, sizeof(nicknames));
        i = 0;
        read(fd[0], &buf, 1);
        fseek(clients, 0, SEEK_SET);
        while (fscanf(clients, "%s", nicknames[i++]) != EOF);
        write(fd[1], "1", 1);
        
        for (i = 0; i <= last_client_num; i++) {
            // считывание номеров ушедших клиентов
            memset(&mas_of_left_clients, 0, sizeof(mas_of_left_clients));
            read(fd[0], &buf, 1);
            fseek(left_clients, 0, SEEK_SET);
            last_i = 0;
            while (fscanf(left_clients, "%d", &mas_of_left_clients[last_i++]) != EOF);
            last_i--;
            write(fd[1], "1", 1);
            
            // отдельный процесс для каждого клиента
            if ((pids[i] = fork()) == 0) {
                signal(SIGINT, SIG_DFL);
                close(own_sockfd);
                signal(SIGUSR1, sig_hndlr);
                
                memset(&private_clients, 0, sizeof(private_clients));
                last_private = 0;
                
                // проверка на онлайн клиента с этим номером
                for (k = 0; k < last_i; k++)
                    if (i == mas_of_left_clients[k])
                        break;
                if (k < last_i && i == mas_of_left_clients[k])
                    kill(getpid(), SIGUSR1);
                
                // для последнего - только что подключившегося - клиента запрашиваем имя с проверкой на уникальность, 
                // далее - рассылка всем остальным, что новый клиент подключился
                if (i == last_client_num) {
                    while (1) {
                        recv(client_sockfds[i], &nickname, BASE_STRING_LEN, 0);
                        for (j = 0; j < last_client_num; j++) {
                            if (strcmp(nickname, nicknames[j]) == 0) {
                                break;
                            }
                        }
                        if (j >= last_client_num)
                            break;
                        else {
                            strcpy(buf_str, "This nickname is reserved. Please choose other: ");
                            send(client_sockfds[i], buf_str, strlen(buf_str) + 1, 0);
                        }
                    }
                    send(client_sockfds[i], "0", 2, 0);
                    
                    memset(&buf_str, 0, sizeof(buf_str));
                    strcat(buf_str, "User ");
                    strcat(buf_str, nickname);
                    strcat(buf_str, " joined the chat.");
                    for (j = 0; j < last_client_num; j++) {
                        for (k = 0; k < last_i; k++)
                            if (j == mas_of_left_clients[k])
                                break;
                        if (k < last_i && j == mas_of_left_clients[k])
                            continue;
                        send(client_sockfds[j], buf_str, strlen(buf_str) + 1, 0);
                    }
                    read(fd[0], &buf, 1);
                    fseek(clients, 0, SEEK_END);
                    fprintf(clients, "%s\n", nickname);
                    write(fd[1], "1", 1);
                    strcpy(nicknames[i], nickname);
                }
                
                while (1) {
                    // считывание имен клиентов
                    read(fd[0], &buf, 1);
                    fseek(clients, 0, SEEK_SET);
                    j = 0;
                    memset(&nicknames, 0, sizeof(nicknames));
                    while (fscanf(clients, "%s", nicknames[j++]) != EOF);
                    write(fd[1], "1", 1);
                    
                    // получение (ожидание получения) сообщения от данного клиента
                    recv(client_sockfds[i], &buf_str, BASE_STRING_LEN, 0);
                    
                    // считывание номеров ушедших клиентов
                    memset(&mas_of_left_clients, 0, sizeof(mas_of_left_clients));
                    read(fd[0], &buf, 1);
                    fseek(left_clients, 0, SEEK_SET);
                    last_i = 0;
                    while (fscanf(left_clients, "%d", &mas_of_left_clients[last_i++]) != EOF);
                    last_i--;
                    write(fd[1], "1", 1);
                    
                    // обработка сообщения: если начинается с \ - служебная команда, иначе - обычное сообщение
                    if (buf_str[0] == '\\') {
                        // реализация \user - отправление данному клиенту всех ников пользователей онлайн
                        if (buf_str[1] == 'u' && buf_str[2] == 's' && buf_str[3] == 'e' && buf_str[4] == 'r' && buf_str[5] == '\n') {
                            read(fd[0], &buf, 1);
                            fseek(clients, 0, SEEK_SET);
                            k = 0;
                            while (fscanf(clients, "%s", nickname) != EOF) {
                                for (x = 0; x < last_i; x++) {
                                    if (k == mas_of_left_clients[x])
                                        break;
                                }
                                if (x >= last_i || k != mas_of_left_clients[x]) {
                                    send(client_sockfds[i], nickname, strlen(nickname) + 1, 0);
                                    for (j = 1; j <= 10000000; j++);
                                }
                                k++;
                            }
                            write(fd[1], "1", 1);
                        }
                        else if (buf_str[1] == 'q' && buf_str[2] == 'u' && buf_str[3] == 'i' && buf_str[4] == 't' && (buf_str[5] == '\n' || buf_str[5] == ' ')) {
                            // реализация \quit - отправка всем остальным информации об уходе с прощальным сообщением и выход из цикла while этого клиента
                            memset(&buf_str2, 0, sizeof(buf_str2));
                            strcat(buf_str2, "User ");
                            strcat(buf_str2, nicknames[i]);
                            strcat(buf_str2, " is leaving: ");
                            for (j = 6; buf_str[j] != '\0'; j++) {
                                buf_str3[j - 6] = buf_str[j];
                            }
                            buf_str3[j - 6] = '\0';
                            strcat(buf_str2, buf_str3);
                            for (j = 0; j <= last_client_num; j++) {
                                for (k = 0; k < last_i; k++)
                                    if (j == mas_of_left_clients[k])
                                        break;
                                if (k < last_i && j == mas_of_left_clients[k])
                                    continue;
                                if (j != i)
                                    send(client_sockfds[j], buf_str2, strlen(buf_str2) + 1, 0);
                            }
                            read(fd[0], &buf, 1);
                            fseek(left_clients, 0, SEEK_END);
                            fprintf(left_clients, "%d\n", i);
                            write(fd[1], "1", 1);
                            kill(getpid(), SIGUSR1);
                        }
                        else if (buf_str[1] == 'p' && buf_str[2] == 'r' && buf_str[3] == 'i' && 
                        buf_str[4] == 'v' && buf_str[5] == 'a' && buf_str[6] == 't' && buf_str[7] == 'e') {
                            if (buf_str[8] == ' ') {
                                // реализация \private - разбивка сообщения на ник и сообщение, если ник нормальный, отправка сообщения тому клиенту
                                p = &buf_str[9];
                                j = 0;
                                memset(&buf_str2, 0, sizeof(buf_str2));
                                while (*p != ' ') {
                                    buf_str2[j++] = *p;
                                    p++;
                                }
                                buf_str2[j] = '\0';
                                p++;
                                memset(&buf_str3, 0, sizeof(buf_str3));
                                strcpy(buf_str3, p);
                                
                                // считывание имен клиентов
                                read(fd[0], &buf, 1);
                                fseek(clients, 0, SEEK_SET);
                                j = 0;
                                memset(&nicknames, 0, sizeof(nicknames));
                                while (fscanf(clients, "%s", nicknames[j++]) != EOF);
                                write(fd[1], "1", 1);
                                
                                for (j = 0; j <= last_client_num; j++) {
                                    if (strcmp(buf_str2, nicknames[j]) == 0) {
                                        memset(&buf_str, 0, sizeof(buf_str));
                                        strcat(buf_str, "(");
                                        strcat(buf_str, nicknames[i]);
                                        strcat(buf_str, ", private): ");
                                        strcat(buf_str, buf_str3);
                                        
                                        for (k = 0; k < last_i; k++) {
                                            if (j == mas_of_left_clients[k])
                                                break;
                                        }
                                        if (k >= last_i) {
                                            send(client_sockfds[j], buf_str, strlen(buf_str) + 1, 0);
                                            strcpy(private_clients[last_private++], nicknames[j]);
                                        }
                                        else {
                                            memset(&buf_str, 0, sizeof(buf_str));
                                            strcpy(buf_str, "Incorrect nickname");
                                            send(client_sockfds[i], buf_str, strlen(buf_str) + 1, 0);
                                        }
                                        break;
                                    }
                                }
                                if (j > last_client_num) {
                                    memset(&buf_str, 0, sizeof(buf_str));
                                    strcpy(buf_str, "Incorrect nickname");
                                    send(client_sockfds[i], buf_str, strlen(buf_str) + 1, 0);
                                }
                            }
                            else if (buf_str[8] == 's' && buf_str[9] == '\n') {
                                // реализация \privates - отправка данному клиенту всех тех, кому он отправлял приватные сообщения
                                for (j = 0; j < last_private; j++) {
                                    send(client_sockfds[i], private_clients[j], strlen(private_clients[j]) + 1, 0);
                                    for (k = 1; k <= 10000000; k++);
                                }
                            }
                        }
                        else if (buf_str[1] == 'h' && buf_str[2] == 'e' && buf_str[3] == 'l' && buf_str[4] == 'p' && buf_str[5] == '\n') {
                            // реализация \help - отправка данному клиенту всех допустимых команд
                            memset(&buf_str3, 0, sizeof(buf_str3));
                            strcpy(buf_str3, "   \\users - returns all online users");
                            send(client_sockfds[i], buf_str3, strlen(buf_str3) + 1, 0);
                            for (k = 1; k <= 10000000; k++);
                            memset(&buf_str3, 0, sizeof(buf_str3));
                            strcpy(buf_str3, "   \\quit <message> - leaving a chat with a last message");
                            send(client_sockfds[i], buf_str3, strlen(buf_str3) + 1, 0);
                            for (k = 1; k <= 10000000; k++);
                            memset(&buf_str3, 0, sizeof(buf_str3));
                            strcpy(buf_str3, "   \\private <nickname> <message> - private message for this nick, if it exists");
                            send(client_sockfds[i], buf_str3, strlen(buf_str3) + 1, 0);
                            for (k = 1; k <= 10000000; k++);
                            memset(&buf_str3, 0, sizeof(buf_str3));
                            strcpy(buf_str3, "   \\privates - returns all users you sent private message");
                            send(client_sockfds[i], buf_str3, strlen(buf_str3) + 1, 0);
                            for (k = 1; k <= 10000000; k++);
                            memset(&buf_str3, 0, sizeof(buf_str3));
                            strcpy(buf_str3, "   \\help - prints all commands");
                            send(client_sockfds[i], buf_str3, strlen(buf_str3) + 1, 0);
                        }
                    }
                    else {
                        // отправка всем клиентам сообщения данного клиента
                        memset(&buf_str2, 0, sizeof(buf_str2));
                        strcat(buf_str2, "(");
                        strcat(buf_str2, nicknames[i]);
                        strcat(buf_str2, "): ");
                        strcat(buf_str2, buf_str);
                        for (j = 0; j <= last_client_num; j++) {
                            for (k = 0; k < last_i; k++) {
                                if (j == mas_of_left_clients[k])
                                    break;
                            }
                            if (k < last_i && j == mas_of_left_clients[k])
                                continue;
                            send(client_sockfds[j], buf_str2, strlen(buf_str2) + 1, 0);
                        }
                    }
                }
                exit(0);
            }
        }
        last_client_num++;
    }
    return 0;
}

// ./chat2_server 50500
