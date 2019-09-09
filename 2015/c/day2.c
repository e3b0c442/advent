#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
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

    printf("Day 2:\n");

    int paper = 0;
    int ribbon = 0;

    /* tokenize the data into lines */
    char *cursor = input;
    char *line = strsep(&cursor, "\n");
    while (line != NULL)
    {
        if (strlen(line) == 0)
            goto tokenize;

        /* scan each line's values */
        int dims[3] = {0};

        int scanned = sscanf(line, "%dx%dx%d", &dims[0], &dims[1], &dims[2]);
        if (scanned != 3)
        {
            fprintf(stderr, "Invalid input line \"%s\"\n", line);
            goto err_cleanup;
        }

        qsort(dims, 3, sizeof(int), cmp_int_asc);

        paper += 2 * dims[0] * dims[1] + 2 * dims[1] * dims[2] + 2 * dims[2] * dims[0] + dims[0] * dims[1];
        ribbon += 2 * dims[0] + 2 * dims[1] + dims[0] * dims[1] * dims[2];
    tokenize:
        line = strsep(&cursor, "\n");
    }

    printf("\tSolution 1: %d\n", paper);
    printf("\tSolution 2: %d\n", ribbon);

    goto cleanup;

err_cleanup:
    rval = 1;
cleanup:
    free(input);
    return rval;
}
