#include <errno.h>
#include <openssl/md5.h>
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
    char *input2 = strdup(input);

    printf("Day 5:\n");

    /* tokenize the data into lines */
    char *cursor = input;
    char *line = strsep(&cursor, "\n");
    int good = 0;
    while (line != NULL)
    {
        if (strlen(line) == 0)
            goto tokenize;

        int vowels = 0;
        int max_cons = 1;
        int cons = 1;
        char last = '\0';

        for (int i = 0; i < strlen(line); i++)
        {
            if (line[i] == last)
            {
                cons++;
                if (cons > max_cons)
                    max_cons = cons;
            }
            else
                cons = 1;

            switch (line[i])
            {
            case 'b':
                if (last == 'a')
                    goto tokenize;
                break;
            case 'd':
                if (last == 'c')
                    goto tokenize;
                break;
            case 'q':
                if (last == 'p')
                    goto tokenize;
                break;
            case 'y':
                if (last == 'x')
                    goto tokenize;
                break;
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                vowels++;
                break;
            }

            last = line[i];
        }
        if (vowels >= 3 && max_cons >= 2)
            good++;

    tokenize:
        line = strsep(&cursor, "\n");
    }

    printf("\tSolution 1: %d\n", good);

    cursor = input2;
    line = strsep(&cursor, "\n");
    good = 0;
    while (line != NULL)
    {
        if (strlen(line) == 0)
            goto tokenize2;

        bool repeater = false;
        bool doubler = false;

        for (int i = 0; i < strlen(line); i++)
        {
            if (i + 2 < strlen(line))
                if (line[i] == line[i + 2])
                    repeater = true;

            if (i + 3 < strlen(line))
            {
                for (int j = i + 2; j < strlen(line) - 1; j++)
                {
                    if (line[i] == line[j] && line[i + 1] == line[j + 1])
                    {
                        doubler = true;
                        break;
                    }
                }
            }
            if (repeater && doubler)
            {
                good++;
                goto tokenize2;
            }
        }

    tokenize2:
        line = strsep(&cursor, "\n");
    }

    printf("\tSolution 2: %d\n", good);

    free(input);
    free(input2);
    return 0;
}