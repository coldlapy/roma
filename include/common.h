#ifndef _COMMON_
#define _COMMON_

typedef enum _line_type line_type;
enum _line_type {
    ASSIGNED,
    METAL,
    MUCH,
    MANY,
    OTHER
};

typedef struct _line_tokens line_tokens;
struct _line_tokens {
    line_type type;
    char **arr;
    int arr_len;
};

typedef struct _assigned_pair assigned_pair;
struct _assigned_pair {
    char word[64];
    char symbols[64];
};

typedef struct _metal_pair metal_pair;
struct _metal_pair {
    char metal[64];
    float credit;
};


extern int split(const char *str, char c, char ***arr);
extern line_type get_line_type(char **arr, int len);
extern int to_number(char *roma);
extern char *get_roma(char **arr, int begin, int len, assigned_pair assigned_pair[], int assigned_len);
extern int check_roma(char *roma);
extern char *answer_begin(char **arr, int begin, int len);



#endif
