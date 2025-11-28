#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STARTING_BUCKETS 8
#define MAX_KEY_SIZE 16

typedef struct HashmapItem
{
  char *key;
  void *value;
} HashmapItem;

typedef struct Hashmap
{
  int length;
  int capacity;
  HashmapItem *items;
} Hashmap;

void Hashmap_set(Hashmap *h, char *key, void *value);

Hashmap *Hashmap_new()
{
  Hashmap *hashmap = malloc(sizeof(Hashmap));

  hashmap->length = 0;
  hashmap->capacity = STARTING_BUCKETS;
  hashmap->items = calloc(STARTING_BUCKETS, sizeof(HashmapItem));

  return hashmap;
}

unsigned int Hashmap_hash_key(char *s)
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

int Hashmap_find_index(Hashmap *h, char *key, unsigned int hash)
{
  int i = hash % h->capacity;
  int start = i;

  while (i < h->capacity && h->items[i].key != NULL && strcmp(h->items[i].key, key) != 0)
  {
    i = (i + 1) % h->capacity;
    if (i == start)
      return -1;
  }

  return i;
}

void Hashmap_resize(Hashmap *h)
{
  int prev_capacity = h->capacity;
  HashmapItem *prev_items = h->items;

  h->capacity *= 2;
  h->items = calloc(h->capacity, sizeof(HashmapItem));
  h->length = 0;

  for (int i = 0; i < prev_capacity; i++)
  {
    if (prev_items[i].key != NULL)
    {
      Hashmap_set(h, prev_items[i].key, prev_items[i].value);
    }
  }

  free(prev_items);
}

void Hashmap_set(Hashmap *h, char *key, void *value)
{
  if (h->length > h->capacity / 2)
  {
    Hashmap_resize(h);
  }

  int i = Hashmap_find_index(h, key, Hashmap_hash_key(key));

  char *key_copy = malloc(strlen(key) + 1);
  strcpy(key_copy, key);

  h->length++;
  h->items[i].key = key_copy;
  h->items[i].value = value;
}

void *Hashmap_get(Hashmap *h, char *key)
{
  int i = Hashmap_find_index(h, key, Hashmap_hash_key(key));

  if (i == h->capacity || h->items[i].key == NULL)
    return NULL;

  return h->items[i].value;
}

void Hashmap_delete(Hashmap *h, char *key)
{
  int i = Hashmap_find_index(h, key, Hashmap_hash_key(key));

  if (i > -1)
  {
    free(h->items[i].key);
    h->length--;
    h->items[i].key = NULL;
    h->items[i].value = NULL;
  };
}

void Hashmap_free(Hashmap *h)
{
  free(h->items);
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
    void *result = Hashmap_get(h, key);
    if (result != &ns[i])
    {
      printf("Error: Hashmap_get failed for key '%s' (expected %p, got %p)\n", key, (void *)&ns[i], result);
      break;
    }
    assert(Hashmap_get(h, key) == &ns[i]);
  }

  Hashmap_free(h);

  printf("ok\n");
}
