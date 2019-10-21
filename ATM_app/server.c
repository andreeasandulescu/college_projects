#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "helpers.h"

int last_nr_card = -1;						//acesta este indicele user-ului din vector care a introdus gresit credentialele

char * login(char *line, user *usersData, int nrusers, int socketcli)
{
	int linelen = 0, nrcard = 0, pin = 0;
	user *u = NULL;
	linelen = strlen(line);
	nrcard = atoi(getWord(line, 2));
	pin = atoi(getWord(line, 3));

	u = getUserByNrCard(line, usersData, nrusers, nrcard, 0);			//se cauta userul cu numarul de card nrcard
	if(u == NULL)
	{
		last_nr_card = nrcard;											//se pastreaza ultimul nr de card cu care s-a incercat login
		return line;
	}
	memset(line,0,BUFFLEN);
	if(u->locked == 1)													//cardul gasit este blocat
	{
		sprintf(line, "ATM> −5 : Card blocat\n") ;
		return line;
	}
	if(u->sesiune_deschisa == 1)
	{
		sprintf(line, "ATM> −2 : Sesiune deja deschisa\n") ;
		return line;
	}
	if(u->pin != pin)													//daca pinul introdus este unul gresit
	{
		if( nrcard == last_nr_card || u->wrong_credentials == 0 )		//
		{
			u->wrong_credentials +=1;	//actualizez nr incercari gresite pt un card doar daca sunt consecutive
		}
		else
		{
			u->wrong_credentials = 1;	//altfel il resetez cu valoarea 1(nu este prima incercare in sesiunea deschisa,
		}
		if(u->wrong_credentials == 3)
		{
			u->locked = 1;
			last_nr_card = nrcard;
			sprintf(line, "ATM> −5 : Card blocat\n") ;
			return line;
		}
		last_nr_card = nrcard;
		sprintf(line, "ATM> −3 : Pin gresit\n") ;
		return line;
	}
	else
	{
		sprintf(line, "ATM> Welcome %s %s\n",u->nume, u->prenume) ;
		u->socket = socketcli;
		u->sesiune_deschisa = 1;
		last_nr_card = nrcard;
		return line;
	}
	return line;
}

double listsold(user *usersData, int nrusers, int socketcli)
{
	user *u = NULL;
	u = getUserBySocket(usersData, nrusers, socketcli);
	if(u != NULL)
		return u->sold;
	return -1;
}

int getmoney(char *line, user *usersData, int nrusers, int socketcli)
{
	int suma_retragere = atoi(getWord(line, 2));
	if( suma_retragere % 10 != 0)
		return (-9);
	user *u = NULL;
	u = getUserBySocket(usersData, nrusers, socketcli);
	if(u == NULL)
		return -1;
	if(suma_retragere > u->sold)
		return (-8);
	u->sold -= suma_retragere;
	return suma_retragere;
}

int putmoney(user *usersData, int nrusers, int socketcli, double suma_depunere)
{
	user *u = NULL;
	u = getUserBySocket(usersData, nrusers, socketcli);
	if(u == NULL)
		return -1;
	u->sold += suma_depunere;
	return 0;
}

int main(int argc,char**argv)
{
    int port; /* port to listen on */
    int newsockfd, i, err, fd, coderr, j;
	int socketUDP, socketTCP, socketSTDIN, socketmax;
	int lencliUDP, lencliTCP;
	int recvUDP, recvTCP, listennr;
	char buff[BUFFLEN]; /* message buffer */
	struct sockaddr_in serverUDP, serverTCP;
  	struct sockaddr_in clientUDP, clientTCP;
	int optval = 1;
	user * usersData = NULL ,*u = NULL;
	int nrusers = 0;
	double suma_depunere = 0;
	char **endptr;
	char aux[BUFFLEN];

	fd_set read_fds;	//multimea de citire folosita in select()
	fd_set tmp_fds;	//multime folosita temporar 
	int fdmax;		//valoare maxima file descriptor din multimea read_fds

	if (argc!=3)
		usageServer(argv[0]);
	port = atoi(argv[1]);
	fd = open(argv[2], O_RDONLY);
	usersData = readUsersData(argv[2], &nrusers);

	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	socketTCP = socket(PF_INET, SOCK_STREAM, 0);
	DIE(socketTCP < 0 , "-10 : Eroare la apel socket");

	memset(&serverTCP, 0, sizeof(serverTCP));
	serverTCP.sin_family = PF_INET;
	serverTCP.sin_port = htons(port);
	inet_aton(IP_ADDR, (struct in_addr *) &(serverTCP.sin_addr));

	setsockopt(socketTCP, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
	err = bind(socketTCP, (struct sockaddr *) &serverTCP, sizeof(serverTCP));
	DIE(err < 0, "-10 : Eroare la apel bind");

	listennr = listen(socketTCP, MAX_CLIENTS);
	DIE(listennr < 0 , "-10 : Eroare la apel bind");

	socketUDP = socket(AF_INET, SOCK_DGRAM, 0);
	DIE(socketUDP < 0 , "-10 : Eroare la apel socket");

 	memset(&serverUDP, 0, sizeof(serverUDP));
	serverUDP.sin_family = AF_INET;
	serverUDP.sin_port = htons(port);
	inet_aton(IP_ADDR, (struct in_addr *) &(serverUDP.sin_addr));

	err = bind(socketUDP, (struct sockaddr *) &serverUDP, sizeof(serverUDP));
	DIE(err < 0, "-10 : Eroare la apel bind");

	socketSTDIN = fileno(stdin);
	FD_SET(socketUDP, &read_fds);
	FD_SET(socketTCP, &read_fds);
	FD_SET(socketSTDIN, &read_fds);

	if(socketTCP > socketUDP)
		socketmax = socketTCP;
	else
		socketmax = socketUDP;
	if(socketSTDIN > socketmax)
		socketmax = socketSTDIN;

	while (1)
	{
		tmp_fds = read_fds; 
		err = select(socketmax + 1, &tmp_fds, NULL, NULL, NULL); 
		DIE(err < 0, "-10 : Eroare la apel selsect");
		for(i = 0; i <= socketmax; i++)
		{
			if (FD_ISSET(i, &tmp_fds))
			{
				if (i == socketSTDIN)
				{
					memset(buff,0, BUFFLEN);
					fgets(buff, BUFFLEN, stdin);
					buff[strcspn(buff, "\n")] = '\0';
					if(strcmp(buff, "quit") == 0)
					{
						for(i = 0; i <= socketmax; i++)				//caut prin vectorul de useri userul cu nr de card cu care se incearca login
						{
							if(FD_ISSET(i, &read_fds) && i != 0 && i!=socketSTDIN && i!= socketUDP)
							{
								memset(buff,0, BUFFLEN);
								strcpy(buff, "ATM> Server closing!");
								err = send(i, buff, strlen(buff), 0);
								DIE(err < 0 , "-10 : Eroare la apel send");
							}
						}
						for(i = 0; i <= socketmax; i++)				//caut prin vectorul de useri userul cu nr de card cu care se incearca login
						{
								close(i);
						}
						close(socketSTDIN);
						close(socketTCP);
						close(socketUDP);
					}
					return 0;

				}
				else if(i == socketTCP)
				{
					// a venit ceva pe socketul inactiv(cel cu listen) = o noua conexiune
					// actiunea serverului: accept()
					lencliTCP = sizeof(clientTCP);
					if ((newsockfd = accept(socketTCP, (struct sockaddr*)&clientTCP, (socklen_t*) &lencliTCP)) == -1)
					{
						error("-10 : Eroare la apel accept");
					} 
					else
					{
						//adaug noul socket intors de accept() la multimea descriptorilor de citire
						FD_SET(newsockfd, &read_fds);
						if (newsockfd > socketmax)
						{ 
							socketmax = newsockfd;
						}
					}
				}
				else if (i == socketUDP)
				{
					lencliUDP = sizeof(clientUDP);
					memset(buff,0, BUFFLEN);
					recvUDP = recvfrom(i, buff, BUFFLEN, 0, (struct sockaddr *) &clientUDP, &lencliUDP);
					DIE(recvUDP < 0 , "-10 : Eroare la apel recvfrom");
					if(recvUDP == 0)
						printf("Socket %d hung up\n", i);
					if(strstr(buff, "unlock"))
					{
						u = NULL;
						u = getUserByNrCard(buff, usersData, nrusers, atoi(getWord(buff, 2)), 1);
						if(u == NULL)
						{
							err = sendto(i, buff, strlen(buff), 0, (struct sockaddr *) &clientUDP, lencliUDP);
							DIE(err < 0 , "-10 : Eroare la apel sendto");
						}
						else if(u->locked == 0)
						{
							memset(buff,0, BUFFLEN);
							sprintf(buff, "UNLOCK > −6 : Operatie esuata");
							err = sendto(i, buff, strlen(buff), 0, (struct sockaddr *) &clientUDP, lencliUDP);
							DIE(err < 0 , "-10 : Eroare la apel sendto");
						}
						else
						{
							memset(buff,0, BUFFLEN);
							sprintf(buff, "UNLOCK> Trimite parola secreta");
							err = sendto(i, buff, strlen(buff), 0, (struct sockaddr *) &clientUDP, lencliUDP);
							DIE(err < 0 , "-10 : Eroare la apel sendto");
						}
					}
					else
					{
						u = getUserByNrCard(buff, usersData, nrusers, atoi(getWord(buff, 1)), 1 );
						if(u == NULL)
						{
							err = sendto(i, buff, strlen(buff), 0, (struct sockaddr *) &clientUDP, lencliUDP);
						}
						else
						{
							if(strcmp(u->parola, getWord(buff, 2))==0)
							{
								u->locked = 0;
								memset(buff,0, BUFFLEN);
								sprintf(buff, "UNLOCK> Client deblocat");
								err = sendto(i, buff, strlen(buff), 0, (struct sockaddr *) &clientUDP, lencliUDP);
								DIE(err < 0 , "-10 : Eroare la apel sendto");
							}
							else
							{
								memset(buff,0, BUFFLEN);
								sprintf(buff, "UNLOCK > −7 : Deblocare esuata");
								err = sendto(i, buff, strlen(buff), 0, (struct sockaddr *) &clientUDP, lencliUDP);
								DIE(err < 0 , "-10 : Eroare la apel sendto");
							}
						}
					}
				}
				else 
				{
					// am primit date pe unul din socketii cu care vorbesc cu clientii
					//actiunea serverului: recv()
					memset(buff,0, BUFFLEN);
					if ((recvTCP = recv(i, buff, BUFFLEN, 0)) <= 0)
					{
						if (recvTCP == 0)
						{
							//conexiunea s-a inchis
							printf("socket %d hung up\n", i);
							close(i); 
							FD_CLR(i, &read_fds); // scoatem din multimea de citire socketul pe care 
						}
						else
						{
							error("-10 : Eroare la apel recv");
						}
					} 
					else
					{ //recv intoarce >0
						if(strstr(buff, "login") != NULL)
						{
							login(buff, usersData, nrusers, i);
							err = send(i, buff, strlen(buff), 0);
							DIE(err < 0 , "-10 : Eroare la apel send");
						}
						else if(strstr(buff, "logout") != NULL)
						{
							printf("Deconectare de la bancomat!\n");
							memset(buff,0, BUFFLEN);
							strcpy(buff, "Deconectare de la bancomat!\n");
							for (j = 0; j < nrusers; j++)				
							{
								if(usersData[j].socket == i)
								{
									usersData[j].socket = 0;
									usersData[j].sesiune_deschisa = 0;
									break;
								}
							}
							err = send(i, buff, strlen(buff), 0);
							DIE(err < 0 , "-10 : Eroare la apel send");
						}
						else if(strstr(buff, "listsold") != NULL)
						{
							memset(buff,0, BUFFLEN);
							sprintf(buff, "ATM> %.2lf", listsold(usersData, nrusers, i) ) ;
							err = send(i, buff, strlen(buff), 0);
							DIE(err < 0 , "-10 : Eroare la apel send");
						}
						else if(strstr(buff, "getmoney") != NULL)
						{
							err = getmoney(buff, usersData, nrusers, i);
							memset(buff,0, BUFFLEN);
							switch(err)
							{
								case(-9):
									sprintf(buff, "ATM> %d : Suma nu este multiplu de 10", err);
									break;
								case(-8):
									sprintf(buff, "ATM> %d : Fonduri insuficiente", err);
									break;
								default:
									sprintf(buff, "ATM> Suma %d retrasa cu succes", err);
									break;
							}
							err = send(i, buff, strlen(buff), 0);
							DIE(err < 0 , "-10 : Eroare la apel send");
						}
						else if(strstr(buff, "putmoney") != NULL)
						{
							suma_depunere = strtod(getWord(buff, 2), endptr);
							err = putmoney(usersData, nrusers, i, suma_depunere);
							memset(buff,0, BUFFLEN);
							if(err == -1)
								printf("-10 : Eroare la apel putmoney");
							if(err == 0)
								sprintf(buff, "ATM> Suma depusa cu succes");
							err = send(i, buff, strlen(buff), 0);
							DIE(err < 0 , "-10 : Eroare la apel send");
						}
						else
						{
							if(strstr(buff, "quit") != NULL)
							{
								shutdown(i,SHUT_WR);
								printf("socket %d hung up\n", i);
								FD_CLR(i, &read_fds);
							}
							else
							{
								printf("Ati introdus gresit operatia:\n");
								err = send(i, buff, strlen(buff), 0);
								DIE(err < 0 , "-10 : Eroare la apel send");
							}
						}
					}
				} 
			}
		}
     }
    close(socketSTDIN);
	close(socketTCP);
	close(socketUDP);
	return 0;
}