#define CL_TARGET_OPENCL_VERSION 300
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

#define sea_size 800
#define movement_limit 50000
#define tests 12000
#define mines_count 1

int main() {
    cl_int ret;

    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;   
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to get platform IDs. Error code: %d\n", ret);
        exit(1);
    }
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to get device IDs. Error code: %d\n", ret);
        exit(1);
    }

    // Create an OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    if (!context) {
        fprintf(stderr, "Failed to create context. Error code: %d\n", ret);
        exit(1);
    }

    // Create a command queue with properties
    cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id, 0, &ret);
    if (!command_queue) {
        fprintf(stderr, "Failed to create command queue. Error code: %d\n", ret);
        exit(1);
    }

    // Create memory buffers on the device for each vector 
    cl_mem sea_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, sea_size * sea_size * sizeof(char), NULL, &ret);
    if (!sea_mem_obj) {
        fprintf(stderr, "Failed to create buffer for sea. Error code: %d\n", ret);
        exit(1);
    }
    cl_mem successes_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, tests * sizeof(int), NULL, &ret);
    if (!successes_mem_obj) {
        fprintf(stderr, "Failed to create buffer for successes. Error code: %d\n", ret);
        exit(1);
    }

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
    if (!program) {
        fprintf(stderr, "Failed to create program. Error code: %d\n", ret);
        exit(1);
    }

    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to build program. Error code: %d\n", ret);
        // Determine the reason for the error
        char build_log[16384];
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(build_log), build_log, NULL);
        fprintf(stderr, "Error in kernel: \n%s\n", build_log);
        exit(1);
    }

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "fillMatrix", &ret);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to create kernel for fillMatrix. Error code: %d\n", ret);
        exit(1);
    }

    // Set the arguments of the kernel
    int size = sea_size;
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&sea_mem_obj);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to set kernel argument 0. Error code: %d\n", ret);
        exit(1);
    }
    ret = clSetKernelArg(kernel, 1, sizeof(int), &size);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to set kernel argument 1. Error code: %d\n", ret);
        exit(1);
    }
    char fill_symbol = '~';
    ret = clSetKernelArg(kernel, 2, sizeof(char), &fill_symbol);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to set kernel argument 2. Error code: %d\n", ret);
        exit(1);
    }


    // Execute the OpenCL kernel on the list
    size_t global_item_size = sea_size; // Process the entire lists
    size_t local_item_size = 1; // Divide work items into groups of 16
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
    
    
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to enqueue NDRange kernel. Error code: %d\n", ret);
        exit(1);
    }

    // Create the OpenCL kernel
    kernel = clCreateKernel(program, "generateRandomElements", &ret);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to create kernel for generateRandomElements. Error code: %d\n", ret);
        exit(1);
    }

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&sea_mem_obj);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to set kernel argument 0. Error code: %d\n", ret);
        exit(1);
    }
    ret = clSetKernelArg(kernel, 1, sizeof(int), &size);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to set kernel argument 1. Error code: %d\n", ret);
        exit(1);
    }
    char element_symbol = '*';
    ret = clSetKernelArg(kernel, 2, sizeof(char), &element_symbol);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to set kernel argument 2. Error code: %d\n", ret);
        exit(1);
    }
    int elements_count = mines_count;
    ret = clSetKernelArg(kernel, 3, sizeof(int), &elements_count);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to set kernel argument 3. Error code: %d\n", ret);
        exit(1);
    }

    // Execute the OpenCL kernel on the list
    global_item_size = elements_count; // Process the entire lists
    local_item_size = 64; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to enqueue NDRange kernel. Error code: %d\n", ret);
        exit(1);
    }

    // Create the OpenCL kernel
    kernel = clCreateKernel(program, "movementTill", &ret);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to create kernel for movementTill. Error code: %d\n", ret);
        exit(1);
    }

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&sea_mem_obj);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to set kernel argument 0. Error code: %d\n", ret);
        exit(1);
    }
    ret = clSetKernelArg(kernel, 1, sizeof(int), &size);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to set kernel argument 1. Error code: %d\n", ret);
        exit(1);
    }
    char destination_symbol = 'E';
    ret = clSetKernelArg(kernel, 2, sizeof(char), &destination_symbol);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to set kernel argument 2. Error code: %d\n", ret);
        exit(1);
    }
    char failure_symbol = '*';
    ret = clSetKernelArg(kernel, 3, sizeof(char), &failure_symbol);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to set kernel argument 3. Error code: %d\n", ret);
        exit(1);
    }
    ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&successes_mem_obj);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to set kernel argument 4. Error code: %d\n", ret);
        exit(1);
    }

    // Execute the OpenCL kernel on the list
    global_item_size = tests; // Process the entire lists
    local_item_size = 64; // Divide work items into groups of 64s
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

    // Create the OpenCL kernel
    kernel = clCreateKernel(program, "generateRandomElements", &ret);
     if (ret != CL_SUCCESS) { 

      printf("Error creating kernel for generateRandomElements\n");

      exit(1); 

    }

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&sea_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(int), &size);
    element_symbol = '*';
    ret = clSetKernelArg(kernel, 2, sizeof(char), &element_symbol);
    elements_count = mines_count;
    ret = clSetKernelArg(kernel, 3, sizeof(int), &elements_count);

    // Execute the OpenCL kernel on the list
    global_item_size = elements_count; // Process the entire lists
    local_item_size = 64; // Divide work items into groups of 64
        ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to enqueue NDRange kernel. Error code: %d\n", ret);
        exit(1);
    }

    // Read the memory buffer C on the device to the local variable C
    int *successes = (int*)malloc(sizeof(int)*tests);
    ret = clEnqueueReadBuffer(command_queue, successes_mem_obj, CL_TRUE, 0, tests * sizeof(int), successes, 0, NULL, NULL);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to read buffer. Error code: %d\n", ret);
        exit(1);
    }

    // Display the result to the screen
    int total_successes = 0;
    for(int i = 0; i < tests; i++)
        total_successes += successes[i];

    float success_probability = (float)total_successes / tests * 100;

    printf("Number of tests: %d\n", tests);
    printf("Number of successes: %d\n", total_successes);
    printf("Probability of ship reaching the end: %.2f%%\n", success_probability);

    // Clean up
    ret = clFlush(command_queue);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to flush command queue. Error code: %d\n", ret);
        exit(1);
    }
    ret = clFinish(command_queue);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to finish command queue. Error code: %d\n", ret);
        exit(1);
    }
    ret = clReleaseKernel(kernel);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to release kernel. Error code: %d\n", ret);
        exit(1);
    }
    ret = clReleaseProgram(program);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to release program. Error code: %d\n", ret);
        exit(1);
    }
    ret = clReleaseMemObject(sea_mem_obj);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to release memory object for sea. Error code: %d\n", ret);
        exit(1);
    }
    ret = clReleaseMemObject(successes_mem_obj);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to release memory object for successes. Error code: %d\n", ret);
        exit(1);
    }
    ret = clReleaseCommandQueue(command_queue);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to release command queue. Error code: %d\n", ret);
        exit(1);
    }
    ret = clReleaseContext(context);
    if (ret != CL_SUCCESS) {
        fprintf(stderr, "Failed to release context. Error code: %d\n", ret);
        exit(1);
    }
    free(successes);
    return 0;
}
