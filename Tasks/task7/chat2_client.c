#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>
#include <signal.h>

#define BASE_STRING_LEN 80

int sockfd;

// правильная обработка сигнала SIGINT
void sig_hndlr(int s)
{
    close(sockfd);
    kill(getpid(), SIGINT);
}

int main(int argc, char * argv[])
{
    struct sockaddr_in addr;
    int pid, portnum, i;
    char nickname[BASE_STRING_LEN], msg[BASE_STRING_LEN];
    
    // работа с сокетом
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]); // IP - из командной строки
    
    // считывание из командной строки номера порта
    strcpy(msg, argv[2]);
    portnum = 0;
    i = 0;
    while (msg[i] != '\0') {
        portnum = portnum * 10 + msg[i++] - '0';
    }
    
    addr.sin_port = htons(portnum);
    connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    
    // запрос имени клиента до тех пор, пока сервер не скажет, что имя уникальное
    printf("Enter your nickname: ");
    scanf("%s", nickname);
    send(sockfd, nickname, strlen(nickname) + 1, 0);
    recv(sockfd, &msg, BASE_STRING_LEN, 0);
    while (msg[0] != '0') {
        printf("%s", msg);
        scanf("%s", nickname);
        send(sockfd, nickname, strlen(nickname) + 1, 0);
        recv(sockfd, &msg, BASE_STRING_LEN, 0);
    }
    
    if ((pid = fork()) == 0) {
        signal(SIGUSR1, sig_hndlr);
        // принятие сообщений
        while (1) {
            recv(sockfd, &msg, BASE_STRING_LEN, 0);
            printf("%s\n", msg);
        }
        exit(0);
    }
    else {
        // ввод и отправка сообщений, при \quit - выход из цикла и завершение работы клиента
        fgets(msg, BASE_STRING_LEN, stdin);
        while (1) {
            fgets(msg, BASE_STRING_LEN, stdin);
            send(sockfd, msg, strlen(msg) + 1, 0);
            if (msg[0] == '\\' && msg[1] == 'q' && msg[2] == 'u' && msg[3] == 'i' && msg[4] == 't' && (msg[5] == '\n' || msg[5] == ' ')) {
                break;
            }
        }
        kill(pid, SIGUSR1);
        close(sockfd);
        wait(NULL);
    }
    return 0;
}
// ./chat2_client 127.0.0.1 50500
