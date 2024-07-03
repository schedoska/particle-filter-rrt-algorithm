# Robot navigation simulation

Project implements particle filtering method (Monte Carlo) to estimate robot position inside building (map is known a priori). After estimating most probable robot position, RRT* algorithm is used to find shortest path from current position to set home position (Blue circle). After detecting the best path, robot goes into traveling mode, and moves from it's current position to home while continuing to estimate it's position with particle filtering. It is assumed that robot is equipped with Lidar sensor allowing him to collect data of distances between him and closest obstacle in 8 different directions.   

## How to use it

### Requirements

* In oreder to build this application on your platform you need to have SFML installed (2.5.0 or newer version).

### Build

* Edit CMakeList.txt file, set SFML_DIR variable to path where you installed SFML library
* Create a build directory where you want executable file to be placed
* From inside this newly created directory use command `cmake` with path to project root directory
* If your build system of choice is Makefile, also add `-G "Unix Makefiles"` argument
* Go to your build directory and build appliaction with `make` command (if your build system is Makefile)
* Executable file will be created inside yout build directory

  Example:

  `
  mkdir build
  cd build
  cmake .. -G "Unix Makefiles"
  make
  .\RobotNavigation.exe
  `
