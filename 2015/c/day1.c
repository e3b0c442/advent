#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

int main(int argc, char const *argv[])
{
    /* check that filename was provided */
    if (argc < 2)
    {
        fprintf(stderr, "Must provide filename of input file\n");
        return -1;
    }

    /* read the file into memory */
    char *input;
    int filesize = read_file_to_buffer(&input, argv[1]);
    if (filesize < 0)
    {
        fprintf(stderr, "File read failed: %s\n", strerror(errno));
        return -1;
    }

    printf("Day 1:\n");

    /* solve the puzzles */
    int floor = 0;
    int basement = 0;
    for (int i = 0; i < filesize; i++)
    {
        switch (input[i])
        {
        case '(':
            floor++;
            break;
        case ')':
            floor--;
            break;
        default:;
        }
        if (floor == -1 && basement == 0)
        {
            basement = i + 1;
        }
    }

    printf("\tSolution 1: %d\n", floor);
    printf("\tSolution 2: %d\n", basement);

    free(input);
    return 0;
}
