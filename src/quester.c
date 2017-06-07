/*********************************************************
 * @file quester.c
 * @author Siddhartha Kollipara skollip
 * CSC230, Summer 2015
 * Homework 5
 *
 * quester - A program that handles to logic of playing
 *           the game. It is responsible for taking a file
 *           arguement and calling on the functions of
 *           world.c and player.c to simulate the game.
 *
 * ssh skollip@remote-linux.eos.ncsu.edu
 *********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "world.h"
#include "player.h"

/* Win exit code */
#define EXIT_WIN 0
/* Lose exit code */
#define EXIT_LOSE 1
/* Quit exit code */
#define EXIT_QUIT 2
/* Incorrect usage exit code */
#define EXIT_USE 3
/* Failure to load file exit code */
#define EXIT_LOAD 4
/* Maximum command length for game */
#define MAX_INPUT 64

/**
 * Prints correct usage for quester.c.
 * @sideeffect Prints usage to output
 */
void usage()
{
    printf("QUESTER: a CSC230 game!\n\n");
    printf("Syntax:\n");
    printf(" quester <world-file>\n\n");
}

/**
 * Simulates the game based on world.c and player.c.
 * @param argc the number of arguements
 * @param argv an array of argements as strings
 * @sideeffect Prints game description to output
 */
int main(int argc, char *argv[])
{
    if (argc != 2) {
        usage();
        return EXIT_USE;
    }
    
    struct world *new_world = NULL;
    new_world = load_world(*(argv + 1));
    
    if (!new_world) {
        printf("%s\n", load_world_error_msg);
        return EXIT_LOAD;
    }
    
    init_player(*(new_world->rooms));
    print_current_room();
    
    char line[MAX_INPUT + 1];
    
    while (1) {
        printf("> ");
        fgets (line, sizeof(line), stdin);
        
        for (int i = 0; i < MAX_INPUT + 1; i++) {
            if (*(line + i) == '\n') {
                *(line + i) = 0;
                break;
            }
            *(line + i) = tolower(*(line + i));
        }
        
        result_t result = do_command(line);
        
        if (result == INVALID) {
            printf("Unknown command: '%s'.\n\n", line);
        }
        else if (result == WIN) {
            destroy_world(new_world);
            return EXIT_WIN;
        }
        else if (result == LOSE) {
            destroy_world(new_world);
            return EXIT_LOSE;
        }
        else if (result == QUIT) {
            destroy_world(new_world);
            return EXIT_QUIT;
        }
    }
}
