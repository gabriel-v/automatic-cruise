# Linux Embedded Challenge 2016

-------------------------------------------------------------------------------------------------------

## Team: Kernel Panic Blues

- Gabriel Vîjială -- Mainly responsible of the UI and the UDOO's general well-being.
- Ştefan Teodorescu -- Mainly responsible of the simulation and the code's general well-being.

We're students of the Faculty of Automatic Control and Computers, Politehnica University of Bucharest. 
First year of study.


-------------------------------------------------------------------------------------------------------

## Project: Adaptive cruise control system for road vehicles

For reasons we cannot explain, we tried to use Java for this project.
It didn't work. So here's the C++ version.

We'll be using OpenGL 2 for the graphical simulation, and GLFW3 as the toolkit.
See [http://www.glfw.org/docs/latest/](glfw.org/docs).

The project is packaged using CMake. We're using CLion.

We're planning to release the code on GitHub after the contest is over, so the code is
licenced under BSD-3.


-------------------------------------------------------------------------------------------------------
### Milestone 0: The setup

- Downloaded `UDOObuntu 2 RC1 (Ubuntu 14.04)` from [http://www.udoo.org/downloads/](udoo.org/downloads)
- Wrote it on the SD card using `dd`
- Connected the touchscreen, modified the boot parameters to display everything on it
- Ran a number of demos on the UDOO to see what it really supports


-------------------------------------------------------------------------------------------------------

### Milestone 1: A graphical simulation of the road and the traffic (April 14th)

> The road will contain 3 lanes and will be displayed on the screen from left to right.
> The animation should behave like in a side-scrolling 2D video game.
> It must render at no less than 30 FPS at all times.
> The motion of all random vehicles will consist of the following: accelerate, slow down, change lane.
> Each of these actions will occur randomly but must be safe.


#### The simulation

Each lane is implemented as a doubly linked list of vehicles.
On each simulation step, each vehicle receives its neighbours from the simulator: distances and relative 
velocities for the vehicle up front, the one trailing it, and the two closest vehicles on each adjacent lane.

#### The graphical UI

As the UI is 2D, viewed from the top, we'll be lazy and use the `OpenGL 1.x` API.
No shaders, no shiny things. Just `glOrtho()`, `glBegin(GL_TRIANGLES)` ... `glEnd()` and that's it.

If we have the time, we may switch to rendering 3D models and having the camera hover around
like a news helicopter but that's in the unforeseeable future.
But, until then, all our cars are coloured squares.


-------------------------------------------------------------------------------------------------------

### Milestone 2: User Input (April 21th)

TODO


-------------------------------------------------------------------------------------------------------

### Milestone 3: The adaptive cruise control system (May 12th)

TODO


-------------------------------------------------------------------------------------------------------

### Milestone 4: Project completion (May 19th)

TODO

-------------------------------------------------------------------------------------------------------

### Milestone 5: Adaptive cruise control with automatic overtaking (May 19th)

TODO

-------------------------------------------------------------------------------------------------------

## End Credits

TODO