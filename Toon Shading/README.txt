// Sandulescu Andreea-Bianca, 331CB


Pentru toon outline, am folosit OutlineVertexShader, in care deplasez pozitiile vertecsilor spre exteriorul 
obiectului de-alungul normalelor, in functie de timp si OutlineFragmentShader, in care setez culoarea neagra, urmand 
sa fie afisate doar fetele spate ale obiectului.
Pentru afisarea obiectului cu iluminare si texturare, am folosit Vertex Shader, in care calculez normala si pozitia 
in spatiul lume, pentru a le trimite in Fragment Shader.Aici, obtin constantele difuza si speculara din texturi,
calculez componenta difuza si speculara dupa formulele din modelul Phong, apoi folosesc Stepped Lighting pentru
ambele intensitati(diffuse_light = floor(aux * num_levels) / num_levels).
Aceasta operatie este aplicata pentru fiecare dintre cele 4 surse de lumina, astfel incat obtin 4 componente finale,
dupa aplicarea formulei din tema (((diffuse_light + material_ka) * kd + specular_light * ks) * fat * lightColor),
a caror suma va reprezenta out_color din FragmentShader.
Cu tasta T se activeaza efectul de Toon Shading, tasta de pauza/resume pentru lumini este Space, iar numLevels 
se modifica de la sageata sus/jos. 