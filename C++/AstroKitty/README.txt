// Sandulescu Andreea-Bianca, 331CB


Am creat o clasa Astronaut(Astro), o clasa Asteroid, o clasa pentru Platforma si alte 3 clase ce o mostenesc pe ultima.

In clasa Astro pastrez in vectorii a,b si c varfurile triunghiului, iar in g centru de greutate al acestuia.Fac asta deoarece
nu mai pot recupera coordonatele triunghiului dupa ce trimit matricele ce trebuie inmultite cu coordonatele varfurilor 
triunghiului placii grafice pt calcul.Radius reprezinta raza cercului centrat in centrul de greutate al acestuia, iar move
este o variabila ce indica daca astronautul trebuie sa continue miscarea sau sa se opreasca(in cazul stationarii/reflexiei etc)
De asemenea, pastrez modelMatrix-ul astronautului, pentru modificarea coordonatelor punctelor si pentru cazul in care am
nevoie sa mut astronautul in pozitita initiala(sa inmultesc fiecare coordonata cu inversul lui modelMatrix).
Tot aici am implementat si metodele distanceBetween, pentru calculul distantei dintre doua puncte si angleBetween, pentru
unghiul format de doua drepte.Metoda transform primeste o matrice 3x3, cu ajutorul careia actualizeaza modelMatrixul si 
coordonatele varfurilor triunghiului.setVertices este folosit pt updatarea coordonatelor varfurilor si calcului noului 
centru de greutate, precum si al razei.
Metoda asteroidCollision intoarce 1 in caz de coliziune si 0 altfel.Verific coliziunea cu asteroidul prin compararea
sumei razelor celor 2 cercuri(cercul centrat in g-ul astronautului si cel centrat in g-ul asteroidului) cu distanta dintre
centrele celor 2 cercuri.Daca distanta dintre centre este <= cu suma razelor, atunci cele doua figuri fac coliziune.
In cazul in care sunt in coliziune, calculez unghiul dintre dreapta formata de varful a al triunghiului si centrul de g al sau
si varful a al triunghiului si centrul de greutate al asteroidului.(in ideea ca figurile se intersecteaza in varful a).
Dupa rotatia cu unghiul calculat, am rotit triunghiul cu 180 de grade si am translatat asteroidul(pentru "distrugerea" 
asteroidului).Am actualizat campul move, deoarece astronautul trebuie sa continue miscarea.
In metoda projectOnDirection, se calculeaza vectorul directie dat de cele 2 puncte, apoi il normalizez si aplic astronautului
translatie cu o viteza constanta, atat pe axa Ox, cat si Oy.
In metoda mouseMove, calculez unghiul facut de mouse(coordonatele mouse-ului in spatiul real, in cazul meu, spatiul logic/
fereastra). si g-ul triunghiului si dreapta determinata de varful a al triunghiului si g-ul sau.

Analog, si pentru Asteroid am creat metodele setVertices si Transform , deoarece si pentru acesta pastrez varfurile, 
centrul de greutate, raza si modelMatrix.Am creat 3 metode, pentru fiecare tip de asteroid(rotatie,scalare si translatie).
Pentru scalare, cresc factorul de scalare(scale) pana la o limita superioara, apoi acesta descreste pana la limita inferioara
si se reia cresterea sa.Pentru translatie, am ales cate o limita superioara si una inferioara atat pentru Ox, cat si pt Oy.

Pentru platforme, pastrez din nou varfurile, centrul de greutate si modelMatrix, asa ca am nevoie de metodele Transform si
setVertices.
Fiecare clasa face override metodei platformCollision.Toate clasele verifica la fel coliziunea cu triunghiul.
Platforma de reflexie are nevoie de unghiul de reflexie(care este 180 - 2 * unghiul format de dreapta determinata
de varful a si centrul de greutate al asteroidului si proiectia acesteia pe Ox).
Platforma de Stationare si cea finala au aceeasi implementare pentru metoda de coliziune, avand nevoie sa stie daca 
triunghiul se afla deasupra platformei sau dedesubt.Am facut o translatie cu coordonata x a pct de coliziune + (respectiv -)
jumatate din lungimea laturii bc, iar pe Oy, am folosit coordonata y a pct de coliziune(adunata cu latimea platoformei,
daca astronautul se afla deasupra acesteia).

In Laborator3_Vis2D, calzulez in metoda DrawScene coordonatele mouse-ului in spatiul de afisare(poarta), doar ca acesta va
avea axa Oy orientata invers fata de ecran(ca in cazul sistemului de axe real), pentru a putea apoi sa inmultesc cu inversul
matricii de vizualizare si sa obtin coordonatele in spatiul logic.
Verific coliziunea cu fiecare asteroid, apoi fiecare platforma, apoi daca a fost dat click, modific orientarea 
astronautului in functie de punctul indicat de mouse.Marginile ecranului sunt platforme de stationare, asa ca, indiferent 
cu care dintre margini ma intersectez, trebuie sa readuc astronautul la coordonatele intiale(sa inmultesc cu inversa lui
modelMatrix), sa il rotesc(optional) si apoi sa il translatez aproape de punctul in care a facut coliziune cu marginile
ecranului.
De fiecare data, daca variabila move are valoarea 1, astronautul se va misca pe directia dreptei determinate de varful a si
centrul de greutate.