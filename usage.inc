
char msg_logo[] = 
"Shar2 version 0 [18.11.2019 22:36]. File archiving utility.\n"
"Copyright (c) 2019 ConeXware, Inc.  All Rights Reserved.\n"
"Written by Eugene D. Shelwien.\n"
"\n"
;

char msg_usage[] = 
"Usage:\n"
"\n"
"  shar a archive path/  -- concatenate files/subdirs at path into \"archive\"\n"
"  shar a - path/        -- output archive data to stdout\n"
"  shar a0 archive path/ -- store files uncompressed\n"
"  shar a3 archive path/ -- compress files with zstd level 3 (solid)\n"
"\n"
"!!! atm only a directory can be added to archive\n"
"!!! last name without trailing slash is turned to \".\"\n"
"!!! because wildcard support is not implemented yet\n"
"\n"
"  shar x archive base -- extract files/dirs from archive to base\\\n"
"  shar x - base       -- extract from stdin to base\\\n"
"\n"
"Notes:\n"
"\n"
"1. File/dir attributes, streams, security info, timestamps are not preserved.\n"
"Links/junctions are not preserved (added as what they point to).\n"
"\n"
"2. Extracted files or archive file at \"a\" command are overwritten without questions.\n"
"\n"
"3. shar2 archive format uses signatures to terminate the files and data streams.\n"
"Thus it's possible to edit the stored archive file, then still extract the archive.\n"
;
