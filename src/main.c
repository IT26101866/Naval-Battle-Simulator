#include <stdio.h>
#include <stdlib.h>
#include "../include/simulator.h"

void print_banner(){
    //This is used to clear the screen
    system("clear || cls");

    printf("    ███╗   ██╗ █████╗ ██╗   ██╗ █████╗ ██╗\n");
    printf("    ████╗  ██║██╔══██╗██║   ██║██╔══██╗██║\n");
    printf("    ██╔██╗ ██║███████║██║   ██║███████║██║\n");
    printf("    ██║╚██╗██║██╔══██║╚██╗ ██╔╝██╔══██║██║\n");
    printf("    ██║ ╚████║██║  ██║ ╚████╔╝ ██║  ██║███████╗\n");
    printf("    ╚═╝  ╚═══╝╚═╝  ╚═╝  ╚═══╝  ╚═╝  ╚═╝╚══════╝\n");

    printf("\n");

    printf(" ██████╗  █████╗ ████████╗████████╗██╗     ███████╗\n");
    printf(" ██╔══██╗██╔══██╗╚══██╔══╝╚══██╔══╝██║     ██╔════╝\n");
    printf(" ██████╔╝███████║   ██║      ██║   ██║     █████╗  \n");
    printf(" ██╔══██╗██╔══██║   ██║      ██║   ██║     ██╔══╝  \n");
    printf(" ██████╔╝██║  ██║   ██║      ██║   ███████╗███████╗\n");
    printf(" ╚═════╝ ╚═╝  ╚═╝   ╚═╝      ╚═╝   ╚══════╝╚══════╝\n");

    printf("\n");
    printf("              NAVAL BATTLE SIMULATOR\n");
    printf("\n");

}

void print_menu(){
    //Main menu
    printf("--> S = Start Simulation\n");
    printf("--> I = View Instructions\n");
    printf("--> T = Simulation Statics\n");
    printf("--> X = Exit\n");

    printf("\n> ");
}


int main(){
    int is_running = 1;
    char choice;

    // Instantiate the master battlefield struct
    Battlefield my_battlefield;  
    my_battlefield.num_escorts = 0; // Default state
    my_battlefield.list_of_escort_ships = NULL; // Safe pointer initialization

    while(is_running){
        print_banner();
        print_menu();
        
        scanf(" %c", &choice);//Get user's choice

        switch (choice)
        {
        case 's':
        case 'S':
        {
            int module_running = 1;
            char module_choice;

            while (module_running)
            {
                printf("\n====== MODULE SELECTION ======\n");
                printf("--> A = Part 1-A (Basic Combat)\n");
                printf("--> B = Part 1-B (Paths & Jams)\n");
                printf("--> C = Part 1-C (Proportional Damage)\n");
                printf("--> R = Return to Main Menu\n");
                printf("\n> ");
                
                scanf(" %c", &module_choice);

                // Exit Module Selection early if user wants to go back
                if (module_choice == 'r' || module_choice == 'R') {
                    module_running = 0;
                    break;
                }

                // Make sure a valid module is selected before going to Execution menu
                if (module_choice != 'a' && module_choice != 'A' && 
                    module_choice != 'b' && module_choice != 'B' && 
                    module_choice != 'c' && module_choice != 'C') {
                    printf("\n[ERROR] Invalid module. Please select A, B, C, or R.\n");
                    continue; // Skip the rest of this loop iteration
                }
            
                int exec_running = 1;
                char exec_choice;
                
                while(exec_running) {
                    printf("\n====== EXECUTION SUBMENU ======\n");
                    printf("--> 1 = Setup (Set variables)\n");
                    printf("--> 2 = Show Simulation\n");
                    printf("--> 3 = Return to Main Menu\n");
                    printf("\n> ");
                
                    scanf(" %c", &exec_choice);
                    
                    switch(exec_choice) {
                        case '1':
                            printf("\nEntering setup module...\n");
                            configure_random_seed();
                            configure_battleship(&my_battlefield);
                            initialize_battlefield(&my_battlefield);
                            save_initial_conditions(&my_battlefield);
                            break;
                        case '2':
                            if (my_battlefield.num_escorts == 0 || my_battlefield.list_of_escort_ships == NULL)
                            {
                                printf("\n[ERROR] You must run Setup (Option 1) before starting the simulation!\n");
                            } else
                            {
                                printf("\nStarting simulation...\n");
                                
                                if (module_choice == 'a' || module_choice == 'A')
                                {
                                    run_part1a(&my_battlefield);
                                }
                                else if (module_choice == 'b' || module_choice == 'B') {
                                    printf("\n[PENDING] Part 1-B logic will execute here.\n");
                                    // run_part1b(&my_battlefield);
                                }
                                else if (module_choice == 'c' || module_choice == 'C') {
                                    printf("\n[PENDING] Part 1-C logic will execute here.\n");
                                    // run_part1c(&my_battlefield);
                                }   
                            }
                            break;
                        case '3':
                            exec_running = 0; // Break out to main menu
                            break;
                        default:
                            printf("\nInvalid choice. Please use 1, 2, or 3.\n");
                    }
                }
            }
            break;
        }        
        case 'i':
        case 'I':
            printf("\n--- INSTRUCTIONS ---\n");
            printf("Defend the stationary Battleship against Axis Escort ships.\n");
            // Expand instructions later
            break;
        
        case 't':
        case 'T':
            printf("\nLoading past statistics from text files...\n");
            // load_statistics(); // Will be built in file_handler.c
            break;

        case 'x':
        case 'X':
            printf("\nExiting simulator...\n");
            is_running = 0;//breaks the loop
            break;
        default:
            printf("\nInvalid choice. Please use S, I, T, or X\n");
            break;
        } 
        // Pause so the user can read the text before the screen clears again
        if (is_running == 1) {
            printf("\nPress Enter to continue...");
            while(getchar() != '\n'); // clear the buffer
            getchar(); // wait for the enter key
        }
    }
    // Free the dynamically allocated memory before exiting to prevent memory leaks
    if (my_battlefield.list_of_escort_ships != NULL) {
        free(my_battlefield.list_of_escort_ships);// returns dynamically allocated memory back to OS
    }
    return 0;
}