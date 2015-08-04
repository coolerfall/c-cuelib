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

#include "cue_common.h"

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
	TRACK_TOTAL,
	TRACK_PERFORMER,
	TRACK_TITLE,
	TRACK_START,
	TRACK_END,
} MetaDataField;

CueSheet *parse_cue(const char *filename);
char *get_string_metadata(CueSheet *cue_sheet, int track_index, MetaDataField filed);

#endif