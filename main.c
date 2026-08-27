#include <stdio.h>
#include <stdlib.h>
#include "file.h"

void read_from_file(char* file_name){
	char* directory = "./c-files/";
	char path_to_c_file[512];

	snprintf(path_to_c_file, sizeof(path_to_c_file), "%s%s", directory, file_name);
	FILE* f_ptr = fopen(path_to_c_file, "r");
	if (f_ptr == NULL) {
        printf("Error: Could not open file at %s\n", path_to_c_file);
        return; 
    }

	File* f = init_file(f_ptr, file_name);

	int ch;

	while((ch = scan_file_c(f)) != EOF){
		printf("%c", ch);
	}

	free(f);
}

int main(int argc, char** argv){
	if (argc != 2){
		printf("Usage : %s <filename>\n", argv[0]);
		return 1;
	}

	read_from_file(argv[1]);

	return 0;
}