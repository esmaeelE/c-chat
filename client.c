///send, recv -> read write
//~ ./client 127.0.0.1 901
///add copy left

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//headers for socket and related functions
#include <sys/types.h>
#include <sys/socket.h>

//for including structures which will store information needed
#include <netinet/in.h>
#include <unistd.h>

//for gethostbyname
#include "netdb.h"
#include "arpa/inet.h"


#include <limits.h>

int toInt(char *str){
		
	char* p;
    long arg = strtol(str, &p, 10);
    if (*p != '\0') {
        return -1; // In main(), returning non-zero means failure
    }

    if (arg < INT_MIN || arg > INT_MAX) {
        return 1;
    }
    int arg_int = arg;
   
	return arg_int; 
}

void sendSide(int socketDescriptor, char *sendBuffer){
	
	while(1){
		
			bzero(sendBuffer,sizeof(&sendBuffer));
			
			/*This function is used to read from server*/
			fgets(sendBuffer,10000,stdin);
		
			/*Send the message to server*/
			send(socketDescriptor,sendBuffer,strlen(sendBuffer)+1,0);
			printf("sendlen: %d\n", strlen(sendBuffer));

		}
	
}


void RecieveSide(int socketDescriptor, char *recvBuffer){
	
	while(1){
			bzero(recvBuffer,sizeof(&recvBuffer));
			/*Receiving the request from client*/
			int a = recv(socketDescriptor,recvBuffer,sizeof(&recvBuffer),0);
			
			///add to end of buffer resolve stange 
			//*(recvBuffer + sizeof(&recvBuffer))	='\0';
			//*(recvBuffer + a)	='\0';
			recvBuffer[a]	='\0';
			///
			
				
			if(a>0)
				printf("\nRecieved>\n\t%s: len: %d\n",recvBuffer, a);
			else{
				exit(0);
			}

		}
}	


int main(int argc,char *argv[])
{
		
	char *hostaddr="127.0.0.1";//127.0.0.1 works
	int port=901;//5500;//default

	int socketDescriptor;

	struct sockaddr_in serverAddress;
	char sendBuffer[1000]={0};
	char recvBuffer[1000]={0};

	pid_t cpid;

	/* Get IP and authorization data */
	if (argv[1] != NULL && argv[2] != NULL) {
		hostaddr = argv[1];
		if (argv[2]!=NULL)
			port = toInt(argv[2]);
	} else {
		;
		//fprintf(stderr, "Usage: %s <target ip> <username> <password> (<port>)\n",argv[0]);
		//	return (EXIT_FAILURE);
	}
	
	bzero(&serverAddress,sizeof(serverAddress));

	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr=inet_addr(hostaddr);
	serverAddress.sin_port=htons(port);//5500
	
	/*Creating a socket, assigning IP address and port number for that socket*/
	socketDescriptor=socket(AF_INET,SOCK_STREAM,0);

	bind(socketDescriptor,(struct sockaddr*)&serverAddress,sizeof(serverAddress));

	/*Connect establishes connection with the server using server IP address*/
	int a =connect(socketDescriptor,(struct sockaddr*)&serverAddress,sizeof(serverAddress));
	printf("%d\n", a);
	
	/*Fork is used to create a new process*/
	cpid=fork();
	 
	if(cpid==0){
		///Send side
		sendSide(socketDescriptor, sendBuffer);
	}
	
	else{ //parent
		///Receive side
		RecieveSide(socketDescriptor, recvBuffer);
	}
	
	 
	//close(socketDescriptor); 
	
	return 0;

}
