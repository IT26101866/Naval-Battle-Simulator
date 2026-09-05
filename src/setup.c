#include <stdio.h>
#include <stdlib.h>
#include <time.h> // To read computer'sclock time
#include <string.h>
#include <math.h>
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
void configure_battleship(Battlefield *field) {
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
        strcpy(field->player_ship.config.notation, "U");
        strcpy(field->player_ship.config.ship_name, "USS Iowa (BB-61)");
        strcpy(field->player_ship.config.gun_name, "50-caliber Mark 7 gun");
        printf("\n[SUCCESS] Selected: USS Iowa (BB-61)\n");
        break;
    case 2:
        strcpy(field->player_ship.config.notation, "M");
        strcpy(field->player_ship.config.ship_name, "MS King George V");
        strcpy(field->player_ship.config.gun_name, "(356 mm) Mark VII gun");
        printf("\n[SUCCESS] Selected: MS King George V\n");
        break;
    case 3:
        strcpy(field->player_ship.config.notation, "R");
        strcpy(field->player_ship.config.ship_name, "Richelieu");
        strcpy(field->player_ship.config.gun_name, "(15 inch) Mle 1935 gun");
        printf("\n[SUCCESS] Selected: Richelieu\n");
        break;
    case 4:
        strcpy(field->player_ship.config.notation, "S");
        strcpy(field->player_ship.config.ship_name, "Sovetsky Soyuz-class");
        strcpy(field->player_ship.config.gun_name, "(16 inch) B-37 gun");
        printf("\n[SUCCESS] Selected: Sovetsky Soyuz-class\n");
        break;    
    default:
        strcpy(field->player_ship.config.notation, "U");
        strcpy(field->player_ship.config.ship_name, "USS Iowa (BB-61)");
        strcpy(field->player_ship.config.gun_name, "50-caliber Mark 7 gun");
        printf("\n[ERROR] Invalid selection. Defaulting to USS Iowa.\n");
        break;
    }
}

// Generate random properties for escort ships based on Table 1 rules
void generate_escort_ships(Battlefield *field) {
    char types[] = {'A', 'B', 'C', 'D', 'E'}; // 5 possible escort ship subtypes
    
    for (int i = 0; i < field->num_escorts; i++) {
        field->list_of_escort_ships[i].id = i + 1; // Unique identifier index
        
        // Randomly select one of the 5 escort types
        int type_index = rand() % 5; // divides a massive random number by 5 and takes the remainder, guaranteeing a result of 0, 1, 2, 3, or 4.
        field->list_of_escort_ships[i].config.notation[0] = 'E'; // every escortship type starts with E
        field->list_of_escort_ships[i].config.notation[1] = types[type_index]; // A, B, C, D, E (eg. EA)
        field->list_of_escort_ships[i].config.notation[2] = '\0'; // null terminator

        // Assign impact power based on escortship type
        // NOTE : types[type_index] this is an array inside another array here type_index returns value from 0 to 4 according to that type will return A to E
        switch (types[type_index]) {
            case 'A': field->list_of_escort_ships[i].config.default_impact = 0.08; break;
            case 'B': field->list_of_escort_ships[i].config.default_impact = 0.06; break;
            case 'C': field->list_of_escort_ships[i].config.default_impact = 0.07; break;
            case 'D': field->list_of_escort_ships[i].config.default_impact = 0.05; break;
            case 'E': field->list_of_escort_ships[i].config.default_impact = 0.04; break;
        }

        // Assign angle range based on escortship type
         // NOTE : types[type_index] this is an array inside another array here type_index returns value from 0 to 4 according to that type will return A to E
        switch (types[type_index]) {
            case 'A': field->list_of_escort_ships[i].config.angle_range = 20; break;
            case 'B': field->list_of_escort_ships[i].config.angle_range = 30; break;
            case 'C': field->list_of_escort_ships[i].config.angle_range = 25; break;
            case 'D': field->list_of_escort_ships[i].config.angle_range = 50; break;
            case 'E': field->list_of_escort_ships[i].config.angle_range = 70; break;
        }

        // Random coordinates within the square canvas (0 to D)
        field->list_of_escort_ships[i].x_pos = ((double)rand() / RAND_MAX) * field->canvas_size;
        field->list_of_escort_ships[i].y_pos = ((double)rand() / RAND_MAX) * field->canvas_size;

        // Random angle and velocity bounds
        field->list_of_escort_ships[i].min_angle = (rand() % 30) + 5; // minimum angle ranges from 5 to 34
        field->list_of_escort_ships[i].max_angle = field->list_of_escort_ships[i].min_angle + field->list_of_escort_ships[i].config.angle_range; 
        
        field->list_of_escort_ships[i].min_velocity = (rand() % 50) + 10;
        field->list_of_escort_ships[i].max_velocity = field->list_of_escort_ships[i].min_velocity + ((rand() % 100) + 50);
        
        field->list_of_escort_ships[i].is_destroyed = 0; 
        field->list_of_escort_ships[i].gamma_value = 0.02; 
    }
}