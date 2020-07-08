#include "libmx.h"

char *get_word_by_char(char *str, char sym) {
    char *word;
    char **all_world = mx_strsplit(str, ' ');
    int i = -1;

    while (all_world[++i]) {
        if (mx_get_char_index(all_world[i],sym) >= 0) {
            word = mx_strdup(all_world[i]);
            return word;
        }
    }
    return NULL;
}
