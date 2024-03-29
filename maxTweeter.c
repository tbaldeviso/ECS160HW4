#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define file_length 20000
#define line_length 1024

// Globals to pass between functions
int token_count;
int line_count;
bool quotes = false;
bool newline = false;

// Object to hold name and number of tweets
typedef struct tweeter{
    char name[line_length];
    int count;
}tweeter;

void get_line_count(char *filename){
    FILE *file = fopen(filename, "r");
    int lines = 0;

    if (file == NULL) {
        printf("Invalid Input Format\n");
        exit(0);
    }

    // Does not store in memory
    while (EOF != (fscanf(file, "%*[^\n]"), fscanf(file,"%*c")))
        ++lines;

    fclose(file);

    if (lines > file_length){
        printf("Invalid Input Format\n");
        exit(0);
    }

    line_count = lines;
}

// Get position of name column
int get_namepos(char *header){
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
        if (strcmp(token, "name") == 0 || strcmp(token, "\"name\"") == 0
            || strcmp(token, "name\n") == 0 || strcmp(token, "\"name\"\n") == 0){
            if (name_check == true){
                printf("Invalid Input Format\n");
                exit(0);
            }
            if (strcmp(token, "\"name\"") == 0 || strcmp(token, "\"name\"\n") == 0)
                quotes = true;
            if (strcmp(token, "name\n") == 0 || strcmp(token, "\"name\"\n") == 0)
                newline = true;
            name_pos = counter;
            name_check = true;
        }
        token = strsep(&string, ",");
        counter++;
    }
    // No name column found
    if (name_pos == -1){
        printf("Invalid Input Format\n");
        exit(0);
    }
    token_count = counter+1;
    return name_pos;
}

void line_check(char *line, int line_num){
    char *token;
    char *string;
    int counter;
    // Checks if line is bigger than line_length or blank line
    if (strchr(line, '\n') == NULL || line[0] == '\n'){ 
        // Last line does not have a new line
        if (line_num != line_count){
            printf("Invalid Input Format\n");
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
        printf("Invalid Input Format\n");
        exit(0);
    }
}

char *quoteCheck(char *name){
    char *temp = malloc(line_length * sizeof(char));
    int i;

    for(i = 0; i < strlen(name)-2; i++) {
        temp[i] = name[i+1];
    }

    return temp;
}

char *newlineCheck(char *name){
    // Invalid if null char detected when expected newline char
    if (name[strlen(name)-1] == '\0'){
        printf("Invalid Input Format\n");
        exit(0);
    }
    char *temp = malloc(line_length * sizeof(char));
    int i;

    for(i = 0; i < strlen(name)-1; i++){
        temp[i] = name[i];
    }

    return temp;
}

char **get_names(char *file_path){
    FILE *file = fopen(file_path, "r");
    char **names = malloc(line_count * sizeof(char *));
    int i;
    for (i = 0; i < line_count; ++i){
        names[i] = (char *)malloc(line_length);
    }

    char line[line_length];
    char *token;
    char *string;
    int counter = 0;
    // Header
    fgets(line, sizeof(line), file);
    // Checks if line is bigger than line_length or blank line
    if (strchr(line, '\n') == NULL || line[0] == '\n'){  
        printf("Invalid Input Format\n");
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
        // Remove newline from name
        if (newline)
            name = newlineCheck(token);
        else
            name = token;
        // Check if names are quoted or not
        if (quotes){
            if (strlen(name) < 2){
                printf("Invalid Input Format\n");
                exit(0);
            }
            if ((name[0] != '\"' || name[strlen(name)-1] != '\"')){
                printf("Invalid Input Format\n");
                exit(0);
            }
            name = quoteCheck(name);
        } else {
            if (name[0] == '\"' && name[strlen(name)-1] == '\"'
                && strlen(name) >= 2){
                printf("Invalid Input Format\n");
                exit(0);
            }
        }
            
        strcpy(names[counter++], name);
    }
    fclose(file);
    return names;
}

// Sort tweeters in descending order
void sort (tweeter list[], int size) {
    int a,b;
    for (a = 0; a < size; ++a){
        for (b = a + 1; b < size; ++b){
            if (list[a].count < list[b].count){
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
    for (i = 0; i < line_count; i++){

        // Skip this element if already processed
        if (visited[i] == true)
            continue;

        // Count frequency
        int counter = 1;
        int j;
        for (j = i + 1; j < line_count; j++){
            if (strcmp(names[i], names[j]) == 0){
                visited[j] = true;
                counter++;
            }
        }
        // Insert tweeter if in top 10
        if (num_tweeters >= 10){
            // Sort first 10 tweeters
            if (!sorted){
                sort(tweeters,10);
                num_tweeters++;
                sorted = true;
            }
            // Insert tweeter
            int c;
            int q;
            for (c = 0; c < 10; c++){
                if (counter > tweeters[c].count){
                    for (q = 8; q >= c; q--)
                        tweeters[q+1] = tweeters[q];
                    strcpy(tweeters[c].name,names[i]);
                    tweeters[c].count = counter;
                    break;
                }
            }
        } else {
            // Get first 10 tweeters before inserting
            strcpy(tweeters[num_tweeters].name,names[i]);
            tweeters[num_tweeters].count = counter;
            num_tweeters++;
        }
    }
    // Sort list if finished before 10
    if (num_tweeters < 10)
        sort(tweeters,num_tweeters);

    // Print output
    for (i = 0; i < num_tweeters-1; i++) {
        printf("%s: %d\n", tweeters[i].name, tweeters[i].count);
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
