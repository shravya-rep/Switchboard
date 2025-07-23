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
#include "client.h"
#include <stdbool.h>

#define TCPPORTM 25542

void dispStartUpMsgClient(){
	printf("Client is up and running\n");
}

//code from Beej
int connectToM(int * portNo){
	int clientSocket;
	struct addrinfo hints, *servinfo,*p;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family =AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo("127.0.0.1","25542", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((clientSocket = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) 
		{
			perror("client: socket");
			continue;
		}
		if (connect(clientSocket, p->ai_addr, p->ai_addrlen) == -1) {
			close(clientSocket);
			perror("client: connect");
			continue;
		}

		break;
	}
		if (p == NULL) {
			fprintf(stderr, "client: failed to connect\n");
			return 2;
		}

	//getmy portno
	struct sockaddr_in myaddr;
	int addrlen=sizeof(myaddr);
	memset(&myaddr,0,sizeof(myaddr));
	myaddr.sin_family=AF_INET;
	myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	bind(clientSocket, (struct sockaddr *) &myaddr, addrlen);

	int Getsock_check=getsockname(clientSocket,(struct sockaddr*)&myaddr, (socklen_t *)&addrlen);
	if (Getsock_check== -1) {
		perror("getsockname");
		exit(1);
	}

	*portNo=ntohs((&myaddr)->sin_port);

	//printf("Client port no is");

	//printf("%d\n",ntohs((&myaddr)->sin_port));

	char ip4[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(myaddr.sin_addr), ip4, INET_ADDRSTRLEN);
	//printf("The IPv4 address is: %s\n", ip4);

	return clientSocket;

}
//Code from Beej







int main(int argc, char *argv[])
{

	if(argc<2)
	{
		dispStartUpMsgClient();
	}
	else
	{
		dispStartUpMsgClient();
		char UN[50];
		char userName[100];
		char pw[50];
		int userNAmeLen=strlen(argv[1]);
		int pwLen=strlen(argv[2]);

		strcpy(UN,argv[1]);
		strcpy(userName,argv[1]);
		strcpy(pw,argv[2]);

		userName[userNAmeLen]=' ';
		for(int i=userNAmeLen+1;i<userNAmeLen+1+pwLen;i++)
		{
			userName[i]=pw[i-(userNAmeLen+1)];
		}

		userName[userNAmeLen+1+pwLen]='\0';
		//printf("%s",userName);


		int clientTCPPortNo;
		int csfd=connectToM(&clientTCPPortNo);
		int sendStatus=send(csfd,userName,100,0);
		if(sendStatus<0){
			fprintf(stderr, "error sending msg from client to server: %s\n", gai_strerror(sendStatus));
			return 1;

		}


		char authFromServer[10];
		int recieveStatus=recv(csfd,authFromServer,10,0);
		if(recieveStatus<0){
			fprintf(stderr, "error sending msg from client to server: %s\n", gai_strerror(recieveStatus));
			exit(1);

		}

		char* guest="guest";
		char* valid="Valid user";
		int resultAuth=strncmp(authFromServer,guest,1);
		if(resultAuth==0)
		{
			printf("You have been granted guest access.\n");


			char cmdInput[50];
			memset(cmdInput,'\0',sizeof(cmdInput));
			char* lookup="lookup";
			
			bool correctg=false;
			while(!correctg)
			{
				printf("Please enter the command: <lookup <username>>\n");
				if (fgets(cmdInput, sizeof(cmdInput), stdin) != NULL) 
				{
					int gcomValidity=strncmp(cmdInput,lookup,6);
					if(gcomValidity!=0){
						printf("Guests can only use the lookup command\n");
						printf("—--Start a new request—--\n");
						continue;
					}
					if(gcomValidity==0)
					{
						if(strlen(cmdInput)==7)
						{
							printf("Error: Username is required. Please specify a username to lookup\n");
							printf("—--Start a new request—--\n");
							continue;
						}
						

						char cmdToSend[50];
						memset(cmdToSend,'\0',sizeof(cmdToSend));
						//printf("%d",strlen(cmdToSend));
						strcpy(cmdToSend,"guest ");
						//printf("%d",strlen(cmdToSend));
						int j;
						for(j=6;j<6+strlen(cmdInput)-1;j++){
							cmdToSend[j]=cmdInput[j-6];
						}
						//printf("%d",strlen(cmdToSend));
						//printf("%s",cmdToSend);

						int sendCmd=send(csfd,cmdToSend,50,0);
						//int sendCmd=send(csfd,cmdInput,50,0);
						if(sendCmd<0){
						fprintf(stderr, "error sending msg from client to server: %s\n", gai_strerror(sendCmd));
						return 1;

						}
						else
						{
							printf("Guest sent a lookup request to the main server\n");

						}
						char fileArray[100];
						int recieveStatus=recv(csfd,fileArray,100,0);
						if(recieveStatus<0){
						fprintf(stderr, "error getting msg from server %s\n", gai_strerror(recieveStatus));
						exit(1);

						}
						else
						{	
								int ifEmpty=strncmp(fileArray,"Empty",5);
								if(ifEmpty==0){
									printf("“The client received the response from the main server using TCP over port %d\n",clientTCPPortNo);
									printf("Empty repository.\n");
									printf("—--Start a new request—--\n");
									continue;
								}
								else{
										int cmpval=strncmp(fileArray,"No",2);
										if(cmpval==0){
										printf("“The client received the response from the main server using TCP over port %d\n",clientTCPPortNo);
										printf("%s does not exist. Please try again.\n",cmdToSend);
										printf("—--Start a new request—--\n");
										continue;

										}
										else{
										printf("The client received the response from the main server using TCP over port %d\n",clientTCPPortNo);
										printf("%s",fileArray);
										printf("—--Start a new request—--\n");
										continue;

										}


								}


						}


					}

			}
			else
			{
				printf("Could not read the command\n");
				continue;
			}

		}
			




		}
		else
		{
			    int resultAuth=strncmp(authFromServer,valid,1);

        		if(resultAuth==0)
        		{

        			printf("You have been granted member access.\n");
        			
					char * lookup="lookup";
					char * push="push";
					char * remove="remove";
					char * deploy="deploy";
					char* log="log";
					
        			bool correct=false;

        			while(!correct)
        			{
		        			char dispUN[50];
		        			memset(dispUN,'\0',sizeof(dispUN));
		        			char cmdInput[50];
							memset(cmdInput,'\0',sizeof(cmdInput));

		        			printf("Please enter the command:\n");
		        			printf("<lookup <username>>\n");
		        			printf("<push <filename>>\n");
		        			printf("<remove <filename>>\n");
		        			printf("<deploy>\n");
		        			printf("<log>\n");

							if (fgets(cmdInput, sizeof(cmdInput), stdin) != NULL) 
							{
								int c1=strncmp(cmdInput,lookup,6);
								bool userNameSpecified=false;
								if(c1==0)
								{
									//int lencheck=strlen(cmdInput);
									//printf("The length of cmdInput is %d",lencheck);

										if(strlen(cmdInput)>8)
										{
											userNameSpecified=true;

										}

											if(userNameSpecified)
											{

				        					
				        					cmdInput[strcspn(cmdInput, "\n")] =' ';
				        					int j=strlen(cmdInput);
				        					for(int ct=7;ct<j;ct++){
				        						dispUN[ct-7]=cmdInput[ct];
				        					}

				        					cmdInput[j]=' ';
				        					for(int k=0;k<strlen(UN);k++){
				        						cmdInput[j+k]=UN[k];
				        					}
				        					//printf("Username is specified and the The command being sent is %s\n",cmdInput);

			        			
											}
										else{
											for(int ct=0;ct<strlen(UN);ct++){
												dispUN[ct]=UN[ct];
											}
											int counter=0;
											while(counter<2){
												cmdInput[strcspn(cmdInput, "\n")] =' ';
												int j=strlen(cmdInput);
												cmdInput[j]=' ';
												for(int k=0;k<strlen(UN);k++){
													cmdInput[j+k]=UN[k];
												}
												counter++;

											}

											printf("The username is not specified. Will lookup %s\n",UN);


											}

										int sendCmd=send(csfd,cmdInput,50,0);
										if(sendCmd<0){
											fprintf(stderr, "error sending msg from client to server: %s\n", gai_strerror(sendCmd));
											return 1;
											}
											else
											{
												printf("%s sent a lookup request to the main server.\n",UN);
											}

										char fileArray[100];
										int recieveStatus=recv(csfd,fileArray,100,0);
										if(recieveStatus<0)
										{
											fprintf(stderr, "error getting msg from server %s\n", gai_strerror(recieveStatus));
											exit(1);
										}
										else{
											int ifEmpty=strncmp(fileArray,"Empty",5);
											if(ifEmpty==0)
											{
												printf("The client received the response from the main server using TCP over port %d\n",clientTCPPortNo);
												printf("Empty repository.\n");
												printf("—--Start a new request—--\n");
												continue;
											}

											else
											{
													int cmpval=strncmp(fileArray,"No",2);
													if(cmpval==0){
													printf("The client received the response from the main server using TCP over port %d\n",clientTCPPortNo);
													printf("%sdoes not exist. Please try again.\n",dispUN);
													printf("—--Start a new request—--\n");
													continue;

													}
													else{
													printf("The client received the response from the main server using TCP over port %d\n",clientTCPPortNo);
													printf("%s",fileArray);
													printf("—--Start a new request—--\n");
													continue;

													}


											}

										}

										continue;
									}
								

								int c2=strncmp(cmdInput,push,4);
								if(c2==0)
								{
									if(strlen(cmdInput)==5)
									{
										printf("Error: Filename is required. Please specify a filename to push.\n");
									}
									else
									{




											char pushfileName[50];
											memset(pushfileName,'\0',sizeof(pushfileName));
				        					cmdInput[strcspn(cmdInput, "\n")] ='\0';
					        				//int j=strlen(cmdInput);

			        						char space_1=' ';
											char *ptr = strchr(cmdInput, space_1);
										 	if (ptr != NULL) 
										 	{
											 	ptr++;
											 	for(int j=0;j<strlen(cmdInput);j++)
											 	{
											 		pushfileName[j]=*ptr++;
											 		if(*ptr==' '){break;}
											 	}
										 	}

											//printf("The filename to push is %s\n",pushfileName);

											FILE* pushFilePt;
											   pushFilePt=fopen(pushfileName, "r");
											   if(pushFilePt==NULL)
											   {
											   	printf("Error: Invalid file: %s\n",pushfileName);
											   	printf("—--Start a new request—--\n");

											   }
											   else
											   {
											   	//printf("The file is valid\n");
											   	cmdInput[strcspn(cmdInput, "\n")] ='\0';
											   	int l=strlen(cmdInput);
											   	cmdInput[l]=' ';
											   	for(int j=0;j<strlen(UN);j++)
											   	{
											   		cmdInput[l+1+j]=UN[j];

											   	}

											   	//printf("The command being sent to R tp push is %s\n",cmdInput );
											   	int sendCmd=send(csfd,cmdInput,50,0);
												if(sendCmd<0)
												{
													fprintf(stderr, "error sending msg from client to server: %s\n", gai_strerror(sendCmd));
													return 1;
												}

												char fileArray[100];
												int recieveStatus=recv(csfd,fileArray,100,0);
												if(recieveStatus<0)
												{
													fprintf(stderr, "error getting msg from server %s\n", gai_strerror(recieveStatus));
													exit(1);
												}
												else
												{
													int ow=strncmp(fileArray,"overwrite question",18);
													if(ow==0)
													{
														printf("%s exists in %s's repository, do you want to overwrite (Y/N)?\n",pushfileName,UN);
														char selection[50];
														memset(selection,'\0',sizeof(selection));
														fgets(selection, sizeof(selection), stdin);
														//char *addrs=selection;
														
														//scanf("%s",addrs);
														selection[1]=' ';
														int j;
														for(j=0;j<strlen(UN);j++)
														{
															selection[j+2]=UN[j];
														}
														selection[j+2]=' ';
														int len=strlen(selection);
														for(int k=0;k<strlen(pushfileName);k++)
														{
															selection[len+k]=pushfileName[k];
														}
														//printf("The confirmation sent is %s",selection);

														int selectSend=send(csfd,selection,50,0);
														if(selectSend<0)
														{
															fprintf(stderr, "error sending msg from client to server: %s\n", gai_strerror(sendCmd));
															return 1;

														}
														char fileArray[100];
														int recieveStatus=recv(csfd,fileArray,100,0);
														if(recieveStatus<0)
														{
															fprintf(stderr, "error getting msg from server %s\n", gai_strerror(recieveStatus));
															exit(1);
														}
														else
														{
															//printf("The data is %s\n", fileArray);
															int ows=strncmp(fileArray,"owsuccess",9);
															int owf=strncmp(fileArray,"owfail",6);
															if(ows==0)
															{
																printf("%s pushed successfully\n",pushfileName);
															}
															if(owf==0)
															{
																printf("%s was not pushed successfully.\n",pushfileName);
															}
															

														}

														




													}
													else
													{
														int ows=strncmp(fileArray,"owsuccess",9);
														if(ows==0)
														{
															printf("%s pushed successfully\n",pushfileName);
														}
														else
														{
															printf("%s was not pushed successfully.\n",pushfileName);

														}
														

													}

												}




											   }
										continue;
									}

								}



								
								int c3=strncmp(cmdInput,remove,6);
								if(c3==0)
								{
									if(strlen(cmdInput)==7)
									{
										printf("Error: Filename is required. Please specify a filename to remove.\n");
									}
									else
									{
											cmdInput[strcspn(cmdInput, "\n")] ='\0';
					        				int j=strlen(cmdInput);
					        				
					        				cmdInput[j]=' ';
					        				for(int k=0;k<strlen(UN);k++){
					        					cmdInput[k+j+1]=UN[k];
					        				}
					        				//printf("%s",cmdInput);

					        				int sStatus=send(csfd,cmdInput,50,0);
											if(sStatus<0){
												fprintf(stderr, "error sending msg from client to server: %s\n", gai_strerror(sStatus));
												return 1;
												}
											else{
												printf("%s sent a remove request to the main server\n",UN);

											}

											char fileArray[100];
											int recieveStatus=recv(csfd,fileArray,100,0);
											if(recieveStatus<0)
											{
												fprintf(stderr, "error getting msg from server %s\n", gai_strerror(recieveStatus));
												exit(1);
											}
											else
											{
												printf("The remove request was successful.\n");
												//printf("%s",fileArray);

											}
										continue;
									}

								}

								
								int c4=strncmp(cmdInput,deploy,6);
								if(c4==0)
								{
					        					cmdInput[strcspn(cmdInput, "\n")] ='\0';
						        				int j=strlen(cmdInput);
						        				cmdInput[j]=' ';
						        				for(int k=0;k<strlen(UN);k++)
						        				{
						        					cmdInput[j+1+k]=UN[k];
						        				}


												//printf("The deploy req is  %s\n",cmdInput);
												int sendCmd=send(csfd,cmdInput,50,0);
												if(sendCmd<0)
												{
													fprintf(stderr, "error sending msg from client to server: %s\n", gai_strerror(sendCmd));
													return 1;
												}
												else
												{
													printf("%s sent a deploy request to the main server.\n",UN);
												}
														char fileArray[100];
														int recieveStatus=recv(csfd,fileArray,100,0);
														if(recieveStatus<0)
														{
															fprintf(stderr, "error getting msg from server %s\n", gai_strerror(recieveStatus));
															exit(1);
														}
														else
														{
															int dStat=strncmp(fileArray,"Nodeploy",8);
															if(dStat==0)
															{
																printf("The client received the response from the main server using TCP over port %d, and no files were found for deployment\n",clientTCPPortNo);
															}
															else
															{
																	char fileName[100];
																	memset(fileName,'\0',sizeof(fileName));
																	int j=0;
																	int k=0;
																	while(j<strlen(fileArray))
																	{
																		while(fileArray[j]!=' ')
																		{
																			j++;

																		}
																		j++;
																		while(fileArray[j]!='\n')
																		{
																			fileName[k]=fileArray[j];
																			k++;
																			j++;

																		}
																		fileName[k]=fileArray[j];
																		k++;
																		j++;
																	}
																	printf("The client received the response from the main server using TCP over port %d.The following files in his/her repository have been deployed.\n%s",clientTCPPortNo,fileName);
																	printf("—--Start a new request—--\n");




																	 	
															}
																
															}

									continue;
								}
								

								int c5=strncmp(cmdInput,log,3);
								if(c5==0)
								{
									
												cmdInput[strcspn(cmdInput, "\n")] ='\0';
						        				int j=strlen(cmdInput);
						        				cmdInput[j]=' ';
						        				for(int k=0;k<strlen(UN);k++)
						        				{
						        					cmdInput[j+1+k]=UN[k];
						        				}


												//printf("The log req is  %s\n",cmdInput);
												int sendCmd=send(csfd,cmdInput,50,0);
												if(sendCmd<0)
												{
													fprintf(stderr, "error sending msg from client to server: %s\n", gai_strerror(sendCmd));
													return 1;
												}
												else
												{
													printf("%s sent a log request to the main server\n",UN);
												}
												char fileArray[1024];
												int recieveStatus=recv(csfd,fileArray,1024,0);
												if(recieveStatus<0)
												{
													fprintf(stderr, "error getting msg from server %s\n", gai_strerror(recieveStatus));
													exit(1);
												}
												else
												{
													printf("The client received the response from the main server using TCP over port %d\n",clientTCPPortNo);
													int i=0;
													int j=0;

													int count=1;
													char buff[100];
													
													while(fileArray[i]!='\0')
													{
														memset(buff,'\0',sizeof(buff));
														printf("%d. ",count);
														int k=0;
														while(fileArray[j]!='\n')
														{
															buff[k]=fileArray[j];
															k++;
															j++;

														}
														j++;
														printf("%s\n",buff);
														i=j+1;
														count++;

													}
													printf("—--Start a new request—--\n");
												}
									
									
									continue;
								}
								printf("Invalid request—--Start a new request—--\n");
							}
					}
					
        		}
        		else
        		{
        			printf("The credentials are incorrect. Please try again.\n");
        		}

		}









	






}

	return 0;
}