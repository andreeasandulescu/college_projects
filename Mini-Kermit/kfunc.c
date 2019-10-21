#include "lib.h"
#include "klib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXL_K 250
#define TIME_K 5
#define NPAD_K 0x00
#define PADC_K 0x00
#define EOL_K  0x0D
#define QCTL_K 0x00
#define QBIN_K 0x00
#define CHKT_K 0x00
#define REPT_K 0x00
#define CAPA_K 0x00
#define R_K 0x00
#define SOH_K 0x01
#define MARK_K 0x0D

static char senderSeq = 0;              // SEQ number initial pentru sender
static char receiverSeq = 1;            // SEQ number initial pentru receiver

msg * check_recv(int timeout)           // functie pentru asteptarea mesajului initial de la sender
{         
    msg * r = NULL;
    r = receive_message_timeout(timeout);
    if (r != NULL)
    {
        printf("[%s] Got msg from ksender", "./kreceiver");
    }
    return r;
}

void initData(SendInitPack * data)
{
    data->MAXL = MAXL_K;
    data->TIME = TIME_K;
    data->NPAD = NPAD_K;
    data->PADC = PADC_K;
    data->EOL = EOL_K;
    data->QCTL = QCTL_K;
    data->QBIN = QBIN_K;
    data->CHKT = CHKT_K;
    data->REPT = REPT_K;
    data->CAPA = CAPA_K;
    data->R = R_K;
    return;
}

void kermitToMsg ( MiniKermit * k, char * payload, int dataSize)    //copierea campurilor din pachetul de tip Kermit in payload
{   
       int len = 4 * sizeof(char);
       unsigned short check = 0;
       memcpy(payload, k, len);                                 //copierea campurilor SOH, LEN, SEQ si TYPE
       memcpy(payload + len , k->DATA, dataSize);               //copierea campului DATA
       len += dataSize;         
       check = crc16_ccitt(payload , len);
       memcpy(payload + len, &check , sizeof(short));          //copierea campului CHECK
       len +=  sizeof(short); 
       memcpy(payload + len, &(k->MARK) , sizeof(char));        //copierea campului MARK
}

void initKermit(MiniKermit * k, char * data, int dataSize, char seq, char type)     //initializarea unui pachet de tip Kermit
{
    k->SOH = SOH_K;
    k->LEN = sizeof(char) * 3 + sizeof(short) + dataSize;
    k->SEQ = seq;
    k->TYPE = type;
    if(data != NULL)
        memcpy(k->DATA, data , dataSize);
    k->MARK = MARK_K;
}

bool msgToKermit(char * payload, MiniKermit * k, int dataSize)      //copierea campurilor din payload in pachetul de tip Kermit
{                                                                   //returneaza true daca valoarea din campul CHECK e validata
    unsigned short crc = 0;
    int len = 4 * sizeof(char) + dataSize;
    memcpy(&crc, payload + len , sizeof(short));
    if(crc16_ccitt(payload, len) != crc )
        return false;
    len = 4 * sizeof(char);
    memcpy(k, payload , len);                     //copierea campurilor SOH, LEN, SEQ si TYPE
    memcpy(k->DATA, payload + len , dataSize );     //copierea campului DATA
    len += dataSize;
    memcpy(&(k->CHECK), payload + len, sizeof(short));  //copierea campului CHECK
    len += sizeof(short);
    memcpy(&(k->MARK), payload + len ,  sizeof(char));   //copierea campului MARK
    return true;
}

char getSeq(char * payload)                 //functie ce intoarce SEQ number pentru un payload
{                
    char c;
    memcpy(&c, payload + 2 * sizeof(char), sizeof(char));
    return c;
}

void setSeq(char * payload, char seq)   //functie ce actualizeaza SEQ number pentru un payload
{
    char c;
    payload[2] = seq;   
}

void setCheck(char * payload)           //functie ce actualizeaza campul CHECK pentru un payload
{
    int intLen = 0;
    unsigned short crc = 0;
    char len = payload[1];
   
    memcpy(&intLen, &len, sizeof(char));
    intLen = intLen + 2 - 3;
    crc = crc16_ccitt(payload, intLen);
    memcpy(payload + intLen, &crc , sizeof(short));
    // return;
}

msg* ifTimeout(msg * t)            //functie ce asteapta de max 3 ori primirea unui pachet si retransmite de max 3 ori un pachet
{
    int i;
    msg * y;
    for(i = 0; i < 3; i++)
    {
        y = receive_message_timeout(TIME_K * 1000);
        if (y == NULL)
            send_message(t); 
        else
             break;
    }
    return y;
}

MiniKermit* receiveAndSend(msg* r, msg* t, MiniKermit *recv_k, MiniKermit *send_k, char seq)
{                                       //functie ce primeste un pachet si trimite NAK (ACK se trimite doar in main)
    int dataSize = 0;
    char wrongSize = 0;
    bool check_ok;
    char type;
    r = ifTimeout(t);
    if(r == NULL)
        return NULL;
    if (getSeq(r->payload) != (receiverSeq + 1) % 64) {      //daca am primit un mesaj cu SEQ diferit de cel asteptat
        return receiveAndSend( r, t, recv_k, send_k, seq);
    }
    while(1){
        wrongSize = r->payload[1] - 3 * sizeof(char) - sizeof(short);     //wrongSize in sensul ca lungimea nu intra pe un char
        memcpy(&dataSize, &wrongSize, sizeof(char));
        check_ok = msgToKermit(r->payload, recv_k, dataSize); 
        if( check_ok == true ) 
            type = 'Y'; 
        else
            type = 'N';
        initKermit(send_k, NULL , 0 , seq, type);
        kermitToMsg (send_k, t->payload, 0);
        
        receiverSeq = (receiverSeq + 2) % 64;               //SEQ pachetului trimis este SEQ ultimului pachet trimis + 2
        setSeq(t->payload, receiverSeq);
        setCheck(t->payload);
        t->len = strlen(t->payload);
        if(send_k->TYPE == 'Y') {                       //intorc pachetul primit si trimit ACK din main
            return recv_k;
        } else {
            send_message(t);                            //altfel, retrimit NAK pana cand se intrerupe comunicatia sau primesc ACK
            r = ifTimeout(t);
        }
        if(r == NULL)
            return NULL;
        if (getSeq(r->payload) != (receiverSeq + 1) % 64) {
            return receiveAndSend( r, t, recv_k, send_k, seq);
        }
    }
    return NULL;
}


msg * sendAndReceive(msg* t, msg* r, MiniKermit *send_k , char * data,  int dataSize, char dataType)
{
    char type, oldSeq;
    initKermit(send_k, data, dataSize, senderSeq, dataType);
    kermitToMsg(send_k, t->payload, dataSize);
    t->len = sizeof(char) * 2 + send_k->LEN;
    // setSeq(t->payload, senderSeq);
    setCheck(t->payload);
    send_message(t);

    r = ifTimeout(t);                        //nu se permite trimiterea pachetului de mai mult de 3 ori in caz de timeout
    if( r == NULL)
        return NULL;
    if (getSeq(r->payload) != (senderSeq + 1) % 64) {
        return sendAndReceive(t, r, send_k ,  data, dataSize, dataType);
    }
    memcpy(&type, r->payload + 3 * sizeof(char), sizeof(char));
    while(type == 'N'){
        senderSeq = (senderSeq + 2) % 64;
        setSeq(t->payload, senderSeq);
        setCheck(t->payload);
        send_message(t);

        r = ifTimeout(t);                        //nu se permite trimiterea pachetului de mai mult de 3 ori in caz de timeout
        if( r == NULL)
            return NULL;
        if (getSeq(r->payload) != (senderSeq + 1) % 64) {
            return sendAndReceive(t, r, send_k ,  data, dataSize, dataType);
        }
        memcpy(&type, r->payload + 3 * sizeof(char), sizeof(char));
    }
    if(type == 'Y') {
        senderSeq = (senderSeq + 2) % 64;
        return r;
    }

}