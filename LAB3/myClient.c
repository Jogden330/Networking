/******************************************************************************
* myClient.c
*
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "networks.h"
#include "PDUlab.h"

#define MAXBUF 1024
#define DEBUG_FLAG 1

int sendToServer(int socketNum);
int readFromStdin(char * buffer);
void checkArgs(int argc, char * argv[]);
void recvFromServer(int serverSocket );

int main(int argc, char * argv[])
{
	int socketNum = 0;         //socket descriptor
        checkArgs(argc, argv);

	/* set up the TCP Client socket  */
	socketNum = tcpClientSetup(argv[1], argv[2], DEBUG_FLAG);
        do{
 
	if(sendToServer(socketNum) == 0){
          break; 
        }
                
        recvFromServer(socketNum);
        
        }while(1);
	close(socketNum);
	
	return 0;
}

int sendToServer(int socketNum)
{
   
 
      uint8_t sendBuf[MAXBUF];   //data buffer
      int sendLen = 0;        //amount of data to send
      int sent = 0;            //actual amount of data sent/* get the data and send it   */
      
       sendLen = readFromStdin((char *) sendBuf);
       printf("read: %s string len: %d (including null)\n",(char *) sendBuf, sendLen);
	
       sent =  sendPDU(socketNum, sendBuf, sendLen);
                                  

	if (sent < 0)
	{
		perror("send call");
		exit(-1);
	}
        
       
	printf("Amount of data sent is: %d\n", sent);
       
     if (!strcmp("exit", (char *) sendBuf)){
         return 0;
     }
     return 1; 
 
}


int readFromStdin(char * buffer)
{
	char aChar = 0;
	int inputLen = 0;        
	
	// Important you don't input more characters than you have space 
	buffer[0] = '\0';
	printf("Enter data: ");
	while (inputLen < (MAXBUF - 1) && aChar != '\n')
	{
		aChar = getchar();
		if (aChar != '\n')
		{
			buffer[inputLen] = aChar;
			inputLen++;
		}
	}
	
	// Null terminate the string
	buffer[inputLen] = '\0';
	inputLen++;
	
	return inputLen;
}

void checkArgs(int argc, char * argv[])
{
	/* check command line arguments  */
	if (argc != 3)
	{
		printf("usage: %s host-name port-number \n", argv[0]);
		exit(1);
	}
}

void recvFromServer(int serverSocket)
{

  uint8_t buf[MAXBUF];
  int messageLen = 0;

  //now get the data from the client_socket
  /* struct pseudoheader *header; */
  if ((messageLen = recvPDU(serverSocket, buf, MAXBUF)) < 0)
    {
      perror("recv call");
      exit(-1);
    }


  printf("Message received, length: %d Data: %s\n", messageLen,(char *) buf);


}
