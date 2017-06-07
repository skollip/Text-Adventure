/*********************************************************
 * @file make_test_world.c
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
#include <stdbool.h>

#include "world.h"

/**
 * Reverses the output of the direction_t enumerated type.
 * @param dir enumerated direction to be reversed
 * @return direction_t the reversed direction or -1 if incorrect
 *         call was made
 */
direction_t reverse(direction_t dir)
{
    switch(dir) {
        case NORTH: return SOUTH;
        case SOUTH: return NORTH;
        case EAST:  return WEST;
        case WEST:  return EAST;
        default: return -1;
    }
}

/**
 * Writes the number of rooms to a binary file.
 * @param fp output file name
 * @param n number of rooms
 * @sideeffect Prints number of rooms to output
 */
void write_room_count(FILE* fp, int n)
{
    fwrite(&n,sizeof(int),1,fp);
}

/**
 * Writes the room descriptions to a binary file.
 * @param fp output file name
 * @param name name of room
 * @param desc description of room
 * @param flags bitflags of objects in room
 * @sideeffect Prints room parameters to output
 */
void write_room(FILE* fp, char* name, char* desc, char flags)
{
    int sz;
    
    sz = strlen(name);
    fwrite(&sz, sizeof(int), 1, fp);
    fwrite(name, sz, 1, fp);
    
    sz = strlen(desc);
    fwrite(&sz, sizeof(int), 1, fp);
    fwrite(desc, sz, 1, fp);
    
    fwrite(&flags, 1, 1, fp);
}

/**
 * Connects two rooms together in one direction.
 * @param fp output file name
 * @param from short to a room
 * @param to short to a room
 * @param dir direction of the connection
 * @sideeffect Prints the room parameters to output 
 */
void write_exit_one_way(FILE* fp, short from, short to, direction_t dir)
{
    fwrite(&from, sizeof(from), 1, fp);
    fwrite(&to, sizeof(to), 1, fp);
    char dir_1byte = (char) dir;
    fwrite(&dir_1byte, sizeof(dir_1byte), 1, fp);
}

/**
 * Connects two rooms together in both directions.
 * @param fp output file name
 * @param from short to a room
 * @param to short to a room
 * @param dir direction of the connection
 * @sideeffect Prints the room parameters to output 
 */
void write_exit(FILE* fp, short from, short to, direction_t dir)
{
    write_exit_one_way(fp, from, to, dir);
    write_exit_one_way(fp, to, from, reverse(dir));
}

/**
 * Starts the process for creating a binary world file.
 * @param filename the name of output file
 * @param num_rooms number of rooms in the file
 * @sideeffect Prints status to console, writes room count to file
 */
FILE* start_world_file(char* filename, int num_rooms)
{
    printf("Writing %s (%d rooms)...\n", filename, num_rooms);
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror(filename);
        exit(1);
    }
    write_room_count(fp, num_rooms);
    return fp;
}

/**
 * Test world 1.
 * @param filename name of output file
 * @sideeffect Writes binary world data to file
 */
void make_test_world_1(char* filename)
{
    FILE* fp = start_world_file(filename, 3);
    write_room(fp, "Start room", "This world is very minimal. This room has the weapon.", OBJ_WEAPON);
    write_room(fp, "Enemy/key room", "This room has the enemy. I hope you got the weapon. After you kill it, you can get the key.", OBJ_KEY|OBJ_ENEMY);
    write_room(fp, "Chest room", "This room has the chest. Using the key, open the chest and win.\nAlso, this room has a one-way exit north to the start.", OBJ_CHEST);
    
    write_exit(fp, 0,1,EAST);
    write_exit(fp, 1,2,NORTH);
    write_exit_one_way(fp, 2,0,NORTH);
    fclose(fp);
}

/**
 * Test world 2.
 * @param filename name of output file
 * @sideeffect Writes binary world data to file
 */
void make_test_world_2(char* filename)
{
    FILE* fp = start_world_file(filename, 33); // num_rooms=1+16+16
    write_room(
        fp, 
        "Starting room",
        "This world is a test area. Stretching to the east are 16 antechambers,\n"
        "and north of each is a room with a different combination of items. In other\n"
        "words, rooms with all legal values of the 'objects' field are available here.\n"
        "\n"
        "Map:\n"
        "         [C00]-[C01]-[C02]-[C03]-[C04]-[C05]-[C06]-[C07]-[C08]-[C09]-[C10]-[C11]-[C12]-[C13]-[C14]-[C15]\n"
        "           |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |  \n"
        "  [here]-[A00]-[A01]-[A02]-[A03]-[A04]-[A05]-[A06]-[A07]-[A08]-[A09]-[A10]-[A11]-[A12]-[A13]-[A14]-[A15]",
        0
        );
    for (int i=0; i<16; i++) {
        char buf[64];
        sprintf(buf, "Antechamber %d (0x%02x)", i, i);
        write_room(fp, buf, "Outside the test room.", 0);
        
        sprintf(buf, "Test chamber %d (0x%02x)", i, i);
        write_room(fp, buf, "A test room.", i);
    }
    
    write_exit(fp, 0, 1, EAST);
    for (int antechamber=1; antechamber<33; antechamber+=2) {
        int chamber=antechamber+1;
        int antechamber_next = antechamber+2;
        int chamber_next = chamber+2;
        write_exit(fp, antechamber, chamber, NORTH);
        if (antechamber_next<33) write_exit(fp, antechamber, antechamber_next, EAST);
        if (chamber_next<33) write_exit(fp, chamber, chamber_next, EAST);
    }
    
    fclose(fp);
}

/**
 * Test world 3.
 * @param filename name of output file
 * @sideeffect Writes binary world data to file
 */
void make_test_world_3(char* filename)
{
    FILE* fp = start_world_file(filename, 11);
    write_room(
        fp, 
        "Starting room",
        "This world has an enemy straight in every direction except north. The\n"
        "distances differ to help you test your long-range enemy detector.\n"
        "To the north, there's an enemy around a corner, which you\n"
        "should NOT be able to see from here.\n"
        "\n"
        "Map:\n"
        "          [___]-[enemy]\n"
        "            |\n"
        "          [___]\n"
        "            |\n"
        "  [enemy]-[here]-[___]-[___]-[___]-[enemy]\n"
        "            |\n"
        "          [___]\n"
        "            |\n"
        "         [enemy]\n",
         OBJ_WEAPON
        );
        
    write_room(fp, "Western enemy room", "You should see this enemy from the start.", OBJ_ENEMY);
    
    write_room(fp, "Northern empty room 1", "Nothing here.", 0);
    write_room(fp, "Northern empty room 2", "Nothing here, but the enemy's to the east.", 0);
    write_room(fp, "Northern around-the-corner enemy room", "You should NOT see this enemy from the start.", OBJ_ENEMY);
    
    write_room(fp, "Eastern empty room 1", "Nothing here.", 0);
    write_room(fp, "Eastern empty room 2", "Nothing here.", 0);
    write_room(fp, "Eastern empty room 3", "Nothing here.", 0);
    write_room(fp, "Eastern enemy room", "You should see this enemy from the start.", OBJ_ENEMY);
    
    write_room(fp, "Southern empty room 1", "Nothing here.", 0);
    write_room(fp, "Southern enemy room", "You should see this enemy from the start.", OBJ_ENEMY);
    
    write_exit(fp, 0, 1, WEST);
    
    write_exit(fp, 0, 2, NORTH);
    write_exit(fp, 2, 3, NORTH);
    write_exit(fp, 3, 4, EAST);
    
    write_exit(fp, 0, 5, EAST);
    write_exit(fp, 5, 6, EAST);
    write_exit(fp, 6, 7, EAST);
    write_exit(fp, 7, 8, EAST);
    
    write_exit(fp, 0, 9, SOUTH);
    write_exit(fp, 9,10, SOUTH);
    
    fclose(fp);
}

/**
 * Test world special.
 * @param filename name of output file
 * @sideeffect Writes binary world data to file
 */
void make_test_world_special(char* filename)
{
    FILE* fp = start_world_file(filename, 9);
    write_room(
        fp,
        "Ground zero",
        "This world has an enemy in each corner of the map. No\n"
        "enemies can be seen from 0, 0 but will show up after the\n"
        "first move.\n"
        "\n"
        "Map:\n"
        "  [enemy]-[_____]-[enemy]\n"
        "     |       |      |\n"
        "  [_____]-[start]-[_____]\n"
        "     |       |      |\n"
        "  [enemy]-[_____]-[enemy]\n",
        OBJ_KEY
        );
        
    write_room(fp, "Coordinate (-1, 0)", "Nothing to see here", 0); //west      1
    write_room(fp, "Coordinate (0,  1)",
            "Nothing to see here", OBJ_WEAPON);                     //north     2
    write_room(fp, "Coordinate (0, -1)", "Nothing to see here", 0); //south     3
    write_room(fp, "Coordinate (1,  0)", "Nothing to see here", 0); //east      4
    write_room(fp, "Coordinate (-1, 1)", "Enemy + Chest",
            (OBJ_CHEST + OBJ_ENEMY));                               //northwest 5
    write_room(fp, "Coordinate (-1,-1)", "Monster", OBJ_ENEMY);     //southwest 6
    write_room(fp, "Coordinate (1,  1)", "Monster", OBJ_ENEMY);     //northeast 7
    write_room(fp, "Coordinate (1, -1)", "Monster", OBJ_ENEMY);     //southeast 8
    
    write_exit(fp, 0, 1, WEST);
    write_exit(fp, 0, 2, NORTH);
    write_exit(fp, 0, 3, SOUTH);
    write_exit(fp, 0, 4, EAST);
    write_exit(fp, 2, 5, WEST);
    write_exit(fp, 3, 6, WEST);
    write_exit(fp, 2, 7, EAST);
    write_exit(fp, 3, 8, EAST);
    write_exit(fp, 1, 5, NORTH);
    write_exit(fp, 1, 6, SOUTH);
    write_exit(fp, 4, 7, NORTH);
    write_exit(fp, 4, 8, SOUTH);
    
    fclose(fp);
}

/**
 * Creates test worlds.
 * @sideeffect Writes multiple binary world files
 */
int main()
{
    make_test_world_special("special_map.dat");
    printf("This program was used to create test world special.\n");
}
