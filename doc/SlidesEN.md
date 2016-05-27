% Automatic Cruise Control - LEC 2016
% Gabriel Vîjială, Ștefan Teodorescu
% May, 2016


# Team: Kernel Panic <span style="color: blue">Blues</span>

## About us

We're students of the Faculty of Automatic Control and Computers,
Politehnica University of Bucharest.
First year of study.

# Project: Adaptive cruise control system for road vehicles

## Description

We've designed a cruise control system (with automatic overtaking)
that controls the vehicle speed by adapting to its environment.

# Milestone 1: The simulation

## The simulation

- Three lanes going right
- A population of random vehicles
- That undertake random (but safe) actions

## 
<div style="color: pink">
SCREENSHOT edited with paint
</div>

## Available information

- Each vehicle receives its immediate neighbours
- Data: distance and relative speed

## 
<div style="color: pink">
SCREENSHOT edited with paint
</div>

## Incomplete information

- Limited sight in all directions
- Limited to adjacent lanes

## Physics

- Acceleration is limited by current speed
- Terminal speed is a random parameter
- Maximum acceleration is a random parameter

## The graphical UI

- Implemented in OpenGL
- Green background
- Dark road surface

## The graphical UI

- White lane markers
- Sprites of cars seen from above
- Random green triangles that serve as foliage

# The random vehicles

## Random characteristics

- Max speed
- Max acceleration
- Overtake speed
- Reaction time
- Panic distance

## Random appearance

- Random Color
- Random Width
- Random Length

## Random actions

Once in a while:

- Randomly change the target speed
- And the target distance
- And the lane

## Exception

If the random vehicle's speed is lower than a threshold, then it can only change lanes to the right.

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



