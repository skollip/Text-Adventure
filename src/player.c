/*********************************************************
 * @file player.c
 * @author Siddhartha Kollipara skollip
 * CSC230, Summer 2015
 * Homework 5
 *
 * player - This program contains the information of the
 *          user playing the game. Actions to initialize
 *          the player, command the player, and keep the
 *          player's location are done with a function
 *          table.
 *
 * ssh skollip@remote-linux.eos.ncsu.edu
 *********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "world.h"
#include "player.h"

/* The marker to track the player's location */
static struct room *the_hero = NULL;
/* The player's inventory */
static char player_inventory;

/**
 * Sets the player's position and clears their inventory.
 * @param starting_room a pointer to the starting room of the game
 * @sideefect Sets the player's position to the start and zeroes
 *            their inventory
 */
void init_player(struct room* starting_room)
{
    the_hero = starting_room;
    player_inventory = 0;
}

/**
 * Prints out the current room description.
 * @sideeffect Prints the information of the room to output
 */
void print_current_room()
{
    print_room(the_hero);
}

/************** BEGIN ACTION FUNCTIONS ******************/

/**
 * Moves player north.
 * @return OK if action is completed without a monster in the room
 * @return LOSE if a monster is in the room
 * @sideefect Prints game descriptions to the output
 */
static result_t action_north()
{
    if (the_hero->objects & OBJ_ENEMY) {
        printf("You try to escape, but it's too late -- the monster grabs you and swallows you\n");
        printf("whole. You lose. Goodbye.\n");
        return LOSE;
    }
    if (*(the_hero->exits + NORTH)) {
        the_hero = *(the_hero->exits + NORTH);
        printf("You travel north.\n\n\n");
        print_current_room();
        return OK;
    }
    else {
        printf("You can't go that way!\n");
        return OK;
    }
}

/**
 * Moves player east.
 * @return OK if action is completed without a monster in the room
 * @return LOSE if a monster is in the room
 * @sideefect Prints game descriptions to the output
 */
static result_t action_east()
{
    if (the_hero->objects & OBJ_ENEMY) {
        printf("You try to escape, but it's too late -- the monster grabs you and swallows you\n");
        printf("whole. You lose. Goodbye.\n");
        return LOSE;
    }
    if (*(the_hero->exits + EAST)) {
        the_hero = *(the_hero->exits + EAST);
        printf("You travel east.\n\n\n");
        print_current_room();
        return OK;
    }
    else {
        printf("You can't go that way!\n");
        return OK;
    }
}

/**
 * Moves player south.
 * @return OK if action is completed without a monster in the room
 * @return LOSE if a monster is in the room
 * @sideefect Prints game descriptions to the output
 */
static result_t action_south()
{
    if (the_hero->objects & OBJ_ENEMY) {
        printf("You try to escape, but it's too late -- the monster grabs you and swallows you\n");
        printf("whole. You lose. Goodbye.\n");
        return LOSE;
    }
    if (*(the_hero->exits + SOUTH)) {
        the_hero = *(the_hero->exits + SOUTH);
        printf("You travel south.\n\n\n");
        print_current_room();
        return OK;
    }
    else {
        printf("You can't go that way!\n");
        return OK;
    }
}

/**
 * Moves player west.
 * @return OK if action is completed without a monster in the room
 * @return LOSE if a monster is in the room
 * @sideefect Prints game descriptions to the output
 */
static result_t action_west()
{
    if (the_hero->objects & OBJ_ENEMY) {
        printf("You try to escape, but it's too late -- the monster grabs you and swallows you\n");
        printf("whole. You lose. Goodbye.\n");
        return LOSE;
    }
    if (*(the_hero->exits + WEST)) {
        the_hero = *(the_hero->exits + WEST);
        printf("You travel west.\n\n\n");
        print_current_room();
        return OK;
    }
    else {
        printf("You can't go that way!\n");
        return OK;
    }
}

/**
 * Player obtains key.
 * @return OK if action is completed without a monster in the room
 * @return LOSE if a monster is in the room
 * @sideefect Prints game descriptions to the output
 */
static result_t action_get_key()
{
    if (the_hero->objects & OBJ_KEY) {
        if (the_hero->objects & OBJ_ENEMY) {
            printf("You scramble to pick it up, but the ");
            printf("monster blocks your path and strikes you\n");
            printf("down in an instant. You lose. Goodbye.\n");
            return LOSE;
        }
        else {
            printf("You pick up the key.\n\n");
            the_hero->objects -= OBJ_KEY;
            if (!(player_inventory & OBJ_KEY)) {
                player_inventory += OBJ_KEY;
            }
            return OK;
        }
    }
    else if (the_hero->objects & OBJ_ENEMY) {
        printf("You spend the final seconds of your life trying to pick up an object that isn't\n");
        printf("there, and the monster swallows you whole. You lose. Goodbye.\n");
        return LOSE;
    }
    else {
        printf("There's no key here!\n");
        return OK;
    }
}

/**
 * Player obtains weapon.
 * @return OK if action is completed without a monster in the room
 * @return LOSE if a monster is in the room
 * @sideefect Prints game descriptions to the output
 */
static result_t action_get_weapon()
{
    if (the_hero->objects & OBJ_WEAPON) {
        if (the_hero->objects & OBJ_ENEMY) {
            printf("You scramble to pick it up, but the ");
            printf("monster blocks your path and strikes you\n");
            printf("down in an instant. You lose. Goodbye.\n");
            return LOSE;
        }
        else {
            printf("You pick up the crescent blade.\n");
            the_hero->objects -= OBJ_WEAPON;
            if (!(player_inventory & OBJ_WEAPON)) {
                player_inventory += OBJ_WEAPON;
            }
            return OK;
        }
    }
    else if (the_hero->objects & OBJ_ENEMY) {
        printf("You spend the final seconds of your life trying to pick up an object that isn't\n");
        printf("there, and the monster swallows you whole. You lose. Goodbye.\n");
        return LOSE;
    }
    else {
        printf("There's nothing like that here!\n");
        return OK;
    }
}

/**
 * Player opens chest.
 * @return OK if action is completed without a monster in the room
 * @return LOSE if a monster is in the room
 * @return WIN if there is not a monster in the room and player has the key
 * @sideefect Prints game descriptions to the output
 */
static result_t action_open_chest()
{
    if (the_hero->objects & OBJ_CHEST) {
        if (the_hero->objects & OBJ_ENEMY) {
            printf("Inexplicably, you ignore the giant shreiking monster, and attempt to calmly\n");
            printf("walk over to the chest and open it up.");
            printf(" The monster is confused for a moment.\n");
            printf("but soon recovers and murders you handily. You lose. Goodbye.\n");
            return LOSE;
        }
        else {
            if (player_inventory & OBJ_KEY) {
                printf("This is it! You insert the key into the ");
                printf("lock on the chest, and give it a turn.\n");
                printf("You hear a click, and the top ");
                printf("bursts open revealing two bearded men from\n");
                printf("the '70s, each made of solid gold! You WIN! Goodbye.\n");
                return WIN;
            }
            else {
                printf("You fumble with the chest's lock to no avail...you need the key!\n");
                return OK;
            }
        }
    }
    else if (the_hero->objects & OBJ_ENEMY) {
        printf("You frantically attempt to open a chest in this room, but there is no chest\n");
        printf("here. Meanwhile, the monster has eaten you. You lose. Goodbye.\n");
        return LOSE;
    }
    else {
        printf("There's no chest here to open!\n\n");
        return OK;
    }
}

/**
 * Player slays beast.
 * @return OK if action is completed with a weapon and monster
 *            is in the room.
 * @return OK if action is completed without a monster in the room
 * @return LOSE if a monster is in the room and player does not have
 *              a weapon
 * @sideefect Prints game descriptions to the output
 */
static result_t action_kill()
{
    if (the_hero->objects & OBJ_ENEMY) {
        if (player_inventory & OBJ_WEAPON) {
            printf("You look the monster in the eye and leap ");
            printf("forward while thrashing your crescent\n");
            printf("blade wildly. You somehow cleave your ");
            printf("foe in half, which turns out to be its\n");
            printf("main weakness. It is dead. I guess this is an ethical thing you did?\n\n");
            the_hero->objects -= OBJ_ENEMY;
            return OK;
        }
        else {
            printf("You cry out and begin flailing your arms ");
            printf("at the beast. He stands still while\n");
            printf("you get it all out of your system, ");
            printf("then when you collapse, exhausted, he calmly\n");
            printf("eats you. You should have brought a weapon. You lose. Goodbye.\n");
            return LOSE;
        }
    }
    else {
        printf("You flail about heroicly, but there's nothing here to fight.\n");
        return OK;
    }
}

/**
 * Player gives up.
 * @return QUIT player gives up
 * @sideefect Prints game descriptions to the output
 */
static result_t action_quit()
{
    printf("You give up.\n");
    return QUIT;
}

/**
 * Player looks around at current room.
 * @return OK player see all entities in the room
 * @sideefect Prints game descriptions to the output
 */
static result_t action_look()
{
    print_current_room();
    return OK;
}

/**
 * Prints a newline.
 * @return OK blank line was submitted
 * @sideefect Prints game descriptions to the output
 */
static result_t action_nothing()
{
    printf("\n");
    return OK;
}

/**
 * Player's inventory is printed.
 * @return OK player sees contents of the inventory
 * @sideefect Prints game descriptions to the output
 */
static result_t action_inventory()
{
    printf("You have:\n");
    printf("  An endless thirst for adventure\n");
    if (player_inventory & OBJ_KEY) {
        printf("  A key\n");
    }
    if (player_inventory & OBJ_WEAPON) {
        printf("  A crescent-shaped blade\n");
    }
    return OK;
}

/************** END ACTION FUNCTIONS ******************/

/* Associates a command with an action */
struct command_action {
    char* command;
    result_t (*action)();
};

/* THE MASTER COMMAND/ACTION TABLE */
static const struct command_action commands[] =
{
    // movement
    { "north",          action_north },
    { "n",              action_north },
    { "east",           action_east  },
    { "e",              action_east  },
    { "south",          action_south },
    { "s",              action_south },
    { "west",           action_west  },
    { "w",              action_west  },
    
    // interaction
    { "get key",        action_get_key    },
    { "get weapon",     action_get_weapon },
    { "get blade",      action_get_weapon },
    { "open chest",     action_open_chest },
    { "unlock",         action_open_chest },
    { "unlock chest",   action_open_chest },
    { "use key",        action_open_chest },
    { "kill",           action_kill       },
    { "attack",         action_kill       },
    { "kill enemy",     action_kill       },
    { "kill javgoblin", action_kill       },
    { "use sword",      action_kill       },
    { "use blade",      action_kill       },
    
    // misc
    { "inv",            action_inventory },
    { "inventory",      action_inventory },
    { "look",           action_look      },
    { "quit",           action_quit      },
    { "exit",           action_quit      },
    { "",               action_nothing   }, // when the user just presses enter
};
/* Constant that equals the total amount of commands */
static const int num_commands = sizeof(commands)/sizeof(*commands);

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
result_t do_command(char* cmd)
{
    for (int i = 0; i < num_commands; i++) {
        if (!strcmp(commands[i].command, cmd)) {
            return commands[i].action();
        }
    }
    
    return INVALID;
}
