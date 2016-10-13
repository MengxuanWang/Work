   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  07/01/05          */
   /*                                                     */
   /*                     MAIN MODULE                     */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Moved UserFunctions and EnvUserFunctions to    */
/*            the new userfunctions.c file.                  */
/*                                                           */
/*************************************************************/

/***************************************************************************/
/*                                                                         */
/* Permission is hereby granted, free of charge, to any person obtaining   */
/* a copy of this software and associated documentation files (the         */
/* "Software"), to deal in the Software without restriction, including     */
/* without limitation the rights to use, copy, modify, merge, publish,     */
/* distribute, and/or sell copies of the Software, and to permit persons   */
/* to whom the Software is furnished to do so.                             */
/*                                                                         */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS */
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT   */
/* OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY  */
/* CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES */
/* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN   */
/* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF */
/* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.          */
/*                                                                         */
/***************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <pthread.h>
#include "setup.h"
#include "clips.h"

#define MYPORT 8887
#define BUFFER_SIZE 200

int main(int,char *[]);
void UserFunctions(void);
void EnvUserFunctions(void *);

/****************************************/
/* main: Starts execution of the expert */
/*   system development environment.    */
/****************************************/


int main(
  int argc,
  char *argv[])
  {
      void *theEnv;
      theEnv = CreateEnvironment();
      
      char rule_file_path[50] = "/Users/mengxuan/Desktop/CLIPSRule_824.clp";
      EnvLoad(theEnv, rule_file_path);
      
      char output_file_path[50] = "/Users/mengxuan/Desktop/output.txt";
      FILE *output_file = freopen(output_file_path, "w+", stdout);
      system("ifconfig");
      
      int sock_cli = socket(AF_INET, SOCK_STREAM, 0);
      
      struct sockaddr_in servaddr;
      memset(&servaddr, 0, sizeof(servaddr));
      servaddr.sin_family = AF_INET;
      servaddr.sin_port = htons(MYPORT);
      servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
      
      if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
          perror("connect error");
          exit(1);
      }
   
      char recvbuf[BUFFER_SIZE];
      
      int run_every = 100;
      long long line_count = 0;
     // char temp[50000][200];
      while (recv(sock_cli, recvbuf, sizeof(recvbuf), 0)) {
          //EnvAssertString(theEnv, recvbuf);
          memset(recvbuf, 0, sizeof(recvbuf));
       //   strcpy(temp[line_count], recvbuf);
          if (++line_count % run_every == 0) {
       //       EnvRun(theEnv, -1);
          }
      }
      
      
      
      printf("line count %lld\n", line_count);
      EnvRun(theEnv, -1);
      
      fclose(output_file);
     //RerouteStdin(theEnv,argc,argv);
     //CommandLoop(theEnv);

   /*==================================================================*/
   /* Control does not normally return from the CommandLoop function.  */
   /* However if you are embedding CLIPS, have replaced CommandLoop    */
   /* with your own embedded calls that will return to this point, and */
   /* are running software that helps detect memory leaks, you need to */
   /* add function calls here to deallocate memory still being used by */
   /* CLIPS. If you have a multi-threaded application, no environments */
   /* can be currently executing. If the ALLOW_ENVIRONMENT_GLOBALS     */
   /* flag in setup.h has been set to TRUE (the default value), you    */
   /* call the DeallocateEnvironmentData function which will call      */
   /* DestroyEnvironment for each existing environment and then        */
   /* deallocate the remaining data used to keep track of allocated    */
   /* environments. Otherwise, you must explicitly call                */
   /* DestroyEnvironment for each environment you create.              */
   /*==================================================================*/
   
   /* DeallocateEnvironmentData(); */
   /* DestroyEnvironment(theEnv); */
   
   return(-1);
  }
