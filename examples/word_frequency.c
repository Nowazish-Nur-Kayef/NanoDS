/**
 * @file word_frequency.c
 * @brief Real-world example:  Word frequency counter using NanoMap
 * 
 * Demonstrates: 
 * - Hash map usage for counting
 * - String key manipulation
 * - Iterator usage
 * - Practical text processing
 */

#define NANODS_IMPLEMENTATION
#include "../nanods.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64

/**
 * Convert string to lowercase
 */
void to_lowercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

/**
 * Extract words from text and count frequencies
 */
void count_word_frequencies(const char* text, NanoMap* freq_map) {
    char word[MAX_WORD_LEN];
    int word_idx = 0;
    
    for (size_t i = 0; i <= strlen(text); i++) {
        char c = text[i];
        
        if (isalnum((unsigned char)c)) {
            if (word_idx < MAX_WORD_LEN - 1) {
                word[word_idx++] = c;
            }
        } else if (word_idx > 0) {
            word[word_idx] = '\0';
            to_lowercase(word);
            
            /* Get current count */
            int* count_ptr = (int*)nm_get(freq_map, word);
            int new_count = count_ptr ? (*count_ptr + 1) : 1;
            
            /* Allocate new count */
            int* new_count_ptr = (int*)malloc(sizeof(int));
            *new_count_ptr = new_count;
            
            nm_set(freq_map, word, new_count_ptr);
            
            word_idx = 0;
        }
    }
}

/**
 * Find most common word
 */
void find_most_common(NanoMap* freq_map, char* most_common_word, int* max_count) {
    NanoMapIterator iter;
    nm_iter_init(&iter, freq_map);
    
    const char* key;
    void* value;
    
    *max_count = 0;
    strcpy(most_common_word, "");
    
    while (nm_iter_next(&iter, &key, &value) == NANODS_OK) {
        int count = *(int*)value;
        if (count > *max_count) {
            *max_count = count;
            strcpy(most_common_word, key);
        }
    }
}

int main(void) {
    printf("==============================================\n");
    printf("  Word Frequency Counter (NanoDS Demo)\n");
    printf("==============================================\n\n");
    
    const char* text = 
        "The quick brown fox jumps over the lazy dog. "
        "The dog was sleeping under the tree. "
        "The fox was quick and clever. "
        "A quick brown fox is a clever fox. ";
    
    printf("Text:\n\"%s\"\n\n", text);
    
    /* Count word frequencies */
    NanoMap freq_map;
    nm_init(&freq_map);
    
    count_word_frequencies(text, &freq_map);
    
    /* Display results */
    printf("Word Frequencies:\n");
    printf("─────────────────────────────────\n");
    
    NanoMapIterator iter;
    nm_iter_init(&iter, &freq_map);
    
    const char* key;
    void* value;
    
    while (nm_iter_next(&iter, &key, &value) == NANODS_OK) {
        int count = *(int*)value;
        printf("%-15s :  %d\n", key, count);
    }
    
    printf("─────────────────────────────────\n");
    printf("Total unique words: %zu\n\n", nm_size(&freq_map));
    
    /* Find most common word */
    char most_common[MAX_WORD_LEN];
    int max_count;
    find_most_common(&freq_map, most_common, &max_count);
    
    printf("🏆 Most common word: '%s' (%d times)\n\n", most_common, max_count);
    
    /* Cleanup */
    nm_iter_init(&iter, &freq_map);
    while (nm_iter_next(&iter, &key, &value) == NANODS_OK) {
        free(value);
    }
    nm_free(&freq_map);
    
    printf("✅ Done!\n");
    return 0;
}
