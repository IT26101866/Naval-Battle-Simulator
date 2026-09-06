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

}