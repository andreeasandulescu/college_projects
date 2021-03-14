// Sandulescu Andreea-Bianca, 331CB

Clasa TuringMachines contine un HashMap (fileTM), cu toate masinile Turing din fisierul de intrare.
Clasa ElementOne reprezinta masini Turing elementare, ce se pot deplasa max o pozitie pe banda/sa scrie un element pe banda.
Clasele ElementMore si ElementMoreDiff reprezinta masini Turing Elementare, ce se pot deplasa minim o pozitie pe banda, 
pana la intalnirea unui anumit element/ cat timp simbolul de pe banda e diferit de elemment.
Trans reprezinta o masina Turing ce contine un HashMap de tranzitii, astefl incat daca exista o multime de simboli
carora le corespunde un sir de tranzitii, se vor insera intrari pentru fiecare simbol, de tipul (simbol, Complex).
Daca in definitia acelei MT multimea de simboli careia ii corespune o lista de tranzitii(o MT Complex) si multimea
este memorata cu un nume (x@{a,b}), metoda replaceAliases() din clasa Trans va inlocui tranzitia [&x] pentru 
ambele simboluri cu [a], respectiv [b].
Se adauga pe stiva instantei tm a clasei TuringMachines:
	-o MT de tip Trans, cand se intalneste "("
	-o MT de tip Complex, cand se intalneste "::=", sau "nume@" sau "->"
In restul cazurilor, se face peek sau pop pe stiva, si MT nou creata se insereaza in MT ce se afla in varful
stivei.