/*
 * File         : utils.c
 * Author       : Vincent Cheung
 * Date         : Jul. 13, 2015
 * Description  : Contains some utils used in lib cue.
 *
 * Copyright (C) 2015, Vincet Cheung<coolingfall@gmail.com>
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include "cue_utils.h"

/**
 * Get file size with specified filename.
 *
 * @param  filename filename
 * @return          file size
 */
long get_file_size(const char *filename)
{
	struct stat statbuf;
	if (stat(filename, &statbuf) < 0)
	{
		return -1;
	}

	return statbuf.st_size;
}

/**
 * Get sub string from source string.
 *
 * @param dest  dest poniter to save string
 * @param src   source string poniter
 * @param start the start index
 * @param end   the end index
 */
void substring(char *dest, const char *src, int start, int end)
{
	int i = start;

	if (start > strlen(src))
	{
		return;
	}

	if (end > strlen(src))
	{
		end = strlen(src);
	}

	while (i < end)
	{
		dest[i - start] = src[i];
		i ++;
	}

	dest[i - start] = '\0';
}

/**
 * To check if the string start with specified string.
 *
 * @param  src    source string pointer
 * @param  prefix prefix string poniter
 * @return        ture if start with specified string, otherwise return false
 */
bool start_with(const char *src, const char *prefix)
{
	int len = strlen(prefix);
	char buf[len];

	substring(buf, src, 0, len);

	return !strcmp(buf, prefix);
}

/**
 * To check if the string end with specified string.
 *
 * @param  src    source string pointer
 * @param  suffix suffix string poniter
 * @return        ture if end with specified string, otherwise return false
 */
bool end_with(const char *src, const char *suffix)
{
	int len = strlen(suffix);
	char buf[len];

	substring(buf, src, strlen(src) - len, strlen(src));

	return !strcmp(buf, suffix);
}

/**
 * To get the index when sub string first appear in src.
 *
 * @param src src string
 * @param sub the string to search
 */
int index_of(const char *src, const char *sub)
{
	char *result = strstr(src, sub);

	return result ? strlen(src) - strlen(result) : -1;
}

/**
 * To get the index when need string last appear in src.
 *
 * @param src  src string
 * @param need the string to search
 */
int last_index_of(const char *src, const char *need)
{
	int i;
	const char *p = src + strlen(src);
	size_t len = strlen(need);
	char *buf;

	for(i = 0; i < strlen(src); i ++)
	{
		buf = strchr(p --, *need);
		if (!buf)
		{
			continue;
		}

		if (strncmp(buf, need, len) == 0)
		{
			
			return strlen(src) - strlen((char *)buf);
		}
	}

	return -1;
}

/**
 * Remove space from string at the beginning or end.
 *
 * @param src pointer to source string 
 */
char *trim(char *src)
{
	int i;
	int len = strlen(src);
	int start = 0, end = len - 1;

	while (start < end && src[start] <= ' ' && src[start] != 0)
	{
		start ++;
	}

	while (end >= start && src[end] <= ' ' && src[end] != 0)
	{
		end --;
	}

	memmove(src, src + start, end - start + 1);
	src[end - start + 1] = '\0';

	return src;
}

/**
 * Read line from input file poniter.
 *
 * @param buf     the buffer to save result
 * @param bufsize the size of buffer
 * @param stream  the pointer to file structure
 */
char *read_line(char *buf, int bufsize, FILE *stream)
{
	char *result = fgets(buf, bufsize, stream);
	if (result != NULL)
	{
		trim(buf);
	}

	return result;
}