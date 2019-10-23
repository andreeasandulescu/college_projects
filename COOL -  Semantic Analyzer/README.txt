//Sandulescu Andreea-Bianca, 342C3
	Avand in vedere posibilitatea existentei de utilizari anticipate pentru clase, atribute
si metode, in tema parcurg arborele AST obtinut in tema 1 de 2 ori(o data cu vizitarea
arborelui antlr si crearea arborelui AST si a doua oara in compiler).
	Astfel, dupa ce se viziteaza nodurile AST corespunzatoare claselor, in visitProgram
are loc crearea de scope-ului global(din SymbolTable), care se va popula cu numele 
claselor valide(metoda populateScope).Se creeaza apoi scope-uri pentru fiecare clasa
(un scope pentru atribute si unul pt variabile, din moment ce un atribut si o metoda
pot avea acelasi nume) si pentru fiecare feature al clasei(atribute si metode).
	Fiecarei clase adaug atributul self, cu tipul = numele clasei.
	Metodele createScope primesc in general scope-ul parinte, pentru a putea pastra 
o referinta la acesta, insa initial, toate clasele au ca parinte scope-ul global.
Pentru clasele ce mostenesc alte clase, parintele va fi setat (prin parinte 
ma refer la scope-ul parinte) in metodele setParAttrScope si setParMethScope, 
deoarece este nevoie ca nodurile AST sa poata accesa atat scope-ul de metode, cat
si cel de variabile.
	Campul parentList din clasa Symbol va contine toti parintii unei clase, pentru a
putea testa compatibilitatea tipurilor.(Metoda checkCompatTypes).
	In metoda checkParentScope se verifica cicluri de mostenire, deoarece aici cu
siguranta toate numele de clase definite se pot gasi in scope-ul global.
	Verificarea de erori are loc atat in metodele CreateScope cat si in alte metode.
	Metoda checkDefType verifica daca tipul nodului AST este definit.
	In clasa Symbol table, am creat simboli pentru clasele de baza, le-am setat lista
de parinti si i-am adaugat in scope-ul global.
	Pentru usurinta, am adaugat o referinta la nodul AST corespunzator in clasa Symbol.
	Fiecare nod AST are o metoda getReturnType, necesara pt a doua parcurgere.
Ea intoarce tipul corespunzator nodului, arunca erori(daca e cazul), primind 2 scope-uri
(pentru atribute si metode).Tot in aceasta metoda se apeleaza getReturnType pe nodurile
(copii) continute in clasa de tip nod AST.
	Pentru case, fac intersectia listei parintilor nodurilor parametru(corespunzatoare
branch-urilor) si intorc cel mai specific tip.(finalParamParents.retainAll(newParamParents)).
La fel si pt constructia if.
	Pentru blocuri de instructiuni, getReturnType intoarce tipul ultimei 
expresii(children.get(size - 1).getReturnType(attrScope, methScope)).
Pentru atributele corespunzatoare:
	- constructiei case, getReturnType intoarce tipul expresiei
	- constructiei let si clasei, getReturnType, se verifica tipul expresiei si tipul
	  declarat al atributului (daca exista), iar daca sunt compatibile, se intoarce
	  tipul declarat.
