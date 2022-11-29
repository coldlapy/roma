#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

typedef struct {
    char sym;
    int val;
} symbol_value;

static symbol_value sym_val[7] = {
    {'I', 1},
    {'V', 5},
    {'X', 10},
    {'L', 50},
    {'C', 100},
    {'D', 500},
    {'M', 1000}
};

int split(const char *str, char c, char ***arr)
{
    int count = 1;
    int token_len = 1;
    int i = 0;
    char *p, *tmp;
    char *t;

    p = str;
    while (*p != '\0') {
        if (*p == c) {
            // multiple spaces are treated as one space
            for (tmp = p; isspace (*tmp); tmp++) p = tmp;
            count++;
        }
        p++;
    }

    *arr = (char **) malloc(sizeof(char *) * count);
    
    if (*arr == NULL) {
        exit(1);
    }

    p = str;
    while (*p != '\0') {
        if (*p == c) {
            for (tmp = p; isspace (*tmp); tmp++) p = tmp;
            (*arr)[i] = (char *) malloc(sizeof(char) * token_len);
            if ((*arr)[i] == NULL) {   
                exit(1);
            }

            token_len = 0;
            i++;
        }
        p++;
        token_len++;
    }
    
    (*arr)[i] = (char *) malloc(sizeof(char) * token_len);

    if ((*arr)[i] == NULL) {
        exit(1);
    }

    i = 0;
    p = str;
    t = ((*arr)[i]);
    while (*p != '\0') {
        if (*p != c && *p != '\0') {
            *t = *p;
            t++;
        } else {
            for (tmp = p; isspace (*tmp); tmp++) p = tmp;
            *t = '\0';
            i++;
            t = ((*arr)[i]);
        }
        p++;
    }

    return count;
}

static int digits_only(const char *s)
{
    while (*s) {
        if (isdigit(*s++) == 0)
            return 0;
    }

    return 1;
}

static int alpha_only(const char *s)
{
    while (*s) {
        if (isalpha(*s++) == 0)
            return 0;
    }

    return 1;
}

line_type get_line_type(char **arr, int len)
{
    if (len == 3 && alpha_only(arr[0]) && strcmp(arr[1], "is") == 0) {
        return ASSIGNED;
    } else if (digits_only(arr[len - 2]) && strcmp(arr[len - 3], "is") == 0) {
        return METAL;
    } else if (strcmp(arr[0], "how") == 0 && strcmp(arr[1], "much") == 0 
        && strcmp(arr[2], "is") == 0) {
        return MUCH;
    } else if (strcmp(arr[0], "how") == 0 && strcmp(arr[1], "many") == 0 
        && strcmp(arr[2], "Credits") == 0 && strcmp(arr[3], "is") == 0) {
        return MANY;
    } else {
        return OTHER;
    }
}
 
int to_number(char *roma)
{
    int number = 0;
    int prev = 0;
    int i, j, temp;
    for (i = strlen(roma) - 2; i >= 0; i--) {

        for (j = 0; j < 7; j++) {
            if (sym_val[j].sym == roma[i]) {
                temp = sym_val[j].val;
                break;
            }
        }
    
        if (temp < prev) {
            number -= temp;
        } else {
            number += temp;
        }
        prev = temp;
    }
    return number;

}

char *get_roma(char **arr, int begin, int len, assigned_pair assigned_pair[], int assigned_len)
{
    char *roma = (char *) malloc(sizeof(char *) * (len + 1)); 

    int i, j;
    int pos = 0;

    for (i = 0; i < len; i++) {

        for (j = 0; j < assigned_len; j++) {
            if (strcmp(arr[begin], assigned_pair[j].word) == 0) 
                break;
        }

        pos += sprintf(roma + pos, "%s", assigned_pair[j].symbols);
        pos -= 1;
        begin += 1;
    }

    return roma;
}

static int check_repeat(char *roma, int i, int num)
{
    if (num == 3) {
        if (!((roma[i] == roma[i+1] && roma[i] == roma[i+2] && roma[i] == roma[i+3])
            ||(roma[i] == roma[i+1] && roma[i] == roma[i+2] && roma[i] == roma[i+4])))
            return -1;
    } else if (num == 4) {
        if (!((roma[i] == roma[i+1] && roma[i] == roma[i+2] && roma[i] == roma[i+3] && roma[i] == roma[i+4])
            ||(roma[i] == roma[i+1] && roma[i] == roma[i+2] && roma[i] == roma[i+3] && roma[i] == roma[i+5]))) 
            return -1;
    } else if (num > 4) {
        return -1;
    }
    return 0;
}

static int get_sym_val(char sym)
{
    int i;
    for (i = 0; i < 7; i++) {
        if (sym == sym_val[i].sym) {
            return sym_val[i].val;
        }
    }
    return 0;
}

static int only_subtracted(char *roma, int i, char m, char n)
{
     if (get_sym_val(roma[i]) < get_sym_val(roma[i+1]) 
         && !(roma[i+1] == m || roma[i+1] == n)) {
         return -1;
    }
    return 0;
}

int check_roma(char *roma)
{
    int i;
    int len = strlen(roma) - 1;

    int i_num = 0;
    int x_num = 0;
    int c_num = 0;
    int m_num = 0;
    
    int v_num = 0;
    int l_num = 0;
    int d_num = 0;
    
    for (i = len - 1; i >= 0; i--) {

        switch (roma[i]) {
            case 'I':
                i_num++;
                if (i_num == 3) {
                    if (!(roma[i] == roma[i-1] && roma[i] == roma[i-2])) {
                        return -1;
                    } 
                } else if (i_num > 3) {
                    return -1;
                }        
                if (i != len -1 && only_subtracted(roma, i, 'V', 'X') != 0) return -1;
                break;
            case 'X':
                x_num++;
                if (check_repeat(roma, i, x_num) != 0) return -1;
                if (i != len -1 && only_subtracted(roma, i, 'L', 'C') != 0) return -1;
                break;
            case 'C':
                c_num++;
                if (check_repeat(roma, i, c_num) != 0) return -1;
                if (i != len -1 && only_subtracted(roma, i, 'D', 'M') != 0) return -1;
                break;
            case 'M':
                m_num++;
                if (check_repeat(roma, i, m_num) != 0) return -1;            
                break;
                
            case 'V':
                v_num++;
                if (v_num > 1) return -1;
                if (i != len -1 && get_sym_val(roma[i]) < get_sym_val(roma[i-1])) return -1;
            case 'L':
                l_num++;
                if (l_num > 1) return -1;
                if (i != len -1 && get_sym_val(roma[i]) < get_sym_val(roma[i-1])) return -1;
            case 'D':
                d_num++;
                if (d_num > 1) return -1;
                if (i != len -1 && get_sym_val(roma[i]) < get_sym_val(roma[i-1])) return -1;
                break;
                
            default:
                return -1;
                break;
        }
    }

    return 0;

}

char *answer_begin(char **arr, int begin, int len)
{
    int i = 0;
    int answer_len = 0;
    int j = begin;
    for (i = 0; i < len; i++) {
        answer_len += strlen(arr[j++]);
    }

    char *answer = (char *) malloc(sizeof(char *) * (answer_len + 1));
    int pos = 0;

    for (i = 0; i < len; i++) {
        pos += sprintf(answer + pos, "%s ", arr[begin++]);
    }

    return answer;

}



