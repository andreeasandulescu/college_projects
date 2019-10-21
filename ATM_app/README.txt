Sandulescu Andreea-Bianca, 321CB

Mentionez ca am pornit de la codul din laborator.

Pentru server:
Am creat structura user, cu campurile necesare pt identificarea acestora si cateva campuri aditionale. 
(locked = 1 pt cardurile blocate, in campul wrong_credentials este memorat numarul de incercari esuate
de a introduce pinul corect, socket = socketul TCP pe care s-a conectat clientul, sesiune_deschisa = 1
daca exista o sesiune deschisa cu succes pt acest client).
Dupa citirea datelor din fisier, am creat un socket de tip TCP, caruia i-am asociat portul si adresa IP
primite primite ca argumente, la rularea serverului.(am folosit functia setsockopt pentru a putea refolosi
portul si imediat dupa inchiderea conexiunii si a procesului).Apoi am apelat functia listen pentru a putea
primi MAX_CLIENTS cereri de conectare la socket.Am creat de asemenea si un socket UDP, legat la acelasi
port si aceeasi adresa IP.
Apoi am adaugat in multimea read_fds descriptorii pentru citire(descriptorul asociat STDIN si cei 2 socketi),
calculand de asemenea valoarea maxima.Am mai creat o copie a multimii read_fds(tmp_fds), ce va fi modificata
de apelul select.Iterez prin descriptorii pe care s-au primit date(verific daca apartin multimii tmp_fds,
tratand 4 cazuri:
1)s-au primit date de la stdin(comanda quit).Serverul trimite mesajul "Server closing" si apoi inchide 
fiecare socket din multimea tmp_fds.
2)s-au primit date pe socketul pe care se face listen, asa ca apelez functia accept, ce intoarce un nou 
socket pentru noua conexiune stabilita.Se adauga socketul in multimea de descriptori de citire si se 
actualizeaza val max(daca e cazul).
3)s-au primit date pe socket-ul UDP, asa ca apelez functia recvfrom pentru a primi mesajul de la client.
Daca mesajul contine comanda unlock, apelez functia getUserByNrCard, ce va intoarce un pointer catre 
structura ce contine informatiile userului pentru care se cere unlock(daca acesta exista in fisierul de 
intrare pt server).Daca se indeplinesc conditiile pt deblocare, se cere parola secreta.
Daca mesajul primit nu contine comanda unlock, inseamna ca s-a primit parola secreta.(clientul a trimis
numarul cardului si parola, deci este nevoie sa caut din nou clientul dupa nr de card).
4)s-au primit date pe unul din socketii prin care am stabilit o conexiune cu un client, asa ca se 
apeleaza recv.Daca am primit comanda de login, apelez functia cu acelasi nume, ce cauta userul dupa nr 
de card si verifica daca acesta poate incerca sa se logheze.Se verifica daca nr de introduceri de pin gresite 
sunt consecutive astfel:Daca userul nu a introdus pin-ul corect, se incremeneaza nr de incercari 
gresite(wrong_credentials) cu 1 doar daca este prima incercare gresita sau una din cele 2 ce pot urma dupa 
aceasta.Altfel, ii resetez valoarea la 1(nu este prima incercare gresita in sesiunea deschisa, dar este prima
intr-un posibil sir de incercari consecutive gresite).Daca se constata ca este a 3-a incercare consecutiva,
se blocheaza cardul.In variabila last_nr_card se memoreaza nrcard al ultimului user cu care s-a incercat login
(pt a verifica daca incercarile sunt consecutive).Nu am implementat ideea cu 3 incercari gresite per cont, 
deoarece nu asa se mentiona in enunt.Pentru listsold, getmoney si putmoney, caut userul dupa socketul pe care 
s-a conectat cu succes clientul si apoi tratez cazurile prezentate in cerinta temei.Pentru logout, verific 
daca pe socketul respectiv s-a conectat cu succes un utilizator.

Pentru client:
Creez cei doi socketi, apelez connect pentru cel TCP, aflu pid-ul procesului pentru fisierul de log si apoi 
continui sa citesc de la tastatura pana cand primesc comanda quit de la stdin sau pana cand nu mai primesc 
mesaje de la sender(recv intoarce 0).Citesc mesajul de la stdin si trimit toata linia serverului pentru prelucrare,
iar in urma primirii mesajelor de la server, scriu rezultatele in fisierul de log.Pentru unlock am trimis 
serverului mesajele dupa cum a fost precizat in enunt(“unlock <numar card>” si “<numar card> <parola secreta>”).