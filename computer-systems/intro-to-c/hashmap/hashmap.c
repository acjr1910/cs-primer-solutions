#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STARTING_BUCKETS 8
#define MAX_KEY_SIZE 16

typedef struct HashmapItem
{
  const char *key;
  void *value;
} HashmapItem;

typedef struct Hashmap
{
  int length;
  int capacity;
  HashmapItem *items;
} Hashmap;

Hashmap *Hashmap_new()
{
  Hashmap *hashmap = malloc(sizeof(Hashmap));

  hashmap->length = 0;
  hashmap->capacity = STARTING_BUCKETS;
  hashmap->items = calloc(STARTING_BUCKETS, sizeof(HashmapItem));

  return hashmap;
}

unsigned int Hashmap_hash(const char *s)
{
  // FNV-1a
  unsigned int hash = 2166136261u;

  while (*s)
  {
    hash ^= (unsigned char)(*s++);
    hash *= 16777619u;
  }

  return hash;
}

void Hashmap_resize(Hashmap *h)
{
  h->capacity = h->capacity * 2;
}

void Hashmap_set(Hashmap *h, const char *key, void *value)
{
  if (h->length > h->capacity / 2)
  {
    Hashmap_resize(h);
  }

  unsigned int hash = Hashmap_hash(key);

  int i = hash % h->capacity;

  while (i < h->capacity && h->items[i].key != NULL && strcmp(h->items[i].key, key) != 0)
  {
    i++;
  }

  char *key_copy = malloc(strlen(key) + 1);
  strcpy(key_copy, key);

  h->items[i].key = key_copy;
  h->items[i].value = value;
}

void *Hashmap_get(Hashmap *h, const char *key)
{

  unsigned int hash = Hashmap_hash(key);

  int i = hash % h->capacity;

  while (i < h->capacity && h->items[i].key != NULL && strcmp(h->items[i].key, key) != 0)
  {
    i++;
  }

  if (i == h->capacity || h->items[i].key == NULL)
    return NULL;

  return h->items[i].value;
}

void Hashmap_delete(Hashmap *h, const char *key)
{
  unsigned int hash = Hashmap_hash(key);

  int i = hash % h->capacity;

  while (i < h->capacity && h->items[i].key != NULL && strcmp(h->items[i].key, key) != 0)
  {
    i++;
  }

  if (h->items[i].key != NULL)
  {
    h->items[i].key = NULL;
    h->items[i].value = NULL;
  };
}

void Hashmap_free(Hashmap *h)
{
  free(h);
}

int main()
{
  Hashmap *h = Hashmap_new();

  // basic get/set functionality
  int a = 5;
  float b = 7.2;
  Hashmap_set(h, "item a", &a);
  Hashmap_set(h, "item b", &b);
  assert(Hashmap_get(h, "item a") == &a);
  assert(Hashmap_get(h, "item b") == &b);

  // using the same key should override the previous value
  int c = 20;
  Hashmap_set(h, "item a", &c);
  assert(Hashmap_get(h, "item a") == &c);

  // basic delete functionality
  Hashmap_delete(h, "item a");
  assert(Hashmap_get(h, "item a") == NULL);

  // handle collisions correctly
  // note: this doesn't necessarily test expansion
  int i, n = STARTING_BUCKETS * 10, ns[n];
  char key[MAX_KEY_SIZE];
  for (i = 0; i < n; i++)
  {
    ns[i] = i;
    sprintf(key, "item %d", i);
    Hashmap_set(h, key, &ns[i]);
  }
  for (i = 0; i < n; i++)
  {
    sprintf(key, "item %d", i);
    assert(Hashmap_get(h, key) == &ns[i]);
  }

  Hashmap_free(h);

  printf("ok\n");
}
