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
    printf("1. Start Simulation - S");
    printf("2. View Instructions - I");
    printf("Simulation Statics - T");
    printf("Exit - X");

    printf("> ");
}


int main(){
    int is_running = 1;
    char choice;

    while(is_running){
        print_banner();
        print_menu();
        
        scanf(" %c", &choice);//Get user's choice
        


    return 0;
}