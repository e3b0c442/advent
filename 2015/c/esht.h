#ifndef E3B0C442_ESHT_H
#define E3B0C442_ESHT_H

#include <stddef.h>

#define ESHT_MAX_FACTOR 0.75
#define ESHT_MIN_FACTOR 0.25

typedef struct esht esht;

esht *esht_create();
void *esht_get(esht *table, char *key, size_t *len);
int esht_update(esht *table, char *key, void *value, size_t len);
int esht_remove(esht *table, char *key);
void esht_destroy(esht *table);

#endif