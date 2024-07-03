# Robot navigation simulation

Project implements particle filtering method (Monte Carlo) to estimate robot position inside building (map is known a priori). After estimating most probable robot position, RRT* algorithm is used to find shortest path 
from current position to set home position (Blue circle). After detecting the best path, robot goes into traveling mode, and moves from it's current position to home while continuing to estimate it's positio nwith
particle filtering.   
