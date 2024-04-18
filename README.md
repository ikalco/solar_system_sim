## A Solar System Simulation

Small hobby project to refamiliarize with C and SDL2.
It accurately simulates newtons gravity equation with
real astronomical numbers and makes it look a little nice.

### Build and Run (linux only)

The path for the save file is hardcoded as ./saves/solar_system.save
which means that you must have that file structure wherever you invoke
the program. However it will work if you follow the steps below.

#### Build Requirements
- gcc
- gnu make
- SDL2 base library (libsdl2-dev)
- SDL2 ttf library  (libsdl2-ttf-dev)

#### Steps
```
git clone git@github.com:ikalco/solar_system_sim.git
cd solar_system_sim
make
./build/solar_system_sim
```
