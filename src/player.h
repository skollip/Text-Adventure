/*********************************************************
 * @file player.h
 * @author Siddhartha Kollipara skollip
 * CSC230, Summer 2015
 * Homework 5
 *
 * player - A header file for player.c. It contains the
 *          enumerated return type for the action commands
 *          and the prototypes for the functions in player.c
 *
 * ssh skollip@remote-linux.eos.ncsu.edu
 *********************************************************/
#ifndef PLAYER_H
#define PLAYER_H

/* Enumerated type used as return values for actions */
typedef enum {OK, WIN, LOSE, QUIT, INVALID} result_t;

/**
 * Sets the player's position and clears their inventory.
 * @param starting_room a pointer to the starting room of the game
 * @sideefect Sets the player's position to the start and zeroes
 *            their inventory
 */
void init_player(struct room* starting_room);

/**
 * Attempt to execute the given command, returning one of the following values:
 *   OK      - command successful (or at least didn't kill them), keep playing
 *   WIN     - player wins!  game over.
 *   LOSE    - player loses! game over.
 *   QUIT    - player quits! game over.
 *   INVALID - no such command
 * @param cmd a string that triggers a command
 * @return result_t a value that determines whether the command
 *         was valid or not.
 */
result_t do_command(char* cmd);

/**
 * Prints out the current room description.
 * @sideeffect Prints the information of the room to output
 */
void print_current_room();

#endif
