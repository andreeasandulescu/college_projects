// Sandulescu Andreea-Bianca, 331CB

Pentru serpi, am creat o noua structura (NewSnake), ce contine un vector(snakeVector), in care tin minte configuratia fiecarui sarpe.Am ales sa pastrez
pozitia fiecarei bucati din sarpe pentru a ma asigura ca voi stii mereu care este coada sarpelui(pentru a o sterge, daca nu apar coliziuni).

De exemplu, pentru configuratia initiala a hartii(capul se afla la (2,5), iar coada (3,0)) si directia V
0 0 0 0 0 0 0 		 0 0 0 0 0 0 0 
0 0 0 0 0 0 0		 0 0 0 0 0 0 0 
0 0 0 0 0 1 1        0 0 0 0 0[1]1
1 1 1 1 0 0 0		 1 1 1 1 0 0 0 
0 0 0 1 0 0 0        0 0 0 1 0 0 0
0 0 0 1 0 0 0        0 0 0 1 0 0 0 
0 0 0 1 0 0 0		 0 0 0<1>0 0 0
,dupa 3 pasi se va ajunge la 
0 0 0 0 0 0 0 		 0 0 0 0 0 0 0 
0 0 0 0 0 0 0		 0 0 0 0 0 0 0 
0 0 1 1 1 1 1        0 0[1]1 1 1 1
1 1 1 1 0 0 0		 1 1 1<1>0 0 0 
0 0 0 0 0 0 0        0 0 0 0 0 0 0
0 0 0 0 0 0 0        0 0 0 0 0 0 0 
0 0 0 0 0 0 0		 0 0 0 0 0 0 0
,unde capul nu mai are un singur vecin, pentru a fi usor de detectat.

Astfel, voi avea nevoie de o functie(snakes_to_entireSnakes) in care sa se copieze informatia utila din vectorul de structuri de serpi deja 
existent(struct snake *snakes) in vectorul nou de serpi si in care sa stochez intraga configuratie a sarpelui(struct coord *snakeVector).
Aici, se copiaza in paralel, pentru fiecare sarpe codificarea, directia si coordonatele capului si se aloca noul vector pentru configuratia sarpelui(initial, de lunigme 1).Se sterg apoi capetele serpilor de pe harta, pentru citirea celorlalte parti ale acestuia
astfel: pentru fiecare sarpe, iau mai intai coordonatele capului si caut vecinii acestuia(vecinii sunt considerati numai acele puncte ce se obtin prin varierea
alternativa cu o unitate a a oricăruia dintre indicii de coloană sau de linie).Avand in vedere ca initial, capul sarpelui va avea un singur segment din sarpe vecin pe harta si ca la orice moment, fiecare segment are max 2 vecini, daca pornesc de la capul sarpelui, il sterg, apoi caut vecini s.a.m.d, voi obtine mereu
un singur vecin, astfel incat, la final sa am intreaga configuratie a sarpelui stocata in vector.Am paralelizat si citirea serpilor, deoarece singura zona 
partajata de toti serpii in care exista posibilitatea de a se scrie in paralel este matricea world, insa chiar si in cazul in care 2 sau mai multi serpi incearca
sa scrie la aceeasi coordonate, cu totii vor incerca scrierea numarului 0.
Dupa aceasta citire, rescriu serpii inapoi pe harta.

In functia run_simulation, aloc un vector de coordonate(auxCoords), in care voi pastra viitoarele capete ale serpilor.
Pentru fiecare pas, sterg cozile serpilor si calculez noile capete, folosind functia get_coords.Apoi verific coliziunea astfel: mai intai, verific daca 
pe harta exista 0 la pozitia unde ar trebui sa isi mute sarpele capul, iar daca se intampla asta, trebuie sa continui verificarile deoarece pe harta se
afla momentan doar corpurile serpilor fara noile capete.Apoi, verific daca noul cap al unui sarpe face coliziune cu oricare dintre noile capete ale 
tuturor celorlalti serpi.Am ales sa verific astfel coliziunea si nu sa scriu noile capete in matrice si apoi sa verific, deoarece nu pot paraleliza 
scrierea in matrice astfel incat aceasta implementare nu ar fi scalat in raport cu numarul de threaduri.Daca are loc coliziunea, scriu pe harta vechile
cozi ale serpilor, iar daca nu, scriu noile capete pe harta, apoi updatez vectorul fiecarui sarpe.Copiez vechiul sarpe incepand de la pozitia 0 peste
vector, incepand de la pozitia 1, astfel incat pe prima pozitie voi scrie noul cap al sarpelui.

Mentionez ca am testat implementarea pe fep.grid.pub.ro, pe testul big_input, pentru 20000 pasi, iar timpii pe care i-am obtinut sunt:
-2 threaduri: 219.503181618
-4 threaduri: 109.978077024
-6 threaduri: 79.196825918
-8 threaduri: 60.614041355