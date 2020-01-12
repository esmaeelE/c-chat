//~ sudo ./server 901

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
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
					
			if(a>0){
				printf("Rcvlen: %d\n", strlen(recvBuffer));
				printf("\nRecieved>\n\t%s\n",recvBuffer);
				
				//~ for(int i=0;a;i++)
					//~ printf("\n%c",*(recvBuffer+i));
				
				}
			else{
				exit(0);
			}

		}
}	

int main(int argc,char *argv[])
{

	//901
	int port=901;//5500;//default
		
	int clientSocketDescriptor,socketDescriptor;

	struct sockaddr_in serverAddress,clientAddress;
	socklen_t clientLength;

	char recvBuffer[1000],sendBuffer[1000];
	pid_t cpid;
	
		/* Get IP and authorization data */
	if (argv[1] != NULL) {
		port = toInt(argv[1]);
	} else {
		;
		//~ fprintf(stderr, "Usage: %s <target ip> <username> <password> (<port>)\n",
				//~ argv[0]);
	//~ //	return (EXIT_FAILURE);
	}
	
	bzero(&serverAddress,sizeof(serverAddress));

	/*Socket address structure*/
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddress.sin_port=htons(port);//5500

	/*TCP socket is created, an Internet socket address structure is filled with
	wildcard address & server’s well known port*/
	socketDescriptor=socket(AF_INET,SOCK_STREAM,0);


/// close socket in next run
///  https://stackoverflow.com/a/52056972/7508077
	int opt = 1;
	if (setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))<0) {
		 perror("setsockopt");
		 exit(1);
    }

///

    // Binding newly created socket to given IP and verification 
    if ((bind(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else{
		printf("Socket successfully binded..\n"); 
	}

	/*Listen function specifies the maximum number of connections that kernel should queue
	for this socket*/
	
	// Now server is ready to listen and verification 
    if ((listen(socketDescriptor, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else{
        printf("Server listening..\n"); 
	}
      
	/*The server to return the next completed connection from the front of the
	completed connection Queue calls it*/
	clientSocketDescriptor=accept(socketDescriptor,(struct sockaddr*)&clientAddress,&clientLength);

	/*Fork system call is used to create a new process*/    
	cpid=fork();
	
	if(cpid==0){
		///Receive side
		RecieveSide(clientSocketDescriptor, recvBuffer);
	}
	else{
		///Send side
		sendSide(clientSocketDescriptor, sendBuffer);
	}
	
	//close(clientSocketDescriptor);
	return 0;
}
