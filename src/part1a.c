#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/simulator.h"

#define GRAVITY 9.81

// Helper function to calculate distance between two coordinates
double calculate_distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void run_part1a(Battlefield *field) {
    printf("\n=== RUNNING PART 1-A ===\n");

    int battleship_sunk = 0;
    int sinking_escort_id = -1; // ship ID numbers are positive integers 0, 1, 2..... -1 bcs no escort ship has destroyed the battleship ye
    int hits_by_battleship = 0;
    double total_battle_time = 0.0;

    // 1. Check if any Escort ship can hit the Battleship
    for (int i = 0; i < field->num_escorts; i++) {
        if (field->list_of_escort_ships[i].is_destroyed) continue; // Checks if the current escort ship is already dead; if dead the continue statement skips it

        // calculate the distance between battleship & the escortship
        double distance = calculate_distance(
            field->player_ship.x_pos, field->player_ship.y_pos,
            field->list_of_escort_ships[i].x_pos, field->list_of_escort_ships[i].y_pos
        );

        // Calculate max range for this escort ship based on its max velocity and max angle
        // Range equation: R = (u^2 * sin(2*theta)) / g
        double max_angle_rad = field->list_of_escort_ships[i].max_angle * (M_PI / 180.0); // Converts the escort ship's maximum vertical gun angle from degrees into radians
        double max_range = (pow(field->list_of_escort_ships[i].max_velocity, 2) * sin(2 * max_angle_rad)) / GRAVITY; // Calculates the maximum horizontal strike distance using the projectile range equation

        // calculate the min range for this escort ship based on its min velocity and min angle 
        double min_angle_rad = field->list_of_escort_ships[i].min_angle * (M_PI / 180.0);
        double min_range = (pow(field->list_of_escort_ships[i].min_velocity, 2) * sin(2 * min_angle_rad)) / GRAVITY;

        // In Part 1-A, if distance falls within the attack range annulus/circle, it can hit
        if (distance >= min_range && distance <= max_range) {
            battleship_sunk = 1;
            sinking_escort_id = field->list_of_escort_ships[i].id; // save the enemy's ID who hit the battle ship
            break; // Single shell impact destroys B in Part 1-A
        }
    }
    // 2. Evaluate outcome based on specification rules
    if (battleship_sunk) {
        printf("\n[DEFEAT] The Battleship was sunk by Escort Ship ID: %d\n", sinking_escort_id);
        // TODO: Save final conditions to text file via file_handler
    } else {
        printf("\n[VICTORY] The Battleship survived the engagement!\n");
        
        // Check how many escort ships the Battleship can hit
        // Battleship can launch at any angle 0-90 degrees, max range based on V_max
        double b_max_range = (pow(field->player_ship.max_velocity, 2) * sin(2 * (45.0 * M_PI / 180.0))) / GRAVITY; // Optimal angle 45 deg for max reach

        for (int i = 0; i < field->num_escorts; i++) {
            double distance = calculate_distance(
                field->player_ship.x_pos, field->player_ship.y_pos,
                field->list_of_escort_ships[i].x_pos, field->list_of_escort_ships[i].y_pos
            );

            if (distance <= b_max_range) {
                hits_by_battleship++;
                field->list_of_escort_ships[i].is_destroyed = 1; // Single hit destroys E in Part 1-A
                
                // Time of flight calculation: t_f = (2 * u * sin(theta)) / g
                // Assuming optimal or average firing angle for simulation output
                double flight_time = (2 * field->player_ship.max_velocity * sin(45.0 * M_PI / 180.0)) / GRAVITY;
                if (flight_time > total_battle_time) {
                    total_battle_time = flight_time;
                }

                printf(" -> Escort Ship ID %d hit and destroyed at distance %.2f meters.\n", 
                    field->list_of_escort_ships[i].id, distance);
            }
        }

        printf("\nTotal Escort Ships Destroyed: %d\n", hits_by_battleship);
        printf("Total Time to End Battle: %.2f seconds\n", total_battle_time);
        // TODO: Save hit details and final battlefield state to text file
    }
}