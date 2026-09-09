#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/simulator.h"

#define BATTLESHIP_INITIAL_IMPACT 1.0

static double calculate_impact_power(double initial_impact, double gamma, int firing_count)
{
    return initial_impact * exp(-gamma * firing_count);
}

static int get_escort_type_index(EscortShip *ship)
{
    switch (ship->config.notation[1])
    {
        case 'A': return 0;
        case 'B': return 1;
        case 'C': return 2;
        case 'D': return 3;
        case 'E': return 4;
        default: return -1;
    }
}

static void configure_gamma_values(Battlefield *field)
{
    printf("\n----- PART 2-C GAMMA CONFIGURATION -----\n");
    printf("1. Enter gamma values manually\n");
    printf("2. Generate gamma values randomly\n");
    printf("Enter choice: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 1)
    {
        do
        {
            printf("\nEnter Battleship gamma value (close to 0): ");
            scanf("%lf", &field->player_ship.gamma_value);
            if (field->player_ship.gamma_value < 0.0) printf("[ERROR] Gamma cannot be negative.\n");
        } while (field->player_ship.gamma_value < 0.0);

        double gamma_A, gamma_B, gamma_C, gamma_D, gamma_E;
        printf("Enter gamma for EA: "); scanf("%lf", &gamma_A);
        printf("Enter gamma for EB: "); scanf("%lf", &gamma_B);
        printf("Enter gamma for EC: "); scanf("%lf", &gamma_C);
        printf("Enter gamma for ED: "); scanf("%lf", &gamma_D);
        printf("Enter gamma for EE: "); scanf("%lf", &gamma_E);

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
        printf("[SUCCESS] Gamma values configured manually.\n");
    }
    else
    {
        field->player_ship.gamma_value = ((double)rand() / RAND_MAX) * 0.005;
        double gamma_values[5];
        for (int i = 0; i < 5; i++)
        {
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
        printf("[SUCCESS] Gamma values generated randomly.\n");
    }
}

static int find_active_target(Battlefield *field)
{
    for (int i = 0; i < field->num_escorts; i++)
    {
        if (!field->list_of_escort_ships[i].is_destroyed) return i;
    }
    return -1;
}

static int all_escorts_destroyed(Battlefield *field)
{
    for (int i = 0; i < field->num_escorts; i++)
    {
        if (!field->list_of_escort_ships[i].is_destroyed) return 0;
    }
    return 1;
}

void run_part2c(Battlefield *field)
{
    if (field == NULL || field->num_escorts <= 0 || field->list_of_escort_ships == NULL)
    {
        printf("[ERROR] Invalid battlefield setup for Part 2-C.\n");
        return;
    }

    printf("\n=== RUNNING PART 2-C (GAMMA DEGRADATION) ===\n");
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
    int total_steps = field->k_points > 0 ? field->k_points : 10;

    for (int step = 1; step <= total_steps; step++)
    {
        printf("\n--- Step %d/%d ---\n", step, total_steps);
        
        field->player_ship.x_pos = ((double)rand() / RAND_MAX) * field->canvas_size;
        field->player_ship.y_pos = ((double)rand() / RAND_MAX) * field->canvas_size;

        int step_hits = 0;
        int is_jammed = (step > field->jam_step) ? 1 : 0;

        if (is_jammed) {
            printf("[WARNING] Gun jammed! Battleship cannot fire.\n");
        }

        // 1. Enemy Firing Phase
        for (int i = 0; i < field->num_escorts; i++)
        {
            EscortShip *escort = &field->list_of_escort_ships[i];
            if (escort->is_destroyed) continue;

            escort_firings[i]++;
            escort->current_impact_power = calculate_impact_power(escort->config.default_impact, escort->gamma_value, escort_firings[i]);

            field->player_ship.current_health -= escort->current_impact_power;
            cumulative_damage += escort->current_impact_power;

            printf(" -> [HIT] Escort ID %d fired! Damage +%.2f%% (Impact: %.4f, Total: %.2f%%)\n", 
                escort->id, escort->current_impact_power * 100.0, escort->current_impact_power, cumulative_damage * 100.0);

            if (field->player_ship.current_health <= 0.0)
            {
                field->player_ship.current_health = 0.0;
                printf("\n[DEFEAT] Cumulative damage reached 100%% at Step %d!\n", step);
                save_step_results_2c(field, step, is_jammed, step_hits, cumulative_damage, battleship_impact, battleship_firings);
                goto simulation_finished;
            }
        }

        // 2. Battleship Counter-Attack Phase
        if (!is_jammed)
        {
            int target_index = find_active_target(field);
            if (target_index != -1)
            {
                battleship_firings++;
                battleship_impact = calculate_impact_power(BATTLESHIP_INITIAL_IMPACT, field->player_ship.gamma_value, battleship_firings);

                EscortShip *target = &field->list_of_escort_ships[target_index];
                escort_damage[target_index] += battleship_impact;

                printf(" -> [FIRE] Battleship attacked Escort ID %d | Impact: %.4f | Target Damage: %.2f%%\n", 
                    target->id, battleship_impact, escort_damage[target_index] * 100.0);

                if (escort_damage[target_index] >= 1.0)
                {
                    escort_damage[target_index] = 1.0;
                    target->is_destroyed = 1;
                    step_hits++;
                    target->current_impact_power = 0.0;
                    printf(" -> [DESTROYED] Escort ID %d neutralized!\n", target->id);
                }
            }
        }

        save_step_results_2c(field, step, is_jammed, step_hits, cumulative_damage, battleship_impact, battleship_firings);

        if (all_escorts_destroyed(field))
        {
            printf("\n[VICTORY] All escort ships neutralized at Step %d!\n", step);
            goto simulation_finished;
        }
    }

simulation_finished:
    printf("\n--- SIMULATION COMPLETE ---\n");
    printf("Final Battleship Health: %.2f%%\n", field->player_ship.current_health * 100.0);
    printf("Total Battleship Firings: %d\n", battleship_firings);

    free(escort_firings);
    free(escort_damage);
}