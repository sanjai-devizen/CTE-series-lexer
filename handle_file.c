#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "file.h"

File* init_file(FILE* f_ptr, char* name){
	File* f = (File*)malloc(sizeof(File));
	f->f_ptr = f_ptr;
	f->row = 1;
	f->column = 1;
	f->name = name;
	f->str = NULL; 

	struct stat st;
    if (fstat(fileno(f_ptr), &st) == -1)
        printf("fstat failed: %s", strerror(errno));
	return f;
}

static int read_from_file_c(File* f){
	int ch = getc(f->f_ptr);
	if (ch == EOF){
		if (f->last == '\n' || f->last == EOF)
			ch = EOF;
		else 
			ch = '\n';
	} else if (ch == '\r'){
		int next_ch = getc(f->f_ptr);
		if (next_ch != '\n')
			ungetc(next_ch, f->f_ptr);
		ch = '\n';
	}
	f->last = ch;
	return ch;
}

static int read_from_string_c(File* f){
	int ch;
	if (*(f->str) == '\0'){
		if (f->last == EOF || f->last == '\n')
			ch = EOF;
		else 
			ch = '\n';
	} else if (*(f->str) == '\r'){
		f->str = f->str + 1;
		if (*(f->str) == '\n')
			f->str = f->str + 1;
		ch = '\n';
	} else {
		ch = *(f->str);
		f->str = f->str + 1;
	}
	f->last = ch;
	return ch;
}

static int get_char(File* f){
	int ch;
	if (f->buflen > 0){
		ch = f->buf[f->buflen - 1];
		f->buflen = f->buflen - 1;
	} else if (f->f_ptr){
		ch = read_from_file_c(f);
	} else {
		ch = read_from_string_c(f);
	}

	if (ch == '\n'){
		f->row = f->row + 1;
		f->column = 1;
	} else {
		f->column = f->column + 1;
	}
	return ch;
}

void return_char(int ch, File* f){
	if (ch == EOF)
		return;
	assert(f->buflen < (sizeof(f->buf) / sizeof(f->buf[0])));
	f->buf[f->buflen] = ch;
	f->buflen = f->buflen + 1;

    if (ch == '\n') {
        f->column = 1;
        f->row = f->row - 1;
    } else {
        f->column = f->column - 1;
    }
}

int scan_file_c(File* f){
	for(;;){
		int ch = get_char(f);
		if(ch == EOF){
			f->str = NULL;
			fclose(f->f_ptr);
			return ch;
		}
		if (ch != '\\'){
			return ch;
		}
		int next_ch = get_char(f);
		if (next_ch == '\n')
			continue;
		return_char(next_ch, f);
		return ch;
	}
}
