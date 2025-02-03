// Name: Alex Nelson (U42895792)
// Description: This program takes in two files, reads the text of one of them, tokenizes each word and outputs a sorted list of the unique words, along with numbers associated with thier positions.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10000

int compare_strings(const void *p, const void *q);

int main(int argc, char *argv[]) {

    // Makes two variables for the input and ouput file.
    FILE *readfile;
    FILE *outfile;

    // Opens the respectives files in the right modes.
    readfile = fopen(argv[1], "r");
    outfile = fopen(argv[2], "a");

    /* Sets variables for the current word, the original list, the list that's going to be sorted,
    and one integer for the number of words and an integer array for the lengths of each sentence. */
    char word[N];
    char *original_list[N];
    char *word_list[N];
    int num_words = 0;
    int sentence_lengths[N] = {0};

    // This loops takes each word as input, and puts it into both original_list and word_list, while keeping track of sentence length.
    int words_in_sentence = 0, num_sentences = 0;
    while (fscanf(readfile, "%s", word) == 1) {
        int length = strlen(word);
        words_in_sentence++;

        // Checks if the word has a period at the end. If so, it removes it, and adds the sentence length to it's array.
        for (char *p = word; *p != '\0'; p++) {
            if (*p == '.') {
                *p = '\0';
                sentence_lengths[num_sentences++] = words_in_sentence;
                words_in_sentence = 0;
                break;
            }
        }

        // Allocates the right amount of memory for the word in both arrays.
        original_list[num_words] = malloc(length + 1);
        word_list[num_words] = malloc(length + 1);

        // Copies the word into both arrays, then increments the number of words.
        strcpy(word_list[num_words], word);
        strcpy(original_list[num_words], word);
        num_words++;
    }

    // Sorts the word_list with quick sort to put them in alphabetical order.
    qsort(word_list, num_words, sizeof(char *), compare_strings);

    // Sets this variable to remember the number of words there were originally.
    int original_num_words = num_words;  

    // Finds how many unique words there are (ignores any duplicates), and updates num_words to match.
    int unique_words = 1;
    for (int i = 1; i < num_words; i++) {
        if (strcmp(word_list[i - 1], word_list[i]) != 0) {
            word_list[unique_words++] = word_list[i];
        }
    }
    num_words = unique_words;

    /* This loop finds the position of the sorted array that corrisponds with each word in the original file.
    And then puts that position into the positions array. */
    int positions[original_num_words];
    for (int i = 0; i < original_num_words; i++) {
        for (int j = 0; j < num_words; j++) {
            if (strcmp(original_list[i], word_list[j]) == 0) {
                positions[i] = j + 1;
                break;
            }
        }
    }

    // prints the number of unique words to the output file.
    fprintf(outfile, "%d\n", num_words);

    // This loops prints the array of sorted words to the ouput file.
    for (int i = 0; i < num_words; i++)
        fprintf(outfile, "%s\n", word_list[i]);

    // This loop prints the positions of each word in the sorted array, and prints a new line for every sentence passed.
    for (int i = 0, j = 0, k = 0; i < original_num_words; i++) {
        if (j == sentence_lengths[k]) {
            fprintf(outfile, "\n");
            j = 0;
            k++;
        }
        fprintf(outfile, "%d ", positions[i]);
        j++;
    }

    // Frees the allocated memory that were in the two word arrays.
    for (int i = 0; i < original_num_words; i++) {
        free(original_list[i]);
        free(word_list[i]);
    }

    // Closes both files.
    fclose(readfile);
    fclose(outfile);

    return 0;
}

// Function used to compare two strings.
int compare_strings(const void *p, const void *q) {
    return strcmp(*(const char **)p, *(const char **)q);
}
