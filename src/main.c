#include <stdio.h>
#include <stdlib.h>

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

    while(is_running){
        print_banner();
        print_menu();
        
        scanf(" %c", &choice);//Get user's choice

        switch (choice)
        {
        case 's':
        case 'S':
            int sub_running = 1;
            char sub_choice;
                
            while(sub_running) {
                printf("\n====== SIMULATION SUBMENU ======\n");
                printf("--> 1 = Setup (Set variables)\n");
                printf("--> 2 = Show Simulation\n");
                printf("--> 3 = Return to Main Menu\n");
                printf("\n> ");
                
                scanf(" %c", &sub_choice);
                    
                switch(sub_choice) {
                    case '1':
                        printf("\nEntering setup module...\n");
                        // run_setup_menu(); // Will be built in setup.c
                        break;
                    case '2':
                        printf("\nStarting simulation...\n");
                        // run_part1a(); // Will be built in part1a.c
                        break;
                    case '3':
                        sub_running = 0; // Break out to main menu
                        break;
                    default:
                        printf("\nInvalid choice. Please use 1, 2, or 3.\n");
                }
            }
            break;        
        case 'i':
        case 'I':
            printf("\nDisplaying Instructions\n");
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

    return 0;
}