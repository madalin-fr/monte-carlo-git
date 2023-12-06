# Week 1 - OpenCL Setup for WSL2 development environment

##  Resources used:

* [Intel | Configure WSL 2 for GPU Workflows](https://www.intel.com/content/www/us/en/docs/oneapi/installation-guide-linux/2024-0/configure-wsl-2-for-gpu-workflows.html#UBUNTU-22-04-JAMMY)

* [GitHub KhronosGroup/OpenCL-Guide | Getting started with OpenCL on Ubuntu Linux](https://github.com/KhronosGroup/OpenCL-Guide/blob/main/chapters/getting_started_linux.md)

## Results

Testing successfully if hardware platform is available for the OpenCL SDK installation.

` gcc -Wall -Wextra -D CL_TARGET_OPENCL_VERSION=100 hello.c -o HelloOpenCL -lOpenCL `
 