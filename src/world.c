/*********************************************************
 * @file world.c
 * @author Siddhartha Kollipara skollip
 * CSC230, Summer 2015
 * Homework 5
 *
 * world - This program reads binary files that contain
 *         information for generating maps to play. It
 *         parses the information and creates the map.
 *
 * ssh skollip@remote-linux.eos.ncsu.edu
 *********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
// ^^ needed for errno, the IO error number. this means you can use strerror(errno)
// to get the standard UNIX error message as a string without printing it like perror does

#include "world.h"
#include "player.h"

// cross-module global -- this gets set to an error message if load_world fails
char* load_world_error_msg = NULL;

/**
 * Prints the information of what is contained in the room and
 * its exits.
 * @param rm a room pointer to reference for printing information
 * @sdieeffect prints the contents of the room to output
 */
void print_room(struct room* rm)
{
    struct room* nextroom = rm;
    printf("= %s =\n\n", rm->name);
    printf("%s\n\n", rm->desc);
    if (rm->objects & OBJ_ENEMY) {
        printf("A bloodthirsty Javgoblin snarls at you, ready to strike!\n");
    }
    if (rm->objects & OBJ_WEAPON) {
        printf("You spot a sharp, crescent-shaped blade resting at your feet.\n");
    }
    if (rm->objects & OBJ_KEY) {
        printf("You spot a key glimmering on the ground.\n");
    }
    if (rm->objects & OBJ_CHEST) {
        printf("A large oak chest sits here.\n");
    }
    printf("Exits:\n");
    if (*(rm->exits + NORTH)) {
        printf("  north");
        while (*(nextroom->exits + NORTH)) {
            nextroom = *(nextroom->exits + NORTH);
            if (nextroom->objects & OBJ_ENEMY) {
                printf(" (but you see a shadow moving in the distance)");
                break;
            }
        }
        printf("\n");
    }
    if (*(rm->exits + EAST)) {
        printf("  east");
        nextroom = rm;
        while (*(nextroom->exits + EAST)) {
            nextroom = *(nextroom->exits + EAST);
            if (nextroom->objects & OBJ_ENEMY) {
                printf(" (but you see a shadow moving in the distance)");
                break;
            }
        }
        printf("\n");
    }
    if (*(rm->exits + SOUTH)) {
        printf("  south");
        nextroom = rm;
        while (*(nextroom->exits + SOUTH)) {
            nextroom = *(nextroom->exits + SOUTH);
            if (nextroom->objects & OBJ_ENEMY) {
                printf(" (but you see a shadow moving in the distance)");
                break;
            }
        }
        printf("\n");
    }
    if (*(rm->exits + WEST)) {
        printf("  west");
        nextroom = rm;
        while (*(nextroom->exits + WEST)) {
            nextroom = *(nextroom->exits + WEST);
            if (nextroom->objects & OBJ_ENEMY) {
                printf(" (but you see a shadow moving in the distance)");
                break;
            }
        }
        printf("\n");
    }
}

/**
 * Reads in a binary file and parses the information to create
 * a map to use for play.
 * @param filename the name of the file
 * @return new_world a pointer to the game world
 * @return NULL sets the error message
 * @sideeffect allocates all of the space for the world on the heap
 */
struct world* load_world(char* filename)
{
    FILE* input = fopen(filename, "rb");
    
    if (input == NULL) {
        load_world_error_msg = strerror(errno);
        return NULL;
    }
    
    int int_buf;
    char char_buf;
    short short_buf[2];
    struct room *new_room = NULL;
    
    if (!fread(&int_buf, sizeof (int), 1, input)) {
        load_world_error_msg = "Unable to read number of rooms";
        return NULL;
    }
    
    struct world *new_world = (struct world*) calloc (1, sizeof (struct world));
    
    new_world->num_rooms = int_buf;
    new_world->rooms = (struct room**) calloc (new_world->num_rooms, sizeof (struct room*));
    
    for (int i = 0; i < new_world->num_rooms; i++) {
        //Allocate space for each room
        *(new_world->rooms + i) = calloc (1, sizeof (struct room));
        new_room = *(new_world->rooms + i);
        
        //Room name
        if (!fread(&int_buf, sizeof (int), 1, input)) {
            load_world_error_msg = "Unable to read room name length";
            return NULL;
        }
        
        if (int_buf > (MAX_NAME_LENGTH)) {
            load_world_error_msg = "Room name length exceeds maximum";
            return NULL;
        }
        
        if (int_buf == 0) {
            fread(new_room->name, sizeof (char), int_buf, input);
        }
        else if (!fread(new_room->name, sizeof (char), int_buf, input)) {
            load_world_error_msg = "Unable to read room name";
            return NULL;
        }
        
        //Room description
        if (!fread(&int_buf, sizeof (int), 1, input)) {
            load_world_error_msg = "Unable to read room description length";
            return NULL;
        }
        
        new_room->desc = calloc (int_buf + 1, sizeof (char));
        
        if (int_buf == 0) {
            fread(new_room->desc, sizeof (char), int_buf, input);
        }
        else if (!fread(new_room->desc, sizeof (char), int_buf, input)) {
            load_world_error_msg = "Unable to read room description";
            return NULL;
        }
        
        //Objects in the room
        if (!fread(&new_room->objects, sizeof (char), 1, input)) {
            load_world_error_msg = "Unable to read object bitfield";
            return NULL;
        }
    }
    
    //Exit mapping
    struct room *exit_src;
    while (fread(short_buf, sizeof (short), 2, input)) {
        if (*(short_buf) > (short) new_world->num_rooms
            || *(short_buf + 1) > (short) new_world->num_rooms) {
            load_world_error_msg = "Unable to read exit linkage";
            return NULL;
        }
        
        if (!fread(&char_buf, sizeof (char), 1, input)) {
            load_world_error_msg = "Unable to read exit linkage";
            return NULL;
        }
        
        exit_src = *(new_world->rooms + *short_buf);
        *(exit_src->exits + char_buf) = *(new_world->rooms + *(short_buf + 1));
    }
    return new_world;
}

/**
 * Frees the memory allocated for world and all of its smaller structures.
 * @param world a pointer to the world map
 * @sideefect frees the allocated memory for all structures
 */
void destroy_world(struct world* world)
{
    struct room *dead_room;
    
    for (int i = 0; i < world->num_rooms; i++) {
        dead_room = *(world->rooms + i);
        free(dead_room->desc);
        free(dead_room);
    }
    
    free(world->rooms);
    free(world);
}
