======================================
Name-Shravya Shashidhar
ID-2763825542
======================================

Completed the optional part(Extra credit section)
==================================================
Ubuntu version- 16.04
==================================================
Code files-
	serverM.c- Main server handling all the requests from the client 
	serverM.h- header file for serverM
	serverA.c- Server for authentication
	serverA.h- Header file for ServerA
	serverR.c- Respository server having the filenames
	serverR.h- Header file for ServerR
	serverD.c- Deployment server to deploy the files 
	serverD.h- Header file for ServerD
	client.c- Client program
	client.h- Header for client


========================================
Format of messages-
Client start command- ./client username password
On user login-
	ServerM sends the request to serverA for authentication
	ServerA displays the messages for authentication or an error message if incorrect
	ServerM sends the response to the client 
On lookup-
	ServerM sends the request to serverR for lookup
	ServerR sends the filenames for the lookup
	ServerM sends the response to the client
On push-
	ServerM sends the push request to serverR
	ServerR checks if it exists and asks for confirmation from the client through the serverM, if it does not exist it pushes the file 
	ServerM sends the response to the client 
On remove
	ServerM sends the remove request to serverR
	ServerR sends confirmation once it is removed to serverM
	ServerM sends the response to the client 
On deploy
	ServerM sends the lookup request to check for files to deploy
	ServerR sends the list to serverM
	ServerM sends the list to serverD for deployment 
	ServerD sends confirmation to serverM on deployment 
	ServerM sends the response to the client


Along with the on-screen messages to be displayed the following messages are added-
	If the request is not valid request from the client- "Invalid request—--Start a new request—--"
	If no files found for deployement- "The client received the response from the main server using TCP over port <portno>, and no files were found for deployment"
	If filename is not specified for removing- "Error: Filename is required. Please specify a filename to remove"
	Array of size 1024 to save the logs. 
	When the main server sent the remove request to server R- "The main server has sent the remove request to server R."
	When the main server recieves confirmation of deployment from serverD- "The main server has received the deploy response from server D using UDP over <UDP prt No>."
	After push and remove serverR confirmation after sending the response to serverM- "Server R has finished sending the response to the main server"

========================================
