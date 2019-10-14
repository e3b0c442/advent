#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "esht.h"

typedef struct esht_entry
{
    char *k;
    void *v;
    size_t l;
    struct esht_entry *n;
} esht_entry;

struct esht
{
    size_t len;
    size_t cap;
    esht_entry **entries;
};

unsigned long esht_hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) ^ c;

    return hash;
}

esht *esht_create()
{
    esht *table;

    table = malloc(sizeof(esht));
    if (table == NULL)
        return NULL;

    table->cap = 1;
    table->len = 0;
    table->entries = calloc(1, sizeof(esht_entry *));
    if (table->entries == NULL)
    {
        free(table);
        return NULL;
    }

    return table;
}

int esht_resize(esht *table, size_t new_cap)
{
    int i, ii;
    esht_entry *cur, *next, **old;

    esht_entry **new_entries = calloc(new_cap, sizeof(esht_entry *));
    if (new_entries == NULL)
        return 1;

    for (i = 0; i < table->cap; i++)
    {
        next = table->entries[i];
        while (next != NULL)
        {
            cur = next;
            next = cur->n;

            ii = esht_hash(cur->k) % new_cap;
            cur->n = new_entries[ii];
            new_entries[ii] = cur;
        }
    }

    old = table->entries;
    table->entries = new_entries;
    table->cap = new_cap;
    free(old);

    return 0;
}

esht_entry *esht_get_entry(esht *table, char *key)
{
    unsigned long i;
    esht_entry *e;

    i = esht_hash(key) % table->cap;
    e = table->entries[i];
    while (e != NULL)
    {
        if (!strcmp(key, e->k))
            return e;
        e = e->n;
    }

    return NULL;
}

void *esht_get(esht *table, char *key, size_t *len)
{
    esht_entry *e;
    void *r;

    e = esht_get_entry(table, key);
    if (e == NULL)
    {
        if (len != NULL)
            *len = 0;
        return NULL;
    }

    r = malloc(e->l);
    if (r == NULL)
    {
        if (len != NULL)
            *len = 0;
        return NULL;
    }
    memcpy(r, e->v, e->l);

    if (len != NULL)
        *len = e->l;
    return r;
}

int esht_update(esht *table, char *key, void *value, size_t len)
{
    unsigned long i;
    esht_entry *e;
    void *v, *k, *old;

    e = esht_get_entry(table, key);
    if (e != NULL)
    {
        v = malloc(len);
        if (v == NULL)
            return 1;
        memcpy(v, value, len);

        old = e->v;
        e->l = len;
        e->v = v;
        free(old);

        return 0;
    }

    e = malloc(sizeof(esht_entry));
    if (e == NULL)
        return 1;

    v = malloc(len);
    if (v == NULL)
    {
        free(e);
        return 1;
    }
    memcpy(v, value, len);

    k = malloc(strlen(key) + 1);
    if (k == NULL)
    {
        free(e);
        free(v);
        return 1;
    }
    strcpy(k, key);

    e->k = k;
    e->v = v;
    e->l = len;

    i = esht_hash(key) % table->cap;
    e->n = table->entries[i];
    table->entries[i] = e;
    table->len++;

    if ((float)table->len / (float)table->cap > ESHT_MAX_FACTOR)
        if (esht_resize(table, table->cap * 2))
            return 1;

    return 0;
}

int esht_remove(esht *table, char *key)
{
    unsigned long i;
    esht_entry *e;

    i = esht_hash(key) % table->cap;
    e = table->entries[i];
    if (e == NULL)
        return 1;

    if (!strcmp(e->k, key))
    {
        table->entries[i] = e->n;
        goto cleanup;
    }

    while (e->n != NULL)
    {
        if (!strcmp(e->n->k, key))
        {
            e->n = e->n->n;
            goto cleanup;
        }
    }

    return 1;

cleanup:
    free(e->k);
    free(e->v);
    free(e);

    table->len--;
    if ((float)table->len / (float)table->cap < ESHT_MIN_FACTOR)
        if (esht_resize(table, table->cap / 2))
            return 1;

    return 0;
}

void esht_destroy(esht *table)
{
    int i;
    esht_entry *e, *n;

    for (i = 0; i < table->cap; i++)
    {
        e = table->entries[i];
        while (e != NULL)
        {
            n = e->n;
            free(e->k);
            free(e->v);
            free(e);
            e = n;
        }
    }

    free(table->entries);
    free(table);
}