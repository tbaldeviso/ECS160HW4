#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define file_length 20000
#define line_length 1024

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

    // Traverse through array elements and
    // count frequencies
    int i;
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
        printf("%s = %d\n", names[i], counter);
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
