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

        // Assign impact power & angle range based on escortship type
        // NOTE : types[type_index] this is an array inside another array here type_index returns value from 0 to 4 according to that type will return A to E
        switch (types[type_index]) 
        {
        case 'A':
            strcpy(field->list_of_escort_ships[i].config.type_name, "1936A-class Destroyer");
            strcpy(field->list_of_escort_ships[i].config.gun_name, "SK C/34 naval gun");
            field->list_of_escort_ships[i].config.default_impact = 0.08;
            field->list_of_escort_ships[i].config.angle_range = 20; 
            break;
        case 'B': 
            strcpy(field->list_of_escort_ships[i].config.type_name, "Gabbiano-class Corvette");
            strcpy(field->list_of_escort_ships[i].config.gun_name, "L/47 dual-purpose gun");
            field->list_of_escort_ships[i].config.default_impact = 0.06; 
            field->list_of_escort_ships[i].config.angle_range = 30;
            break;
        case 'C':
            strcpy(field->list_of_escort_ships[i].config.type_name, "Matsu-class Destroyer");
            strcpy(field->list_of_escort_ships[i].config.gun_name, "Type 89 dual-purpose gun");
            field->list_of_escort_ships[i].config.default_impact = 0.07; 
            field->list_of_escort_ships[i].config.angle_range = 25;
            break;
        case 'D': 
            strcpy(field->list_of_escort_ships[i].config.type_name, "F-class Escort Ships");
            strcpy(field->list_of_escort_ships[i].config.gun_name, "SK C/32 naval gun");
            field->list_of_escort_ships[i].config.default_impact = 0.05; 
            field->list_of_escort_ships[i].config.angle_range = 50;
            break;
        case 'E':
            strcpy(field->list_of_escort_ships[i].config.type_name, "Japanese Kaibōkan");
            strcpy(field->list_of_escort_ships[i].config.gun_name, "(4.7 inch) naval gun");
            field->list_of_escort_ships[i].config.default_impact = 0.04; 
            field->list_of_escort_ships[i].config.angle_range = 70;
            break;
        }

        // Random coordinates within the square canvas (0 to D)
        field->list_of_escort_ships[i].x_pos = ((double)rand() / RAND_MAX) * field->canvas_size;
        field->list_of_escort_ships[i].y_pos = ((double)rand() / RAND_MAX) * field->canvas_size;

        // Random angle bounds
        // Generate a valid minimum and maximum firing angle.
        // The maximum angle must not exceed 90 degrees.
        double angle_range = field->list_of_escort_ships[i].config.angle_range;
        double max_possible_min_angle = 90.0 - angle_range;

        field->list_of_escort_ships[i].min_angle = 5.0 + (rand() % (int)(max_possible_min_angle - 5.0 + 1));
        field->list_of_escort_ships[i].max_angle = field->list_of_escort_ships[i].min_angle + angle_range; 

        // Generate minimum velocity first.
        field->list_of_escort_ships[i].min_velocity = 50.0 + (rand() % 101);

        // Max velocity logic since EA escortship's max velocity is not random
        if (types[type_index] == 'A')
        {
            // EA has a special maximum velocity:
            // Vmax = 1.2 × Battleship Vmax
            field->list_of_escort_ships[i].max_velocity = field->player_ship.max_velocity * 1.2;
        }else
        {
            double e_min_v = field->list_of_escort_ships[i].min_velocity;
            double b_max_v = field->player_ship.max_velocity;

            // Ensure there is room for Vmax to be greater than Vmin while still remaining below Battleship Vmax
            if (e_min_v >= b_max_v - 1)
            {
                e_min_v = b_max_v - 2;
                field->list_of_escort_ships[i].min_velocity = e_min_v;
            }

            // Generate Vmax between Vmin + 1 and B Vmax - 1
            field->list_of_escort_ships[i].max_velocity = e_min_v + 1 + (rand() % (int)(b_max_v - e_min_v - 1));
            
        }

        field->list_of_escort_ships[i].is_destroyed = 0; 
        field->list_of_escort_ships[i].gamma_value = 0.02; 

        field->list_of_escort_ships[i].current_impact_power = field->list_of_escort_ships[i].config.default_impact;
    }
}

// Initialize canvas size, escort count, and battleship starting position
void initialize_battlefield(Battlefield *field) {
    printf("\n----------- BATTLEFIELD SETUP -----------\n");
    
    // Canvas size D
    printf("--> Enter canvas upper-right coordinate D (e.g., 5000 for a 5000x5000 grid): ");
    scanf("%lf", &field->canvas_size);

    // Number of escort ships N
    printf("--> Enter number of escort ships (N): ");
    scanf("%d", &field->num_escorts);

    // Battleship starting position
    printf("--> Enter Battleship X coordinate (0 to %.2f): ", field->canvas_size);
    scanf("%lf", &field->player_ship.x_pos);
    printf("--> Enter Battleship Y coordinate (0 to %.2f): ", field->canvas_size);
    scanf("%lf", &field->player_ship.y_pos);

    // Battleship max velocity
    printf("--> Enter Battleship maximum shell velocity (V_max): ");
    scanf("%lf", &field->player_ship.max_velocity);

    field->player_ship.current_health = 1.0; 
    field->player_ship.gamma_value = 0.001;  

    // Allocate memory for escort ships dynamically using malloc
    field->list_of_escort_ships = (EscortShip *)malloc(field->num_escorts * sizeof(EscortShip));
    
    if (field->list_of_escort_ships == NULL) {
        printf("[ERROR] Memory allocation failed for escort ships!\n");
        exit(1);
    }

    generate_escort_ships(field);
    printf("\n[SUCCESS] Battlefield successfully configured with %d escort ships!\n", field->num_escorts);
}