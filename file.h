#ifndef FILE_H
#define FILE_H

#include <time.h>
#include <stdio.h>

typedef struct {
	FILE* f_ptr;
	char* str;
	char* name;
	int column;
	int row;
	int n_token;
	int last;
	int buf[3];
	int buflen;
	time_t m_time;
} File;

File* init_file(FILE* f_ptr, char* name);

static int read_from_file_c(File* f);
static int read_from_string_c(File* f);
static int get_char(File* f);

void return_char(int ch, File* f);

int scan_file_c(File* f);

#endif 