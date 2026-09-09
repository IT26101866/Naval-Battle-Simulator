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
    }

}