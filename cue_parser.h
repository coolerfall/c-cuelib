/*
 * File         : cue_parser.h
 * Author       : Vincent Cheung
 * Date         : Jul. 13, 2015
 * Description  : The header file of cue_parser.
 *
 * Copyright (C) 2015, Vincet Cheung<coolingfall@gmail.com>
 */

#ifndef	__CUE_PARSER_H__
#define	__CUE_PARSER_H__

typedef struct TrackData {
	int track_index;
	char title[1024];
	char performer[1024];
	char date[64];
	char filename[1024];
	int index0;				/* index0 in millisecond */
	int index1;				/* index1 in millisecond */
} TrackData;

typedef struct CueSheet {
	char title[1024];
	char performer[1024];
	char comment[1024];
	char date[64];
	char genre[512];
	char filename[1024];
	int total_track;

	TrackData tracks_data[128];
} CueSheet;

/**
 * A enumeration type defination for MetaDataField
 */
typedef enum MetaDataField {
	ALBUM_TITLE = 1,
	ALBUM_PREFORMER,
	ALBUM_DATE,
	COMMENT,
	FILENAME,
	GENRE,
	TRACK_DATE,
	TRACK_NUM,
	TRACK_INDEX,
	TRACK_PERFORMER,
	TRACK_TITLE,
	TRACK_START,
	TRACK_END,
} MetaDataField;

CueSheet *parse_cue(const char *filename);
char *get_string_metadata(CueSheet *cue_sheet, int track_index, MetaDataField filed);

#endif