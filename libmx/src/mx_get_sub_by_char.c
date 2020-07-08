#include "libmx.h"
#include <stdio.h>

char *mx_get_substr_start_end(char *str, char start, char end) {
    int b = mx_get_char_index_reverse(str, end);
    int a = mx_get_char_index(str, start);
    char *finish = mx_strnew(b - a);
    int i = 0;

    if (!str || !start || !end)
        return NULL;
    while (1) {
        finish[i] = str[a + i];
        if (str[i + a] == end && i > 0 && a + i == b)
            break;
        i++;
    }
    finish[i + 1] = '\0';
    return finish;
}
