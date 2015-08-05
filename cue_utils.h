/*
 * File         : utils.h
 * Author       : Vincent Cheung
 * Date         : Jul. 13, 2015
 * Description  : The header file of utils.c.
 *
 * Copyright (C) 2015, Vincet Cheung<coolingfall@gmail.com>
 */

#ifndef	__CUE_UTILS_H__
#define	__CUE_UTILS_H__

#include <stdio.h>
#include <stdbool.h>

long get_file_size(const char *filename);
void substring(char *dest, const char *src, int start, int end);
bool starts_with(const char *src, const char *prefix);
bool ends_with(const char *src, const char *suffix);
int index_of(const char *src, const char *sub);
int last_index_of(const char *src, const char *sub);
char *trim(char *src);
char *read_line(char *buf, int bufsize, FILE *stream);
void write_string(const char *input, FILE *fp);

#endif