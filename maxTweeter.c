#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define file_length 20000
#define line_length 1024

int get_namepos(char* filename){ //gets positon of the name column
    char line[line_length];
    char *token;
    int name_pos;
    FILE* file = fopen(filename, "r");

    fgets(line, line_length, file);
    token = strtok(line, ",");
    name_pos = 0;
    while (token != NULL){
        if ((strcmp(token, "name") == 0 || strcmp(token, "\"name\"") == 0))
            return name_pos;
        token = strtok(NULL, ",");
        name_pos++;
    }
    fclose(file);
    return 0;
}

int main(int argc, char **argv) { //for now puts the names in a array of strings
	`FILE* stream = fopen(argv[1], "r");

    char **names = malloc(file_length * sizeof(char *));
    int i;
    for (i = 0; i < file_length; ++i) {
        names[i] = (char *)malloc(line_length+1);
    }

    char line[line_length];
    char *token;
    int counter = 0;
    int name_pos = get_namepos(argv[1]);
    while(fgets(line, sizeof(line), stream)){
        token = strtok(line, ",");
        int j = 0;
        while(token != NULL){
            if (j == name_pos){
                break;
            }
            token = strtok(NULL, ",");
            j++;
        }
        strcpy(names[counter++], token);
    }

    for (i = 0; i < 20; ++i) {
        printf("%s\n", names[i]);
    }


    for (i = 0; i < file_length; ++i) {
        free(names[i]);
    }
    free(names);

	return 0;
}
