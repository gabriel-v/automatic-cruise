# Linux Embedded Challenge 2016

-------------------------------------------------------------------------------------------------------

## Team: Kernel Panic Blues

- Gabriel Vîjială
- Ştefan Teodorescu

We're students of the Faculty of Automatic Control and Computers,
Politehnica University of Bucharest.
First year of study.


-------------------------------------------------------------------------------------------------------

## Project: Adaptive cruise control system for road vehicles

For reasons we cannot explain, we tried to use Java for this project.
It didn't work. So here's the C++11 version.

We'll be using OpenGL 2 for the graphical simulation, and GLFW3 as the toolkit.
See [http://www.glfw.org/docs/latest/](glfw.org/docs). 
The UI is implemented using [https://github.com/ocornut/imgui/](imgui).

The project is packaged using CMake. We're using CLion.

We're planning to release the code on GitHub after the contest is over, so the code is
licenced under BSD-3.


-------------------------------------------------------------------------------------------------------
### Milestone 0: The setup

- Downloaded `UDOObuntu 2 RC1 (Ubuntu 14.04)` from [http://www.udoo.org/downloads/](udoo.org/downloads).
- Wrote it on the SD card using `dd`.
- Connected the touchscreen, modified the boot parameters to display everything on it.
- Ran a number of demos on the UDOO to see what it really supports.
- Settled on C++11, GLFW3 and OpenGL 2.

-------------------------------------------------------------------------------------------------------

### Milestone 1: A graphical simulation of the road and the traffic

Milestone statement:

> The road will contain 3 lanes and will be displayed on the screen from left to right.
> The animation should behave like in a side-scrolling 2D video game.
> It must render at no less than 30 FPS at all times.
> The motion of all random vehicles will consist of the following: accelerate, slow down, change lane.
> Each of these actions will occur randomly but must be safe.


#### The simulation

Each lane is implemented as a sorted deque of vehicles.
On each simulation step, each vehicle receives its neighbours from the simulator: distances and relative 
velocities for the vehicle up front, the one trailing it, and the two closest vehicles on each adjacent lane.
The vehicles can't see farther than a set distance, so some of the neighbours will be at 'infinite' distance.

The highway has one preferred vehicle, the one with the Cruise Control System.

Code: the `Highway` class; the `Lane` class; the `Vehicle` class

#### The graphical UI

As the UI is 2D, viewed from the top, we'll be lazy and use the `OpenGL 1.x` API to draw the road and the cars.
The OpenGL 2 API is only used when rendering the UI. All the windowing functions are provided by the GLFW3 library.

The animation behaves like in a side-scrolling 2D video game. The user can zoom in or out of the simulation while camera
tracks the ACC vehicle. The user can select any vehicle by tapping on it and issue orders to it.

##### Note
If we have the time, we may switch to rendering 3D models and having the camera hover around
like a news helicopter but that's in the unforeseeable future.
But, until then, all our cars are images of vehicles seen from above.

Code: the `Window` and `Window2D` class
Submodule: `GLFW3`

#### The random vehicles

All vehicles other than the ACC system take periodic random actions, like changing their target
speed or target distance (to the vehicle up front), or changing lanes.

All these actions are safe, because at each step the random vehicle calculates a desired acceleration,
that satisfies the following: 

- tries to reach the target distance
- tries to reach the target speed
- applies strong negative acceleration (breaking) when the car up front is closer than a "panic distance"

Changing lanes is done only when it is safe to do so (there are no adjacent or incoming cars on the target lane).

Code: the `RandomVehicle` class

-------------------------------------------------------------------------------------------------------

### Milestone 2: User Input

The UI is implemented using `ImGui` Library, which provides exactly the basic functions that we need
(buttons, sliders, text labels).

The user can interact directly with the simulation, through a series of command windows, designed to issue basic orders
to the cars, such as changing lanes, or setting a target speed. A control panel is offered not only for the preferred
vehicle, but also for a random, user selected vehicle on the highway.

Code: the `UIPresenter` class
Submodule: `ImGui`


-------------------------------------------------------------------------------------------------------

### Milestone 3: The adaptive cruise control system

The Adaptive Cruise Control system can detect the status of the closest vehicle ahead of it and the closest behind it,
up to a maximum distance of a few hundred meters. Data for the adjacent lanes is also available.
The data available for each vehicle is distance and relative velocity. Using this data, the ACC system maintains
its target speed for as long as possible, while also ensuring that the vehicle in front is farther than the allowed
minimum distance. If the vehicle ahead is closer or is approaching too fast, the ACC triggers a moderate braking action,
then tries to match its speed. If the distance then grows, the ACC triggers a moderate acceleration and attempts
to once again reach the target speed.

Code: the `ACCVehicle` class

-------------------------------------------------------------------------------------------------------

### Milestone 4: Project completion

The documentation is compiled using Doxygen.
I'll host it (and any videos we may have by then) on one of my sites before the presentation.

TODO: automated testing, parameter optimisation, with genetic algorithm?
TODO: convert videos to webm

-------------------------------------------------------------------------------------------------------

### Bonus: Adaptive cruise control with automatic overtaking

The ACC system keeps a counter of the number of seconds the ACC can't reach it's target speed because of the vehicle
ahead. related to the current speed. When the counter reaches a limit, the ACC system will try and overtake that
vehicle. The ACC prefers changing lanes to the left, because all the slow vehicles are forced to change lanes
towards the right side of the road.

Code: `ACCVehicle::think`, `ACCVehicle::shouldChangeLane`

-------------------------------------------------------------------------------------------------------

## End Credits

TODO