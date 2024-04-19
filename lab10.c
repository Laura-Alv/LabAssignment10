#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAX_WORD_LENGTH 100

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int word_count; 
};

// Initializes a Trie node
struct TrieNode *createNode() {
    struct TrieNode *newNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (newNode) {
        newNode->word_count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

// Inserts the word into the Trie structure
void insert(struct TrieNode *root, const char *word) {
    struct TrieNode *current = root;
    while (*word) {
        if (*word < 'a' || *word > 'z') {
            word++;
            continue;
        }
        int index = *word - 'a';
        if (!current->children[index]) {
            current->children[index] = createNode();
        }
        current = current->children[index];
        word++;
    }
    current->word_count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct TrieNode *root, const char *word) {
    struct TrieNode *current = root;
    while (*word) {
        if (*word < 'a' || *word > 'z') {
            return 0;
        }
        int index = *word - 'a';
        if (!current->children[index]) {
            return 0; 
        }
        current = current->children[index];
        word++;
    }
    return current->word_count; 
}

// Deallocates the Trie structure recursively
void deallocateTrie(struct TrieNode *root) {
    if (!root) {
        return;
    }
    // Recursively deallocate all children
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        deallocateTrie(root->children[i]);
    }
    free(root);
}

// Reads words from the dictionary file and populates the array
int readDictionary(const char *filename, char **inWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open dictionary file");
        return -1;
    }
    
    char line[MAX_WORD_LENGTH];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        inWords[count] = (char *)malloc(len);
        if (!inWords[count]) {
            perror("Failed to allocate memory for the word");
            fclose(file);
            return -1;
        }
        strcpy(inWords[count], line);
        
        count++;
        if (count >= 256) {
            break; 
        }
    }
    
    fclose(file);
    return count;
}

int main(void) {
    char *inWords[256]; // Array to hold the words from the dictionary

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    if (numWords == -1) {
        return 1; // Failed to read the dictionary
    }

    // Create a Trie
    struct TrieNode *pTrie = createNode();

    // Insert each word from the dictionary into the Trie
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    // Words to search for their occurrences
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate the Trie structure
    deallocateTrie(pTrie);

    // Free the allocated words
    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}
