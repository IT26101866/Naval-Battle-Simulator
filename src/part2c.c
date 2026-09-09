#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/simulator.h"

#define BATTLESHIP_INITIAL_IMPACT 1.0

// Calculate impact power using:IPn = IP0 * e^(-gamma * n)
static double calculate_impact_power(double initial_impact,
                                     double gamma,
                                     int firing_count)
{
    return initial_impact * exp(-gamma * firing_count);
}

//Get a readable ship type from notation
static int get_escort_type_index(EscortShip *ship)
{
    switch (ship->config.notation[1])
    {
        case 'A':
            return 0;

        case 'B':
            return 1;

        case 'C':
            return 2;

        case 'D':
            return 3;

        case 'E':
            return 4;

        default:
            return -1;
    }
}

static void configure_gamma_values(Battlefield *field)
{
    printf("\n========================================================\n");
    printf("         PART 2-C: GAMMA DEGRADATION CONFIGURATION       \n");
    printf("========================================================\n");

    printf("Gamma controls impact power degradation over time.\n");
    printf("Formula: IPn = IP0 * e^(-gamma * n)\n");

    printf("\n[1] Enter gamma values manually\n");
    printf("[2] Generate gamma values randomly\n");
    printf("\nEnter choice (1 or 2): ");

    int choice;
    scanf("%d", &choice);

    if (choice == 1)
    {
        // Battleship gamma 
        do
        {
            printf("\nEnter Battleship gamma value (close to 0): ");
            scanf("%lf", &field->player_ship.gamma_value);

            if (field->player_ship.gamma_value < 0.0)
            {
                printf("[ERROR] Gamma cannot be negative. Try again.\n");
            }

        } while (field->player_ship.gamma_value < 0.0);

        double gamma_A, gamma_B, gamma_C, gamma_D, gamma_E;

        printf("\nEnter degradation gamma factors for each Escort Type:\n");
        do { printf("  -> Gamma for EA: "); scanf("%lf", &gamma_A); } while (gamma_A < 0.0);
        do { printf("  -> Gamma for EB: "); scanf("%lf", &gamma_B); } while (gamma_B < 0.0);
        do { printf("  -> Gamma for EC: "); scanf("%lf", &gamma_C); } while (gamma_C < 0.0);
        do { printf("  -> Gamma for ED: "); scanf("%lf", &gamma_D); } while (gamma_D < 0.0);
        do { printf("  -> Gamma for EE: "); scanf("%lf", &gamma_E); } while (gamma_E < 0.0);

        //Apply gamma to every escort according to its type
        for (int i = 0; i < field->num_escorts; i++)
        {
            switch (field->list_of_escort_ships[i].config.notation[1])
            {
                case 'A': field->list_of_escort_ships[i].gamma_value = gamma_A; break;
                case 'B': field->list_of_escort_ships[i].gamma_value = gamma_B; break;
                case 'C': field->list_of_escort_ships[i].gamma_value = gamma_C; break;
                case 'D': field->list_of_escort_ships[i].gamma_value = gamma_D; break;
                case 'E': field->list_of_escort_ships[i].gamma_value = gamma_E; break;
            }
        }

        printf("\n[SUCCESS] Gamma values configured manually.\n");
    }
    else
    {
        if (choice != 2) {
            printf("\n[ERROR] Invalid choice. Defaulting to random configuration.\n");
        } else {
            printf("\n[SUCCESS] Gamma values generated randomly.\n");
        }

        field->player_ship.gamma_value = ((double)rand() / RAND_MAX) * 0.005;

        double gamma_values[5];
        for (int i = 0; i < 5; i++)
        {
            // Generate escort gamma between 0.02 and 0.10
            gamma_values[i] = 0.02 + ((double)rand() / RAND_MAX) * 0.08;
        }

        for (int i = 0; i < field->num_escorts; i++)
        {
            int type_index = get_escort_type_index(&field->list_of_escort_ships[i]);
            if (type_index >= 0)
            {
                field->list_of_escort_ships[i].gamma_value = gamma_values[type_index];
            }
        }
    }

    printf("\n---------------- FINAL GAMMA SUMMARY ----------------\n");
    printf(" Battleship Gamma : %.6f\n", field->player_ship.gamma_value);

    for (int type = 0; type < 5; type++)
    {
        char required_type = 'A' + type;
        int printed = 0;

        for (int i = 0; i < field->num_escorts; i++)
        {
            if (field->list_of_escort_ships[i].config.notation[1] == required_type)
            {
                printf(" E%c Class Gamma    : %.6f\n", required_type, field->list_of_escort_ships[i].gamma_value);
                printed = 1;
                break;
            }
        }
        if (!printed)
        {
            printf(" E%c Class Gamma    : [Not present in simulation]\n", required_type);
        }
    }
    printf("-----------------------------------------------------\n");
}

//   Save Part 2-C step results
static void save_step_results_2c(
    Battlefield *field,
    int step,
    int is_jammed,
    int step_hits,
    double cumulative_damage,
    double battleship_impact,
    int battleship_firings)
{
    char filename[100];
    sprintf(filename, "data/step_%d_results_2c.txt", step);

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("[ERROR] Could not open Part 2-C result file");
        return;
    }

    fprintf(file, "=== SIMULATION STEP %d (PART 2-C) ===\n\n", step);
    fprintf(file, "--- BATTLEFIELD STATUS ---\n");
    fprintf(file, "Battleship Position: (%.2f, %.2f)\n", field->player_ship.x_pos, field->player_ship.y_pos);
    fprintf(file, "Battleship Health: %.2f%%\n", field->player_ship.current_health * 100.0);
    fprintf(file, "Cumulative Damage Taken: %.2f%%\n", cumulative_damage * 100.0);

    if (field->player_ship.current_health > 0.0)
        fprintf(file, "Battleship Status: ALIVE\n");
    else
        fprintf(file, "Battleship Status: DESTROYED\n");

    fprintf(file, "\n--- BATTLESHIP IMPACT POWER ---\n");
    fprintf(file, "Initial Impact Power (IP0): %.6f\n", BATTLESHIP_INITIAL_IMPACT);
    fprintf(file, "Gamma: %.6f\n", field->player_ship.gamma_value);
    fprintf(file, "Number of Firings (n): %d\n", battleship_firings);
    fprintf(file, "Current Impact Power (IPn): %.6f\n", battleship_impact);

    if (is_jammed)
        fprintf(file, "Gun Status: JAMMED (Restricted Arc)\n");
    else
        fprintf(file, "Gun Status: NORMAL\n");

    fprintf(file, "\nEscort Ships Destroyed in This Step: %d\n", step_hits);
    fprintf(file, "\n--- ESCORT SHIP STATUS ---\n");

    int active_threats = 0;
    for (int i = 0; i < field->num_escorts; i++)
    {
        EscortShip *ship = &field->list_of_escort_ships[i];
        fprintf(file, "\nID %d | Type: %s [%s]\n", ship->id, ship->config.type_name, ship->config.notation);
        fprintf(file, "  Gamma: %.6f\n", ship->gamma_value);
        fprintf(file, "  Current Impact Power: %.6f\n", ship->current_impact_power);

        if (ship->is_destroyed)
        {
            fprintf(file, "  Status: DESTROYED\n");
        }
        else
        {
            active_threats++;
            fprintf(file, "  Status: ALIVE\n");
            fprintf(file, "  Position: (%.2f, %.2f)\n", ship->x_pos, ship->y_pos);
        }
    }

    fprintf(file, "\nTotal Active Threats Remaining: %d\n", active_threats);
    fclose(file);

    printf(" -> [LOG SAVED] Step %d results written to '%s'\n", step, filename);
}

static int find_active_target(Battlefield *field)
{
    for (int i = 0; i < field->num_escorts; i++)
    {
        if (!field->list_of_escort_ships[i].is_destroyed)
        {
            return i;
        }
    }
    return -1;
}

// Check whether every escort ship is destroyed
static int all_escorts_destroyed(Battlefield *field)
{
    for (int i = 0; i < field->num_escorts; i++)
    {
        if (!field->list_of_escort_ships[i].is_destroyed)
        {
            return 0;
        }
    }
    return 1;
}

// PART 2-C MAIN SIMULATION
void run_part2c(Battlefield *field)
{
    if (field == NULL)
    {
        printf("[ERROR] Battlefield pointer is NULL.\n");
        return;
    }

    if (field->num_escorts <= 0 || field->list_of_escort_ships == NULL)
    {
        printf("[ERROR] No escort ships are configured.\n");
        return;
    }

    // Configure gamma values before starting.
    configure_gamma_values(field);

    int *escort_firings = malloc(field->num_escorts * sizeof(int));
    double *escort_damage = malloc(field->num_escorts * sizeof(double));

    if (escort_firings == NULL || escort_damage == NULL)
    {
        printf("[ERROR] Memory allocation failed.\n");
        free(escort_firings);
        free(escort_damage);
        return;
    }

    // Initialise Part 2C variables
    for (int i = 0; i < field->num_escorts; i++)
    {
        escort_firings[i] = 0;
        escort_damage[i] = 0.0;
        field->list_of_escort_ships[i].is_destroyed = 0;
        field->list_of_escort_ships[i].current_impact_power = field->list_of_escort_ships[i].config.default_impact;
    }

    field->player_ship.current_health = 1.0;

    int battleship_firings = 0;
    double battleship_impact = BATTLESHIP_INITIAL_IMPACT;
    double cumulative_damage = 0.0;
    int total_steps = field->k_points;

    if (total_steps <= 0)
    {
        total_steps = 100;
    }

    printf("\n========================================================\n");
    printf("            RUNNING PART-2C COMBAT SIMULATION           \n");
    printf("========================================================\n");
    printf(" Max Simulation Steps : %d\n", total_steps);
    printf(" Initial Fleet Size   : %d Escort Ships\n", field->num_escorts);
    printf("========================================================\n");

    for (int step = 1; step <= total_steps; step++)
    {
        int step_hits = 0;
        int is_jammed = 0;

        printf("\n--------------------------------------------------------\n");
        printf("                       STEP %d / %d                       \n", step, total_steps);
        printf("--------------------------------------------------------\n");

        // --- ENEMY FIRING PHASE ---
        for (int i = 0; i < field->num_escorts; i++)
        {
            EscortShip *escort = &field->list_of_escort_ships[i];

            if (escort->is_destroyed)
            {
                continue;
            }

            escort_firings[i]++;

            // Calculate degraded impact power
            escort->current_impact_power = calculate_impact_power(
                escort->config.default_impact,
                escort->gamma_value,
                escort_firings[i]
            );

            // Apply impact to Battleship health
            field->player_ship.current_health -= escort->current_impact_power;
            cumulative_damage += escort->current_impact_power;

            printf("[INCOMING] Escort ID %d [%s] fired salvo #%d\n", 
                   escort->id, escort->config.notation, escort_firings[i]);
            printf("           ├── Gamma Rating   : %.6f\n", escort->gamma_value);
            printf("           ├── Current Impact : %.6f\n", escort->current_impact_power);
            printf("           └── Ship Health    : %.2f%%\n", fmax(0.0, field->player_ship.current_health * 100.0));

            // Check if Battleship is destroyed
            if (field->player_ship.current_health <= 0.0)
            {
                field->player_ship.current_health = 0.0;

                printf("\n========================================================\n");
                printf("                [DEFEAT] BATTLESHIP SUNK                \n");
                printf("========================================================\n");

                save_step_results_2c(field, step, is_jammed, step_hits, cumulative_damage, battleship_impact, battleship_firings);
                goto simulation_finished;
            }
        }

        // --- BATTLESHIP COUNTER-ATTACK PHASE ---
        if (step == field->jam_step)
        {
            is_jammed = 1;
            printf("\n[GUN STATUS WARNING] Battleship gun jammed during step %d! Action skipped.\n", step);
        }
        else
        {
            int target_index = find_active_target(field);

            if (target_index != -1)
            {
                battleship_firings++;

                // Recalculate degraded battleship impact power using exponential decay
                battleship_impact = calculate_impact_power(
                    BATTLESHIP_INITIAL_IMPACT,
                    field->player_ship.gamma_value,
                    battleship_firings
                );

                EscortShip *target = &field->list_of_escort_ships[target_index];
                escort_damage[target_index] += battleship_impact;

                printf("\n[COUNTER-ATTACK] Battleship fired at Escort ID %d [%s]\n", target->id, target->config.notation);
                printf("                 ├── Firing Count  : %d\n", battleship_firings);
                printf("                 ├── Impact Power  : %.6f\n", battleship_impact);
                printf("                 └── Target Damage : %.2f%%\n", fmin(100.0, escort_damage[target_index] * 100.0));

                if (escort_damage[target_index] >= 1.0)
                {
                    escort_damage[target_index] = 1.0;
                    target->is_destroyed = 1;
                    step_hits++;
                    target->current_impact_power = 0.0;

                    printf("\n >>> [TARGET DESTROYED] Escort ID %d neutralized! <<<\n", target->id);
                }
            }
            else
            {
                printf("\n[INFO] No active escort targets remain to fire upon.\n");
            }
        }

        // Save progress for the step
        save_step_results_2c(field, step, is_jammed, step_hits, cumulative_damage, battleship_impact, battleship_firings);

        // --- STEP SUMMARY ---
        printf("\n------------------- STEP %d SUMMARY -------------------\n", step);
        printf(" Battleship Health   : %.2f%%\n", fmax(0.0, field->player_ship.current_health * 100.0));
        printf(" Battleship Firings  : %d (Current Impact: %.6f)\n", battleship_firings, battleship_impact);
        printf(" Escorts Destroyed   : %d this step\n", step_hits);
        printf("-------------------------------------------------------\n");

        // Check victory condition
        if (all_escorts_destroyed(field))
        {
            printf("\n========================================================\n");
            printf("            [VICTORY] ALL ESCORTS DESTROYED             \n");
            printf("========================================================\n");
            goto simulation_finished;
        }
    }

simulation_finished:

    printf("\n========================================================\n");
    printf("           PART-2C SIMULATION FINAL STATISTICS          \n");
    printf("========================================================\n");
    printf(" Final Battleship Health   : %.2f%%\n", fmax(0.0, field->player_ship.current_health * 100.0));
    printf(" Total Battleship Firings  : %d\n", battleship_firings);
    printf(" Final Battleship Impact   : %.6f\n", battleship_impact);
    printf("========================================================\n");

    printf("\n--- FINAL ESCORT FLEET STATUS ---\n");
    int remaining = 0;

    for (int i = 0; i < field->num_escorts; i++)
    {
        EscortShip *escort = &field->list_of_escort_ships[i];

        printf("\n Escort ID %d [%s] - %s\n", 
               escort->id, 
               escort->config.notation, 
               escort->config.type_name);
        printf("   ├── Gamma Rating   : %.6f\n", escort->gamma_value);
        printf("   ├── Total Firings  : %d\n", escort_firings[i]);
        printf("   ├── Final Impact   : %.6f\n", escort->current_impact_power);
        printf("   ├── Total Damage   : %.2f%%\n", fmin(100.0, escort_damage[i] * 100.0));
        printf("   └── Status         : %s\n", escort->is_destroyed ? "DESTROYED" : "ALIVE");

        if (!escort->is_destroyed)
        {
            remaining++;
        }
    }

    printf("\n========================================================\n");
    printf(" Summary: %d Escort Ships Remaining Alive\n", remaining);
    printf("========================================================\n");

    free(escort_firings);
    free(escort_damage);

    printf("\n[CLEANUP] Part 2-C resources released successfully.\n\n");
}