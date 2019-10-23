//Sandulescu Andreea-Bianca, 331CB

Tabela este reprezentata ca o lista de coloane si contine un hashtable pentru a putea gasi indicele unei coloane 
daca avem numele acesteia.
Pentru cele 3 metode, select, insert si update exista 2 cazuri:
1.Are loc o tranzactie, caz in care nu se mai incearca achizitionarea semaforului ce reprezinta resursa(tabela),
deoarece acesta a fost deja achizitionat in startTransaction
2.Nu are loc o tranzactie si se foloseste paradigma cititor-scriitor.

Pentru insert, doar verific daca este valid si apoi inserez in fiecare coloana elementul corespunzator din values.

Pentru update, verific daca numarul de linii ce trebuie inlocuit este > numWorkerThreads * 3, iar daca da, fac
submit la numWorkerThreads noi instante ale clasei UpdateOp, astfel incat fiecare va primi un interval(cei doi indici,
startInd si finInd), in care trebuie sa updateze valorile, adica un numar de linii in care sa faca scrierile.
Daca nu este adevarata conditia nr linii > numWorkerThrds * 3, fac submit unei singure instante, ce va updata 
toate liniile ce respecta conditia primita ca parametru. 

Pentru select, folosesc clasa ConditionOp, a carei metode call intoarce un ArrayList<Integer>(indicii liniilor ce
respecta conditia data ca parametru).Din nou, daca se respecta conditia nr linii de inlocuit > numWThrds * 3, 
atunci instantiez numWorkerThreads obiecte ConditionOp, ce vor primi cate 
(lungimea ArrayListului de indici / numWThrds) linii pe care vor face verificarile, iar daca nu, o singura instanta
ce va primi toate liniile.
Apoi, daca este cazul, se instantiaza numWThrds obiecte SelectOp, ce primesc ArrayListurile de indici ce respecta
conditia si intorc un ArrayList<ArrayList<Object>> cu rezultatul partial(doar pt liniile respective).
Daca nu, se creeaza o singura instanta, ce va primi intreg ArrayListul de indici.
In final, creez o singura instanca a clasei SelectResult, ce trebuie sa imbine rezultatele intoarse de
instanta/instantele clasei SelectOp, ce reprezinta rezultatul final ce trebuie intors de select.

Pentru startTransaction/endTransaction:
Daca are loc o tranzactie, accesul pe acea tabela este restrictionat de un reentrantLock.In momentul in care un user
doreste sa efectueze o operatie de insert/select/update pe o tabela si exista deja o tranzactie pe aceasta, userul va trebui
sa achizitioneze mai intai reentrantLockul.Daca acel user este cel care a pornit tranzactia, va avea acces imediat la 
lock, iar daca nu, acesta va astepta pana cand se va apela unlock pe reentrantlock.

Mentionez ca primesc eroarea GC overhead limit exceeded la testul testScalability() pentru 10_000_000 iteratii, insa pentru 
1_000_000 este ok.
In Netbeans, ProfileProject->Telemetry, am observat ca GarbageCollectorul foloseste mai mult de 60% de CPU pentru select(),
motiv pentru care timpii pentru mai multe threaduri sunt afectati.
Timpii obtinuti pe masina locala(laptop cu I5-7200U) pentru 1_000_000 iteratii in testScalability():

Insert time 2094
[[-1456759948]]
Update time 21
Select time 179
There are now 2 Threads
[[-1456759948]]
Insert time 3075
Update time 8
Select time 158
There are now 4 Threads
Insert time 1210
Update time 8
Select time 59

