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
 * 2. If the key for the Entry at that array index is NULL and its value is NULL as well,
 *    then that means its an empty entry.
 *    If a tombstone has been found then we return it. Otherwise we return the entry.
 *    If its value is not nil, then we have found a tombstone. We note it and keep going.
 *
 * 3. If the key in the bucket is equal to the key we’re looking for, then that key is already present in the table.
 *    If we’re doing a lookup, that’s good—we’ve found the key we seek.
 *    If we’re doing an insert, this means we’ll be replacing the value for that key instead of adding a new entry.
 *
 *    If we’re using findEntry() to look up something in the hash table, this means it isn’t there.
 *    If we’re using it to insert, it means we’ve found a place to add the new entry.
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
    Entry* tombstone = NULL;

    for (;;) {
        Entry* entry = &entries[index];
        if (entry->key == NULL) {
            if (IS_NIL(entry->value)) {
                // Empty entry.
                return tombstone != NULL ? tombstone : entry;
            } else {
                // We found a tombstone.
                if (tombstone == NULL) tombstone = entry;
            }
        } else if (entry->key == key) {
            // We found the key.
            return entry;
        }

        index = (index + 1) % capacity;
    }
}

/**
 * Returns true if entry was found. False otherwise.
 * If the entry exists, it is assigned to the `value` output parameter.
 */
bool tableGet(Table* table, ObjString* key, Value* value) {
    if (table->count == 0) return false;

    Entry* entry = findEntry(table->entries, table->capacity, key);
    if (entry->key == NULL) return false;

    *value = entry->value;
    return true;
}

static void adjustCapacity(Table* table, int capacity) {
    Entry* entries = ALLOCATE(Entry, capacity);
    for (int i = 0; i < capacity; i++) {
        entries[i].key = NULL;
        entries[i].value = NIL_VAL;
    }

    /**
     * In order to choose the bucket for each entry, we take its hash key modulo(%) the array size.
     * That means that when the array size changes, entries may end up in different buckets.
     * Those new buckets may have new collisions that we need to deal with.
     * So the simplest way to get every entry where it belongs is to rebuild the table from scratch by re-inserting every entry into the new empty array.
     *
     * We walk through the old array front to back. Any time we find a non-empty bucket, we insert that entry into the new array.
     * We use findEntry(), passing in the new array instead of the one currently stored in the Table.
     * (This is why findEntry() takes a pointer directly to an Entry array and not the whole Table struct.
     * That way, we can pass the new array and capacity before we’ve stored those in the struct.)
     */
    for (int i = 0; i < table->capacity; i++) {
        Entry* entry = &table->entries[i];
        if (entry->key == NULL) continue;

        Entry* dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
    }

    // After the loop is finished, we can release the memory for the old array.
    FREE_ARRAY(Entry, table->entries, table->capacity);
    table->entries = entries;
    table->capacity = capacity;
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

/**
 * Placing an empty value in the entry that is to be deleted, is called a tombstone.
 * This is done in order to not break the probe sequence and leave trailing entries orphaned and unreachable.
 */
bool tableDelete(Table* table, ObjString* key) {
    if (table->count == 0) return false;

    // Find the entry.
    Entry* entry = findEntry(table->entries, table->capacity, key);
    if (entry->key == NULL) return false;

    // Place a tombstone in the entry.
    entry->key = NULL;
    entry->value = BOOL_VAL(true);
    return true;
}

/**
 * Copies entries from one table to the other.
 * Is used to support method inheritance.
 */
void tableAddAll(Table* from, Table* to) {
    for (int i = 0; i < from->capacity; i++) {
        Entry* entry = &from->entries[i];
        if (entry->key != NULL) {
            tableSet(to, entry->key, entry->value);
        }
    }
}