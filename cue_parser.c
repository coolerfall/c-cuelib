/*
 * File         : cue_parser.c
 * Author       : Vincent Cheung
 * Date         : Jul. 13, 2015
 * Description  : Cue file parser library.
 *
 * Copyright (C) 2015, Vincet Cheung<coolingfall@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "cue_utils.h"
#include "cue_parser.h"

/* parse label REM COMMENT */
static void parse_comment(CueSheet *cue_sheet, const char *input)
{
	int len = strlen(input);
	char buf[len];

	if (!start_with(input, "COMMENT"))
	{
		return;
	}

	substring(buf, input, strlen("COMMENT"), len);
	input = trim(buf);

	if (input[0] == '"' && input[strlen(input) - 1] == '"')
	{
		substring(buf, input, 1, strlen(input) - 1);
		input = buf;
	}

	strcpy(cue_sheet->comment, input);
}

/* parse label REM DATE */
static void parse_date(CueSheet *cue_sheet, const char *input)
{
	int len = strlen(input);
	char buf[len];

	if (!start_with(input, "DATE"))
	{
		return;
	}

	substring(buf, input, strlen("DATE"), len);
	input = trim(buf);

	/* if total track is 0, then current date is global date */
	if (cue_sheet->total_track == 0)
	{
		strcpy(cue_sheet->date, input);
	}
	else
	{
		TrackData *track_data = &cue_sheet->tracks_data[cue_sheet->total_track - 1];
		strcpy(track_data->date, input);
	}
}

/* parse label REM GENRE */
static void parse_genre(CueSheet *cue_sheet, const char *input)
{
	int len = strlen(input);
	char buf[len];

	if (!start_with(input, "GENRE"))
	{
		return;
	}

	substring(buf, input, strlen("GENRE"), len);
	input = trim(buf);

	strcpy(cue_sheet->genre, input);
}

/* parse label REM */
static void parse_rem(CueSheet *cue_sheet, const char *input)
{
	int len = strlen(input);
	char buf[len];

	if (!start_with(input, "REM"))
	{
		return;
	}

	substring(buf, input, strlen("REM"), len);
	input = trim(buf);

	switch (input[0])
	{
	case 'c':
	case 'C':
		parse_comment(cue_sheet, input);
		break;

	case 'd':
	case 'D':
		parse_date(cue_sheet, input);
		break;

	case 'g':
	case 'G':
		parse_genre(cue_sheet, input);
		break;

	default:
		break;
	}
}

/* parse label PERFORMER */
static void parse_performer(CueSheet *cue_sheet, const char *input)
{
	int len = strlen(input);
	char buf[len];

	if (!start_with(input, "PERFORMER"))
	{
		return;
	}

	substring(buf, input, strlen("PERFORMER"), len);
	input = trim(buf);
	
	if (input[0] == '"' && input[strlen(input) - 1] == '"')
	{
		substring(buf, input, 1, strlen(input) - 1);
		input = buf;
	}

	/* if total track is 0, then current title is global title */
	if (cue_sheet->total_track == 0)
	{
		strcpy(cue_sheet->performer, input);
	}
	else
	{
		TrackData *track_data = &cue_sheet->tracks_data[cue_sheet->total_track - 1];
		strcpy(track_data->performer, input);
	}
}

/* parse label FILE */
static void parse_file(CueSheet *cue_sheet, const char *input, const char *filename)
{
	int len = strlen(input);
	char buf[len];
	char real_path[1024];

	if (!start_with(input, "FILE") || !end_with(input, "WAVE"))
	{
		return;
	}

	substring(buf, input, strlen("FILE"), len - strlen("WAVE"));
	input = trim(buf);
	
	if (input[0] == '"' && input[strlen(input) - 1] == '"')
	{
		substring(buf, input, 1, strlen(input) - 1);
		input = buf;
	}

	/* get the real path for cue file */
	realpath(filename, real_path);
	int index = last_index_of(real_path, "/");
	if (index < 0)
	{
		return;
	}

	substring(cue_sheet->filename, real_path, 0, index + 1);
	strcat(cue_sheet->filename, input);
}

/* parse label TITLE */
static void parse_title(CueSheet *cue_sheet, const char *input)
{
	int len = strlen(input);
	char buf[len];

	if (!start_with(input, "TITLE"))
	{
		return;
	}

	substring(buf, input, strlen("TITLE"), len);
	input = trim(buf);
	
	if (input[0] == '"' && input[strlen(input) - 1] == '"')
	{
		substring(buf, input, 1, strlen(input) - 1);
		input = buf;
	}

	/* if total track is 0, then current performer is global performer */
	if (cue_sheet->total_track == 0)
	{
		strcpy(cue_sheet->title, input);
	}
	else
	{
		TrackData *track_data = &cue_sheet->tracks_data[cue_sheet->total_track - 1];
		strcpy(track_data->title, input);
	}
}

/**
 * Parse label TRACK.
 * First track number may be > 1, but all others must be sequential.
 */
static void parse_track(CueSheet *cue_sheet, const char *input)
{
	int len = strlen(input);
	char buf[len];

	if (!start_with(input, "TRACK") || !end_with(input, "AUDIO"))
	{
		return;
	}

	substring(buf, input, strlen("TRACK"), len);
	input = trim(buf);

	cue_sheet->total_track ++;

	int track_num = atoi(input);
	TrackData *track_data = &cue_sheet->tracks_data[cue_sheet->total_track - 1];
	track_data->track_num = track_num;
}

/* calculate mm:ss:ff to millisecond, ff = frames (75 per second) */
static int calculate_duration(const char *input)
{
	int len = strlen(input);
	char buf[len];

	substring(buf, input, 0, len - 6);
	int minute = atoi(trim(buf));
	substring(buf, input, len - 5, len - 3);
	int second = atoi(trim(buf));
	substring(buf, input, len - 2, len);
	int frame = atoi(trim(buf));

	return (int) ((minute * 60 + second + (float) frame / 75) * 1000);
}

/**
 * INDEX [number] [mm:ss:ff], First index must be 0 or 1. 
 * First index must be 00:00:00
 */
static void parse_index(CueSheet *cue_sheet, const char *input)
{
	int len = strlen(input);
	char buf[len];

	if (!start_with(input, "INDEX") || strlen(input) < 8)
	{
		return;
	}

	substring(buf, input, strlen("INDEX"), len);
	input = trim(buf);

	TrackData *track_data = &cue_sheet->tracks_data[cue_sheet->total_track - 1];

	if (start_with(input, "00"))
	{
		substring(buf, input, strlen("00"), len);
		input = trim(buf);
		track_data->index0 = calculate_duration(input);
	}
	else if (start_with(input, "01"))
	{
		substring(buf, input, strlen("01"), len);
		input = trim(buf);
		track_data->index1 = calculate_duration(input);
	}
}

/**
 * Parse cue file into cue sheet.
 *
 * @param filename the filename of cue file
 */
CueSheet *parse_cue(const char *filename)
{
	long file_size = get_file_size(filename);
	char buffer[file_size];

	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		return NULL;
	}

	CueSheet *cue_sheet = malloc(sizeof(CueSheet));
	if (!cue_sheet)
	{
		fclose(fp);
		fp = NULL;
		return NULL;
	}
	memset(cue_sheet, 0, sizeof(CueSheet));

	while (read_line(buffer, file_size, fp) != NULL)
	{
		switch (buffer[0])
		{
		case 'f':
		case 'F':
			parse_file(cue_sheet, buffer, filename);
			break;

		case 'i':
		case 'I':
			parse_index(cue_sheet, buffer);

		case 'p':
		case 'P':
			parse_performer(cue_sheet, buffer);
			break;

		case 'r':
		case 'R':
			parse_rem(cue_sheet, buffer);
			break;

		case 't':
		case 'T':
			switch (buffer[1])
			{
			case 'r':
			case 'R':
				parse_track(cue_sheet, buffer);
				break;

			case 'i':
			case 'I':
				parse_title(cue_sheet, buffer);
				break;

			default:
				printf("unknown");
				break;
			}
			break;

		default:
			printf("unknown");
			break;
		}
	}

	fclose(fp);
	fp = NULL;

	return cue_sheet;
}

/**
 * Convenience function for getting string metadata from the cue sheet.
 *
 * @param cue_sheet   poniter to CueSheet structure
 * @param track_index the index in tracks_data, if want to get global meta data, this
 *                    param just set to -1.
 * @param filed       meta data filed
 */
char *get_string_metadata(CueSheet *cue_sheet, int track_index, MetaDataField filed)
{
	if (!cue_sheet || track_index >= cue_sheet->total_track)
	{
		return NULL;
	}

	TrackData *track_data = &cue_sheet->tracks_data[track_index];

	switch (filed)
	{
	case ALBUM_TITLE:
		return cue_sheet->title;

	case ALBUM_PREFORMER:
		return cue_sheet->performer;

	case ALBUM_DATE:
		return cue_sheet->date;

	case COMMENT:
		return cue_sheet->comment;

	case FILENAME:
		return cue_sheet->filename;

	case GENRE:
		return cue_sheet->genre;

	case TRACK_DATE:
		return strlen(track_data->date) ? track_data->date : cue_sheet->date;

	case TRACK_PERFORMER:
		return strlen(track_data->performer) ? track_data->performer : cue_sheet->performer;

	case TRACK_TITLE:
		return track_data->title;

	default:
		printf("unknown");
		return NULL;
	}
}

/**
 * Convenience function for getting string metadata from the cue sheet.
 *
 * @param cue_sheet   poniter to CueSheet structure
 * @param track_index the index in tracks_data, if want to get global meta data, this
 *                    param just set to -1.
 * @param filed       meta data filed
 */
int get_int_metadata(CueSheet *cue_sheet, int track_index, MetaDataField filed)
{
	if (!cue_sheet || track_index >= cue_sheet->total_track)
	{
		return -1;
	}

	TrackData *track_data = &cue_sheet->tracks_data[track_index];
	TrackData *next_track_data;
	int index0 = track_data->index0;
	int index1 = track_data->index1;
	int next_index0;
	int next_index1;

	switch (filed)
	{
	case TRACK_TOTAL:
		return cue_sheet->total_track;

	case TRACK_NUM:
		return track_data->track_num;

	case TRACK_START:
		return index1 ? index1 : index0;

	case TRACK_END:
		/* if the track is the last track, then there's no end time */
		if (track_index < cue_sheet->total_track - 1)
		{
			next_track_data = &cue_sheet->tracks_data[track_index + 1];
			next_index0 = next_track_data->index0;
			next_index1 = next_track_data->index1;

			return next_index0 ? next_index0 : next_index1;
		}
		else
		{
			return -1;
		}

	default:
		return 0;
	}
}