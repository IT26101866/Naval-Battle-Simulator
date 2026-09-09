#include <stdio.h>
#include <stdlib.h>
#include "../include/simulator.h"

void save_initial_conditions(Battlefield *field) {
    // Open file in write mode. It creates the file if it doesn't exist.
    FILE *file = fopen("data/initial_conditions.txt", "w");
    
    if (file == NULL) {
        printf("[ERROR] Could not open data/initial_conditions.txt for writing.\n");
        return;
    }

    fprintf(file, "=== NAVAL BATTLE SIMULATOR: INITIAL CONDITIONS ===\n\n");
    
    fprintf(file, "--- BATTLEFIELD ---\n");
    fprintf(file, "Canvas Size: %.2f x %.2f\n", field->canvas_size, field->canvas_size);
    fprintf(file, "Total Escort Ships (N): %d\n\n", field->num_escorts);

    fprintf(file, "--- BATTLESHIP (ALLIED) ---\n");
    fprintf(file, "Type: %s [%s]\n", field->player_ship.config.ship_name, field->player_ship.config.notation);
    fprintf(file, "Gun: %s\n", field->player_ship.config.gun_name);
    fprintf(file, "Position: (%.2f, %.2f)\n", field->player_ship.x_pos, field->player_ship.y_pos);
    fprintf(file, "Max Velocity: %.2f m/s\n\n", field->player_ship.max_velocity);

    fprintf(file, "--- ESCORT SHIPS (AXIS) ---\n");
    for (int i = 0; i < field->num_escorts; i++) {
        fprintf(file, "ID: %d | Type: %s [%s] | Gun: %s\n", 
            field->list_of_escort_ships[i].id,
            field->list_of_escort_ships[i].config.type_name,
            field->list_of_escort_ships[i].config.notation,
            field->list_of_escort_ships[i].config.gun_name);
            
        fprintf(file, "  Position: (%.2f, %.2f)\n", field->list_of_escort_ships[i].x_pos, field->list_of_escort_ships[i].y_pos);
        fprintf(file, "  Velocity Range: %.2f m/s to %.2f m/s\n", field->list_of_escort_ships[i].min_velocity, field->list_of_escort_ships[i].max_velocity);
        fprintf(file, "  Angle Range: %.2f deg to %.2f deg\n", field->list_of_escort_ships[i].min_angle, field->list_of_escort_ships[i].max_angle);
        fprintf(file, "  Impact Power: %.2f\n\n", field->list_of_escort_ships[i].current_impact_power);
    }

    fclose(file);
    printf("\n[SUCCESS] Initial conditions saved to 'data/initial_conditions.txt'\n");
}

void save_step_results(Battlefield *field, int step, int is_jammed, int step_hits) {
    char filename[100];
    // Create a dynamic filename like "data/step_1_results.txt"
    sprintf(filename, "data/step_%d_results.txt", step);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("[ERROR] Could not open step file for writing");
        return;
    }

    fprintf(file, "=== SIMULATION STEP %d ===\n", step);
    fprintf(file, "Battleship Position: (%.2f, %.2f)\n", field->player_ship.x_pos, field->player_ship.y_pos);
    
    if (is_jammed) {
        fprintf(file, "Gun Status: JAMMED (Restricted Arc)\n");
    } else {
        fprintf(file, "Gun Status: NORMAL\n");
    }

    fprintf(file, "Escort Ships Destroyed in this step: %d\n", step_hits);
    fprintf(file, "\n--- ESCORT SHIP STATUS ---\n");
    
    int active_threats = 0;
    for (int i = 0; i < field->num_escorts; i++) {
        if (field->list_of_escort_ships[i].is_destroyed) {
            fprintf(file, "ID %d: DESTROYED\n", field->list_of_escort_ships[i].id);
        } else {
            active_threats++;
            fprintf(file, "ID %d: ALIVE | Position: (%.2f, %.2f)\n", 
                field->list_of_escort_ships[i].id, 
                field->list_of_escort_ships[i].x_pos, 
                field->list_of_escort_ships[i].y_pos);
        }
    }
    
    fprintf(file, "\nTotal Active Threats Remaining: %d\n", active_threats);
    fclose(file);
    printf("[SUCCESS] Step %d results saved to '%s'\n", step, filename);
}