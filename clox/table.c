#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void initTable(Table* table) {
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void freeTable(Table* table) {
    FREE_ARRAY(Entry, table->entries, table->capacity);
    initTable(table);
}

/**
 * findEntry is responsible for taking a key and an array of buckets, and figuring out which bucket the entry belongs in.
 * It's also where linear probing and collision handling come into play.
 * It's used both to look up existing entries in the hash table and to decide where to insert new ones.
 *
 * 1. First, we use modulo(%) to map the key’s hash code to an index within the array’s bounds.
 *    That gives us a bucket index where, ideally, we’ll be able to find or place the entry.
 *
 * 2. If the key for the Entry at that array index is NULL, then the bucket is empty.
 *    If we’re using findEntry() to look up something in the hash table, this means it isn’t there.
 *    If we’re using it to insert, it means we’ve found a place to add the new entry.
 *
 * 3. If the key in the bucket is equal to the key we’re looking for, then that key is already present in the table.
 *    If we’re doing a lookup, that’s good—we’ve found the key we seek.
 *    If we’re doing an insert, this means we’ll be replacing the value for that key instead of adding a new entry.
 *
 * 4. Otherwise, the bucket has an entry in it, but with a different key. This is a collision.
 *    In that case, we start probing. That’s what that for loop does. We start at the bucket where the entry would ideally go.
 *    If that bucket is empty or has the same key, we’re done.
 *    Otherwise, we advance to the next element (this is the linear part of “linear probing”) and check there.
 *    If we go past the end of the array, that second modulo operator (index = (index + 1) % capacity;) wraps us back around to the beginning.
 *
 * Infinite loops are not possible thank to our load factor. As soon an the array gets close to being full,
 * we grow it in `tableSet`. So we know there will always be empty buckets.
 */
static Entry* findEntry(Entry* entries, int capacity, ObjString* key) {
    uint32_t index = key->hash % capacity;
    for (;;) {
        Entry* entry = &entries[index];
        if (entry->key == key || entry->key == NULL) {
            return entry;
        }

        index = (index + 1) % capacity;
    }
}

bool tableSet(Table* table, ObjString* key, Value value) {
    if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(table->capacity);
        adjustCapacity(table, capacity);
    }

    Entry* entry = findEntry(table->entries, table->capacity, key);
    bool isNewKey = entry->key == NULL;
    if (isNewKey) table->count++;

    entry->key = key;
    entry->value = value;
    return isNewKey;
}