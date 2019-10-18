#include <errno.h>
#include <stdbool.h>
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

    printf("Day 8:\n");

    int code = 0;
    int literal = 0;
    bool in = false;

    int i = 0;
    char c;
    while (i < filesize)
    {
        c = input[i];
        code++;
        i++;

        switch (c)
        {
        case '\0':
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            if (!in)
                code--;
            break;
        case '"':
            in = !in;
            break;
        case '\\':
            if (in)
            {
                c = input[i];
                code++;
                i++;
                switch (c)
                {
                case '\\':
                case '"':
                    literal++;
                    break;
                case 'x':
                    code += 2;
                    i += 2;
                    literal++;
                    break;
                default:
                    literal += 2;
                    break;
                }
                break;
            }
        default:
            if (in)
                literal++;
            break;
        }
    }

    printf("\tSolution 1: %d\n", code - literal);

    in = false;
    literal = 0;
    code = 0;
    i = 0;
    while (i < filesize)
    {
        c = input[i];
        i++;
        literal++;

        switch (c)
        {
        case '\n':
            literal--;
        case '\\':
        case '"':
            code += 2;
            break;
        default:
            code++;
            break;
        }
    }
    code += 2;

    printf("\tSolution 2: %d\n", code - literal);

    free(input);
    return 0;
}