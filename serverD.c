#include <stdio.h>
#include <stdlib.h>
#include "serverD.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <stdbool.h>

#define PORT 23542
#define UDP "23542"
#define MPORT 24542

//Code from Beej
int createUDPSocket(){
   struct addrinfo hints, *ai, *p;
   memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_DGRAM;
   hints.ai_flags = AI_PASSIVE;
   int rv;
   if ((rv = getaddrinfo("127.0.0.1", UDP, &hints, &ai)) != 0) {
      fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
      exit(1);
   }
   int A_UDP_Socket ;
   for(p = ai; p != NULL; p = p->ai_next) {
    A_UDP_Socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (A_UDP_Socket < 0) {
    continue;
   }


   int yes=1;
   setsockopt(A_UDP_Socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
   if (bind(A_UDP_Socket, p->ai_addr, p->ai_addrlen) < 0) {
    close(A_UDP_Socket);
    continue;
    }

    break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
    fprintf(stderr, "selectserver: failed to bind\n");
    exit(2);
    }

    freeaddrinfo(ai); // all done with this
    return A_UDP_Socket;

}

void dispStartUpMsgServerD(){
	printf("Server D is up and running using UDP on port %d\n",PORT);
}

int main(){
	dispStartUpMsgServerD();
	int myUDPSocket=createUDPSocket();
	int created=0;
	FILE *fp;
	char userUName[50];
   	memset(userUName,'\0',sizeof(userUName));
   	char * toSend="Nothing";
							   
	 while(1)
	 {
	 	char recieverBuffer[100];
	   struct sockaddr_storage their_addr;
	   socklen_t addr_len = sizeof their_addr;
	   int bytesrecieved= recvfrom(myUDPSocket,recieverBuffer,sizeof(recieverBuffer),0,(struct sockaddr*)&their_addr,&addr_len);
	   if(bytesrecieved<0){
	       perror("ERROR on recvfrom");
	       exit(1);

	   }
	   printf("Server D has received a deploy request from the main server\n");
	   created++;
	   //printf("listener: packet is %d bytes long\n", bytesrecieved);
	   recieverBuffer[bytesrecieved] = '\0';
	   //printf("listener: packet contains \n%s", recieverBuffer);

	   int j=0;
	   bool done=false;
	   while(j<strlen(recieverBuffer)&&!done)
	   {
	   	while(recieverBuffer[j]!=' ')
	   	{
	   		userUName[j]=recieverBuffer[j];
	   		j++;

	   	}
	   	done=true;
	   }



	   if(created==1)
	   {
		fp=fopen("deployed.txt", "w");
		if(fp==NULL)
		{
			printf("Error opening deploy file!\n");
			return 1;
		}
			if(fputs("Username Filename\n", fp)==EOF)
			{
				perror("Error writing to file"); 
										    
			}
			else
			{
				fputs(recieverBuffer,fp);
				fclose(fp);
				printf("Server D has deployed the user %s's reporsitory\n",userUName);
				toSend="success";
				
	   		}
	   	}
	   	else
	   	{
	   		fp=fopen("deployed.txt", "a");
	   									if(fp==NULL)
									    {
									      fprintf(stderr,"Error opening Members file");
									      exit(1);
									    }
									    else
									    {
										    if(fputs(recieverBuffer, fp)==EOF)
										   	{
										   		perror("Error writing to deployed file"); 
											    
										   	}
										   	else{
										   		fclose(fp);
										   		//printf("deployed successfully.\n");
										   		toSend="success";
									   	}
									    }

	   	}
	   




	   




	   	struct sockaddr_in serverMaddr;
	    memset(&serverMaddr, 0, sizeof(serverMaddr));
	    serverMaddr.sin_family = AF_INET;
	    serverMaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	    serverMaddr.sin_port = htons(MPORT);

	    



	    //Code from Beej
	    int sendStatus=sendto(myUDPSocket,toSend,strlen(toSend),0,(struct sockaddr *) &serverMaddr,sizeof(serverMaddr));
	    if(sendStatus<0){
	      fprintf(stderr, "error creating socket to D: %s\n", gai_strerror(sendStatus));
	       exit(1);
	    }
	    //printf("Server D has finished sending the response to the main server\n");
	 }
	return 0;
}
