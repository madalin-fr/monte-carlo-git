# Monte Carlo Simulation

## Who are we?
* @alexandru.ifrimenco - pthreads
* @emil.mateescu - MPI
* @madalin.frincu - OpenCL
* @razvan.costache - OpenMP

## What's it about?

Monte Carlo Analysis for a ship traversing a sea full of mines. Based on this [repository](https://github.com/dmitrijbes/monte-carlo-ship/)

## Description
We run 500000 experiments on a 30x30 matrix with 10 mines distributed randomly to determine the probability of the ship successfully crossing the sea.

Each experiment has the following rules:
* The ship must move from the bottom-left corner to the top-right corner 
* The ship can move only up and right with 1/2 probability for each direction.
* The ship is destroyed on contact with a mine cell.


There are 4 functions 
1. 'main' - Initializes a sea matrix and performs Monte Carlo experiments. Calculates and prints the success probability and other relevant information.
2. 'fillMatrix' - Fills the given matrix with a specified symbol
3. 'generateRandomElements' - Generates random coordinates and places a specified number of elements (mines) in the matrix. It ensures that each element is placed in a unique location.
4. 'movementTill' - Simulates the movement of the ship through the sea matrix. The function returns 'true' if the ship reaches the destination or 'false' viceversa.
