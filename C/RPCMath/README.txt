//Sandulescu Andreea-Bianca, 342C3

In server, pentru fiecare cerere se apeleaza functia compute_1_svc,
ce intoarce un pointer la o structura response.Campul is_prime va
avea valoarea 0 pt numere neprime, impare, 1 pentru nr prime, impare
si -1 pentru numerele pare.
In functie de valoarea campului, generez output-ul in client.
In functia compute_1_svc, exista 2 cazuri:
-numar impar, pentru care se apeleaza is_prime
-numar par, pentru care se apeleaza get_operands.

Functia is_prime intoarce 1 pt numar prim, 0 altfel.
Avand in vedere ca orice numar prim se poate scrie ca 6k+1 sau 6k-1,
o imbunatatire a verificarii primalitatii unui numar este sa verificam
daca numarul este divizibil cu 2, 3 sau toate numerele de forma 6k+1, 
6k-1 <= sqrt(numar).

Functia get_operands intoarce cei doi termeni ai caror suma este egala 
cu parametrul primit.Avand in vedere ca va primi ca parametru numai nr
pare, impart numarul la 2, iar apoi scad/adun 1 celor 2 termeni, pana
cand obtin 2 numere prime.Pentru a mai scuti din calcule, scad/adun 2
termenilor pentru a evita verificarea primalitatii numerelor pare.
Nu am mai adaugat un caz de exceptie pt cifra 4, deoarece, considerand
1 numar prim, get_operands(4) va intoarce ca termeni 1 si 3, nu 2 si 2. 