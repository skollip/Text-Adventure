/*********************************************************
 * @file world.h
 * @author Siddhartha Kollipara skollip
 * CSC230, Summer 2015
 * Homework 5
 *
 * world - This is the header for world.c. It contains the
 *         constants and prototypes for world.c.
 *
 * ssh skollip@remote-linux.eos.ncsu.edu
 *********************************************************/
#ifndef WORLD_H
#define WORLD_H

/* Max length of the room name field */
#define MAX_NAME_LENGTH 63

/* Bit flags used to detect the presence of objects */
#define OBJ_KEY    0x01
#define OBJ_CHEST  0x02
#define OBJ_WEAPON 0x04
#define OBJ_ENEMY  0x08

/* Max number of exits in each room */
#define MAX_EXITS 4

/* An enumerated type to represent the cardinal directions */
typedef enum {NORTH=0, EAST=1, SOUTH=2, WEST=3} direction_t;

/* cross-module global -- this gets set to an error message if load_world fails */
extern char* load_world_error_msg;

/* world map structure */
struct world {
    struct room** rooms;
    int num_rooms;
};

/* Single room structure */
struct room {
    char name[MAX_NAME_LENGTH+1];
    char* desc;
    char objects; // bit mask for OBJ_* flags
    struct room* exits[MAX_EXITS];
};

/**
 * Prints the information of what is contained in the room and
 * its exits.
 * @param rm a room pointer to reference for printing information
 * @sdieeffect prints the contents of the room to output
 */
void print_room(struct room* rm);

/**
 * Reads in a binary file and parses the information to create
 * a map to use for play.
 * @param filename the name of the file
 * @return new_world a pointer to the game world
 * @return NULL sets the error message
 * @sideeffect allocates all of the space for the world on the heap
 */
struct world* load_world(char* filename);

/**
 * Frees the memory allocated for world and all of its smaller structures.
 * @param world a pointer to the world map
 * @sideefect frees the allocated memory for all structures
 */
void destroy_world(struct world* world);

#endif
