#include "libmx.h"


static int count_reverse_tmp(char *tmp, int number);

char *mx_itoa(int number) {
    char *res = mx_strnew(11);
    int i = 1;
    char *tmp = mx_strnew(10);
    int j = count_reverse_tmp(tmp, number);;

    if (number == 0)
        res[0] = '0';
    else if (number < 0)
        res[0] = '-';
    else i = 0;
    while (j - 1 >= 0) {
        j--;
        res[i] = tmp[j];
        i++;
    }
    free(tmp);
    res[i] = '\0';
    return res;
}

static int count_reverse_tmp(char *tmp, int number) {
    int j = 0;
    while (number != 0) {
        if (number > 0)
            tmp[j] = number % 10 + 48;
        if (number < 0)
            tmp[j] = number % 10 * (-1) + 48;
        number /= 10;
        j++;
    }
    return j;
}
