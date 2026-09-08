#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/simulator.h"

void run_part1b(Battlefield *field) {
    int k_points, jam_step;
    double theta_min;

    printf("\n=== RUNNING PART 1-B (PATHS & JAMS) ===\n");
    printf("Enter number of path points (k): ");
    scanf("%d", &k_points);

    printf("Enter gun jam step (t where t < %d): ", k_points);
    scanf("%d", &jam_step);

    printf("Enter restricted minimum vertical angle theta_min (0 < theta_min < 30): ");
    scanf("%lf", &theta_min);

}