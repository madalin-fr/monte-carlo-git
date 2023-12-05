# Week 2 - OpenCL Programming Using OpenCL C Kernels

##  Research resources used:

* [Github KhronosGroup/OpenCL-Guide | Programming OpenCL Kernels](https://github.com/KhronosGroup/OpenCL-Guide/blob/main/chapters/programming_opencl_kernels.md)

* [Github HandsOnOpenCL/Exercises-Solutions | Exercise 6 - Matrix Multiplication](https://github.com/HandsOnOpenCL/Exercises-Solutions/tree/master/Exercises/Exercise06)

* [National Energy Research Scientific Computing Center | OpenCL: A Hands-on Introduction](https://www.nersc.gov/assets/pubs_presos/MattsonTutorialSC14.pdf)

* [Matrix Multiplication OpenCL](https://ecatue.gitlab.io/GPU2016/cookbook/matrix_multiplication_opencl/)

## Results

I have sourced an exercise program for matrix multiplication in OpenCL to understand the API features. 

In C we use a kernel and a host code to compile the program on the available accelerator platform.  

To compile the program use the provided Makefile and `make` command or type the following gcc command: 

`gcc -o matrixmul matrixmul_host.c -lOpenCL`


TODO: Make Intel V-Tune Profiler display better results in hotspots collection and performance snapshot targeting the exercise program.

`vtune`

`vtune -collect performance-snapshot a.out`

`vtune -collect hotspots a.out`


```
marius_@Marius:~/monte-carlo-simulation/week-2/opencl$ vtune -collect performance-snapshot ./matrixmul
vtune: Collection started. To stop the collection, either press CTRL-C or enter from another console window: vtune -r /home/marius_/monte-carlo-simulation/week-2/opencl/r001ps -command stop.
Initializing OpenCL device...
Running matrix multiplication for matrices A (1024x1024) and B (1024x1024) ...
Matrix multiplication completed...
vtune: Collection stopped.
vtune: Using result path `/home/marius_/monte-carlo-simulation/week-2/opencl/r001ps'
vtune: Executing actions 75 % Generating a report                              Elapsed Time: 0.419s
 | Application execution time is too short. Metrics data may be unreliable.
 | Consider reducing the sampling interval or increasing your application
 | execution time.
 |
    IPC: 1.536
    SP GFLOPS: 0.019
    DP GFLOPS: 0.000
    Average CPU Frequency: 4.067 GHz
Logical Core Utilization: 17.2% (0.690 out of 4)
 | The metric value is low, which may signal a poor logical CPU cores
 | utilization. Consider improving physical core utilization as the first step
 | and then look at opportunities to utilize logical cores, which in some cases
 | can improve processor throughput and overall performance of multi-threaded
 | applications.
 |
    Physical Core Utilization: 33.8% (0.677 out of 2)
     | The metric value is low, which may signal a poor physical CPU cores
     | utilization caused by:
     |     - load imbalance
     |     - threading runtime overhead
     |     - contended synchronization
     |     - thread/process underutilization
     |     - incorrect affinity that utilizes logical cores instead of physical
     |       cores
     | Run the HPC Performance Characterization analysis to estimate the
     | efficiency of MPI and OpenMP parallelism or run the Locks and Waits
     | analysis to identify parallel bottlenecks for other parallel runtimes.
     |
Microarchitecture Usage: 33.1% of Pipeline Slots
 | You code efficiency on this platform is too low.
 |
 | Possible cause: memory stalls, instruction starvation, branch misprediction
 | or long latency instructions.
 |
 | Next steps: Run Microarchitecture Exploration analysis to identify the cause
 | of the low microarchitecture usage efficiency.
 |
    Retiring: 33.1% of Pipeline Slots
    Front-End Bound: 20.2% of Pipeline Slots
     | Issue: A significant portion of Pipeline Slots is remaining empty due to
     | issues in the Front-End.
     |
     | Tips:  Make sure the code working size is not too large, the code layout
     | does not require too many memory accesses per cycle to get enough
     | instructions for filling four pipeline slots, or check for microcode
     | assists.
     |
    Bad Speculation: 13.1% of Pipeline Slots
    Back-End Bound: 33.5% of Pipeline Slots
     | A significant portion of pipeline slots are remaining empty. When
     | operations take too long in the back-end, they introduce bubbles in the
     | pipeline that ultimately cause fewer pipeline slots containing useful
     | work to be retired per cycle than the machine is capable to support. This
     | opportunity cost results in slower execution. Long-latency operations
     | like divides and memory operations can cause this, as can too many
     | operations being directed to a single execution port (for example, more
     | multiply operations arriving in the back-end per cycle than the execution
     | unit can support).
     |
        Memory Bound: 16.0% of Pipeline Slots
            L1 Bound: 12.9% of Clockticks
            L2 Bound: 2.8% of Clockticks
            L3 Bound: 0.9% of Clockticks
                L3 Latency: 1.5% of Clockticks
            DRAM Bound: 2.2% of Clockticks
                Memory Bandwidth: 5.3% of Clockticks
                Memory Latency: 13.0% of Clockticks
            Store Bound: 1.6% of Clockticks
        Core Bound: 17.6% of Pipeline Slots
         | This metric represents how much Core non-memory issues were of a
         | bottleneck. Shortage in hardware compute resources, or dependencies
         | software's instructions are both categorized under Core Bound. Hence
         | it may indicate the machine ran out of an OOO resources, certain
         | execution units are overloaded or dependencies in program's data- or
         | instruction- flow are limiting the performance (e.g. FP-chained long-
         | latency arithmetic operations).
         |
Memory Bound: 16.0% of Pipeline Slots
    Cache Bound: 16.6% of Clockticks
    DRAM Bound: 2.2% of Clockticks
Vectorization: 0.0% of Packed FP Operations
    Instruction Mix
        SP FLOPs: 0.4% of uOps
            Packed: 0.0% from SP FP
                128-bit: 0.0% from SP FP
                256-bit: 0.0% from SP FP
                512-bit: 0.0% from SP FP
            Scalar: 100.0% from SP FP
        DP FLOPs: 0.0% of uOps
            Packed: 0.0% from DP FP
                128-bit: 0.0% from DP FP
                256-bit: 0.0% from DP FP
                512-bit: 0.0% from DP FP
            Scalar: 100.0% from DP FP
        x87 FLOPs: 0.0% of uOps
        Non-FP: 99.6% of uOps
    FP Arith/Mem Rd Instr. Ratio: 0.016
    FP Arith/Mem Wr Instr. Ratio: 0.029
Collection and Platform Info
    Application Command Line: ./matrixmul
    Operating System: 5.15.90.1-microsoft-standard-WSL2 DISTRIB_ID=Ubuntu DISTRIB_RELEASE=22.04 DISTRIB_CODENAME=jammy DISTRIB_DESCRIPTION="Ubuntu 22.04.3 LTS"
    Computer Name: Marius
    Result Size: 3.6 MB
    Collection start time: 16:43:27 28/11/2023 UTC
    Collection stop time: 16:43:28 28/11/2023 UTC
    Collector Type: Driverless Perf per-process counting
    CPU
        Name: Intel(R) microarchitecture code named Tigerlake
        Frequency: 2.995 GHz
        Logical CPU Count: 4
        Cache Allocation Technology
            Level 2 capability: not detected
            Level 3 capability: not detected

Recommendations:
    Increase execution time:
     | Application execution time is too short. Metrics data may be unreliable.
     | Consider increasing your application execution time.
    Hotspots: Start with Hotspots analysis to understand the efficiency of your algorithm.
     | Use Hotspots analysis to identify the most time consuming functions.
     | Drill down to see the time spent on every line of code.
    Threading: There is poor utilization of logical CPU cores (17.2%) in your application.
     |  Use Threading to explore more opportunities to increase parallelism in
     | your application.
    Microarchitecture Exploration: There is low microarchitecture usage (33.1%) of available hardware resources.
     | Run Microarchitecture Exploration analysis to analyze CPU
     | microarchitecture bottlenecks that can affect application performance.

If you want to skip descriptions of detected performance issues in the report,
enter: vtune -report summary -report-knob show-issues=false -r <my_result_dir>.
Alternatively, you may view the report in the csv format: vtune -report
<report_name> -format=csv.
vtune: Executing actions 100 % done
marius_@Marius:~/monte-carlo-simulation/week-2/opencl$
```