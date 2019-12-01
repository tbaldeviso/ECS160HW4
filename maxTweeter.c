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

char **get_names(char *file_path){
    FILE *file = fopen(file_path, "r");
    char **names = malloc(file_length * sizeof(char *));
    int i;
    for (i = 0; i < file_length; ++i) {
        names[i] = (char *)malloc(line_length+1);
    }

    char line[line_length];
    char *token;
    char *string;
    int counter = 0;
    fgets(line, sizeof(line), file);    //header 
    int name_pos = get_namepos(line);

    while(fgets(line, sizeof(line), file)){
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

void get_freq(char **names){
    bool visited[file_length] = { false };
    tweeter tweeters[11];
/*
    tweeter **tweeters = malloc(11*sizeof(tweeter*));
    for (int t = 0; t < 11; ++t) {
        tweeters[t] = malloc(sizeof(tweeter));
        memset(tweeters[t].name,'\0',line_length);
    }
*/
    // Traverse through array elements and
    // count frequencies
    int i, num_tweeters = 0;
    for (i = 0; i < file_length; i++) {

        // Skip this element if already processed
        if (visited[i] == true)
            continue;

        // Count frequency
        int counter = 1;
        int j;
        for (j = i + 1; j < file_length; j++) {
            if (strcmp(names[i], names[j]) == 0) {
                visited[j] = true;
                counter++;
            }
        }
        // insert tweeter if in top 10
        if (num_tweeters >= 10) {
            // sort first 10 tweeters
            if (num_tweeters == 10) {
                for (int a = 0; a < 10; ++a) {
                    for (int b = a + 1; b < 10; ++b) {
                        if (tweeters[a].count < tweeters[b].count) {
                            tweeter temp = tweeters[a];
                            tweeters[a] = tweeters[b];
                            tweeters[b] = temp;
                        }
                    }
                }
            }
            // insert tweeter
            for (int c = 0; c < 10; c++) {
                if (counter > tweeters[c].count) {
                    for (int q = 9; q >= c; q--)
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

    for (int i = 0; i < 10; i++) {
        printf("%s = %d\n", tweeters[i].name, tweeters[i].count); 
    }
}

int main(int argc, char **argv) {   //for now puts the names in a array of strings
    char **names;
    names = get_names(argv[1]);
    get_freq(names);

    int i;
    for (i = 0; i < file_length; ++i) {
        free(names[i]);
    }
    free(names);

	return 0;
}
