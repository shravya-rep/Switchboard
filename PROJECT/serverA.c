#include <stdio.h>
#include <stdlib.h>
#include "serverA.h"
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

#define PORT 21542
#define UDP "21542"
#define MPORT 24542

void dispStartUpMsgServerA(){
   printf("Server A is up and running using UDP on port %d\n",PORT);
}

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


int authenticateUser(char *checkBf){
   FILE *fp;
   char user[100];
   fp=fopen("members.txt", "r");
   if(fp==NULL)
   {
      fprintf(stderr,"Error opening Members file");
      exit(1);
   }
   int found=0;
   int count=0;
   while(fgets(user,sizeof(user),fp)&&!found){
      count++;
      if(count==1){
         continue;
      }
      int j=0;
      bool valid=true;
      while(checkBf[j]!='\0'&&valid)
      {
         if(user[j]!=checkBf[j])
         {
            valid=false;
         }
         j++;

      }
      if(valid==true)
      {
         found=1;
      }
   }

   return found;


}

//Code from Beej
void connectToM(int A_UDP_Socket, char* authRes){

   char recieverBuffer[100];
   struct sockaddr_storage their_addr;
   socklen_t addr_len = sizeof their_addr;
   int bytesrecieved= recvfrom(A_UDP_Socket,recieverBuffer,sizeof(recieverBuffer),0,(struct sockaddr*)&their_addr,&addr_len);
   if(bytesrecieved<0){
       perror("ERROR on recvfrom");
       exit(1);

   }
   //printf("listener: packet is %d bytes long\n", bytesrecieved);
   recieverBuffer[bytesrecieved] = '\0';
   //printf("listener: packet contains \"%s\"\n", recieverBuffer);

   char userName[50];
   int j=0;
   while(recieverBuffer[j]!=' ')
   {
      userName[j]=recieverBuffer[j];
      j++;

   }
   userName[j]='\0';
   printf("ServerA received username %s and password ******\n",userName);
   int fUser=authenticateUser(recieverBuffer);

   char* valid="Valid user";
   char* notvalid="Invalid user";
   if(fUser==1){
      printf("Member with %s has been authenticated\n",userName);
      strcpy(authRes,valid);
   }
   else{
      printf("The username %s or password ****** is incorrect\n",userName);
      strcpy(authRes,notvalid);
   }

      struct sockaddr_in serverMaddr;
    memset(&serverMaddr, 0, sizeof(serverMaddr));
    serverMaddr.sin_family = AF_INET;
    serverMaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverMaddr.sin_port = htons(MPORT);


    int sendStatus=sendto(A_UDP_Socket,authRes,strlen(authRes),0,(struct sockaddr *) &serverMaddr,sizeof(serverMaddr));
    if(sendStatus<0){
      fprintf(stderr, "error creating socket to A: %s\n", gai_strerror(sendStatus));
       exit(1);
    }







   //socket to recieve ends 

}


int main(){
   dispStartUpMsgServerA();
   int myUDPSocket=createUDPSocket();
   while(1){
      char authSend[10];
      connectToM(myUDPSocket,authSend);

   }


   return 0;
}
