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

#define PORT 22542
#define UDP "22542"
#define MPORT 24542

void dispStartUpMsgServerR(){
	printf("Server R is up and running using UDP on port %d\n",PORT);
}

//Code form Beej
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

void collectTheFileNames(char* recieverBuffer, char* authRes){
	
   	return;
   	}   	


void connectToM(int A_UDP_Socket, char* authSend){
  

   











   //socket to recieve ends 
   return;

}


int main(){
	dispStartUpMsgServerR();
	int myUDPSocket=createUDPSocket();
   while(1){
   	  char authSend[100];
   	  memset(authSend,'\0',100);

   	char recieverBuffer[100];
   struct sockaddr_storage their_addr;
   socklen_t addr_len = sizeof their_addr;
   int bytesrecieved= recvfrom(myUDPSocket,recieverBuffer,sizeof(recieverBuffer),0,(struct sockaddr*)&their_addr,&addr_len);
   if(bytesrecieved<0){
       perror("ERROR on recvfrom");
       exit(1);

   }
   //printf("listener: packet is %d bytes long\n", bytesrecieved);
   recieverBuffer[bytesrecieved] = '\0';
   //printf("listener: packet contains \"%s\"\n", recieverBuffer);
   char * toSend;

   char* reMove="remove";
   int rm=strncmp(recieverBuffer,reMove,6);

   char *push="push";
   int p=strncmp(recieverBuffer,push,4);

   int yes=strncmp(recieverBuffer,"y",1);
   int no=strncmp(recieverBuffer,"n",1);

   char *deploy="deploy";
   int dep=strncmp(recieverBuffer,deploy,6);

   if(dep==0)
   {
   	printf("Server R has received a deploy request from the main server.\n");

   								char userUName[50];
   								memset(userUName,'\0',sizeof(userUName));
								

 								char space_1=' ';
								char *ptr = strchr(recieverBuffer, space_1);
								 if (ptr != NULL) {
								 	ptr++;
								 	for(int j=0;j<strlen(recieverBuffer);j++){
								 		userUName[j]=*ptr++;
								 		if(*ptr==' '){break;}
								 	}
								 	
								 }

								 //printf("The username is %s\n",userUName);


   								FILE *fp;
							   char user[100];
							   fp=fopen("filenames.txt", "r");
							   if(fp==NULL)
							   {
							      fprintf(stderr,"Error opening Members file");
							      exit(1);
							   }
							   int count=0;
							   int k=0;
							     while(fgets(user,sizeof(user),fp)!=NULL){
							     	
							     	int cm=strncmp(user,userUName,strlen(userUName));
							     	if(cm==0){
							     		count++;
							     		int len=strlen(user);
							     		int j;
							     		for(j=0;j<len;j++)
							     		{
							     			authSend[k]=user[j];
							     			k++;
							     		}
							     			

							     	}


							   	}
							   	//printf("%s",authSend);
							    	    if(count<=0){
										toSend="Nodeploy";
							    		}
							    		else{
							    	    toSend=authSend;
							    		//printf("%s",toSend);

							    		}

							    
   }
   else if(yes==0)
   {
   	printf("User requested overwrite; overwrite successful\n");
   	toSend="owsuccess";
   }
   else if(no==0)
   {
   	printf("Overwrite denied\n");
   	toSend="owfail";

   }
   else if(p==0)
   {
   	printf("Server R has received a push request from the main server.\n");
   								char userUName[50];
							   	char fileName[50];
   								memset(userUName,'\0',sizeof(userUName));
								memset(fileName,'\0',sizeof(fileName));

 								char space_1=' ';
								char *ptr = strchr(recieverBuffer, space_1);
								 if (ptr != NULL) {
								 	ptr++;
								 	for(int j=0;j<strlen(recieverBuffer);j++){
								 		fileName[j]=*ptr++;
								 		if(*ptr==' '){break;}
								 	}
								 	ptr++;
								 	for(int j=0;j<strlen(recieverBuffer);j++){
								 		userUName[j]=*ptr++;
								 		if(*ptr=='\0'){break;}
								 	}

								 	
								 }
								  //printf("The filename is %s\n",fileName);
								 //printf("The username is %s\n",userUName);

								 char lineToCheck[100];
								memset(lineToCheck,'\0',sizeof(lineToCheck));
								int index;
								for(index=0;index<strlen(userUName);index++)
								{
									lineToCheck[index]=userUName[index];

								}
								lineToCheck[index]=' ';
								for(int z=0;z<strlen(fileName);z++)
								{
									lineToCheck[index+1+z]=fileName[z];
								}

								//printf("The line to check is %s\n",lineToCheck);

								FILE *fp;
							   char user[100];
							   fp=fopen("filenames.txt", "r");
							   if(fp==NULL)
							   {
							      fprintf(stderr,"Error opening Members file");
							      exit(1);
							   }
							   else
							   {
							   	int found=false;
							   	while(fgets(user,sizeof(user),fp)!=NULL&&!found)
							    {
							    	int n=strncmp(user,lineToCheck,strlen(lineToCheck));
							    	if(n==0)
							    	{
							    		printf("%s exists in %s's repository; requesting overwrite confirmation.\n",fileName,userUName);
							    		found=true;
							    	}
							    }
							    fclose(fp);

							    if(!found)
							    {
									    lineToCheck[strlen(lineToCheck)]='\n';
									    //printf("the line being inserted is %s\n", lineToCheck);

									    fp=fopen("filenames.txt", "a");
									    if(fp==NULL)
									   {
									      fprintf(stderr,"Error opening Members file");
									      exit(1);
									   }
									   else
									   {
									   	if(fputs(lineToCheck, fp)==EOF)
									   	{
									   		perror("Error writing to file"); 
										    
									   	}
									   	else{
									   		fclose(fp);
									   		printf("%s uploaded successfully.\n",fileName);
									   		toSend="owsuccess";
									   	}

									   }

							    }
							    else
							    {
							    	toSend="overwrite question";


							    }
							    



							   }



   }
   else if (rm==0)
   {
							   	printf("Server R has received a remove request from the main server\n");
							   	//printf("%s\n",recieverBuffer);

							   	char userUName[50];
							   	char fileName[50];
   								memset(userUName,'\0',sizeof(userUName));
								memset(fileName,'\0',sizeof(fileName));

 								char space_1=' ';
								char *ptr = strchr(recieverBuffer, space_1);
								 if (ptr != NULL) {
								 	ptr++;
								 	for(int j=0;j<strlen(recieverBuffer);j++){
								 		fileName[j]=*ptr++;
								 		if(*ptr==' '){break;}
								 	}
								 	ptr++;
								 	for(int j=0;j<strlen(recieverBuffer);j++){
								 		userUName[j]=*ptr++;
								 		if(*ptr=='\0'){break;}
								 	}

								 	
								 }
								  //printf("The filename is %s\n",fileName);
								 //printf("The username is %s\n",userUName);

								char lineToDelete[100];
								memset(lineToDelete,'\0',sizeof(lineToDelete));
								int index;
								for(index=0;index<strlen(userUName);index++)
								{
									lineToDelete[index]=userUName[index];

								}
								lineToDelete[index]=' ';
								for(int z=0;z<strlen(fileName);z++)
								{
									lineToDelete[index+1+z]=fileName[z];
								}

								//printf("The line to delete is %s\n",lineToDelete);



								FILE *fp,*tempFile;
								char user[100];

								fp=fopen("filenames.txt", "r");
								tempFile=fopen("tempStoreNeeded","w");
								if(fp==NULL||tempFile==NULL)
							   {
							      fprintf(stderr,"Error opening files");
							      exit(1);
							   }


							    while(fgets(user,sizeof(user),fp)!=NULL)
							    {
							    	int n=strncmp(user,lineToDelete,strlen(lineToDelete));
							    	if(n==0)
							    	{
							    		continue;
							    	}
							    	else
							    	{
							    		fputs(user,tempFile);

							    	}
							    }

							   fclose(fp);
							   fclose(tempFile);

							   remove("filenames.txt");
							   rename("tempStoreNeeded","filenames.txt");

							    toSend="removed";










							   	

   }
   else
   {


  

							   	printf("Server R has received a lookup request from the main server.\n");

							   FILE *fp;
							   char user[100];
							   fp=fopen("filenames.txt", "r");
							   if(fp==NULL)
							   {
							      fprintf(stderr,"Error opening Members file");
							      exit(1);
							   }
							   int k=0;
							   int count=0;
							   int empty=0;
							     while(fgets(user,sizeof(user),fp)!=NULL){
							     	empty++;
							     	
							     	int cm=strncmp(user,recieverBuffer,strlen(recieverBuffer));
							     	if(cm==0){
							     		count++;
							     		
							     		//printf("Found it \n");
							     		int j=strlen(recieverBuffer)+1;
							     		while(user[j]!='\0'){
							     			authSend[k]=user[j];
							     			j++;
							     			k++;
							     		}
							     		

							     	}


							   	}
							   	//printf("%s",authSend);
								if(empty==0)
								{
							    	toSend="Empty";
							    }
							    else{
							    	    if(count<=0){
										toSend="No";
							    		}
							    		else{
							    	    toSend=authSend;
							    		//printf("%s",toSend);

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
      fprintf(stderr, "error creating socket to A: %s\n", gai_strerror(sendStatus));
       exit(1);
    }
    printf("Server R has finished sending the response to the main server\n");


	
}
return 0;
}
