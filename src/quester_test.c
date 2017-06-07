/*********************************************************
 * @file quester_test.c
 * @author Siddhartha Kollipara skollip
 * CSC230, Summer 2015
 * Homework 5
 *
 * questertest - A program that tests the output and logic
 *               of quester.c.
 *
 * ssh skollip@remote-linux.eos.ncsu.edu
 *********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "world.h"
#include "player.h"

// This file has code to create a rudimentary world in memory without the use of load_world
// This allows you to do test some basic functions before your loader is working

/* functions to fabricate a fake debug world */
struct room* new_room(char* name, char* desc, char objects)
{
    struct room* rm = calloc(1, sizeof(struct room));
    strcpy(rm->name, name);
    rm->desc = desc;
    rm->objects = objects;
    // pointers are null by virtue of calloc zeroing
    return rm;
}
/**
 * Reverses the output of the direction_t enumerated type.
 * @param dir enumerated direction to be reversed
 * @return direction_t the reversed direction or -1 if incorrect
 *         call was made
 */
direction_t reverse(direction_t dir)
{
    switch (dir) {
        case NORTH: return SOUTH;
        case SOUTH: return NORTH;
        case EAST:  return WEST;
        case WEST:  return EAST;
        default: return -1;
    }
}

/**
 * Connects two rooms together in one direction.
 * @param a pointer to a room
 * @param b pointer to a room
 * @param dir direction of the connection
 * @sideeffect Changes pointer targets for rooms
 */
void connect_one_way(struct room* a, struct room* b, direction_t dir)
{
    a->exits[dir]          = b;
}

/**
 * Connects two rooms together in both directions.
 * @param a pointer to a room
 * @param b pointer to a room
 * @param dir direction of the connection
 * @sideeffect Changes pointer targets for rooms
 */
void connect_two_way(struct room* a, struct room* b, direction_t dir)
{
    a->exits[dir]          = b;
    b->exits[reverse(dir)] = a;
}

/* Debug world used  for testing */
struct room* make_debug_world()
{
    // make debug environment
    // we don't track pointers, since we don't plan to reclaim this memory...
    //these unit tests are just testing core functionality
    struct room* rm0 =
        new_room("A start room", "You are in a starting room. It has a weapon", OBJ_WEAPON);
    struct room* rm1 = new_room("A second room",
             "This is room two. It has an enemy and a key.", OBJ_ENEMY|OBJ_KEY);
    struct room* rm2 =
        new_room("The final room", "This is the last room. It has a chest.", OBJ_CHEST);
    connect_two_way(rm0, rm1, 1); // 1=east
    connect_two_way(rm1, rm2, 0); // 0=north
    return rm0;
}

/**
 * Runs a simulation that creates a fake world and tests it.
 * @param argc number of arguements
 * @param argv an array of strings that represent the arguements
 * @sideeffect Prints test status to the output
 */
int main(int argc, char* argv[])
{
    printf("Running unit tests...\n");
    
    // fabricate a world in memory
    struct room* start_room = make_debug_world();
    struct room* room1 = NULL;
    struct room* room2 = NULL;
    
    // prepare player
    init_player(start_room);
    
    print_current_room();
    
    // do some test commands
    do_command("get weapon");
    do_command("inventory");
    do_command("look");
    printf("\n\n");
    
    if (start_room->objects & OBJ_WEAPON) {
        printf("Did room lose the weapon? FALSE\n\n");
    }
    else {
        printf("Did room lose the weapon? TRUE\n\n");
    }
    
    do_command("east");
    room1 = *(start_room->exits + EAST);
    printf("\n\n");
    
    if (*(start_room->exits + EAST) == room1) {
        printf("Did I move east? TRUE\n\n");
    }
    else {
        printf("Did I move east? FALSE\n\n");
    }
    
    do_command("kill");
    do_command("look");
    printf("\n\n");
    
    if (room1->objects & OBJ_ENEMY) {
        printf("Did the monster die? FALSE\n\n");
    }
    else {
        printf("Did the monster die? TRUE\n\n");
    }
    
    do_command("get key");
    do_command("inventory");
    printf("\n\n");
    
    if (room1->objects & OBJ_KEY) {
        printf("Did room lose the key? FLASE\n\n");
    }
    else {
        printf("Did room lose the key? TRUE\n\n");
    }
    
    do_command("north");
    room2 = *(room1->exits + NORTH);
    printf("\n\n");
    
    if (*(room1->exits + NORTH) == room2) {
        printf("Did I move north? TRUE\n\n");
    }
    else {
        printf("Did I move north? FALSE\n\n");
    }
    
    if (room2->objects & OBJ_CHEST) {
        printf("Does the room have the chest? TRUE\n\n");
    }
    else {
        printf("Does the room have the chest? FALSE\n\n");
    }
    
    do_command("open chest");
    
    free(start_room);
    free(room1);
    free(room2);
}
