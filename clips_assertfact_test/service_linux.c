//
//  main.c
//  Service
//
//  Created by 孟玄 on 16/10/8.
//  Copyright © 2016年 孟玄. All rights reserved.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>

#define MYPORT  8887
#define QUEUE  20

int main()
{
    char fact_file_path[50] = "/Users/mengxuan/Desktop/CLIPSFact_723.txt";
    FILE *pFile = fopen(fact_file_path, "r");
    if (pFile == NULL) printf("file error!\n");
    
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(server_sockfd, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) == -1){
        perror("bind error");
        exit(1);
    }
    
    if (listen(server_sockfd, QUEUE) == -1) {
        perror("listen error");
        exit(1);
    }
    
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    
    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if (conn < 0) {
        perror("connect error");
        exit(1);
    }
    else {
        printf("connect sucessfully!\n");
    }
    
    char tmpBuffer[200];
    long long line_count = 0;
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("%ld\n", tv.tv_sec);
    while (1) {
        memset(tmpBuffer, 0, sizeof(tmpBuffer));
        fgets(tmpBuffer, 100, pFile);
        send(conn, tmpBuffer, sizeof(tmpBuffer), 0);
        
        if (++line_count > 45000) break;
        
       // usleep(50);
    }
    struct timeval cur;
    gettimeofday(&cur, NULL);
    printf("%ld\n", cur.tv_sec);
    printf("%ld\n", cur.tv_usec / 1000 + cur.tv_sec * 1000 - tv.tv_usec / 1000 - tv.tv_sec * 1000);
    close(conn);
    close(server_sockfd);
    return 0;
}



































