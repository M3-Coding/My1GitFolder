#include"header.h"
#define PORT 12345
#define MAX 80 
// Function designed for chat between client and server. 
void func(int connfd) 
{ 
	char buff[MAX];
	int n; 
	// infinite loop for chat 
	for (;;) { 
		bzero(buff, MAX); 

		// read the message from client and copy it in buffer 
		read(connfd, buff, sizeof(buff)); 
		// print buffer which contains the client contents 
		printf("From client: %s\t To client : ", buff); 
		bzero(buff, MAX); 
		n = 0; 
		// copy server message in the buffer 
		while ((buff[n++] = getchar()) != '\n'); 

		// and send that buffer to client 
		write(connfd, buff, sizeof(buff)); 

		// if msg contains "Exit" then server exit and chat ended. 
		if (strncmp("exit", buff, 4) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		} 
	} 
}

int main()
{                 
	int welcomeSocket, newSocket;
	char buffer[1514];
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t  addr_size;
	//////////////////////////////////////////////////////////////////////
	//---- Create the socket. The three arguments are: ----
	// 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) 
	welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (welcomeSocket == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	}
	else
		printf("Socket successfully created..\n"); 
//	bzero(&serverAddr,sizeof(serverAddr)); 
	//////////////////////////////////////////////////////////////////////
	/*---- Configure settings of the server address struct ----*/
	/* Address family = Internet */
	serverAddr.sin_family = AF_INET;   //IPv4 Internet protocols
	/* Set port number, using htons function to use proper byte order */
	serverAddr.sin_port = htons(PORT);
//	printf("port ------%d",htons(PORT));
	/* Set IP address to localhost */
//	serverAddr.sin_addr.s_addr = inet_addr("192.168.103.133");
//	printf("Ip ------%d",inet_addr("192.168.103.133"));
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	/* Set all bits of the padding field to 0 */
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	///////////////////////////////////////////////////////////////////////////
	/*---- Bind the address struct to the socket ----*/
	if((bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)))!=0){
		printf("Socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket Successfully binded..\n");
	//////////////////////////////////////////////////////////////////////
	/*---- Listen on the socket, with 5 max connection requests queued ----*/
	if(listen(welcomeSocket,5)!=0){
		printf("Listen Failed..\n");
		exit(0);
	}
	else
		printf("Server Listening \n");
	//////////////////////////////////////////////////////////////////////
	/*---- Accept call creates a new socket for the incoming connection ----*/
	addr_size = sizeof serverStorage;
	newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
	if(newSocket<0){
		printf("Server Accept Failed...\n");
		exit(0);
	}
	else
		printf("Server Accept The Client...\n");
	//////////////////////////////////////////////////////////////////////
	/*---- Send message to the socket of the incoming connection ----*/
	// strcpy(buffer,"Hello World\n");
	//  send(newSocket,buffer,13,0);
	////////////////////////////////////////////////////////////////////////

	// Function for chatting between client and server 
	func(newSocket); 

	// After chatting close the socket 
	close(welcomeSocket); 
}
