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
            printf("\n======SIMULATION SUBMENU======\n");
            printf("--> Setup\n");//setup variables
            printf("--> Show Simulation\n");//display the statics
            break;

        case 'i':
        case 'I':
            printf("\nDisplaying Instructions\n");
            break;
        
        case 't':
        case 'T':
            printf("\nLoading past statistics from text files...\n");
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