#define RAND_A 1103515245
#define RAND_C 12345
#define RAND_M (1 << 31)

#define movement_limit 50000

#pragma OPENCL EXTENSION cl_amd_printf : enable

inline int lcg_rand(int seed) {
  return (RAND_A * seed + RAND_C) % RAND_M; 
}

__kernel void fillMatrix(__global char* sea, int sea_size, 
                         char fill_symbol) {
  
  int i = get_global_id(0);
  int x = i / sea_size;
  int y = i % sea_size;
  
  sea[x * sea_size + y] = fill_symbol; 
}

__kernel void generateRandomElements(__global char* sea, int sea_size,  
                               char element_symbol, int elements_count) {
  
  int i = get_global_id(0);

  if(i < elements_count) {
    int seed = i;
    int x = lcg_rand(seed) % sea_size;
    seed = lcg_rand(seed);
    int y = lcg_rand(seed) % sea_size;

    while(sea[x * sea_size + y] == element_symbol) {
      seed = lcg_rand(seed);
      x = lcg_rand(seed) % sea_size;
      seed = lcg_rand(seed);
      y = lcg_rand(seed) % sea_size;
    }

    sea[x * sea_size + y] = element_symbol;
  }
}

__kernel void movementTill(__global char* sea, int sea_size, char destination_symbol, char failure_symbol, __global int* successes) {
    int i = get_global_id(0);
    int x = 0;
    int y = 0;
    printf("i = %d\n", i);
    int seed = i;  // Use the global ID as a seed
    for (int j = 0; j < movement_limit; ++j) {
        if (sea[x * sea_size + y] == failure_symbol) return;
        if (sea[x * sea_size + y] == destination_symbol) atomic_inc(successes);

        if (x == sea_size - 1) {
            ++y;
            continue;
        }
        if (y == sea_size - 1) {
            ++x;
            continue;
        }

        seed = lcg_rand(seed);
        if (lcg_rand(seed) % 2) {
            ++x;
        } else {
            ++y;
        }
    }
}