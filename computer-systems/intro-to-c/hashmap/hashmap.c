#include <assert.h>
#include <stdio.h>

#define STARTING_BUCKETS 8

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

int Hashmap_set(Hashmap *hashmap, const char *key, void *value)
{
  if (hashmap->length > hashmap->capacity / 2)
  {
    Hashmap_resize(hashmap);
  }

  unsigned int hash = Hashmap_hash(key);

  int i = hash % hashmap->capacity;

  while (hashmap->items[i].key != NULL)
  {
    i++;
  }

  hashmap->items[i].key = key;
  hashmap->items[i].value = value;
}

int Hashmap_get() {}

int Hashmap_delete() {}

int Hashmap_free() {}

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
  /*
     stretch goals:
     - expand the underlying array if we start to get a lot of collisions
     - support non-string keys
     - try different hash functions
     - switch from chaining to open addressing
     - use a sophisticated rehashing scheme to avoid clustered collisions
     - implement some features from Python dicts, such as reducing space use,
     maintaing key ordering etc. see https://www.youtube.com/watch?v=npw4s1QTmPg
     for ideas
     */
  printf("ok\n");
}
