#include <stdio.h>
#include <stdlib.h>
#include <time.h> // To read computer'sclock time
#include "../include/simulator.h"

void configure_random_seed(){
    int seed_value;
    
    printf("\n-----------CONFIGURE RANDOM SEED-----------\n");
    printf("Enter a specific number for same repeated generation or just enter 0 for completely random\n");
    printf("\n>");

    scanf("%d", &seed_value);

    if (seed_value == 0)
    {
        // time(NULL) grabs the current time down to the second.
        // srand() stands for "seed random". It feeds that time into the generator
        srand(time(NULL)); // using NULL because we are passing it to an empty memory address
        printf("\n[SUCCESS] Random generator seeded with current system time\n");
    }else 
    {
        srand(seed_value);
        printf("\n[SUCCESS] Random generator seeded with value : %d", seed_value);
    }    
}