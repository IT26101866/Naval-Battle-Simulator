#include <stdio.h>
#include <stdlib.h>
#include <time.h> // To read computer'sclock time
#include "../include/simulator.h"

// Seed Configuration
void configure_random_seed() {
    int seed_value;
    
    printf("\n----------- CONFIGURE RANDOM SEED -----------\n");
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

// Battleship selection Interactive Menu
void configure_battleship() {
    int choice;

    printf("\n------ CONFIGURE BATTLESHIP PROPERTIES ------\n");
    printf("Select Battleship Type:\n");
    printf("--> 1. USS Iowa (BB-61) [Notation: U]\n");
    printf("--> 2. MS King George V [Notation: M]\n");
    printf("--> 3. Richelieu [Notation: R]\n");
    printf("--> 4. Sovetsky Soyuz-class [Notation: S]\n");
    printf("Enter choice (1-4): ");

    scanf("%d", &choice);
    // initialize the player ship based on this choice
    switch (choice)
    {
    case 1:
        printf("\n[Success] Selected: USS Iowa (BB-61)\n");
        // Set up configurations here later
        break;
    case 2:
        printf("\n[Success] Selected: MS King George V\n");
        break;
    case 3:
        printf("\n[Success] Selected: Richelieu\n");
        break;
    case 4:
        printf("\n[Success] Selected: Sovetsky Soyuz-class\n");
        break;    
    default:
        printf("\n[Error] Invalid selection. Defaulting to USS Iowa.\n");
        break;
    }
}