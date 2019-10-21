#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"
#include "klib.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(int argc, char** argv) {
    msg *r = NULL, *t = NULL;
    MiniKermit * send_k = calloc(1, sizeof(MiniKermit));
    send_k->DATA = calloc(250, sizeof(char));
    MiniKermit * recv_k = calloc(1, sizeof(MiniKermit));
    recv_k->DATA = calloc(250, sizeof(char));
    SendInitPack initPack;
    char a = 0;
    char * filename;
    bool check_ok;
    int dataSize = 0, fd = 0, written = 0;
    
    init(HOST, PORT);
    t = (msg *)calloc(1, sizeof(msg));
    r = check_recv(5000);
    if(r == NULL)
      { 
            r = check_recv(5000);
            if(r == NULL)
            { 
                r = check_recv(5000);
                if (r == NULL)
                return -1;
            }
    }
    check_ok = msgToKermit(r->payload, recv_k, sizeof(SendInitPack)); 
    memcpy(&initPack, recv_k->DATA, 11 * sizeof(char));
    if( check_ok == true )                                  //se trimite ACK pt primul pachet
    { 
        a = recv_k->LEN - 3 * sizeof(char) - sizeof(short);
        memcpy(&dataSize, &a ,sizeof(char));
        initKermit(send_k, recv_k->DATA , dataSize , 1, 'Y');
        kermitToMsg (send_k, t->payload, dataSize);
    }
    else                                             //se trimite NAK pt primul pachet
    {
        initKermit(send_k, NULL , 0 , 1, 'N');
        kermitToMsg (send_k, t->payload, 0);
    }
    t->len = strlen(t->payload);
    send_message(t);
    while(1){
        recv_k = receiveAndSend(r, t, recv_k, send_k, 2);
        if(recv_k == NULL)
            return -1;
        if(recv_k->TYPE == 'F')
        {
            filename = calloc( 100, sizeof(char));
            strcat(filename, "recv_");
            strcat(filename, recv_k->DATA);
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            if (fd == -1)
                printf("%s : %s\n", "Nu s-a putut creea fisierul", filename);
            free(filename);
        }
        if(recv_k->TYPE == 'D')
        {
           a = recv_k->LEN - 3 * sizeof(char) - sizeof(short);
           memcpy(&dataSize, &a,sizeof(char));
            written = write(fd, recv_k->DATA, dataSize);
            if(written < 0)
                printf("Eroare la scriere\n");
        }
        if(recv_k->TYPE == 'Z')
        {
            close(fd);
        }
        if(recv_k->TYPE == 'B')
        {
            send_message(t);
            return 0;
        }
        send_message(t);
    }
    return 0;
}
