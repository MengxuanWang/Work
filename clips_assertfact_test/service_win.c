//
//  main.c
//  Service
//
//  Created by 孟玄 on 16/10/8.
//  Copyright © 2016年 孟玄. All rights reserved.
//

#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#pragma comment(lib,"ws2_32.lib")

int main(int argc, char *argv[])
{
	char fact_file_path[50] = "C:/Users/Administrator/Desktop/CLIPSFact_105.txt";
	FILE *pFile = fopen(fact_file_path, "r");
	if (pFile == NULL) {
		printf("file error!\n");
	} 

    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
	   return 0;
    }
    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (slisten == INVALID_SOCKET)
    {
	   printf("socket error !");
       return 0;
    }
 
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR){
		printf("bind error !");
	}

	if (listen(slisten, 5) == SOCKET_ERROR) {
		printf("listen error!");
		return 0;
	}

	SOCKET sClient;
	struct sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);

	printf("waiting for connect...\n");
	sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
	if (sClient == INVALID_SOCKET)
	{
		printf("connect error !");
		exit(1);
	}
	else {
		printf("connect successfuly!\n");
	}

	char tmpBuffer[200];
	long long line_count = 0;
	
	long long time1 = GetTickCount();

	while (1) {
		memset(tmpBuffer, 0, sizeof(tmpBuffer));
		fgets(tmpBuffer, 100, pFile);
		send(sClient, tmpBuffer, sizeof(tmpBuffer), 0);

		if (++line_count > 300000) break;
	}

	long long time2 = GetTickCount();
	struct timeval cur;

	printf("%ld\n", time2 - time1);

	closesocket(sClient);
	closesocket(slisten);
	WSACleanup();
	Sleep(10*1000);
	return 0;
}

































