#define bool int
#define true 1
#define false 0

typedef struct {            //pachet Mini-Kermit
    char SOH;
    char LEN;
    char SEQ;
    char TYPE;
    char * DATA;
    short CHECK;
    char MARK;
} MiniKermit;

typedef struct
{
    char MAXL;
    char TIME;
    char NPAD;
    char PADC;
    char EOL;
    char QCTL;
    char QBIN;
    char CHKT;
    char REPT;
    char CAPA;
    char R;

}SendInitPack;

msg * check_recv(int timeout);
void initData(SendInitPack * data);
void kermitToMsg (MiniKermit *k, char * payload, int dataSize);
void initKermit(MiniKermit * k, char * data, int dataSize, char seq, char type);
bool msgToKermit(char * payload, MiniKermit * k, int dataSize);
char getSeq(char * payload);
void setSeq(char * payload, char seq);
void setCheck(char * payload);
msg * ifTimeout(msg * t);
MiniKermit * receiveAndSend(msg* r, msg* t, MiniKermit *recv_k, MiniKermit *send_k, char seq);
msg * sendAndReceive(msg* t, msg* r, MiniKermit *send_k, char * data,  int dataSize, char dataType);