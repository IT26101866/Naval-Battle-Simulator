#ifndef SIMULATOR_H
#define SIMULATOR_H

typedef struct {
    char notation[10];       // Type notation | e.g., "EA", "EB"[cite: 1]
    char type_name[40];      // Type name | e.g., "1936A-class Destroyer"[cite: 1]
    char gun_name[40];       // Gun name | e.g., "SK C/34 naval gun"[cite: 1]
    double default_impact;   // Impact power | e.g., 0.08[cite: 1]
    double angle_range;      // Angle range | e.g., 20[cite: 1]
} EscortTypeConfig;

#endif