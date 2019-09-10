#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

int main(int argc, char const *argv[])
{
    int rval = 0;

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

    printf("Day 3:\n");

    /* size the grid */
    int x = 0, y = 0, min_x = 0, min_y = 0, max_x = 0, max_y = 0;
    int x2[2] = {0}, y2[2] = {0}, min_x2 = 0, min_y2 = 0, max_x2 = 0, max_y2 = 0;
    int s = 0;
    for (int i = 0; i < filesize; i++)
    {
        switch (input[i])
        {
        case '>':
            x++;
            x2[s]++;
            break;
        case 'v':
            y--;
            y2[s]--;
            break;
        case '<':
            x--;
            x2[s]--;
            break;
        case '^':
            y++;
            y2[s]++;
            break;
        default:;
        }

        if (x > max_x)
            max_x = x;
        if (y > max_y)
            max_y = y;
        if (x < min_x)
            min_x = x;
        if (y < min_y)
            min_y = y;

        if (x2[s] > max_x2)
            max_x2 = x2[s];
        if (y2[s] > max_y2)
            max_y2 = y2[s];
        if (x2[s] < min_x2)
            min_x2 = x2[s];
        if (y2[s] < min_y2)
            min_y2 = y2[s];
        s ^= 1;
    }

    int w = (max_x - min_x) + 1;
    int h = (max_y - min_y) + 1;
    int w2 = (max_x2 - min_x2) + 1;
    int h2 = (max_y2 - min_y2) + 1;

    /* create the grids and travel again */
    bool grid[w][h];
    bool grid2[w2][h2];
    memset(grid, 0, w * h * sizeof(bool));
    memset(grid2, 0, w2 * h2 * sizeof(bool));

    x = 0 - min_x;
    y = 0 - min_y;
    x2[0] = 0 - min_x2;
    x2[1] = 0 - min_x2;
    y2[0] = 0 - min_y2;
    y2[1] = 0 - min_y2;

    s = 0;
    grid[x][y] = true;
    grid[x2[0]][y2[0]] = true;
    for (int i = 0; i < filesize; i++)
    {
        switch (input[i])
        {
        case '>':
            x++;
            x2[s]++;
            break;
        case 'v':
            y--;
            y2[s]--;
            break;
        case '<':
            x--;
            x2[s]--;
            break;
        case '^':
            y++;
            y2[s]++;
            break;
        default:;
        }
        grid[x][y] = true;
        grid2[x2[s]][y2[s]] = true;
        s ^= 1;
    }

    /* count the houses touched */
    int houses = 0;
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
            houses += grid[i][j] ? 1 : 0;
    printf("\tSolution 1: %d\n", houses);

    houses = 0;
    for (int i = 0; i < w2; i++)
        for (int j = 0; j < h2; j++)
            houses += grid2[i][j] ? 1 : 0;
    printf("\tSolution 2: %d\n", houses);

    free(input);
    return rval;
}