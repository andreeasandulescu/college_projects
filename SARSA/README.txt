//Sandulescu Andreea, 342C3

Variere parametrii pt epsilon-greedy:
	
Pe hartile "MiniGrid-Empty":
	Cele mai bune rezultate le-am obtinut pt perechea (alpha=0.8, gamma=0.1, c=0.9, q_init=0.5),
	iar cele mai proaste pt (alpha=0.2, gamma=0.8, c=0.2, q_init=5).Aici este util ca rata de invatare
	sa fie mai mare si ca valorile initiale (q_init) sa fie mai mari ca 0, dar nu foarte mari, pentru
	a incuraja explorarea.De asemenea, o valoare mai mare a lui c a dat rezultate mai bune.

Pe hartile "MiniGrid-DoorKey":
	Cele mai bune rezultate le-am obtinut pt perechea (alpha=0.9, gamma=0.2, c=0.9, q_init=0),
	iar cele mai proaste pt (alpha=0.5, gamma=0.05, c=0.2, q_init!=0).Cele mai bune rezultate
	se obtin pt valorile initiale (q_init) = 0.

Variere parametrii pt Softmax (Boltzmann):
	
Pe hartile "MiniGrid-Empty":
	Cele mai bune rezultate le-am obtinut pt perechea (alpha=0.1, gamma=0.9, q_init=0.5),
	iar cele mai proaste pt (alpha=0.2, gamma=0.5, q_init=5).In cazul in care valorile 
	initiale (q_init) sunt mai mari ca 0, se obtin rezultate mai bune, iar rata de invatare
	sa aiba valori mici si gamma valori mari.

Pe hartile "MiniGrid-DoorKey":
	Cele mai bune rezultate le-am obtinut pt perechea (alpha=0.1, gamma=0.9, q_init=0.5),
	iar cele mai proaste pt (alpha=0.2, gamma=0.05, q_init=5).

epsilon-greedy vs Boltzmann:
	In general, metoda Softmax a dat rezultate mai bune decat epsilon-greedy, astfel incat
	initializarea optimista(q0 > 0) a imbunatati rezultatele pt Softmax mai mult decat pt 
	eps-greedy.
