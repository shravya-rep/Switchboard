#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <ctype.h>

#define TCP "25542"
#define UDP "24542"

#define TCPPORT 25542
#define UDPPORT 24542
#define BACKLOG 10

#define APORT 21542
#define RPORT 22542
#define DPORT 23542

void dispStartUpMsgServerM(){
	printf("Server M is up and running using UDP on port %d\n",UDPPORT);
}

void processUPW(char* pw,char* encpw, char* un,char* pwd)
{
	int j=0;
	while(pw[j]!=' ')
	{
		encpw[j]=pw[j];
		un[j]=pw[j];
		j++;

	}
	un[j]='\0';
	encpw[j]=pw[j];
	j++;
	int k=0;
	while(pw[j]!='\0')
	{
		pwd[k]=pw[j];
		k++;
		if(isalpha(pw[j]))
		{
			if(islower(pw[j]))
			{
				encpw[j]=((pw[j]-'a'+3))%26+'a';

			}
			else
			{
				encpw[j]=((pw[j]-'A'+3))%26+'A';
			}
		}
		else
		{
			if(isdigit(pw[j]))
			{
				encpw[j]=((pw[j]-'0'+3))%10+'0';
			}
			else
			{
				encpw[j]=pw[j];
			}
		}
		j++;


	}
	encpw[j]=pw[j];
	pwd[k]='\0';
}

int checkifguest(char*uname, char*passwd, char*authmsg){
	char g[10]="guest";
	char notg[10]="not guest";
	int guvalid=strncmp(uname,g,5);
	int gpvalid=strncmp(passwd,g,5);
	if(guvalid==0&&gpvalid==0){
		strcpy(authmsg,g);
		return 0;
	}

		strcpy(authmsg,notg);
		return -1;
}

//code from Beej
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
	int SM_UDP_Socket ;
	for(p = ai; p != NULL; p = p->ai_next) {
	 SM_UDP_Socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
	 if (SM_UDP_Socket < 0) {
	 continue;
	}


	int yes=1;
	setsockopt(SM_UDP_Socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	if (bind(SM_UDP_Socket, p->ai_addr, p->ai_addrlen) < 0) {
	 close(SM_UDP_Socket);
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
	 return SM_UDP_Socket;

}

//code from Beej
void connectToA(int SM_UDP_Socket,char* tosend,char* recieverBuffer){
	struct sockaddr_in serverAaddr;
	memset(&serverAaddr, 0, sizeof(serverAaddr));
    serverAaddr.sin_family = AF_INET;
    serverAaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAaddr.sin_port = htons(APORT);


    //char tosend[20]="Hello from M";
    int sendStatus=sendto(SM_UDP_Socket,tosend,strlen(tosend),0,(struct sockaddr *) &serverAaddr,sizeof(serverAaddr));
    if(sendStatus<0){
    	fprintf(stderr, "error creating socket to A: %s\n", gai_strerror(sendStatus));
		 exit(1);
    }
    struct sockaddr_storage their_addr;
   socklen_t addr_len = sizeof their_addr;
   int bytesrecieved= recvfrom(SM_UDP_Socket,recieverBuffer,sizeof(recieverBuffer),0,(struct sockaddr*)&their_addr,&addr_len);
   if(bytesrecieved<0){
       perror("ERROR on recvfrom");
       exit(1);

   }
   //printf("listener: packet is %d bytes long\n", bytesrecieved);
   recieverBuffer[bytesrecieved] = '\0';
   //printf("listener: packet contains \"%s\"\n", recieverBuffer);
   printf("The main server has received the response from server A using UDP over %d\n",UDPPORT);


}
//Code from Beej

void connectToR(int SM_UDP_Socket,char* tosend,char* fileArray){
	struct sockaddr_in serverAaddr;
	memset(&serverAaddr, 0, sizeof(serverAaddr));
    serverAaddr.sin_family = AF_INET;
    serverAaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAaddr.sin_port = htons(RPORT);


    //char tosend[20]="Hello from M";
    int sendStatus=sendto(SM_UDP_Socket,tosend,strlen(tosend),0,(struct sockaddr *) &serverAaddr,sizeof(serverAaddr));
    if(sendStatus<0){
    	fprintf(stderr, "error creating socket to R: %s\n", gai_strerror(sendStatus));
		 exit(1);
    }
    int p=strncmp(tosend,"push",4);
    int confy=strncmp(tosend,"y",1);
    int confn=strncmp(tosend,"n",1);
    int removeconf=strncmp(tosend,"remove",6);
    if(confy==0||confn==0)
    {
    	printf("The main server has sent the overwrite confirmation response to server R.\n");
    }
    else if(p==0)
    {
    	printf("The main server has sent the push request to server R.\n");

    }
    else if(removeconf==0)
    {
    	printf("The main server has sent the remove request to server R.\n");
    }
    else
    {
    	printf("The main server has sent the lookup request to server R.\n");

    }
    


    struct sockaddr_storage their_addr;
   socklen_t addr_len = sizeof their_addr;
   int bytesrecieved= recvfrom(SM_UDP_Socket,fileArray,sizeof(fileArray)*100,0,(struct sockaddr*)&their_addr,&addr_len);
   if(bytesrecieved<0){
       perror("ERROR on recvfrom");
       exit(1);

   }
   //printf("listener: packet is %d bytes long\n", bytesrecieved);
   fileArray[bytesrecieved] = '\0';
   //printf("listener: packet contains\n%s", fileArray);
   //printf("The main server has received the response from server R using UDP over %d\n",UDPPORT);



}

//Code from Beej
void connectToD(int SM_UDP_Socket,char* tosend,char* fileArray){
	struct sockaddr_in serverAaddr;
	memset(&serverAaddr, 0, sizeof(serverAaddr));
    serverAaddr.sin_family = AF_INET;
    serverAaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAaddr.sin_port = htons(DPORT);


    //char tosend[20]="Hello from M";
    int sendStatus=sendto(SM_UDP_Socket,tosend,strlen(tosend),0,(struct sockaddr *) &serverAaddr,sizeof(serverAaddr));
    if(sendStatus<0){
    	fprintf(stderr, "error creating socket to R: %s\n", gai_strerror(sendStatus));
		 exit(1);
    }
    else
    {
    	printf("The main server has sent the deploy request to server D\n");
    }



    struct sockaddr_storage their_addr;
   socklen_t addr_len = sizeof their_addr;
   int bytesrecieved= recvfrom(SM_UDP_Socket,fileArray,sizeof(fileArray)*100,0,(struct sockaddr*)&their_addr,&addr_len);
   if(bytesrecieved<0){
       perror("ERROR on recvfrom");
       exit(1);

   }
   //printf("listener: packet is %d bytes long\n", bytesrecieved);
   fileArray[bytesrecieved] = '\0';
  // printf("listener: packet contains\n%s", fileArray);
   printf("The main server has received the deploy response from server D using UDP over %d\n",UDPPORT);



}


//Code from Beej

void *get_in_addr(struct sockaddr *sa)
{
if (sa->sa_family == AF_INET) {
	return &(((struct sockaddr_in*)sa)->sin_addr);
	 }

return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//Code from Beej
int getSocketandBind(){
	struct addrinfo hints, *ai, *p;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int rv,listener;
	if ((rv = getaddrinfo("127.0.0.1", TCP, &hints, &ai)) != 0) {
		fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
		exit(1);
	}

	for(p = ai; p != NULL; p = p->ai_next) {
	 listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
	 if (listener < 0) {
	 continue;
	}


	int yes=1;
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
	 close(listener);
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
	 return listener;

}





int main(void)
{

	dispStartUpMsgServerM();


	//getting a socket and binding to it code from Beej
	int listener=getSocketandBind();
	int myUDPSocket=createUDPSocket();
	//printf("%d",listener);

	fd_set master; 
	fd_set read_fds; 
	FD_ZERO(&master); 
 	FD_ZERO(&read_fds);

    int fdmax;
    int newfd; 
    int i;

    if (listen(listener, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	FD_SET(listener, &master);


	fdmax = listener; 
	struct sockaddr_storage remoteaddr; 
    socklen_t addrlen;

    //char remoteIP[INET6_ADDRSTRLEN];
    int firsttime=0;


	for(;;) {
		read_fds = master;
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
 			perror("select");
 			exit(4);
 			}

 			for(i = 0; i <= fdmax; i++) 
 			{
 				if (FD_ISSET(i, &read_fds)) 
 				{ // we got one!!
	 				if (i == listener) 
	 				{
		 				// handle new connections
		 				addrlen = sizeof remoteaddr;
		 				newfd = accept(listener,(struct sockaddr *)&remoteaddr,&addrlen);
		 				if (newfd == -1) {
					 		perror("accept");
					 		} else 
					 		{
					 			FD_SET(newfd, &master); // add to master set
					 			if (newfd > fdmax) { // keep track of the max
					 				fdmax = newfd;
					 				}
					  			//printf("selectserver: new connection from %s on ""socket %d\n",inet_ntop(remoteaddr.ss_family,get_in_addr((struct sockaddr*)&remoteaddr),remoteIP, INET6_ADDRSTRLEN),newfd);
					 		}
				 	}
				 	else 
				 	{
					 	// handle data from a client
					 	//printf("Entering here\n");
					 	firsttime++;
					 	char recvBuffer[100];
					 	int nbytes;
						if((nbytes =recv(i,recvBuffer,100,0))<=0){
							continue;
						}
						//printf("data recived at M from Client : %s\n",recvBuffer);
						char userUName[50];
						char guestUName[50];
						char fileName[50];
						char logData[50];
						memset(logData,'\0',sizeof(logData));
						memset(guestUName,'\0',sizeof(guestUName));
						memset(userUName,'\0',sizeof(userUName));
						memset(fileName,'\0',sizeof(fileName));
						char *line="guest lookup";
						char *userline="lookup";
						char *remove="remove";
						char *push="push";
						char *yes="y";
						char *no="n";
						char *deploy="deploy";
						char *log="log";
						int lg=strncmp(recvBuffer,log,3);
						int dep=strncmp(recvBuffer,deploy,6);
						int p=strncmp(recvBuffer,push,4);
						int ok=strncmp(recvBuffer,yes,1);
						int notok=strncmp(recvBuffer,no,1);

						int n=strncmp(recvBuffer,line,12);
						int m=strncmp(recvBuffer,userline,6);

						int rem=strncmp(recvBuffer,remove,6);




						if(firsttime==1)
						{
							FILE* logfp=fopen("loghistory","w");
							if(logfp==NULL)
							{
								printf("Error opening a log file\n");
								return 1;
							}
							else
							{
								if(fputs("Username Action\n", logfp)==EOF)
								{
									perror("Error writing to into logfile"); 

								}
								else
								{
									fclose(logfp);
								}

							}

						}

						if(lg==0)
						{



									char space_1=' ';
									char *ptr = strchr(recvBuffer, space_1);
									 if (ptr != NULL) {
									 	ptr++;
									 	for(int j=0;j<strlen(recvBuffer);j++){
									 		userUName[j]=*ptr++;
									 		if(*ptr==' '){break;}
									 	}
									 }
									 printf("The main server has received a log request from member %s over TCP port %d\n",userUName,TCPPORT);

									 //tolog
									char op5[10]="log";
									int j;
									for(j=0;j<strlen(userUName);j++)
									{
										logData[j]=userUName[j];

									}
									logData[j]=' ';
									j++;
									for(int k=0;k<strlen(op5);k++)
									{
										logData[j]=op5[k];
										j++;
									}
									logData[j]='\n';
									FILE* logfp=fopen("loghistory","a");
									if(logfp==NULL)
									{
										printf("Error appending to a log file\n");
										return 1;

									}
									else
									{
										fputs(logData,logfp);
										fclose(logfp);
									}
									//tolog

									 char user[100];
									 FILE* logfpread=fopen("loghistory","r");
									 if(logfpread==NULL)
									{
										printf("Error appending to a log file\n");
										return 1;

									}
									else
									{
										char fileArray[1024];
										memset(fileArray,'\0',sizeof(fileArray));
										int k=0;
										 while(fgets(user,sizeof(user),logfpread)!=NULL)
										 {
										 	int cm=strncmp(user,userUName,strlen(userUName));
										 	if(cm==0)
										 	{
										 		int j=0;
										 		while(user[j]!=' ')
										 		{
										 			j++;

										 		}
										 		j++;
										 		
										 		while(user[j]!='\0')
										 		{
										 			fileArray[k]=user[j];
										 			k++;
										 			j++;

										 		}
										 	}
										 }
										 fclose(logfpread);
										 //printf("The log data is \n%s",fileArray);
										 int sendStatus=send(i,fileArray,1024,0);
										 	if(sendStatus<0)
											{
												fprintf(stderr, "error sending msg from serverM to client: %s\n", gai_strerror(sendStatus));
												exit(1);

											}
											else
											{
												printf("The main server has sent the response to the client\n");

											}


									}
									 continue;


						}


						if(dep==0)
						{
									char space_1=' ';
									char *ptr = strchr(recvBuffer, space_1);
									 if (ptr != NULL) {
									 	ptr++;
									 	for(int j=0;j<strlen(recvBuffer);j++){
									 		userUName[j]=*ptr++;
									 		if(*ptr==' '){break;}
									 	}
									 }
									 printf("The main server has received a deploy request from member %s over TCP port %d\n",userUName,TCPPORT);
									char fileArray[100];
									connectToR(myUDPSocket,recvBuffer,fileArray);
									printf("The main server received the lookup response from server R.\n");
									int d=strncmp(fileArray,"Nodeploy",8);
									if(d!=0)
									{
										char depRes[100];
										connectToD(myUDPSocket,fileArray,depRes);
										int succDep=strncmp(depRes,"success",7);
										if(succDep==0)
										{
											printf("The user %s's repository has been deployed at server D\n",userUName);

										}

									}
									int sendStatus=send(i,fileArray,100,0);
									if(sendStatus<0)
											{
												fprintf(stderr, "error sending msg from serverM to client: %s\n", gai_strerror(sendStatus));
												exit(1);

											}


									//tolog
									char op1[10]="deploy";
									int j;
									for(j=0;j<strlen(userUName);j++)
									{
										logData[j]=userUName[j];

									}
									logData[j]=' ';
									j++;
									for(int k=0;k<strlen(op1);k++)
									{
										logData[j]=op1[k];
										j++;
									}
									logData[j]='\n';
									FILE* logfp=fopen("loghistory","a");
									if(logfp==NULL)
									{
										printf("Error appending to a log file\n");
										return 1;

									}
									else
									{
										fputs(logData,logfp);
										fclose(logfp);
									}
									

									continue;

						}

						if(ok==0||notok==0)
						{
									char space_1=' ';
									char *ptr = strchr(recvBuffer, space_1);
									 if (ptr != NULL) {
									 	ptr++;
									 	for(int j=0;j<strlen(recvBuffer);j++){
									 		userUName[j]=*ptr++;
									 		if(*ptr==' '){break;}
									 	}
									 	ptr++;
									 	for(int j=0;j<strlen(recvBuffer);j++){
									 		fileName[j]=*ptr++;
									 		if(*ptr=='\0'){break;}
									 	}

									 	
									 }
							printf("The main server has received the overwrite confirmation response from %s using TCP over PORT %d\n",userUName,TCPPORT);
							char fileArray[100];
							connectToR(myUDPSocket,recvBuffer,fileArray);
							int ows=strncmp(fileArray,"owsuccess",9);
							int owf=strncmp(fileArray,"owfail",6);
							if(ows==0||owf==0)
							{
								printf("The main server has received the response from server R using UDP over %d\n",UDPPORT);
									int sendStatus=send(i,fileArray,100,0);
									if(sendStatus<0)
									{
										fprintf(stderr, "error sending msg from serverM to client: %s\n", gai_strerror(sendStatus));
										exit(1);

									}
									else
									{
										printf("The main server has sent the response to the client\n");

									}

							}


							continue;
						}

						if(n==0||m==0)
						{
							if(n==0){
									int k;
									for(k=13;k<strlen(recvBuffer);k++){
										guestUName[k-13]=recvBuffer[k];
									}
									guestUName[k]='\0';
									//printf("the look up name is %s\n",guestUName);
									printf("The main server has received a lookup request from Guest to lookup %s's repository using TCP over port %d\n",guestUName,TCPPORT);

									char fileArray[100];
									connectToR(myUDPSocket,guestUName,fileArray);
									printf("The main server has received the response from server R using UDP over %d\n",UDPPORT);
									int sendStatus=send(i,fileArray,100,0);
									if(sendStatus<0)
									{
										fprintf(stderr, "error sending msg from serverM to client: %s\n", gai_strerror(sendStatus));
										exit(1);

									}
									else
									{
										printf("The main server has sent the response to the client\n");

									}
								
									continue;
							

							}
							else{

								char space_1=' ';
								char *ptr = strchr(recvBuffer, space_1);
								 if (ptr != NULL) {
								 	ptr++;
								 	for(int j=0;j<strlen(recvBuffer);j++){
								 		guestUName[j]=*ptr++;
								 		if(*ptr==' '){break;}
								 	}
								 	ptr++;
								 	for(int j=0;j<strlen(recvBuffer);j++){
								 		userUName[j]=*ptr++;
								 		if(*ptr=='\0'){break;}
								 	}

								 	
								 }

								 //printf("The GU is %s\n",guestUName);
								 //printf("The GU is %s\n",userUName);
								 printf("The main server has received a lookup request from %s to lookup %s's repository using TCP over port %d\n",userUName,guestUName,TCPPORT);

								char fileArray[100];
								connectToR(myUDPSocket,guestUName,fileArray);
								printf("The main server has received the response from server R using UDP over %d\n",UDPPORT);
								int sendStatus=send(i,fileArray,100,0);
									if(sendStatus<0)
									{
										fprintf(stderr, "error sending msg from serverM to client: %s\n", gai_strerror(sendStatus));
										exit(1);

									}
									else
									{
										printf("The main server has sent the response to the client.\n");
									}

									//tolog
									char op2[50]="lookup";

									
									int j;
									for(j=0;j<strlen(userUName);j++)
									{
										logData[j]=userUName[j];

									}
									//printf("The log data after username is %s\n",logData);
									logData[j]=' ';
									j++;
									for(int k=0;k<strlen(op2);k++)
									{
										logData[j]=op2[k];
										j++;
									}
									logData[j]=' ';
									j++;
									//printf("The log data after op is %s\n",logData);
									for(int t=0;t<strlen(guestUName);t++)
									{
										logData[j]=guestUName[t];
										j++;
									}

									logData[j]='\n';
									//printf("The log data after all is %s\n",logData);
									FILE* logfp=fopen("loghistory","a");
									if(logfp==NULL)
									{
										printf("Error appending to a log file\n");
										return 1;

									}
									else
									{
										fputs(logData,logfp);
										fclose(logfp);
									}



								 continue;

								 



							}
							


							



						}
						if(rem==0)
						{
								char space_1=' ';
								char *ptr = strchr(recvBuffer, space_1);
								 if (ptr != NULL) {
								 	ptr++;
								 	for(int j=0;j<strlen(recvBuffer);j++){
								 		fileName[j]=*ptr++;
								 		if(*ptr==' '){break;}
								 	}
								 	ptr++;
								 	for(int j=0;j<strlen(recvBuffer);j++){
								 		userUName[j]=*ptr++;
								 		if(*ptr=='\0'){break;}
								 	}

								 	
								 }
							//printf("The filename is %s\n",fileName);
							//printf("The username is %s\n",userUName);

							printf("The main server has received a remove request from member %s over TCP port %d\n",userUName,TCPPORT);
							char fileArray[100];
							connectToR(myUDPSocket,recvBuffer,fileArray);
							int n=strncmp(fileArray,"removed",7);
							if(n==0)
							{
									printf("The main server has received confirmation of the remove request done by the server R\n");
									int sendStatus=send(i,fileArray,100,0);
									if(sendStatus<0)
									{
										fprintf(stderr, "error sending msg from serverM to client: %s\n", gai_strerror(sendStatus));
										exit(1);

									}

							}

									//tolog
									char op3[10]="remove";
									int j;
									for(j=0;j<strlen(userUName);j++)
									{
										logData[j]=userUName[j];

									}
									logData[j]=' ';
									j++;
									for(int k=0;k<strlen(op3);k++)
									{
										logData[j]=op3[k];
										j++;
									}
									logData[j]=' ';
									j++;
									for(int t=0;t<strlen(fileName);t++)
									{
										logData[j]=fileName[t];
										j++;

									}
									logData[j]='\n';
									FILE* logfp=fopen("loghistory","a");
									if(logfp==NULL)
									{
										printf("Error appending to a log file\n");
										return 1;

									}
									else
									{
										fputs(logData,logfp);
										fclose(logfp);
									}


							continue;

						}

						if(p==0)
						{
								char space_1=' ';
								char *ptr = strchr(recvBuffer, space_1);
								 if (ptr != NULL) {
								 	ptr++;
								 	for(int j=0;j<strlen(recvBuffer);j++){
								 		fileName[j]=*ptr++;
								 		if(*ptr==' '){break;}
								 	}
								 	ptr++;
								 	for(int j=0;j<strlen(recvBuffer);j++){
								 		userUName[j]=*ptr++;
								 		if(*ptr=='\0'){break;}
								 	}

								 	
								 }
							//printf("Entered push\n");
							printf("The main server has received a push request from %s,using TCP over port %d\n",userUName,TCPPORT);
							char fileArray[100];
							connectToR(myUDPSocket,recvBuffer,fileArray);
							int ow=strncmp(fileArray,"overwrite question",18);
							int ows=strncmp(fileArray,"owsuccess",9);
							if(ow==0)
							{
								printf("The main server has received the response from server R using UDP over %d, asking for overwrite confirmation.\n",UDPPORT);
								int sendStatus=send(i,fileArray,100,0);
									if(sendStatus<0)
									{
										fprintf(stderr, "error sending msg from serverM to client: %s\n", gai_strerror(sendStatus));
										exit(1);

									}
									else
									{
										printf("The main server has sent the overwrite confirmation request to the client.\n");
									}

							}
							if(ows==0)
							{
								printf("The main server has received the response from server R using UDP over %d\n",UDPPORT);
									int sendStatus=send(i,fileArray,100,0);
									if(sendStatus<0)
									{
										fprintf(stderr, "error sending msg from serverM to client: %s\n", gai_strerror(sendStatus));
										exit(1);

									}


							}

									//tolog
									char op4[10]="push";
									int j;
									for(j=0;j<strlen(userUName);j++)
									{
										logData[j]=userUName[j];

									}
									logData[j]=' ';
									j++;
									for(int k=0;k<strlen(op4);k++)
									{
										logData[j]=op4[k];
										j++;
									}
									logData[j]=' ';
									j++;
									for(int t=0;t<strlen(fileName);t++)
									{
										logData[j]=fileName[t];
										j++;
									}
									logData[j]='\n';
									FILE* logfp=fopen("loghistory","a");
									if(logfp==NULL)
									{
										printf("Error appending to a log file\n");
										return 1;

									}
									else
									{
										fputs(logData,logfp);
										fclose(logfp);
									}



							continue;

						}



						char userName[50];
						char passWord[50];
						char encBuffer[100];
						char authToClient[10];
						processUPW(recvBuffer,encBuffer,userName,passWord);
						int check=checkifguest(userName,passWord,authToClient);
						if(check<0){
							printf("Server M has received username %s and password ****\n",userName);
							//printf("data after encrption : %s\n", encBuffer);
							
							printf("Server M has sent authentication request to Server A\n");
							connectToA(myUDPSocket,encBuffer,authToClient);
							int sendStatus=send(i,authToClient,10,0);
							if(sendStatus<0)
							{
								fprintf(stderr, "error sending msg from serverM to client: %s\n", gai_strerror(sendStatus));
								exit(1);

							}
							else
							{
								printf("The main server has sent the response from server A to client using TCP over port %d\n",TCPPORT);

							}
						}
						else
						{
							int sendStatus=send(i,authToClient,10,0);
							if(sendStatus<0)
								{
									fprintf(stderr, "error sending msg from serverM to client as guest auth: %s\n", gai_strerror(sendStatus));
									exit(1);

								}

						}

				}
			}

		}
	}







 
return 0;
}












