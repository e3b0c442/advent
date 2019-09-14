#include <errno.h>
#include <regex.h>
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

    printf("Day 5:\n");

    regex_t rule1, rule2, rule3, rule4, rule5;
    if (regcomp(&rule1, "[aeiou].*[aeiou].*[aeiou]", 0))
        goto err_cleanup;

    if (regcomp(&rule2, "\\(.\\)\\1", 0))
        goto err_cleanup;

    if (regcomp(&rule3, "\\(ab\\|cd\\|pq\\|xy\\)", 0))
        goto err_cleanup;

    if (regcomp(&rule4, "\\(..\\).*\\1", 0))
        goto err_cleanup;

    if (regcomp(&rule5, "\\(.\\).\\1", 0))
        goto err_cleanup;

    int good = 0;
    int great = 0;

    char *cursor = input;
    char *line = strsep(&cursor, "\n");
    while (line != NULL)
    {
        if (strlen(line) == 0)
            goto tokenize;

        if (regexec(&rule3, line, 0, NULL, 0) && !regexec(&rule1, line, 0, NULL, 0) && !regexec(&rule2, line, 0, NULL, 0))
            good++;

        if (!regexec(&rule4, line, 0, NULL, 0) && !regexec(&rule5, line, 0, NULL, 0))
            great++;

    tokenize:
        line = strsep(&cursor, "\n");
    }

    printf("\tSolution 1: %d\n", good);
    printf("\tSolution 2: %d\n", great);

    goto cleanup;

err_cleanup:
    rc = -1;
    printf("badman\n");
cleanup:
    regfree(&rule1);
    regfree(&rule2);
    regfree(&rule3);
    regfree(&rule4);
    regfree(&rule5);
    free(input);
    return rc;
}
