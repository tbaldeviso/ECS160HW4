#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define file_length 20000
#define line_length 1024

int get_namepos(char* file_path){   //gets positon of the name column
    char line[line_length];
    char *token;
    char *string;
    int name_pos;
    FILE* file = fopen(file_path, "r");

    fgets(line, line_length, file);
    string = strdup(line);
    token = strsep(&string, ",");
    name_pos = 0;
    while (token != NULL){
        if (strcmp(token, "name") == 0 )
            return name_pos;
        token = strsep(&string, ",");
        name_pos++;
    }
    fclose(file);
    return 0;
}

char** get_names(char* file_path){
    FILE* file = fopen(file_path, "r");
    char **names = malloc(file_length * sizeof(char *));
    int i;
    for (i = 0; i < file_length; ++i) {
        names[i] = (char *)malloc(line_length+1);
    }

    char line[line_length];
    char *token;
    char *string;
    int counter = 0;
    int name_pos = get_namepos(file_path);
    fgets(line, sizeof(line), file);    //skip header
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

int main(int argc, char **argv) {   //for now puts the names in a array of strings
    char **names;
    names = get_names(argv[1]);

    int i;
    for (i = 0; i < 11329; ++i) {
        printf("%s\n", names[i]);
    }

    for (i = 0; i < file_length; ++i) {
        free(names[i]);
    }
    free(names);

	return 0;
}
