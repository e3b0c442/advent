#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "common.h"

int read_file_to_buffer(char **buf, const char *filename)
{
    int rval = 0;
    FILE *f = NULL;
    *buf = NULL;

    /* check that the file exists and is valid to read */
    struct stat s;
    int rc = stat(filename, &s);
    if (rc == -1)
        goto err_cleanup; /* errno is set by stat() */

    /* check that we are working with an actual file */
    if (!S_ISREG(s.st_mode))
    {
        errno = EINVAL;
        goto err_cleanup;
    }

    /* open the file */
    f = fopen(filename, "r");
    if (f == NULL)
        goto err_cleanup; /* errno is set by fopen() */

    /* allocate the buffer */
    *buf = malloc(s.st_size + 1);
    if (*buf == NULL)
        goto err_cleanup; /* errno is set by malloc() */

    /* read the file into the buffer */
    size_t rd = fread(*buf, 1, s.st_size, f);
    if (rd < s.st_size)
        if (ferror(f))
            goto err_cleanup;

    /* file is read into the buffer, return the number of bytes read */
    (*buf)[rd] = '\0'; // add a null terminator
    rval = rd;
    goto cleanup;

err_cleanup:
    rval = -1;
    if (*buf != NULL)
        free(*buf);
    *buf = NULL;
cleanup:
    if (f != NULL)
        fclose(f);
    return rval;
}

int cmp_int_asc(const void *a, const void *b)
{
    int l = *(const int *)a;
    int r = *(const int *)b;

    return l < r ? -1 : l > r ? 1 : 0;
}