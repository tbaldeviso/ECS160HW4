#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define file_length 20000
#define line_length 1024

typedef struct tweeter{
    char name[line_length];
    int count;
}tweeter;

int get_line_count(char *filename){
    FILE *file = fopen(filename, "r");
    long int lines = 0;

    if ( file == NULL ) {
        printf("Invalid file");
        exit(0);
    }

    while (EOF != (fscanf(file, "%*[^\n]"), fscanf(file,"%*c"))) //does not store in memory
        ++lines;

    //printf("Lines : %li\n", lines);
    fclose(file);

    if (lines > file_length){
        printf("Invalid file");
        exit(0);
    }

    return lines;
}


int get_namepos(char *header){   //gets positon of the name column
    char *token;
    char *string;
    int name_pos;

    string = strdup(header);
    token = strsep(&string, ",");
    name_pos = 0;
    while (token != NULL){
        if (strcmp(token, "name") == 0 )
            return name_pos;
        token = strsep(&string, ",");
        name_pos++;
    }
    return 0;
}

char **get_names(char *file_path, int line_count){
    FILE *file = fopen(file_path, "r");
    char **names = malloc(line_count * sizeof(char *));
    int i;
    for (i = 0; i < line_count; ++i) {
        names[i] = (char *)malloc(line_length);
    }

    char line[line_length];
    char *token;
    char *string;
    int counter = 0;
    fgets(line, sizeof(line), file);    //header
    int name_pos = get_namepos(line);

    while(fgets(line, sizeof(line), file)){

        if (strchr(line, '\n') == NULL) { // checks if line is bigger than line_length
            if ((counter+1) != line_count){// last line does not have a new line
                printf("Invalid line\n");
                exit(0);
            }
        }

        string = strdup(line);
        token = strsep(&string, ",");
        int j = 0;
        while(token != NULL){
            if (j == name_pos){
                break;
            }
            token = strsep(&string, ",");
            j++;
        }
        strcpy(names[counter++], token);
    }
    fclose(file);
    return names;
}

void get_freq(char **names, int line_count){
    bool visited[file_length] = { false };
    tweeter tweeters[11];

    /*
    tweeter **tweeters = malloc(11*sizeof(tweeter*));
    for (int t = 0; t < 11; ++t) {
        tweeters[t] = malloc(sizeof(tweeter));
        memset(tweeters[t].name,'\0',line_length);
    } */

    // Traverse through array elements and
    // count frequencies
    int i, num_tweeters = 0;
    for (i = 0; i < line_count; i++) {

        // Skip this element if already processed
        if (visited[i] == true)
            continue;

        // Count frequency
        int counter = 1;
        int j;
        for (j = i + 1; j < line_count; j++) {
            if (strcmp(names[i], names[j]) == 0) {
                visited[j] = true;
                counter++;
            }
        }
        // insert tweeter if in top 10
        if (num_tweeters >= 10) {
            // sort first 11 tweeters
            if (num_tweeters == 10) {
                int a;
                int b;
                for (a = 0; a < 10; ++a) {
                    for (b = a + 1; b < 10; ++b) {
                        if (tweeters[a].count < tweeters[b].count) {
                            tweeter temp = tweeters[a];
                            tweeters[a] = tweeters[b];
                            tweeters[b] = temp;
                        }
                    }
                }
            }
            // insert tweeter
            int c;
            int q;
            for (c = 0; c < 10; c++) {
                if (counter > tweeters[c].count) {
                    for (q = 9; q >= c; q--)
                        tweeters[q+1] = tweeters[q];
                    strcpy(tweeters[c].name,names[i]);
                    tweeters[c].count = counter;
                    num_tweeters++;
                    break;
                }
            }
        } else { // get first 10 tweeters
            strcpy(tweeters[num_tweeters].name,names[i]);
            tweeters[num_tweeters].count = counter;
            num_tweeters++;
        }
    }

    for (i = 0; i < 10; i++) {
        printf("%s = %d\n", tweeters[i].name, tweeters[i].count);
    }
}

int main(int argc, char **argv) {
    char **names;
    int line_count;

    line_count = get_line_count(argv[1]);
    names = get_names(argv[1], line_count);
    get_freq(names, line_count);

    int i;
    for (i = 0; i < line_count; ++i) {
        free(names[i]);
    }
    free(names);

	return 0;
}
