Sandulescu Andreea-Bianca, 321CB

Am declarat structura Mini-Kermit pentru pachetele cu aceeasi denumire, si structura SendInitPack, pentru a retine campul DATA al 
pachetului de tip Send-Init.

Pentru sender:
Am apelat functia initData pentru initializarea campului DATA pentru primul pachet (de tip S), apoi sendAndReceive pentru trimiterea si 
primirea cate unui mesaj.In sendAndReceive, se initializeaza pachetul Kermit cu informatiile necesare(data etc), apoi se apeleaza functia
kermitToMsg pentru copierea informatiei din Kermit in campul payload al mesajului msg.Se actualizeaza SEQ cu valoarea pastrata in campul senderSeq, iar apoi CHECK (prin intermediul functiei setCheck).Ulterior, se trimite pachetul si se asteapta un alt pachet(ACK sau NAK).
Daca nu apare timeout, se verifica SEQ-ul pachetului primit.Daca nu coincide cu cel asteptat, se reapeleaza functia, iar daca SEQ-urile
coincid, se verifica tipul pachetului primit.Daca am primit ACK, doar se actualizeaza senderSeq si se intoarce mesajul primit.
Daca am primit NAK, se actualizeaza SEQ si CHECK si se retrimite mesajul pana cand primesc timeout sau un mesaj de tip ACK.
Daca primesc ACK, se continua restul operatiilor(citire din fisier etc).


Pentru receiver:
Se asteapta de max 3 ori primul pachet fara a se retrimite un alt pachet(nu avem ce pachet sa trimitem initial din receiver), apoi
se pastreaza intr-o variabila de tip SendInitPack informatiile primite in pachetul Send-Init.Daca valoarea din campul CHECK este validata, se trimite ACK(cu SEQ 1), respectiv NAK(cu acelasi SEQ).In receiver se efectueaza operatiile "in oglinda" fata de sender.
In functie de ce tip de pachet primesc, se efectueaza operatiile necesare.