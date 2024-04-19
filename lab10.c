#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26 //Lowercase only

// Trie structure
struct Trie {
    struct Trie *children[ALPHABET_SIZE];
    int count; //Number of times a word is inserted
};

//Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *node = (struct Trie *)malloc(sizeof(struct Trie));
    node->count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

//Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    struct Trie *pCrawl = pTrie;
    while (*word) {
        if (*word < 'a' || *word > 'z') {
            fprintf(stderr, "Invalid character: %c\n", *word);
            return; 
        }
        int index = *word - 'a';
        if (!pCrawl->children[index]) {
            pCrawl->children[index] = createTrie();
        }
        pCrawl = pCrawl->children[index];
        word++;
    }
    pCrawl->count++;
}


//Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word) {
    struct Trie *pCrawl = pTrie;
    while (*word) {
        int index = *word - 'a';
        if (!pCrawl->children[index]) {
            return 0; // Word not present
        }
        pCrawl = pCrawl->children[index];
        word++;
    }
    return pCrawl->count;
}

//Deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie) {
    if (pTrie) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (pTrie->children[i]) {
                deallocateTrie(pTrie->children[i]);
            }
        }
        free(pTrie);
    }
    return NULL;
}

// This function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    char line[128];
    int count = 0;
    while (fgets(line, sizeof(line), file) && count < 256) { //Ensure no overflow of inWords
        line[strcspn(line, "\n")] = 0; //Removes newline character
        pInWords[count] = strdup(line); //Stores the word
        if (pInWords[count] == NULL) {
            fprintf(stderr, "Failed to duplicate string\n");
            fclose(file);
            return 0; // Handle error appropriately
        }
        count++;
    }
    fclose(file);
    return count;
}

//Main provided in instructions
int main(void) {
    char *inWords[256];
    
    // Read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }
    
    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }
    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");

    return 0;
}
