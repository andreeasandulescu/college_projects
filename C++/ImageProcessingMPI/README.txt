// Sandulescu Andreea-Bianca, 331CB

Initial, fiecare proces MPI isi va citi lista de adiacenta din fisier.
Apoi, radacina va citi fisierul "imagini.in" si va incepe parsarea fisierelor pgm de intrare.
Dupa parsare, f (instanta clasei File) va contine matricea de pixeli, "antetul" fisierului pgm(toate liniile pana la matricea de pixeli), whitespace-urile dintre pixeli (ce trebuie scrise in fisierul 
pgm de iesire), etc.

Dupa parsare, se apeleaza functia send_to_children, pentru a trimite fiilor radacinii liniile
de pixeli, cu tag-ul filtru.In aceasta functie are loc calculul numarului de linii corespunzator
fiecarui nod fiu si transmiterea lor, urmand ca functia sa intoarca un vector ce contine numarul de
linii trimise fiecarui nod fiu.(mai intai se trimite fiului un mesaj cu numarul de linii ce urmeaza
sa fie trimise, apoi unul cu lungimea fiecarei linii, iar in final se trimit liniile).
Dupa trimiterea matricii de pixeli pe bucati catre copii se asteapta liniile prelucrate inapoi, insa
de aceasta data se asteapta cu 2 linii mai putin decat au fost trimise, deoarece frunzele vor sterge
liniile auxiliare(de granita) necesare doar pentru calcul.(functia recv_from_children).
Radacina sterge din matricea primita prima si ultima coloana(la citire, matricea va fi bordata cu 0),
apoi scrie in fisierul pgm de iesire.

Dupa ce a scris in fisier ultima imagine, radacina trimite mesajul cu tag-ul de terminare fiilor, 
iar apoi primeste de la acestia un mesaj ce contine numarul de elemente din vectorul de structuri
(Processed_lines) si in final, si vectorul cu statistici.Vectorii primiti de la fii sunt concatenati
in alt vector.La parcurgerea acestuia, se pun intr-un alt vector(nodes), pe pozitia 'rank', numarul 
de linii procesate de fiecare proces (nr_lines).Astfel, vectorul nodes, care a fost initializat cu
valoarea 0 va contine valori diferite de 0 doar pe pozitiile corespunzatoare frunzelor, deoarece
doar acestea insereaza in vectorii de structuri numarul de linii procesate.(Radacina si frunzele
proceseaza 0 linii).

Nodurile intermediare primesc de la parinti matricea de pixeli/mesajul cu tag de terminare in functia 
recv_from parent.

Daca este vorba de matricea de pixeli, dupa ce sterge din lista de adiacenta nodul parinte (measjele
ce trebuie trimise nodurilor din lista de adiacenta difera de cele ce trebuie trimise parintelui),
nodul intermediar trimite fiilor matricea primita, iar dupa ce primeste de la acestia matricea
procesata, o trimite mai departe, parintelui.
Daca se primeste tag-ul de terminare, nodul trimite fiilor mesajul cu acelasi tag, urmand sa
primeasca de la acestia vectori de structuri ce contin rank-ul frunzei si nr de linii procesata de 
aceasta.Nodul intermediar nu face decat sa concateneze vectorii primiti, astfel incat, atunci cand
parintele va concatena toti vectorii primiti de la fii, acesta sa contina nr frunze elemente.(doar
rank-urile ale caror nr de linii procesate sunt diferite de 0).
Dupa concatenarea intr-un vector de structuri (stat), nodul intermediar trimite parintelui numarul
de elemente ale acestui vector si vectorul, urmand sa isi finalizeze executia.

Frunzele primesc matricea de pixeli de la parinti, incremeneaza numarul de linii procesate, iar apoi
aplica filtrul cerut, urmand sa stearga din matrice liniile de granita primite.Apoi, frunzele trimit
parintilor matricea modificata, pana cand primesc un mesaj cu tag de terminare.In acel moment,
frunza creeaza un vector cu o structura, in care isi pune rank-ul si numarul de linii procesate,
pe care o trimite parintelui si isi finalizeaza executia.

