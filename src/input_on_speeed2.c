#include "ush.h"

void mx_skip_expansion(char *input, unsigned int *i) {
    int br = 0;

    if (input[*i] == '$' && !mx_isescape_char(input, *i)) {
        if (input[*i + 1] == '(' && !mx_isescape_char(input, *i)) {
            *i += 2;
            br++;
            while (input[*i]) {
                if (input[*i] == '(' && !mx_isescape_char(input, *i))
                    br++;
                if (input[*i] == ')' && !mx_isescape_char(input, *i))
                    br--;
                if (!br)
                    break;
                (*i)++;
            }
        }
    }
}

bool mx_check_quotes(char *input) {
    size_t len = strlen(input);
    bool s_qu = false;
    bool d_qu = false;

    for (unsigned int i = 0; i < len; i++) {
        mx_skip_quotes(input, &i, '`');
        mx_skip_expansion(input, &i);
        if (input[i] == '\"'
            && !mx_isescape_char(input, i) && !s_qu) {
            d_qu = !d_qu;
        }
        if (input[i] == '\''
            && !mx_isescape_char(input, i) && !d_qu) {
            s_qu = !s_qu;
        }
    }
    return !s_qu && !d_qu;
}

bool mx_check_brackets(char *c) {
    unsigned int len = strlen(c);
    int stack[len];
    int top = -1;

    for (unsigned int i = 0; i < len; i++) {
        mx_skip_quotes(c, &i, '`');
        if ((c[i] == '(' && !mx_isescape_char(c, i))
            || (c[i] == '{' && !mx_isescape_char(c, i))
            || (c[i] == ')' && !mx_isescape_char(c, i))
            || (c[i] == '}' && !mx_isescape_char(c, i))) {
            top++;
            stack[top] = c[i];
        }
        if ((stack[top] == ')' && (top - 1 >= 0 && stack[top - 1] == '('))
            || (stack[top] == '}' && (top - 1 >= 0 && stack[top - 1] == '{')))
            top = top - 2;
    }
    return top == -1;
}
