#ifndef SIMULATOR_H
#define SIMULATOR_H

// STRUCTS FOR SHIP TYPE CONFIGURATIONS
typedef struct {
    char notation[10];       // Type notation | e.g., "EA", "EB"
    char type_name[40];      // Type name | e.g., "1936A-class Destroyer"
    char gun_name[40];       // Gun name | e.g., "SK C/34 naval gun"
    double default_impact;   // Impact power | e.g., 0.08
    double angle_range;      // Angle range | e.g., 20
} EscortTypeConfig;

typedef struct {
        char notation[10];      // Type Notation | e.g., "U", "M"
        char ship_name[40];     // Ship name | e.g., "USS Iowa (BB-61)"
        char gun_name[40];      // Gun name | e.g., "50-caliber Mark 7 gun"
} BattleshipTypeConfig;

// CORE DATA OF SHIPS
typedef struct {
    int id;                     // Unique index id for each escort ship
    EscortTypeConfig config;    // embeds the blueprint data inside this specific ship.
    double x_pos, y_pos;         // Coordinates on canvas
    double min_angle, max_angle; // Firing angle range
    double min_velocity;         // Random minimum speed
    double max_velocity;         // Maximum speed
    double current_impact_power; // Degrades over time (Part 2-C)
    double gamma_value;          // Degradation rate
    int is_destroyed;            // 0 = active, 1 = destroyed 
} EscortShip;

typedef struct {
    BattleshipTypeConfig config; // embeds the blueprint data inside this specific ship.
    double x_pos, y_pos;         // Starting coordinates
    double max_velocity;         // V_max
    double current_health;       // Starts at 1.0 (100%)
    double gamma_value;          // Degradation rate
} Battleship;


#endif