/*
 * File         : cue_serializer.c
 * Author       : Vincent Cheung
 * Date         : Jul. 31, 2015
 * Description  : Cue file serializer library.
 *
 * Copyright (C) 2015, Vincet Cheung<coolingfall@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>  
#include <unistd.h>
#include "cue_utils.h"
#include "cue_serializer.h"

/* format millisecond into cue sheet timeline */
static void format_timeline(char *output, int index)
{
	int minute = index / 60000;
	int second = index % 60000 / 1000;
	int frame = index % 60000 % 1000 * 75 / 1000;

	char format[16] = {0};

	if (minute < 10)
	{
		strcat(format, "0%d:");
	}
	else
	{
		strcat(format, "%d:");
	}

	if (second < 10)
	{
		strcat(format, "0%d:");
	}
	else
	{
		strcat(format, "%d:");
	}

	if (frame < 10)
	{
		strcat(format, "0%d");
	}
	else
	{
		strcat(format, "%d");
	}

	sprintf(output, format, minute, second, frame);
}

/**
 * Serialize cue sheet into a file.
 */
void serialize_cue(CueSheet *cue_sheet, const char *out_path)
{
	int i;

	if (!out_path || !cue_sheet || cue_sheet->total_track <= 0)
	{
		return;
	}

	for (i = 0; i < cue_sheet->total_track; i ++)
	{
		if (!strlen(cue_sheet->tracks_data[i].title))
		{
			printf("track title cannot be null");
			return;
		}
	}

	/* clear file if existed */
	truncate(out_path, 0);

	FILE *fp = fopen(out_path, "w");
	if (!fp)
	{
		return;
	}

	/* write global data, such as genre, album performer, file and so on */
	if (strlen(cue_sheet->genre))
	{
		write_string("REM GENRE ", fp);
		write_string(cue_sheet->genre, fp);
		write_string("\n", fp);
	}

	if (strlen(cue_sheet->date))
	{
		write_string("REM DATE ", fp);
		write_string(cue_sheet->date, fp);
		write_string("\n", fp);
	}

	if (strlen(cue_sheet->comment))
	{
		write_string("REM COMMENT ", fp);
		write_string(cue_sheet->comment, fp);
		write_string("\n", fp);
	}

	if (strlen(cue_sheet->performer))
	{
		write_string("PERFORMER \"", fp);
		write_string(cue_sheet->performer, fp);
		write_string("\"\n", fp);
	}

	if (strlen(cue_sheet->title))
	{
		write_string("TITLE \"", fp);
		write_string(cue_sheet->title, fp);
		write_string("\"\n", fp);
	}

	int len = strlen(cue_sheet->filename);
	char relative_path[len];
	int index = last_index_of(cue_sheet->filename, "/");
	if (index > 0)
	{
		substring(relative_path, cue_sheet->filename, index + 1, strlen(cue_sheet->filename));
	}
	else
	{
		strcpy(relative_path, cue_sheet->filename);
	}

	write_string("FILE \"", fp);
	write_string(relative_path, fp);
	write_string("\" WAVE\n", fp);

	/* write track data */
	for (i = 0; i < cue_sheet->total_track; i ++)
	{
		TrackData track_data = cue_sheet->tracks_data[i];

		/* write track audio */
		int track_num = track_data.track_num;
		char num_string[4] = {0};
		
		if (track_num < 10)
		{
			sprintf(num_string, "0%d", track_num);
		}
		else
		{
			sprintf(num_string, "%d", track_num);
		}

		write_string("  TRACK ", fp);
		write_string(num_string, fp);
		write_string(" AUDIO\n", fp);

		/* write title */
		write_string("    TITLE \"", fp);
		write_string(track_data.title, fp);
		write_string("\"\n", fp);

		/* write track performer */
		if (strlen(track_data.performer))
		{
			trim(track_data.performer);
			write_string("    PERFORMER \"", fp);
			write_string(track_data.performer, fp);
			write_string("\"\n", fp);
		}

		char index0[16] = {0};
		char index1[16] = {0};
		if (i == 0)
		{
			write_string("    INDEX 01 00:00:00\n", fp);
		}

		if (track_data.index0 != 0)
		{
			write_string("    INDEX 00 ", fp);
			format_timeline(index0, track_data.index0);
			write_string(index0, fp);
			write_string("\n", fp);
		}

		if (track_data.index1 != 0)
		{
			write_string("    INDEX 01 ", fp);
			format_timeline(index1, track_data.index1);
			write_string(index1, fp);
			write_string("\n", fp);
		}
	}

	fclose(fp);
}