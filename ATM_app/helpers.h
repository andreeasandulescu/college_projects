#ifndef _HELPERS_H
#define _HELPERS_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(EXIT_FAILURE);				\
		}							\
	} while(0)
	

#define BUFFLEN 1500
#define IP_ADDR "127.0.0.1"
#define MAX_LEN_WORD 20
#define LENNUME	12
#define LENPAROLA 16
#define FILENAMELEN 50
#define MAX_CLIENTS	100

#endif

typedef struct{
	char *nume;
	char *prenume;
	char *parola;
	int pin;
	int nrcard;
	double sold;
	int locked;
	int wrong_credentials;
	int socket;
	int sesiune_deschisa;
}user;


void usageClient(char *file);
void usageServer(char *file);
void aloc(user *u);
char* getWord(char *line, int nr);
user * getUserByNrCard(char *line, user *usersData, int nrusers, int nrcard, int unlock);
user * getUserBySocket(user *usersData, int nrusers, int socketcli);
user* read_usersData(int nrcard, int pin, char * filename, int * nrusers);
user* readUsersData(char * filename, int * nrusers);