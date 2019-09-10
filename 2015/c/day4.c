#include <errno.h>
#include <openssl/md5.h>
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

    printf("Day 4:\n");

    /* initialize variables */
    int i = 1;
    char buf[strlen(input) + 11];
    memset(buf, 0, strlen(input) + 11);
    strcpy(buf, input);
    unsigned char digest[16];

    /* find the first matching digest */
    while (1)
    {
        sprintf(buf + strlen(input), "%d", i);
        MD5((const unsigned char *)buf, strlen(buf), digest);

        if (digest[0] == 0 && digest[1] == 0 && digest[2] < 16)
            break;
        i++;
    }

    printf("\tSolution 1: %d\n", i);

    /* find the first matching digest */
    while (1)
    {
        sprintf(buf + strlen(input), "%d", i);
        MD5((const unsigned char *)buf, strlen(buf), digest);

        if (digest[0] == 0 && digest[1] == 0 && digest[2] == 0)
            break;
        i++;
    }

    printf("\tSolution 2: %d\n", i);

    free(input);
    return 0;
}