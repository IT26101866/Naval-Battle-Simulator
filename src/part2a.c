#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/simulator.h"

#define GRAVITY 9.81

// Struct to track targets for sorting
typedef struct {
    int array_index;
    int ship_id;
    double distance;
    double threat_score;
} TargetThreat;

static double calculate_distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Comparison function for qsort (sorts descending by threat score)
static int compare_threats(const void *a, const void *b) {
    TargetThreat *t1 = (TargetThreat *)a;
    TargetThreat *t2 = (TargetThreat *)b;
    if (t1->threat_score < t2->threat_score) return 1;
    if (t1->threat_score > t2->threat_score) return -1;
    return 0;
}

void run_part2a(Battlefield *field) {
    printf("\n=== RUNNING PART 2-A (TARGETING STRATEGY & RELOADS) ===\n");
    
    int battleship_sunk = 0;
    double battleship_damage = 0.0; 
    int total_hits_by_battleship = 0;
    double total_battle_time = 0.0;

    for (int step = 1; step <= field->k_points; step++)
    {
        printf("\n--- Step %d/%d ---\n", step, field->k_points);

        field->player_ship.x_pos = ((double)rand() / RAND_MAX) * field->canvas_size;
        field->player_ship.y_pos = ((double)rand() / RAND_MAX) * field->canvas_size;

        printf("Battleship moved to coordinates: (%.2f, %.2f)\n", field->player_ship.x_pos, field->player_ship.y_pos);

        int is_jammed = 0;
        if (step > field->jam_step) {
            is_jammed = 1;
            printf("[WARNING] Gun jammed! Restricted to %.2f degrees.\n", field->theta_min);
        }

        // 1. Enemy Attack Phase (Part 1-C Cumulative Logic)
        for (int i = 0; i < field->num_escorts; i++)
        {
            if (field->list_of_escort_ships[i].is_destroyed) continue;

            double distance = calculate_distance(field->player_ship.x_pos, field->player_ship.y_pos,
                                                 field->list_of_escort_ships[i].x_pos, field->list_of_escort_ships[i].y_pos);

            double max_angle_rad = field->list_of_escort_ships[i].max_angle * (M_PI / 180.0);
            double max_range = (pow(field->list_of_escort_ships[i].max_velocity, 2) * sin(2 * max_angle_rad)) / GRAVITY;
            double min_angle_rad = field->list_of_escort_ships[i].min_angle * (M_PI / 180.0);
            double min_range = (pow(field->list_of_escort_ships[i].min_velocity, 2) * sin(2 * min_angle_rad)) / GRAVITY;

            if (distance >= min_range && distance <= max_range) {
                double power = field->list_of_escort_ships[i].config.default_impact;
                battleship_damage += power;
                printf(" -> [HIT] Escort ID %d hit Battleship! Damage +%.2f%% (Total: %.2f%%)\n", 
                    field->list_of_escort_ships[i].id, power * 100.0, battleship_damage * 100.0);

                if (battleship_damage >= 1.0) battleship_sunk = 1;
            }
        }
        
        if (battleship_sunk) {
            printf("\n[DEFEAT] Cumulative damage reached 100%%! Sunk at Step %d.\n", step);
            break;
        }

        // 2. Battleship Counter-Attack Strategy Phase (Part 2-A)
        printf("[STRATEGY] Scanning for targets and calculating Threat Scores...\n");
        double b_max_range = (pow(field->player_ship.max_velocity, 2) * sin(2 * (45.0 * M_PI / 180.0))) / GRAVITY;
        
        TargetThreat targets[field->num_escorts];
        int target_count = 0;

        // Scan Phase
        for (int i = 0; i < field->num_escorts; i++) {
            if (field->list_of_escort_ships[i].is_destroyed) continue;

            double distance = calculate_distance(field->player_ship.x_pos, field->player_ship.y_pos,
                                                 field->list_of_escort_ships[i].x_pos, field->list_of_escort_ships[i].y_pos);

            if (distance <= b_max_range) {
                targets[target_count].array_index = i;
                targets[target_count].ship_id = field->list_of_escort_ships[i].id;
                targets[target_count].distance = distance;
                // THREAT CALCULATION: High power and close distance = High Threat
                targets[target_count].threat_score = field->list_of_escort_ships[i].config.default_impact / distance;
                target_count++;
            }
        }

        int attack_order_ids[field->num_escorts]; // Array to pass to file handler

        if (target_count > 0) {
            // Sort targets descending by threat score
            qsort(targets, target_count, sizeof(TargetThreat), compare_threats);

            printf("[ACTION] Engaging targets in optimized order:\n");
            for (int t = 0; t < target_count; t++) {
                int i = targets[t].array_index;
                attack_order_ids[t] = targets[t].ship_id;
                
                field->list_of_escort_ships[i].is_destroyed = 1;
                total_hits_by_battleship++;

                double val = (targets[t].distance * GRAVITY) / pow(field->player_ship.max_velocity, 2);
                if (val > 1.0) val = 1.0;
                
                double theta_rad = 0.5 * asin(val);
                double theta_deg = theta_rad * (180.0 / M_PI);

                if (is_jammed && theta_deg < field->theta_min) {
                    theta_deg = 90.0 - theta_deg; 
                    theta_rad = theta_deg * (M_PI / 180.0);
                }

                double flight_time = (2 * field->player_ship.max_velocity * sin(theta_rad)) / GRAVITY;
                
                // Add reload time for consecutive shots (skip for the very first shot)
                double reload_penalty = (t > 0) ? field->reload_time : 0.0;
                total_battle_time += (flight_time + reload_penalty); 

                printf(" -> [FIRE %d] Destroyed ID %d | Score: %.6f | Distance: %.2fm | Reload Delay: %.1fs\n", 
                    t + 1, targets[t].ship_id, targets[t].threat_score, targets[t].distance, reload_penalty);
            }
        }
    }
    
}