/*
  Lab2, CS 6027 – Advanced Computer Networking - Ahyoung Lee.
  This programming exercise is that a simple reverse-echo server and echo client using
  TCP socket. This is EchoClient which receives a message from a user and sends the
  message to the connected reverse echo server. This EchoClient-example.c was written
  based on the reference book: TCP/IP Sockets in C by Michael & Kenneth.
*/

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <netdb.h>	/* for host name*/

#define RCVBUFSIZE 32   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    char *servIP;                    /* Server IP address (dotted quad) */
    int echoServPort;                /* Echo server port */
    char echoString[RCVBUFSIZE];     /* String to send to echo server */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv()
                                        and total bytes read */
    int done = 0;

    if (argc != 3)    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> <Port Number>\n", argv[0]);
       exit(1);
    }

    servIP = argv[1];             /* Second arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]); /* Thred arg: server port number */



    /* Create a reliable, stream socket using TCP */
    /* (1) => Fill out the blank if() */
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family = AF_INET;             /* Internet address family */

    /* If argv is numbers(0~9) go to echoServAddr, else go to struct hostent;
    in this case, you need to have a if condition for checking the first input argument value should be within 0~9 numbers
    so that is for checking Server IP address*/
    /* (2) => Fill out the blank if() */
    if(argv[1][0] >= '0' && argv[1][0] <= '9')	/* If argv is numbers(0~9) go to echoServAddr, else go to struct hostent */
    {
        echoServAddr.sin_addr.s_addr = inet_addr(servIP); /* Server IP address */
    }
    else
    {
    	struct hostent *hname;
	if((hname=gethostbyname(argv[1])) == NULL)
	{
	   printf("gethostbyname failed");
	   exit(1);
	}
	echoServAddr.sin_addr = *((struct in_addr*)hname->h_addr); /* Server name of machine*/
    }
    echoServAddr.sin_port     = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    /* (3) => Fill out the blank if() */
    if (connect(sock, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");


    while (done == 0)
    {
    	scanf("%s", echoString);
    	echoStringLen = strlen(echoString); /* Determine input length */

        /* Determine input length; if EchoClient sents "fin" to server, first get the input-data "fin" using scanf(),
        second change the varialbe "done" to make no more while-loop but in this case you need to use echoString[]
        for matching input string "fin" */
        /* (4) => Fill out the blank if() and {} */
        if(strncmp(echoString, "fin", 3) == 0)
                {
                     done = 1;
                }
    	/* Send the string to the server */
   	    if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
        	DieWithError("send() sent a different number of bytes than expected");

    	/* Receive the same string back from the server */
    	totalBytesRcvd = 0;
    	printf("Received: ");                /* Setup to print the echoed string */
    	while (totalBytesRcvd < echoStringLen)
    	{
        	/* Receive up to the buffer size (minus 1 to leave space for
           	a null terminator) bytes from the sender */
        	if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
            		DieWithError("recv() failed or connection closed prematurely");
        	totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
        	echoBuffer[bytesRcvd] = '\0';  /* Terminate the string! */
        	printf(echoBuffer);            /* Print the echo buffer */
    	}

    	printf("\n");    /* Print a final linefeed */

    }
    close(sock);
    exit(0);
}

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
