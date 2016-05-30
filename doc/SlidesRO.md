% Automatic Cruise Control - LEC 2016
% Gabriel Vîjială, Ștefan Teodorescu
% May, 2016


# Echipa: Kernel Panic <span style="color: cyan">Blues</span>

## Despre noi

Suntem studenți la Facultatea de Automatică și Calculatoare,
Universitatea Politehnica București.
Anul I de studiu.

# Proiect: Sistem de cruise control adaptiv pentru vehicule

## Descriere

Am implementat un sistem de cruise control care
controlează viteza vehiculului in funcție de
vecinii săi din trafic.

## Descriere

Am implementat:

- O simulare a traficului pe o autostradă
- Comportamentul haotic al participanților la trafic
- Un sistem stabil de cruise control adaptiv

# Simularea

## Simularea

- Trei benzi cu sensul spre dreapta
- Vehicule patricipante la trafic
- Vehiculele iau decizii aleatoare (dar fără riscuri)

## Informații disponibile

- Fiecare vehicul are informații despre vecinii proximi
- Distanța si viteza relativă


## Informațiile sunt incomplete

- Câmp vizual limitat in toate direcțiile
- Limitat la benzile adiacente

## Modelul fizic

- Accelerația este limitată in funcție de viteza curentă
- Viteza terminală este un parametru aleator
- Accelerația maximă este un parametru aleator

## Interfața grafică

- Implementată cu OpenGL
- Fundal verde, elemente vizuale verzi
- Carosabil închis la culoare

## Interfața grafică

- Marcaje rutiere albe
- Imagini cu vehicule văzute de deasupra
- Triunghiuri verzi aleatoare drept peisaj


# Participanții la trafic

## Caracteristici aleatoare

- Viteza maximă
- Accelerația maximă
- Timpul de reacție
- Distanța de panică

## Infățișare aleatoare

- Culoare aleatoare
- Lățime aleatoare
- Lungime aleatoare

## Comportament aleator

Din când in când, participanții la trafic vor face următoarele:

- Vor schimba viteza țintă
- Și distanța minimă
- Și vor încerca să schimbe banda

## Excepție

Dacă viteza unui vehicul este mai mică decât o limită,
acesta poate schimba banda doar la dreapta.

# Interfața de control

## Controlul participanților la trafic

Utilizatorul poate să comande sistemului ACC să:

- Schimbe banda
- Schimbe viteza țintă
- Schimbe distanța minimă

## Controlul simulării

Utilizatorul poate să:

- Adauge un vehicul în fața mașinii echipate cu ACC
- Sau oriunde altundeva pe autostradă

## Controlul participanților la trafic

Comenzile posibile sunt similare cu cele ale
sistemului ACC.

## Statistici

- FPS
- Date despre sistemul ACC

# Sistemul de Cruise Control Adaptiv

## Controlul vitezei

Sistemul ACC își controlează accelerația astfel încat:

- Să atingă viteza țintă
- Să mențina distanța minimă
- Să frâneze puternic in cazul unui pericol

## Strategia de depășire

- Se contorizează timpul de când vehiculul nu conduce
cu viteza dorită
- Dacă a trecut destul timp, se va încerca o depășire

## Strategia de depășire

- Se verifică dacă schimbarea benzii ar fi sigură
- Se alege banda care permite o accelerație mai bună

# Finalizarea proiectului

## Testare automată

Nu există coliziuni pe durata simulării.
Acest fapt este verificat la fiecare pas al simulării.

## Documentație

- Documentația a fost generată cu Doxygen
- Slides -- markdown, pandoc, reveal.js

# Întrebări?




