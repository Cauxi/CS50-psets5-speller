// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 65536;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char lower[LENGTH + 1];
    int i = 0;
    while(word[i] != '\0') {
        lower[i] = tolower(word[i]);
        i++;
    }
    lower[i] = '\0';
    int hash_low = hash(lower);
    node *index = table[hash_low];
    while(index != NULL) {
        if(strcasecmp(index->word, lower) == 0) {
            return true;
        }
        index = index->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int organize = 0;
    int i = 0;
    while(word[i] != '\0') {
        organize = (organize << 2) ^ word[i];
        i++;
    }
    return organize % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    for (int i = 0; i < N; i++) {
        table[i] = NULL;
    }
    char word[LENGTH + 1];
    FILE *source = fopen(dictionary, "r");
    if (source == NULL) {
        return false;
    }

    while (fscanf(source, "%s", word) != EOF) {

    node *n = malloc(sizeof(node));

    if (n == NULL) {
        unload();
        return false;
    } else {
        strcpy(n->word, word);
        int index = hash(n->word);
        n->next = table[index];
        table[index] = n;
    }
    }
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int count = 0;
    for(int i = 0; i < N; i++) {
        node *now = table[i];
        while(now != NULL) {
            count++;
            now = now->next;
        }
    }
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for(int i = 0; i < N; i++) {
        node *index = table[i];
        while(index != NULL) {
            node *buffer = index;
            index = index->next;
            free(buffer);
        }
    }
    return true;
}
