% Automatic Cruise Control - LEC 2016
% Gabriel Vîjială, Ștefan Teodorescu
% May, 2016


# Echipa: Kernel Panic <span style="color: cyan">Blues</span>

## Despre noi

Suntem studenti la Facultatea de Automatica si Calculatoare,
Universitatea Politehnica Bucuresti.
Anul I de studiu.

# Proiect: Sistem de cruise control adaptiv pentru vehicule

## Descriere

Am implementat un sistem de cruise control care
controleaza viteza vehiculului in functie de
vecinii sai din trafic.

## Descriere

Am implementat:

- O simulare a traficului pe o autostrada
- Comportamentul haotic al participantilor la trafic
- Un sistem stabil de cruise control adaptiv

# Simularea

## Simularea

- Trei benzi cu sensul spre dreapta
- Vehicule patricipante la trafic
- Vehiculele decizii aleatoare (dar fara riscuri)

## Informatii disponibile

- Fiecare vehicul are informatii despre vecinii proximi
- Distanta si viteza relativa


## Informatiile sunt incomplete

- Camp vizual limitat in toate directiile
- Limitat la benzile adiacente

## Modelul fizic

- Acceleratia este limitata in functie de viteza curenta
- Viteza terminala este un parametru aleator
- Acceleratia maxima este un parametru aleator

## Interfata grafica

- Implementata cu OpenGL
- Fundal verde, elemente vizuale verzi
- Carosabil inchis la culoare

## Interfata grafica

- Marcaje rutiere albe
- Imagini cu vehicule vazute de deasupra
- Triunghiuri verzi aleatoare drept peisaj


# Participantii la trafic

## Caracteristici aleatoare

- Viteza maxima
- Acceleratia maxima
- Timp de reactie
- Distanta de panica

## Infatisare aleatoare

- Culoare aleatoare
- Latime aleatoare
- Lungime aleatoare

## Comportament aleator

Din cand in cand, participantii la trafic vor face urmatoarele:

- Vor schimba viteza tinta
- Si distanta minima
- Si vor incerca sa schimbe banda

## Exceptie

Daca viteza unui vehicul este mai mica decat o limita,
acesta poate schimba banda doar la dreapta.

# Interfata de control

## Controlul participantilor la trafic

Utilizatorul poate sa comande sistemului ACC sa:

- Schimbe banda
- Schimbe viteza tinta
- Schimbe distanta minima

## Controlul simularii

Utilizatorul poate sa:

- Adauge un vehicul in fata masinii echipate cu ACC
- Sau oriunde altundeva pe autostrada

## Controlul participantilor la trafic

Comenzile posibile sunt similare cu cele ale
sistemului ACC.

## Statistici

- FPS
- Date despre sistemul ACC

# Sistemul de Cruise Control Adaptiv

## Controlul vitezei

Sistemul ACC isi controleaza acceleratia astfel incat:

- Sa atinga viteza tinta
- Sa mentina distanta minima
- Sa franeze puternic in cazul unui pericol

## Strategia de depasire

- Se contorizeaza timpul de cand vehiculul nu conduce
cu viteza dorita
- Daca a trecut destul timp, se va incerca o depasire

## Strategia de depasire

- Se verifica daca schimbarea benzii ar fi sigura
- Se alege banda care permite o acceleratie mai buna

# Finalizarea proiectului

## Testare automata

Nu exista coliziuni pe durata simularii.
Acest fapt este verificat la fiecare pas al simularii.

## Documentatie

- Documentatia a fost generata cu Doxygen
- Slides -- markdown, pandoc, reveal.js

# Intrebari?




