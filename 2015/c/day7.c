#include <errno.h>
#include <pcreposix.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "esht.h"

static regex_t instr_r;

uint16_t do_op(esht *instrs, esht *cache, char *key)
{
    uint16_t rval;
    uint16_t *cached = (uint16_t *)esht_get(cache, key, NULL);
    if (cached != NULL)
    {
        rval = *cached;
        free(cached);
        return rval;
    }

    char *instr = (char *)esht_get(instrs, key, NULL);
    if (instr == NULL)
    {
        errno = ENOKEY;
        goto err_cleanup;
    }
    regmatch_t matches[4] = {0};
    int res = regexec(&instr_r, instr, 4, matches, 0);
    if (res == REG_NOMATCH)
    {
        errno = EINVAL;
        goto err_cleanup;
    }

    char ls[6] = {0};
    char op[7] = {0};
    char rs[6] = {0};

    char *check;
    unsigned long l, r = 0;

    if (matches[1].rm_so != -1)
    {
        strncpy(ls, instr + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
        l = strtoul(ls, &check, 10);
        if (strlen(ls) > 0)
            if (l == 0 && check == ls)
            {
                l = do_op(instrs, cache, ls);
                if (l == UINT16_MAX && errno != 0)
                    goto err_cleanup;
            }
    }

    if (matches[3].rm_so != -1)
    {
        strncpy(rs, instr + matches[3].rm_so, matches[3].rm_eo - matches[3].rm_so);
        r = strtoul(rs, &check, 10);
        if (strlen(rs) > 0)
        {
            if (r == 0 && check == rs)
            {
                r = do_op(instrs, cache, rs);
                if (r == UINT16_MAX && errno != 0)
                    goto err_cleanup;
            }
        }
        else
        {
            errno = EINVAL;
            goto err_cleanup;
        }
    }

    if (matches[2].rm_so != -1)
    {
        strncpy(op, instr + matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);

        if (!strcmp(op, "LSHIFT"))
            rval = l << r;
        else if (!strcmp(op, "RSHIFT"))
            rval = l >> r;
        else if (!strcmp(op, "AND"))
            rval = l & r;
        else if (!strcmp(op, "OR"))
            rval = l | r;
        else if (!strcmp(op, "NOT"))
            rval = ~r;
        else
        {
            errno = EINVAL;
            goto err_cleanup;
        }
    }
    else
        rval = r;

    esht_update(cache, key, &rval, sizeof(uint16_t));
    goto cleanup;

err_cleanup:
    rval = UINT16_MAX;
cleanup:
    if (instr != NULL)
        free(instr);
    return rval;
}

int main(int argc, char const *argv[])
{
    int rc, res = 0;

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

    printf("Day 7:\n");

    /* create the regular expression for parsing into the table */
    regex_t gate_r;
    int result = regcomp(&gate_r, "(.*) -> ([a-z]+)", 0);
    if (result != 0)
        goto err_cleanup;

    char *cursor = input;
    char *line = strsep(&cursor, "\n");
    regmatch_t matches[3] = {0};
    esht *inst_table = esht_create();
    if (inst_table == NULL)
        goto err_cleanup;

    /* parse the gates into the hash table */
    while (line != NULL)
    {
        if (strlen(line) == 0)
            goto tokenize;

        memset(matches, 0, 3 * sizeof(regmatch_t));
        result = regexec(&gate_r, line, 3, matches, 0);
        if (result != 0)
            goto err_cleanup;

        char key[4] = {0};
        char val[64] = {0};

        strncpy(val, line + matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so);
        strncpy(key, line + matches[2].rm_so, matches[2].rm_eo - matches[2].rm_so);

        if ((res = esht_update(inst_table, key, val, strlen(val) + 1)) != 0)
            goto err_cleanup;

    tokenize:
        line = strsep(&cursor, "\n");
    }

    result = regcomp(&instr_r, "^(?:([a-z0-9]+) )?(?:(AND|OR|RSHIFT|LSHIFT|NOT|) )?([a-z0-9]+)$", 0);
    if (result)
        goto err_cleanup;

    /* create a cache and perform the recursive operation to get the gate value */
    esht *cache = esht_create();
    if (cache == NULL)
        goto err_cleanup;
    uint16_t a = do_op(inst_table, cache, "a");
    printf("\tSolution 1: %d\n", a);

    /* clear cache and set b to value of a */
    esht_destroy(cache);
    cache = esht_create();
    if (cache == NULL)
        goto err_cleanup;
    if ((res = esht_update(cache, "b", &a, sizeof(uint16_t))) != 0)
        goto err_cleanup;

    /* get solution 2 */
    a = do_op(inst_table, cache, "a");
    printf("\tSolution 2: %d\n", a);
    goto cleanup;

err_cleanup:
    rc = -1;
cleanup:
    if (cache != NULL)
        esht_destroy(cache);
    regfree(&instr_r);
    if (inst_table != NULL)
        esht_destroy(inst_table);
    regfree(&gate_r);
    free(input);
    return rc;
}