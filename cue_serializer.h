/*
 * File         : cue_serializer.h
 * Author       : Vincent Cheung
 * Date         : Jul. 31, 2015
 * Description  : The header file of serializer.c.
 *
 * Copyright (C) 2015, Vincet Cheung<coolingfall@gmail.com>
 */

#ifndef	__CUE_SERIALIZER_H__
#define	__CUE_SERIALIZER_H__

#include "cue_common.h"

void serialize_cue(CueSheet *cue_sheet, const char *out_path);

#endif