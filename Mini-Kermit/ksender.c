#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib.h"
#include "klib.h"

#define HOST "127.0.0.1"
#define PORT 10000
#define TO_READ 245

int main(int argc, char** argv) {
    msg * t = NULL, * y = NULL;                 // t este mesajul trimis, y cel primit
    SendInitPack data;                          // pachet in care se pastreaza campul DATA al pachetului de tip Send-Init
    int fd = 0, i, nbytes = 0;
    char * buff, seq = 0;
    buff = calloc(250, sizeof(char));
    t = calloc(1, sizeof(msg));
    MiniKermit * send_k = calloc(1, sizeof(MiniKermit));
    send_k->DATA = calloc(250, sizeof(char));

    init(HOST, PORT);
    initData(&data);                            // initializarea campului DATA
    y = sendAndReceive(t, y, send_k, (char *) &data, sizeof(SendInitPack), 'S');
    if(y == NULL)
        return -1;
    for( i = 1; i < argc ; i++)
    {
        y = sendAndReceive(t, y, send_k, argv[i],  strlen(argv[i]) + 1, 'F');
        if(y == NULL)
            return -1;
        fd = open(argv[i], O_RDONLY);
        if (fd == -1)
            printf("%s : %s\n", "Nu s-a putut deschide fisierul", argv[i]);
        if(fd > 0)
            nbytes = 1;
        while(nbytes > 0)
        {
            nbytes = read(fd, buff, TO_READ);
            if(nbytes == -1)
                printf("%s\n", "Eroare la citirea din fisier!" );
            if(nbytes != 0)
            {
                y = sendAndReceive(t, y, send_k, buff, nbytes, 'D');
                if(y == NULL)
                   return -1;
            }
        }
        y = sendAndReceive(t, y, send_k, NULL, 0, 'Z');
        if(y == NULL)
            return -1;
        close(fd);
    }
        y = sendAndReceive(t, y, send_k, NULL, 0, 'B');
        if(y == NULL)
            return -1;
    return 0;
}