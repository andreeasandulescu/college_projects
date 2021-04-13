// Sandulescu Andreea-Bianca, 331CB


Clasa Fragment reprezinta un fragment de drum(2 cuburi si un dreptungi), ce poate contine un obstacol.Astfel, verificarea coliziunii masinii se face cu fiecare fragment al drumului
verificand daca are loc coliziunea cu bordurile sau obstacolul ce apartine fiecarui fragment(Am ales ca fiecare fragment de drum sa aiba max un obstacol).Dupa coliziunea/depasirea
unui obstacol,acesta dispare de pe sosea.
Drumul este alcatuit din mai multe segmente, iar curbele sunt obtinute prin translatari stanga/dreapta.
Clasa Road contine un vector de fragmente, iar in constructorul clasei se citesc din fisierul de configuratie tipurile de drum si lungimea acestora.
Clasa Car contine un vector cu cele 4 roti si vectorul directie de care am nevoie pentru rotatia masinii.Astfel, in metoda move, in functie de tasta apasata, fie rotesc masina, fie
o translatez in functie de vectorul directie.Singurele transformari aplicate numai rotilor sunt scalarea initiala a acestora, in constructor si rotatia lor, fata de axa Ox.
In clasa Laborator6, verific coliziunea masinii cu bordurile si obstacolele, iar in cazul in care aceasta are loc, translatez masina in centrul fragmentului de drum ce se
afla cu 5 pozitii inapoi(daca se poate).
Scorul creste daca se evita un obstacol si scade daca au loc coliziuni.Acesta este scris in fisierul score.txt.Numarul maxim de coliziuni este 4.
In caz de coliziune, aliniez masina cu axa Oz, rotind-o cu unghiul facut de directie si axa Oz, respectiv -Oz.
Am creat 3 shadere, pentru iarba, pamant si cer.Am obtinut culorile prin interpolare trigonometrica, folosind o suma de sinusuri si cosinusuri, pentru ciclitate.