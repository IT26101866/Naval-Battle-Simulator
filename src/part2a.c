#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/simulator.h"

#define GRAVITY 9.81

// Struct to track targets for sorting
typedef struct {
    int array_index;
    int ship_id;
    double distance;
    double threat_score;
} TargetThreat;

static double calculate_distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Comparison function for qsort (sorts descending by threat score)
static int compare_threats(const void *a, const void *b) {
    TargetThreat *t1 = (TargetThreat *)a;
    TargetThreat *t2 = (TargetThreat *)b;
    if (t1->threat_score < t2->threat_score) return 1;
    if (t1->threat_score > t2->threat_score) return -1;
    return 0;
}