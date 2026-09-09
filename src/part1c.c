#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/simulator.h"

#define GRAVITY 9.81

static double calculate_distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void run_part1c(Battlefield *field) {
    printf("\n=== RUNNING PART 1-C (CUMULATIVE DAMAGE & PATHS) ===\n");
    
    int battleship_sunk = 0;
    double battleship_damage = 0.0; // Cumulative damage tracker (0.0 to 1.0+)
    int total_hits_by_battleship = 0;
    double total_battle_time = 0.0;
    
    // Loop through each path point
    for (int step = 1; step <= field->k_points; step++) {
        printf("\n--- Step %d/%d ---\n", step, field->k_points);

        // Update Battleship position for this step
        field->player_ship.x_pos = ((double)rand() / RAND_MAX) * field->canvas_size;
        field->player_ship.y_pos = ((double)rand() / RAND_MAX) * field->canvas_size;

        printf("Battleship moved to coordinates: (%.2f, %.2f)\n", field->player_ship.x_pos, field->player_ship.y_pos);

        // Check gun jam status
        int is_jammed = 0;
        if (step > field->jam_step) {
            is_jammed = 1;
            printf("[WARNING] Gun jammed! Minimum vertical angle restricted to %.2f degrees.\n", field->theta_min);
        }
 
        // 1. Enemy Attack Phase: Accumulate Fractional Damage
        int step_damage_inflicted = 0;
        for (int i = 0; i < field->num_escorts; i++) {
            if (field->list_of_escort_ships[i].is_destroyed) continue;

            double distance = calculate_distance(
                field->player_ship.x_pos, field->player_ship.y_pos,
                field->list_of_escort_ships[i].x_pos, field->list_of_escort_ships[i].y_pos
            );

            double max_angle_rad = field->list_of_escort_ships[i].max_angle * (M_PI / 180.0);
            double max_range = (pow(field->list_of_escort_ships[i].max_velocity, 2) * sin(2 * max_angle_rad)) / GRAVITY;
            
            double min_angle_rad = field->list_of_escort_ships[i].min_angle * (M_PI / 180.0);
            double min_range = (pow(field->list_of_escort_ships[i].min_velocity, 2) * sin(2 * min_angle_rad)) / GRAVITY;

            if (distance >= min_range && distance <= max_range) {
                // Add cumulative damage based on escort ship impact power
                double power = field->list_of_escort_ships[i].config.default_impact;
                battleship_damage += power;
                step_damage_inflicted++;
                
                printf(" -> [HIT] Escort ID %d hit Battleship! Damage +%.2f%% (Total: %.2f%%)\n", 
                    field->list_of_escort_ships[i].id, power * 100.0, battleship_damage * 100.0);

                if (battleship_damage >= 1.0) {
                    battleship_sunk = 1;
                }
            }
        }

        if (battleship_sunk) {
            printf("\n[DEFEAT] Cumulative damage reached 100%%! The Battleship was sunk at Step %d.\n", step);
            break;
        }
        
        // 2. Battleship Counter-Attack Phase
        printf("[SURVIVED] Battleship survived enemy fire. Returning fire...\n");
        
        double b_max_range = (pow(field->player_ship.max_velocity, 2) * sin(2 * (45.0 * M_PI / 180.0))) / GRAVITY;
        
        int step_hits = 0;
        int active_threats = 0;

        for (int i = 0; i < field->num_escorts; i++) {
            if (field->list_of_escort_ships[i].is_destroyed) continue;

            active_threats++; 

            double distance = calculate_distance(
                field->player_ship.x_pos, field->player_ship.y_pos,
                field->list_of_escort_ships[i].x_pos, field->list_of_escort_ships[i].y_pos
            );

            if (distance <= b_max_range) {
                step_hits++;
                total_hits_by_battleship++;
                field->list_of_escort_ships[i].is_destroyed = 1; 

                double val = (distance * GRAVITY) / pow(field->player_ship.max_velocity, 2);
                if (val > 1.0) val = 1.0;
                
                double theta_rad = 0.5 * asin(val);
                double theta_deg = theta_rad * (180.0 / M_PI);

                if (is_jammed && theta_deg < field->theta_min) {
                    theta_deg = 90.0 - theta_deg; 
                    theta_rad = theta_deg * (M_PI / 180.0);
                }

                double flight_time = (2 * field->player_ship.max_velocity * sin(theta_rad)) / GRAVITY;
                total_battle_time += flight_time; 

                printf(" -> Escort Ship ID %d hit and destroyed at %.2f meters (Flight time: %.2fs).\n", 
                    field->list_of_escort_ships[i].id, distance, flight_time);
            }
        }

        // Save step results including cumulative damage metrics
        save_step_results_1c(field, step, is_jammed, step_hits, battleship_damage);

        if (active_threats - step_hits <= 0) {
            printf("\n[VICTORY] All escort ships neutralized along the path at Step %d!\n", step);
            break; 
        }
   }

   if (!battleship_sunk) {
        printf("\n--- SIMULATION COMPLETE ---\n");
        printf("Battleship survived the entire %d-step path.\n", field->k_points);
        printf("Final Cumulative Damage Taken: %.2f%%\n", battleship_damage * 100.0);
        printf("Total Escort Ships Destroyed: %d / %d\n", total_hits_by_battleship, field->num_escorts);
        printf("Total Time Engaging Targets: %.2f seconds\n", total_battle_time);
    }

}