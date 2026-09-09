#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/simulator.h"

#define GRAVITY 9.81

// Helper function to calculate distance between two coordinates
double calculate_distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void run_part1b(Battlefield *field) {
    int k_points, jam_step;
    double theta_min;

    printf("\n=== RUNNING PART 1-B (PATHS & JAMS) ===\n");
    printf("Enter number of path points (k): ");
    scanf("%d", &k_points);

    printf("Enter gun jam step (t where t < %d): ", k_points);
    scanf("%d", &jam_step);

    printf("Enter restricted minimum vertical angle theta_min (0 < theta_min < 30): ");
    scanf("%lf", &theta_min);

    int battleship_sunk = 0;
    int total_hits_by_battleship = 0;
    double total_battle_time = 0.0;
    
    // Loop through each path point (Simulation 1 & 2 integration)
    for (int step = 1; step <= k_points; step++) {
        printf("\n--- Step %d/%d ---\n", step, k_points);

        // Generate or update Battleship position for this step (e.g., random or linear shift)
        field->player_ship.x_pos = ((double)rand() / RAND_MAX) * field->canvas_size;
        field->player_ship.y_pos = ((double)rand() / RAND_MAX) * field->canvas_size;

        printf("Battleship moved to coordinates: (%.2f, %.2f)\n", field->player_ship.x_pos, field->player_ship.y_pos);

        // Check if gun jam is active (Simulation 2 rule)
        int is_jammed = 0;
        if (step > jam_step) {
            is_jammed = 1;
            printf("[WARNING] Gun jammed! Minimum vertical angle restricted to %.2f degrees.\n", theta_min);
        }

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

            // check if distance falls within the attack range annulus/circle, it can hit
            if (distance >= min_range && distance <= max_range) {
                battleship_sunk = 1;
                printf("[DEFEAT] The Battleship was sunk at Step %d by Escort Ship ID: %d\n", step, field->list_of_escort_ships[i].id);
                break; // Break the inner loop, Battleship is destroyed
            }
        }

        // If the Battleship was destroyed, break the multi-step path loop completely
        if (battleship_sunk) {
            break;
        }

        // 2. Battleship Counter-Attack Phase
        printf("[SURVIVED] Battleship survived enemy fire. Returning fire...\n");

        // Calculate max range based on optimal 45 degree angle
        double b_max_range = (pow(field->player_ship.max_velocity, 2) * sin(2 * (45.0 * M_PI / 180.0))) / GRAVITY;

        int step_hits = 0;
        int active_threats = 0;

        for (int i = 0; i < field->num_escorts; i++)
        {
            if (field->list_of_escort_ships[i].is_destroyed) continue;// skips the rest of the steps if the enemy is destroyed

            active_threats++; // Count how many enemies are currently alive

            double distance = calculate_distance(
                field->player_ship.x_pos, field->player_ship.y_pos,
                field->list_of_escort_ships[i].x_pos, field->list_of_escort_ships[i].y_pos
            );

            if (distance <= b_max_range)
            {
                step_hits++;
                total_hits_by_battleship++;
                field->list_of_escort_ships[i].is_destroyed = 1;

                // --- NEW GUN JAM & TIME LOGIC ---
                // Calculate the lower angle required to hit this exact distance
                double val = (distance * GRAVITY) / pow(field->player_ship.max_velocity, 2);
                if (val > 1.0) val = 1.0; // Math safety for floating point rounding
            }
            
        }
        
    }
}