## Monte Carlo Simulation

# Who are we?
@alexandru.ifrimenco
@emil.mateescu
@madalin.frincu
@razvan.costache

# What's it about?

Monte Carlo Analysis. Based on this [repository](https://github.com/dmitrijbes/monte-carlo-ship/)

# Testing environment
For testing, we use a laptop with a 12th Gen i7-1260P with (12 cores, 16 threads) running a Ubuntu 20.04 Operating System

# Week 1 - Profiling of serial implementation

1. Serial Implementation - sligthly modified the original code
2. Measuring execution time of serial implementation 
> Program duration: 11.091308 seconds

3. Profiling of serial implementation
![](image/serial_profiling.png)

As it's seen above, the movementTill, generateRandomElements and fillMatrix take the most amount of time. They call resource-heavy functions for random generation and would clearly benefit from code parallelization.