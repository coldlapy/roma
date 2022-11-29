#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "log.h"

#define INPUT_FILE_PATH                "../input.txt"
#define OUTPUT_FILE_PATH               "../output.txt"
#define OUTPUT_ANSWER_LINE_OTHER       "I have no idea what you are talking about"
#define OUTPUT_INVALID_ROMA            "invalid roman number format"

int main(int argc, char *argv[])
{

    FILE *fp_log;
    fp_log = fopen("./run.log", "w");
    if (fp_log == NULL) {
        exit(EXIT_FAILURE);
    }

    log_add_fp(fp_log, 0);

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_no = 0;
    int i = 0;;

    line_tokens line_tokens[16] = { 0 };

    fp = fopen(INPUT_FILE_PATH, "r");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, fp)) != -1) {

        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
        log_trace("Retrieved line of length %zu:\n", read);
        log_trace("%s", line);

        
        int count = 0;
        char **arr = NULL;        

        // parse input line to array of tokens
        count = split(line, ' ', &arr);

        printf("found %d tokens.\n", count);
        for (i = 0; i < count; i++) {
            printf("string #%d: %s\n", i, arr[i]);
            log_trace("string #%d: %s\n", i, arr[i]);
        }

        line_tokens[line_no].type = get_line_type(arr, count);
        line_tokens[line_no].arr = arr;
        line_tokens[line_no].arr_len = count;
        line_no++;
    }

    fclose(fp);
    if (line) {
        free(line);
    }   

    assigned_pair assigned_pair[16] = { 0 };
    metal_pair metal_pair[16] = { 0 };
    char answer_line[16][64] = { 0 };

    int assigned_len = 0;
    int metal_len = 0;
    int answer_len = 0;

    int j, metal_no, number;
    char *roma, *much, *many;
    float credits, value;

    for (i = 0; i < line_no; i++) {

        switch (line_tokens[i].type) {
            case ASSIGNED:
                strcpy(assigned_pair[assigned_len].word, line_tokens[i].arr[0]);
                strcpy(assigned_pair[assigned_len].symbols, line_tokens[i].arr[2]);
                assigned_len++;
                break;
                
            case METAL:
                metal_no = line_tokens[i].arr_len - 4;                
                strcpy(metal_pair[metal_len].metal, line_tokens[i].arr[metal_no]);

                value = (float)atoi(line_tokens[i].arr[metal_no + 2]);
                roma = get_roma(line_tokens[i].arr, 0, line_tokens[i].arr_len - 4, assigned_pair, assigned_len);
                metal_pair[metal_len].credit = value / (float) to_number(roma); 
                metal_len++;
                free(roma);
                break;
                
            case MUCH:
                roma = get_roma(line_tokens[i].arr, 3, line_tokens[i].arr_len - 4, assigned_pair, assigned_len);
                if (check_roma(roma) != 0) {
                    sprintf(answer_line[answer_len++],"%s", OUTPUT_INVALID_ROMA);
                    free(roma);
                    break;
                } 
                number = to_number(roma);
                much = answer_begin(line_tokens[i].arr, 3, line_tokens[i].arr_len - 4);
                sprintf(answer_line[answer_len++],"%sis %d", much, number);
                free(much);
                free(roma);                
                break;
                
            case MANY:
                roma = get_roma(line_tokens[i].arr, 4, line_tokens[i].arr_len - 5, assigned_pair, assigned_len);
                if (check_roma(roma) != 0) {
                    sprintf(answer_line[answer_len++],"%s", OUTPUT_INVALID_ROMA);
                    free(roma);
                    break;
                } 

                number = to_number(roma);
                
                metal_no = line_tokens[i].arr_len - 2;    
                for (j = 0; j < metal_len; j++) {
                    if (strcmp(line_tokens[i].arr[metal_no], metal_pair[j].metal) == 0) {
                        break;
                    }
                }
                credits = number * metal_pair[j].credit;
                
                many = answer_begin(line_tokens[i].arr, 4, line_tokens[i].arr_len - 6);                
                sprintf(answer_line[answer_len++], "%s%s is %.2f Credits", many, line_tokens[i].arr[metal_no], credits);
                free(roma);
                free(many);
                break;
                
            case OTHER:        
                sprintf(answer_line[answer_len++], OUTPUT_ANSWER_LINE_OTHER);
                break;

            default:
                break;
        }

        free(line_tokens[i].arr);
    }


    fp = fopen(OUTPUT_FILE_PATH, "w");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < answer_len; i++) {
        fprintf(fp, "%s\n", answer_line[i]);
    }

    fclose(fp);
 
    exit(EXIT_SUCCESS);
}
