#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define file_length 20000
#define line_length 1024

int token_count;
int line_count;
bool quotes = false;

typedef struct tweeter{
    char name[line_length];
    int count;
}tweeter;

void get_line_count(char *filename){
    FILE *file = fopen(filename, "r");
    int lines = 0;

    if ( file == NULL ) {
        printf("Invalid Input Format : Could not open file.\n");
        exit(0);
    }

    while (EOF != (fscanf(file, "%*[^\n]"), fscanf(file,"%*c"))) //does not store in memory
        ++lines;

    //printf("Lines : %li\n", lines);
    fclose(file);

    if (lines > file_length){
        printf("Invalid Input Format : more than 20000 lines\n");
        exit(0);
    }

    line_count = lines;
}


int get_namepos(char *header){   //gets positon of the name column
    char *token;
    char *string;
    bool name_check = false;
    int counter;
    int name_pos;

    string = strdup(header);
    token = strsep(&string, ",");
    name_pos = -1;
    counter = 0;
    while (token != NULL){
        if (strcmp(token, "name") == 0 || strcmp(token, "\"name\"") == 0){
            if (name_check == true){
                printf("Invalid Input Format : Invalid header\n");
                exit(0);
            }
            if (strcmp(token, "\"name\"") == 0)
                quotes = true;
            name_pos = counter;
            name_check = true;
        }
        token = strsep(&string, ",");
        counter++;
    }
    if (name_pos == -1){ // no name column found
        printf("Invalid Input Format : Invalid header\n");
        exit(0);
    }
    token_count = counter+1;
    return name_pos;
}

void line_check(char *line, int line_num){
    char *token;
    char *string;
    int counter;

    if (strchr(line, '\n') == NULL || line[0] == '\n') { // checks if line is bigger than line_length or blank line
        if (line_num != line_count){// last line does not have a new line
            printf("Invalid Input Format : Invalid line format\n");
            exit(0);
        }
    }

    string = strdup(line);
    token = strsep(&string, ",");

    counter = 1;
    while(token != NULL){
        token = strsep(&string, ",");
        counter++;
    }
    if (counter != token_count){
        printf("Invalid Input Format: Invalid line format\n");
        exit(0);
    }
}

char *quoteCheck(char *name){
    char *temp = malloc(line_length * sizeof(char));
    int i;

    for(i = 0; i < strlen(name)-1; i++) {
        temp[i] = name[i+1];
    }
    temp[strlen(name)-2] = '\0';

    return temp;
}

char **get_names(char *file_path){
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
    if (strchr(line, '\n') == NULL || line[0] == '\n') {   // checks if line is bigger than line_length or blank line
        printf("Invalid Input Format : Invalid line format\n");
        exit(0);
    }
    int name_pos = get_namepos(line);

    while(fgets(line, sizeof(line), file)){
        line_check(line, counter+1);
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
        char *name;
        if (quotes){
            name = quoteCheck(token);
        }
        else  {
            name = token;
        }
        strcpy(names[counter++], name);
    }
    fclose(file);
    return names;
}

void sort (tweeter list[], int size) {
    int a,b;
    for (a = 0; a < size; ++a) {
        for (b = a + 1; b < size; ++b) {
            if (list[a].count < list[b].count) {
                tweeter temp = list[a];
                list[a] = list[b];
                list[b] = temp;
            }
        }
    }
}

void get_freq(char **names){
    bool visited[file_length] = { false };
    bool sorted = false;
    tweeter tweeters[10];

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
        if (num_tweeters == 10) {
            // sort first 10 tweeters
            if (!sorted)
                sort(tweeters,10);
            // insert tweeter
            int c;
            int q;
            for (c = 0; c < 10; c++) {
                if (counter > tweeters[c].count) {
                    for (q = 8; q >= c; q--)
                        tweeters[q+1] = tweeters[q];
                    strcpy(tweeters[c].name,names[i]);
                    tweeters[c].count = counter;
                    sorted = true;
                    break;
                }
            }
        } else { // get first 10 tweeters
            strcpy(tweeters[num_tweeters].name,names[i]);
            tweeters[num_tweeters].count = counter;
            num_tweeters++;
        }
    }
    // sort list if finished before 10
    if (num_tweeters < 10)
        sort(tweeters,num_tweeters);

    for (i = 0; i < num_tweeters-1; i++) {
        printf("%s = %d\n", tweeters[i].name, tweeters[i].count);
    }
}

int main(int argc, char **argv) {
    char **names;

    get_line_count(argv[1]);
    names = get_names(argv[1]);
    get_freq(names);

    int i;
    for (i = 0; i < line_count; ++i) {
        free(names[i]);
    }
    free(names);

	return 0;
}
