#include <errno.h>
#include <pcreposix.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

int main(int argc, char const *argv[])
{
    int rc = 0;
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

    printf("Day 6:\n");
    regex_t inst_r;
    int result = regcomp(&inst_r, "(turn on|turn off|toggle) (\\d+),(\\d+) through (\\d+),(\\d+)", 0);
    if (result != 0)
        goto err_cleanup;

    char *cursor = input;
    char *line = strsep(&cursor, "\n");
    regmatch_t matches[6] = {0};
    bool grid[1000][1000];
    int grid2[1000][1000];
    memset(grid, 0, 1000 * 1000 * sizeof(bool));
    memset(grid2, 0, 1000 * 1000 * sizeof(int));

    while (line != NULL)
    {
        if (strlen(line) == 0)
            goto tokenize;

        memset(matches, 0, 6 * sizeof(regmatch_t));
        result = regexec(&inst_r, line, 6, matches, 0);
        if (result != 0)
            goto err_cleanup;

        char instr[9] = {0};
        char oxs[4] = {0};
        char oys[4] = {0};
        char dxs[4] = {0};
        char dys[4] = {0};

        strncpy(instr, line + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
        strncpy(oxs, line + matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);
        strncpy(oys, line + matches[3].rm_so, matches[3].rm_eo - matches[3].rm_so);
        strncpy(dxs, line + matches[4].rm_so, matches[4].rm_eo - matches[4].rm_so);
        strncpy(dys, line + matches[5].rm_so, matches[5].rm_eo - matches[5].rm_so);

        unsigned long ox = strtoul(oxs, NULL, 10);
        if (ox == 0 && errno == EINVAL)
            goto err_cleanup;
        unsigned long oy = strtoul(oys, NULL, 10);
        if (oy == 0 && errno == EINVAL)
            goto err_cleanup;
        unsigned long dx = strtoul(dxs, NULL, 10);
        if (dx == 0 && errno == EINVAL)
            goto err_cleanup;
        unsigned long dy = strtoul(dys, NULL, 10);
        if (dy == 0 && errno == EINVAL)
            goto err_cleanup;

        if (strcmp("turn on", instr) == 0)
        {
            for (int x = ox; x <= dx; x++)
                for (int y = oy; y <= dy; y++)
                {
                    grid[x][y] = true;
                    grid2[x][y]++;
                }
        }
        else if (strcmp("turn off", instr) == 0)
        {
            for (int x = ox; x <= dx; x++)
                for (int y = oy; y <= dy; y++)
                {
                    grid[x][y] = false;
                    grid2[x][y] = grid2[x][y] > 0 ? grid2[x][y] - 1 : 0;
                }
        }
        else if (strcmp("toggle", instr) == 0)
        {
            for (int x = ox; x <= dx; x++)
                for (int y = oy; y <= dy; y++)
                {
                    grid[x][y] = !grid[x][y];
                    grid2[x][y] += 2;
                }
        }
        else
        {
            goto err_cleanup;
        }

    tokenize:
        line = strsep(&cursor, "\n");
    }

    int on = 0;
    int bright = 0;
    for (int x = 0; x < 1000; x++)
        for (int y = 0; y < 1000; y++)
        {
            if (grid[x][y])
                on++;
            bright += grid2[x][y];
        }

    printf("\tSolution 1: %d\n", on);
    printf("\tSolution 2: %d\n", bright);

    goto cleanup;

err_cleanup:
    rc = -1;
cleanup:
    regfree(&inst_r);
    free(input);
    return rc;
}