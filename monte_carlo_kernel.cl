#include <CL/cl.h>

// Define constants
const size_t SEA_SIZE = 30;
const size_t MINES_COUNT = 10;
const size_t TESTS = 500000;

// Define types
typedef struct {
    cl_uint x;
    cl_uint y;
} Coord;

typedef struct {
    cl_uchar symbol;
    cl_ushort count;
} Element;

// Define kernels
__kernel void fillMatrix(__global uchar* sea, __constant uint size) {
    // Get global ID
    size_t id = get_global_id(0);
    
    // Fill matrix with water symbols
    sea[id] = '~';
}

__kernel void generateRandomElements(__global uchar* sea, __constant uint size, __constant uint elementsCount) {
    // Get global ID
    size_t id = get_global_id(0);
    
    // Generate random coordinates
    Coord coord = {
        .x = rand() % size,
        .y = rand() % size
    };
    
    // Check if coordinate already has an element
    bool found = false;
    for (size_t i = 0; i < elementsCount && !found; i++) {
        if (coord.x == sea[i].x && coord.y == sea[i].y) {
            found = true;
        }
    }
    
    // If not found, add new element
    if (!found) {
        sea[elementsCount++] = {
            .x = coord.x,
            .y = coord.y,
            .symbol = '*',
            .count = 1
        };
    }
}

__kernel void movementTill(__global uchar* sea, __constant uint size, __constant uint movementsLimit) {
    // Get global ID
    size_t id = get_global_id(0);
    
    // Initialize variables
    size_t x = 0;
    size_t y = 0;
    size_t movements = 0;
    
    // Loop until we reach the end of the sea or exceed the number of movements
    while (x < size && y < size && movements <= movementsLimit) {
        // Check if current position contains a mine
        if (sea[x][y].symbol == '*') {
            break;
        }
        
        // Update position based on random movement
        if (rand() % 2 == 0) {
            x++;
        } else {
            y++;
        }
        
        // Increment number of movements
        movements++;
    }
    
    // Write result to buffer
    sea[id].result = (x >= size || y >= size) ? 1 : 0;
}

int main() {
    // Create context and command queue
    cl_context context = clCreateContextFromType(NULL, CL_DEVICE_TYPE_GPU, NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, NULL, 0, &errcode);
    
    // Create buffers for input and output data
    cl_mem seaBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR, sizeof(uchar) * SEA_SIZE * SEA_SIZE, NULL, &errcode);
    cl_mem elementsBuffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Element) * MINES_COUNT, NULL, &errcode);
    cl_mem resultBuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR, sizeof(cl_ulong), NULL, &errcode);
    
    // Set kernel arguments
    clSetKernelArg(fillMatrix, 0, sizeof(cl_mem), &seaBuffer);
    clSetKernelArg(fillMatrix, 1, sizeof(cl_uint), &SEA_SIZE);
    clSetKernelArg(generateRandomElements, 0, sizeof(cl_mem), &seaBuffer);
    clSetKernelArg(generateRandomElements, 1, sizeof(cl_uint), &MINES_COUNT);
    clSetKernelArg(movementTill, 0, sizeof(cl_mem), &seaBuffer);
    clSetKernelArg(movementTill, 1, sizeof(cl_uint), &SEA_SIZE);
    clSetKernelArg(movementTill, 2, sizeof(cl_uint), &TESTS);
    
    // Enqueue kernels
    clEnqueueTask(queue, fillMatrix, 0, NULL, NULL);
    clEnqueueTask(queue, generateRandomElements, 0, NULL, NULL);
    clEnqueueTask(queue, movementTill, 0, NULL, NULL);
    
    // Wait for completion
    clFinish(queue);
    
    // Read results from buffer
    cl_ulong* resultPtr = (cl_ulong*)clEnqueueMapBuffer(queue, resultBuffer, CL_TRUE, CL_MAP_READ, 0, sizeof(cl_ulong), 0, NULL, NULL, &errcode);
    cl_ulong result = *resultPtr;
    clEnqueueUnmapMemObject(queue, resultBuffer, resultPtr, 0, NULL, NULL);
    
    // Print results
    printf("Number of successful movements: %llu\n", result);
    
    // Release resources
    clReleaseMemObject(seaBuffer);
    clReleaseMemObject(elementsBuffer);
    clReleaseMemObject(resultBuffer);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    
    return 0;
}
