#include "helpers.h"

void usageClient(char*file)
{
	fprintf(stderr,"Usage: %s <IP_server> <port_server> \n",file);
	exit(0);
}

void usageServer(char *file)
{
	fprintf(stderr,"Usage: %s ./server <port_server> <users_data_file>\n",file);
	exit(0);
}

void aloc(user *u)
{
	u->nume = calloc(LENNUME, sizeof(char));
	u->prenume = calloc(LENNUME, sizeof(char));
	u->parola = calloc(LENPAROLA, sizeof(char));
	u->pin = 0;
	u->nrcard = 0;
	u->sold = 0;
	u->locked = 0;
	u->sesiune_deschisa = 0;
	u->wrong_credentials = 0;
	u->socket = 0;
	u->sesiune_deschisa = 0;
}

char* getWord(char *line, int nr)		//intoarce al nr-lea cuvant dintr-o linie
{		
	int i, start = 0, stop = 0;
	char * word = (char *)calloc(20, sizeof(char));
	nr = nr - 1;
	
	//gaseste inceputul cuvantului
	for (i = 0; i < strlen(line) && nr != 0; i++)
	{
		if (line[i] == ' ')
		{
			nr = nr - 1;
			start = i + 1;
		}
	}
	//gaseste finalul cuvantului
	for (; line[i] != '\n' && line[i] != '\0' && line[i] != ' '; i++)
		;
	stop = i;
	memcpy(word, line + start, (stop - start));
	return word;
}

user * getUserBySocket(user *usersData, int nrusers, int socketcli)
{
	int i=0, found = 0;
	for (i = 0; i < nrusers; i++)				//caut prin vectorul de useri pe cel care are asignat socket-ul socketcli
	{
		if(usersData[i].socket == socketcli)
		{
			found = 1;
			break;
		}
	}
	if(found == 0)								
	{
		printf("ATM> −4 : Numar card inexistent\n");
		return NULL;		
	}
	return &usersData[i];
}

user * getUserByNrCard(char *line, user *usersData, int nrusers, int nrcard, int unlock)
{
	int i=0, found = 0;
	for (i = 0; i < nrusers; i++)				//caut prin vectorul de useri userul cu nr de card nrcard
	{
		if(usersData[i].nrcard == nrcard)
		{
			found = 1;
			break;
		}
	}
	if(found == 0)								
	{
		memset(line,0,BUFFLEN);
		if(unlock == 0)							//variabila unlock este folosita pentru mesajele de eroare 
			sprintf(line, "ATM> −4 : Numar card inexistent\n");			//unlock = 0 inseamna ca se apeleaza fctia pt login
		else
			sprintf(line, "UNLOCK> −4 : Numar card inexistent\n");		///unlock = 1 inseamna ca se apeleaza fctia pt unlock
		return NULL;		
	}
	return &usersData[i];
}

user* readUsersData(char * filename, int * nrusers)	//citirea userilor din fisier
{
	FILE *fp;
	int n = 0, i = 0;
	user *usersData = NULL, u;
	usersData = calloc(MAX_CLIENTS, sizeof(user));
	fp = fopen (filename, "r");

	fscanf(fp, "%d", &n);
	*nrusers = n;
	for(i = 0; i < n; i++)
	{
		aloc(&u);
		fscanf(fp, "%s %s %d %d %s %lf", u.nume, u.prenume, &u.nrcard, &u.pin, u.parola, &u.sold );
		usersData[i] = u;
	}
	return usersData;
}