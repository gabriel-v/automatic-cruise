% Automatic Cruise Control - LEC 2016
% Gabriel Vîjială, Ștefan Teodorescu
% May, 2016


# Echipa: Kernel Panic <span style="color: blue">Blues</span>

## Despre noi

Suntem studenti la Facultatea de Automatica si Calculatoare,
Universitatea Politehnica Bucuresti.
Anul I de studiu.

# Proiect: Sistem de cruise control adaptiv pentru vehicule

## Descriere

- Simulare de autostrada cu trafic
- Sistem de cruise control adaptiv

## Descriere

Controleaza viteza vehiculului in functie de
vecinii sai din trafic.

# Milestone 1: Simularea

## Simularea

- Trei benzi cu sensul spre dreapta
- Vehicule patricipante la trafic
- Iau decizii aleatoare (dar fara riscuri)

## 
<div style="color: pink">
SCREENSHOT edited with paint
</div>

## Informatii disponibile

- Fiecare vehicul are informatii despre vecinii proximi
- Distanta si viteza relativa

## 
<div style="color: pink">
SCREENSHOT edited with paint
</div>

## Informatii incomplete

- Camp vizual limitat in toate directiile
- Limitat la benzile adiacente

## Fizica

- Acceleratia este limitata in functie de viteza curenta
- Viteza terminala este un parametru aleator
- Acceleratia maxima este un parametru aleator

## Interfata grafica

- Implementata cu OpenGL
- Fundal verde
- Carosabil inchis la culoare

## Interfata grafica

- Marcaje rutiere albe
- Imaginile vehiculelor vazute de deasupra
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

## Actiuni aleatoare

Din cand in cand:

- Schimbarea vitezei terminale
- A distantei limita
- Si a benzii

## Exceptie

Daca viteza unui vehicul este mai mica decat o limita,
acesta poate schimba banda doar la dreapta.

## How it's done

Each vehicle applies an acceleretion that:

- Tries to reach the target distance
- Tries to reach the target speed
- Strong breaking when the car up front is closer than a "panic distance"


# Milestone 2: User Input

## Command the Cruise Control System

- Change lanes
- Set target speed
- Set target distance

## Command the simulation

- Add another vehicle in front
- Or anywhere else on the highway

## Command the random vehicles

- Set target speed
- Set target distance
- Change lanes

After a random vehicle receives an order, it will wait longer before changing them randomly.

## Stats

- Simulation FPS
- Data about the ACC

# Milestone 3: The adaptive cruise control system

## Tralalala

tralala


# Milestone 4: Project completion

## Testing

tralala

## Documentation

- Documentation generated with Doxygen
- Slides -- markdown, pandoc, reveal.js


# Tools

## Tools

- C++11 with CMake
- Graphics: OpenGL 2 with GLFW3
- UI: ImGui
- IDE: IntelliJ CLion

# Screenshots

##

##

# More screenshots

##

##



