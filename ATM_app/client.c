#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include "helpers.h"

int main(int argc,char**argv)
{
    int socketUDP, socketTCP;
    int lenservUDP, lenservTCP;
    int pid = 0, port, n, err, coderr, loggedin = 0;
    struct sockaddr_in serverUDP, serverTCP;
    char *ipAddr, *filename = NULL;
    char buff[BUFFLEN], aux[BUFFLEN];
    char *lasttolog = NULL;
  
    filename = calloc(FILENAMELEN, sizeof(char));
    if(filename == NULL)
    	exit(-1);

	if (argc!=3)
		usageClient(argv[0]);

    ipAddr = argv[1];													//IP-ul serverului
    port = atoi(argv[2]);												//portul pe care se conecteaza clientul
    socketTCP = socket(PF_INET, SOCK_STREAM, 0);						//mai intai creez socket-ul TCP
    DIE(socketUDP < 0, "-10 : Eroare la apel socket pentru TCP");

	memset(&serverTCP, 0, sizeof(serverTCP));
	serverTCP.sin_family = AF_INET;
	serverTCP.sin_port = htons(port);
	err = inet_aton(ipAddr, (struct in_addr *) &(serverTCP.sin_addr));
	DIE(err < 0, "-10 : Eroare la apel inet_aton");
	
	err = connect( socketTCP, (struct sockaddr *) &serverTCP,  sizeof(struct sockaddr));	// incerc sa ma conectez la server
    DIE(err < 0, "-10 : Eroare la apel connect");

	pid = getpid();
	sprintf(filename, "client-%d.log", pid);
	FILE *fp = fopen (filename, "w+");

	socketUDP = socket(AF_INET, SOCK_DGRAM, 0);							//apoi creez si un socket UDP
    DIE(socketUDP < 0, "-10 : Eroare la apel socket pentru UDP");
	
 	memset(&serverUDP, 0, sizeof(serverUDP));
	serverUDP.sin_family = AF_INET;
	serverUDP.sin_port = htons(port);
	err = inet_aton(ipAddr, (struct in_addr *) &(serverUDP.sin_addr));
	DIE(err < 0, "-10 : Eroare la apel inet_aton");
	
    while(1)					//cat timp nu primesc comanda quit, continui sa primesc mesaje de la stdin
    {
		memset(buff, 0, BUFFLEN);
	    fgets(buff, BUFFLEN, stdin);
	    fprintf(fp, "%s", buff);
	    if(strstr(buff, "login") != NULL)
	    {
	    	lasttolog = getWord(buff, 2);			//verific daca exista deja o sesiune deschisa
	    	if(loggedin == 1)
	    	{
	    		printf("−2 : Sesiune deja deschisa\n" );
	    		fprintf(fp, "−2 : Sesiune deja deschisa\n");
	    	}
	    	else
	    	{
				err = send(socketTCP, buff, strlen(buff), 0);			//daca nu exista, atunci trimit linia citita serverului 
				DIE(err < 0, "-10 : Eroare la apel send");  
			    err = recv(socketTCP, buff, BUFFLEN, 0);
			    DIE(err < 0, "-10 : Eroare la apel recv");  
			    if(strstr(buff, "Welcome") != NULL)						//daca primesc un mesaj de la server ce contine "Welcome", 
			    	loggedin = 1;										//inseamna ca tocmai am deschis o sesiune cu un user
			    printf("%s\n", buff);
			    fprintf(fp,"%s", buff);
	    	}
	    }
	    else if(strstr(buff, "logout") != NULL)
	    {
	    	if(loggedin == 0)
	    	{
	    		printf("-1 : Clientul nu este autentificat\n");			//nu pot sa deloghez un user care nu este autentificat
	    		fprintf(fp,"-1 : Clientul nu este autentificat");	
	    	}										
	    	else
	    	{
	    		err = send(socketTCP, buff, strlen(buff), 0);
				DIE(err < 0, "-10 : Eroare la apel send");  
				memset(buff,0, BUFFLEN);
			 	err = recv(socketTCP, buff, BUFFLEN, 0);
			 	DIE(err < 0, "-10 : Eroare la apel recv");
			 	if(strcmp("Deconectare de la bancomat!\n", buff) == 0)
			 		loggedin = 0;
			 	printf("%s", buff);
			 	fprintf(fp,"%s", buff);
	    	}
	    }
	    else if(strstr(buff, "listsold") != NULL)
	    {
	    	if(loggedin == 1)										//functia listsold se poate apela doar pt un client logat
	    	{
				err = send(socketTCP, buff, strlen(buff), 0);
				DIE(err < 0, "-10 : Eroare la apel send");
				memset(buff,0, BUFFLEN);
			 	err = recv(socketTCP, buff, BUFFLEN, 0);
			 	DIE(err < 0, "-10 : Eroare la apel recv");
			 	printf("%s\n", buff);
			 	fprintf(fp,"%s", buff);
		 	}
		 	else
		 	{
		 		printf("-1 : Clientul nu este autentificat\n");
			 	fprintf(fp,"-1 : Clientul nu este autentificat");
		 	}
	    }
	    else if(strstr(buff, "getmoney") != NULL)
	    {
	    	if(loggedin == 1)
	    	{
				err = send(socketTCP, buff, strlen(buff), 0);
				DIE(err < 0, "-10 : Eroare la apel send");
				memset(buff,0, BUFFLEN);
			 	err = recv(socketTCP, buff, BUFFLEN, 0);
			 	DIE(err < 0, "-10 : Eroare la apel recv");
			 	printf("%s\n", buff);
			 	fprintf(fp,"%s", buff);
		 	}
		 	else
	 		{
		 		printf("-1 : Clientul nu este autentificat\n");
			 	fprintf(fp,"-1 : Clientul nu este autentificat");
		 	}
	    }
	    else if(strstr(buff, "putmoney") != NULL)
	    {
	    	if(loggedin == 1)
	    	{
				err = send(socketTCP, buff, strlen(buff), 0);
				DIE(err < 0, "-10 : Eroare la apel send");
				memset(buff,0, BUFFLEN);
			 	err = recv(socketTCP, buff, BUFFLEN, 0);
			 	DIE(err < 0, "-10 : Eroare la apel recv");
			 	printf("%s\n", buff);
			 	fprintf(fp,"%s", buff);
		 	}
		 	else
	 		{
		 		printf("-1 : Clientul nu este autentificat\n");
			 	fprintf(fp,"-1 : Clientul nu este autentificat");
		 	}
	    }
	    else if(strstr(buff, "unlock") != NULL)
	    {
	    	buff[strcspn(buff, "\n")] = ' ';						//inlocuiesc \n cu spatiu, pentru a putea concatena sirul "unlock"
	    	strcat(buff,lasttolog);									//cu nr de card al ultimului user pt care a fost apelata
	   		lenservUDP = sizeof(serverUDP);							//comanda login, iar sirul rezultat va fi trimis serverului
	 		err = sendto(socketUDP, buff, strlen(buff), 0, (struct sockaddr *) &serverUDP, lenservUDP);
			DIE(err < 0, "-10 : Eroare la apel sendto");
	    
		    err = recvfrom(socketUDP, buff, BUFFLEN, 0, (struct sockaddr *) &serverUDP, &lenservUDP);
		    DIE(err < 0, "-10 : Eroare la apel recvfrom");
		 	printf("%s\n", buff);
		 	fprintf(fp,"%s", buff);
		 	if(strstr(buff, "UNLOCK> Trimite parola secreta"))			//daca primesc acest mesaj, astept parola secreta de la user
		 	{
		 		memset(buff, 0, BUFFLEN);
			    fgets(buff, BUFFLEN, stdin);
			    buff[strcspn(buff, "\n")] = '\0';				//din nou inlocuiesc \n, pt a putea trimite sirul			    											
			    memset(aux, 0, BUFFLEN);						//“<numar card> <parola secreta>” serverului
			    sprintf(aux,"%s %s",lasttolog,buff);
			    printf("%s\n", aux);
		 		fprintf(fp,"%s", aux);
			    err = sendto(socketUDP, aux, strlen(aux), 0, (struct sockaddr *) &serverUDP, lenservUDP);
				DIE(err < 0, "-10 : Eroare la apel sendto");
				err = recvfrom(socketUDP, buff, BUFFLEN, 0, (struct sockaddr *) &serverUDP, &lenservUDP);
			    DIE(err < 0, "-10 : Eroare la apel recvfrom");
			 	printf("%s\n", buff);
			 	fprintf(fp,"%s", buff);
		 	}
	    }
	    else
	    {
	    	if(strstr(buff, "quit") != NULL)
			{
				err = send(socketTCP, buff, strlen(buff), 0);
				DIE(err < 0, "-10 : Eroare la apel send");	
			  	close(socketUDP);
				err =  shutdown(socketTCP, SHUT_RD);
				close(socketTCP);
				break;
			}
			else
		    {
	    		printf("-1 : Clientul nu este autentificat\n");
	   		    fprintf(fp,"-1 : Clientul nu este autentificat");
	   		    err = send(socketTCP, buff, strlen(buff), 0);
	   			DIE(err < 0, "-10 : Eroare la apel send");	
	   		    
	   		    memset(buff,0, BUFFLEN);
	   		    err = recv(socketTCP, buff, BUFFLEN, 0);
	   		    if(err == 0)
	   				{
	   					close(socketUDP);
	   					close(socketTCP);
	   					break;
	   				}
	   		    DIE(err < 0, "ERROR in recvfrom");
	   			printf("%s\n", buff);
			 	fprintf(fp,"%s", buff);
		   	}
	    }
	}
	close(fp);
	return 0;
}