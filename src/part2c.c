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
    int choice;

    printf("----- PART 2-C GAMMA CONFIGURATION -----\n");

    printf("\nGamma controls impact power degradation.\n");
    printf("IPn = IP0 * e^(-gamma*n)\n");

    printf("\n1. Enter gamma values manually\n");
    printf("2. Generate gamma values randomly\n");
    printf("\nEnter choice: ");

    scanf("%d", &choice);

    if (choice == 1)
    {
        // Battleship gamma 
        do
        {
            printf("\nEnter Battleship gamma value");
            printf(" (close to 0): ");

            scanf("%lf", &field->player_ship.gamma_value);

            if (field->player_ship.gamma_value < 0.0)
            {
                printf("[ERROR] Gamma cannot be negative.\n");
            }

        } while (field->player_ship.gamma_value < 0.0);

        double gamma_A;
        double gamma_B;
        double gamma_C;
        double gamma_D;
        double gamma_E;

        do
        {
            printf("Enter gamma for EA: ");
            scanf("%lf", &gamma_A);
        } while (gamma_A < 0.0);

        do
        {
            printf("Enter gamma for EB: ");
            scanf("%lf", &gamma_B);
        } while (gamma_B < 0.0);

        do
        {
            printf("Enter gamma for EC: ");
            scanf("%lf", &gamma_C);
        } while (gamma_C < 0.0);

        do
        {
            printf("Enter gamma for ED: ");
            scanf("%lf", &gamma_D);
        } while (gamma_D < 0.0);

        do
        {
            printf("Enter gamma for EE: ");
            scanf("%lf", &gamma_E);
        } while (gamma_E < 0.0);

        //Apply gamma to every escort according to its type *
        for (int i = 0; i < field->num_escorts; i++)
        {
            switch (field->list_of_escort_ships[i].config.notation[1])
            {
                case 'A':
                    field->list_of_escort_ships[i].gamma_value = gamma_A;
                    break;

                case 'B':
                    field->list_of_escort_ships[i].gamma_value = gamma_B;
                    break;

                case 'C':
                    field->list_of_escort_ships[i].gamma_value = gamma_C;
                    break;

                case 'D':
                    field->list_of_escort_ships[i].gamma_value = gamma_D;
                    break;

                case 'E':
                    field->list_of_escort_ships[i].gamma_value = gamma_E;
                    break;
            }
        }

        printf("\n[SUCCESS] Gamma values configured manually.\n");
    }
    else if (choice == 2)
    {
        field->player_ship.gamma_value =
            ((double)rand() / RAND_MAX) * 0.005;

        double gamma_values[5];

        for (int i = 0; i < 5; i++)
        {
            
            //  Generate escort gamma between 0.02 and 0.10
            gamma_values[i] =
                0.02 +
                ((double)rand() / RAND_MAX) * 0.08;
        }

        for (int i = 0; i < field->num_escorts; i++)
        {
            int type_index =
                get_escort_type_index(
                    &field->list_of_escort_ships[i]);

            if (type_index >= 0)
            {
                field->list_of_escort_ships[i].gamma_value =
                    gamma_values[type_index];
            }
        }

        printf("\n[SUCCESS] Gamma values generated randomly.\n");
    }
    else
    {
        printf("\n[ERROR] Invalid choice.\n");
        printf("Defaulting to random gamma values.\n");

        field->player_ship.gamma_value =
            ((double)rand() / RAND_MAX) * 0.005;

        double gamma_values[5];

        for (int i = 0; i < 5; i++)
        {
            gamma_values[i] =
                0.02 +
                ((double)rand() / RAND_MAX) * 0.08;
        }

        for (int i = 0; i < field->num_escorts; i++)
        {
            int type_index =
                get_escort_type_index(
                    &field->list_of_escort_ships[i]);

            if (type_index >= 0)
            {
                field->list_of_escort_ships[i].gamma_value =
                    gamma_values[type_index];
            }
        }
    }

    printf("\n--- FINAL GAMMA VALUES ---\n");

    printf("Battleship gamma: %.6f\n",
           field->player_ship.gamma_value);

    for (int type = 0; type < 5; type++)
    {
        char required_type = 'A' + type;

        int printed = 0;

        for (int i = 0; i < field->num_escorts; i++)
        {
            if (field->list_of_escort_ships[i].config.notation[1]
                == required_type)
            {
                printf("E%c gamma: %.6f\n",
                       required_type,
                       field->list_of_escort_ships[i].gamma_value);

                printed = 1;
                break;
            }
        }

        if (!printed)
        {
            printf("E%c gamma: No ship of this type generated\n",
                   required_type);
        }
    }
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

    sprintf(filename,
            "data/step_%d_results_2c.txt",
            step);

    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        perror("[ERROR] Could not open Part 2-C result file");
        return;
    }

    fprintf(file,
            "=== SIMULATION STEP %d (PART 2-C) ===\n\n",
            step);

    fprintf(file,
            "--- BATTLEFIELD STATUS ---\n");

    fprintf(file,
            "Battleship Position: (%.2f, %.2f)\n",
            field->player_ship.x_pos,
            field->player_ship.y_pos);

    fprintf(file,
            "Battleship Health: %.2f%%\n",
            field->player_ship.current_health * 100.0);

    fprintf(file,
            "Cumulative Damage Taken: %.2f%%\n",
            cumulative_damage * 100.0);

    if (field->player_ship.current_health > 0.0)
    {
        fprintf(file,
                "Battleship Status: ALIVE\n");
    }
    else
    {
        fprintf(file,
                "Battleship Status: DESTROYED\n");
    }

    fprintf(file,
            "\n--- BATTLESHIP IMPACT POWER ---\n");

    fprintf(file,
            "Initial Impact Power (IP0): %.6f\n",
            BATTLESHIP_INITIAL_IMPACT);

    fprintf(file,
            "Gamma: %.6f\n",
            field->player_ship.gamma_value);

    fprintf(file,
            "Number of Firings (n): %d\n",
            battleship_firings);

    fprintf(file,
            "Current Impact Power (IPn): %.6f\n",
            battleship_impact);

    if (is_jammed)
    {
        fprintf(file,
                "Gun Status: JAMMED (Restricted Arc)\n");
    }
    else
    {
        fprintf(file,
                "Gun Status: NORMAL\n");
    }

    fprintf(file,
            "\nEscort Ships Destroyed in This Step: %d\n",
            step_hits);

    fprintf(file,
            "\n--- ESCORT SHIP STATUS ---\n");

    int active_threats = 0;

    for (int i = 0; i < field->num_escorts; i++)
    {
        EscortShip *ship =
            &field->list_of_escort_ships[i];

        fprintf(file,
                "\nID %d\n",
                ship->id);

        fprintf(file,
                "Type: %s [%s]\n",
                ship->config.type_name,
                ship->config.notation);

        fprintf(file,
                "Gamma: %.6f\n",
                ship->gamma_value);

        fprintf(file,
                "Current Impact Power: %.6f\n",
                ship->current_impact_power);

        if (ship->is_destroyed)
        {
            fprintf(file,
                    "Status: DESTROYED\n");
        }
        else
        {
            active_threats++;

            fprintf(file,
                    "Status: ALIVE\n");

            fprintf(file,
                    "Position: (%.2f, %.2f)\n",
                    ship->x_pos,
                    ship->y_pos);
        }
    }

    fprintf(file,
            "\nTotal Active Threats Remaining: %d\n",
            active_threats);

    fclose(file);

    printf("[SUCCESS] Part 2-C step %d saved to '%s'\n",
           step,
           filename);
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
//   Check whether every escort ship is destroyed
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

//   PART 2-C MAIN SIMULATION
void run_part2c(Battlefield *field)
{
    if (field == NULL)
    {
        printf("[ERROR] Battlefield pointer is NULL.\n");
        return;
    }

    if (field->num_escorts <= 0 ||
        field->list_of_escort_ships == NULL)
    {
        printf("[ERROR] No escort ships are configured.\n");
        return;
    }

    printf("\n");
    printf("PART 2-C SIMULATION\n");

    //   Configure gamma values before starting.
    configure_gamma_values(field);

    int *escort_firings =
        malloc(field->num_escorts * sizeof(int));

    double *escort_damage =
        malloc(field->num_escorts * sizeof(double));

    if (escort_firings == NULL ||
        escort_damage == NULL)
    {
        printf("[ERROR] Memory allocation failed.\n");

        free(escort_firings);
        free(escort_damage);

        return;
    }

    // Initialise Part 2C
    for (int i = 0; i < field->num_escorts; i++)
    {
        escort_firings[i] = 0;
        escort_damage[i] = 0.0;

        field->list_of_escort_ships[i].is_destroyed = 0;
        //   IP0 for each escort is its default impact.
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

    printf("\nSimulation will run for a maximum of %d steps.\n",
           total_steps);

    printf("\nStarting Part 2-C...\n\n");

    for (int step = 1; step <= total_steps; step++)
    {
        int step_hits = 0;

        int is_jammed = 0;

        printf("SIMULATION STEP %d\n", step);

        for (int i = 0; i < field->num_escorts; i++)
        {
            EscortShip *escort =
                &field->list_of_escort_ships[i];

            if (escort->is_destroyed)
            {
                continue;
            }

            escort_firings[i]++;

            //Calculate degraded impact.
            escort->current_impact_power =
                calculate_impact_power(
                    escort->config.default_impact,
                    escort->gamma_value,
                    escort_firings[i]);

            
            //Apply impact to Battleship.
            field->player_ship.current_health -=
                escort->current_impact_power;

            cumulative_damage +=
                escort->current_impact_power;

            printf("Escort ID %d fired.\n",
                   escort->id);

            printf("  Gamma: %.6f\n",
                   escort->gamma_value);

            printf("  Firing count: %d\n",
                   escort_firings[i]);

            printf("  Current impact: %.6f\n",
                   escort->current_impact_power);

            printf("  Battleship health: %.2f%%\n",
                   field->player_ship.current_health * 100.0);

            
            //   Battleship destroyed?
            if (field->player_ship.current_health <= 0.0)
            {
                field->player_ship.current_health = 0.0;

                printf("\n[BATTLESHIP DESTROYED]\n");

                save_step_results_2c(
                    field,
                    step,
                    is_jammed,
                    step_hits,
                    cumulative_damage,
                    battleship_impact,
                    battleship_firings);

                goto simulation_finished;
            }
        }

        if (step == field->jam_step)
        {
            is_jammed = 1;

            printf("\n[BATTLESHIP GUN JAMMED]\n");
            printf("Battleship cannot fire during step %d.\n",
                   step);
        }
        else
        {
            
            //   Find an active escort target.
            int target_index =
                find_active_target(field);

            if (target_index != -1)
            {
                
            //Battleship has fired one more time.
                battleship_firings++;

                
                

                //IPn = IP0 * e^(-gamma*n)
                battleship_impact =
                    calculate_impact_power(
                        BATTLESHIP_INITIAL_IMPACT,
                        field->player_ship.gamma_value,
                        battleship_firings);

                EscortShip *target =
                    &field->list_of_escort_ships[target_index];

                
                //Apply Battleship damage to target.
                escort_damage[target_index] +=
                    battleship_impact;

                printf("\nBattleship fired at Escort ID %d.\n",
                       target->id);

                printf("  Battleship gamma: %.6f\n",
                       field->player_ship.gamma_value);

                printf("  Battleship firing count: %d\n",
                       battleship_firings);

                printf("  Battleship current impact: %.6f\n",
                       battleship_impact);

                printf("  Escort accumulated damage: %.6f\n",
                       escort_damage[target_index]);

                
                //Check whether target has received100% damage.
                if (escort_damage[target_index] >= 1.0)
                {
                    escort_damage[target_index] = 1.0;

                    target->is_destroyed = 1;

                    step_hits++;
                    
                    //Destroyed ships no longer participate in future firing.
                    target->current_impact_power = 0.0;

                    printf("\n[ESCORT DESTROYED]\n");
                    printf("Escort ID %d has been destroyed.\n",
                           target->id);
                }
            }
            else
            {
                printf("\nNo active escort targets remain.\n");
            }
        }
        //SAVE CURRENT STEP
        save_step_results_2c(
            field,
            step,
            is_jammed,
            step_hits,
            cumulative_damage,
            battleship_impact,
            battleship_firings);

        //DISPLAY STEP SUMMARY
        printf("\n--- STEP %d SUMMARY ---\n",
               step);

        printf("Battleship Health: %.2f%%\n",
               field->player_ship.current_health * 100.0);

        printf("Battleship Firings: %d\n",
               battleship_firings);

        printf("Battleship Current Impact: %.6f\n",
               battleship_impact);

        printf("Escort Ships Destroyed This Step: %d\n",
               step_hits);

        //check victory
        if (all_escorts_destroyed(field))
        {
            printf("\n===============================================\n");
            printf("ALL ESCORT SHIPS DESTROYED\n");
            printf("BATTLESHIP SURVIVED\n");

            goto simulation_finished;
        }
    }

simulation_finished:

    printf("----- PART 2-C SIMULATION COMPLETE -----\n");

    printf("\nFinal Battleship Health: %.2f%%\n",
           field->player_ship.current_health * 100.0);

    printf("Total Battleship Firings: %d\n",
           battleship_firings);

    printf("Final Battleship Impact Power: %.6f\n",
           battleship_impact);

    printf("\n--- FINAL ESCORT STATUS ---\n");

    int remaining = 0;

    for (int i = 0; i < field->num_escorts; i++)
    {
        EscortShip *escort =
            &field->list_of_escort_ships[i];

        printf("\nEscort ID %d [%s]\n",
               escort->id,
               escort->config.notation);

        printf("Gamma: %.6f\n",
               escort->gamma_value);

        printf("Total Firings: %d\n",
               escort_firings[i]);

        printf("Final Impact Power: %.6f\n",
               escort->current_impact_power);

        printf("Accumulated Damage Received: %.2f%%\n",
               escort_damage[i] * 100.0);

        if (escort->is_destroyed)
        {
            printf("Status: DESTROYED\n");
        }
        else
        {
            printf("Status: ALIVE\n");
            remaining++;
        }
    }

    printf("\nEscort Ships Remaining: %d\n",
           remaining);

    free(escort_firings);
    free(escort_damage);

    printf("\nPart 2-C resources released successfully.\n");
}