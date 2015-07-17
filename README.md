c-cuelib
========

A simple cue parser C library for parsing cue sheet.

Usage
=====
* It's simple to use like this:

>		#include "cue_paser.h"
>		
>		CueSheet *cue_sheet = parse_cue(filename);
>
>		/* get meta data */
>		int total_track = cue_sheet->total_track;
>		.....

* There's also a convenience way to get meta data:

>		#include "cue_paser.h"
>		
>		CueSheet *cue_sheet = parse_cue(filename);
>		
>		int track_num = get_int_metadata(cue_sheet, i, TRACK_NUM);
>		int track_start = get_int_metadata(cue_sheet, i, TRACK_START);
>		int track_end = get_int_metadata(cue_sheet, i, TRACK_END));
>		...

All the metadata filed can be found in `MetaDataField`.

License
=======
c-cuelib is release under LGPL license.

