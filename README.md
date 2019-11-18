# shar2
Shar2 archiver

Shar2 version 0 [18.11.2019 22:36]. File archiving utility.
Copyright (c) 2019 ConeXware, Inc.  All Rights Reserved.
Written by Eugene D. Shelwien.

Usage:

  shar a archive path/  -- concatenate files/subdirs at path into "archive"
  shar a - path/        -- output archive data to stdout
  shar a0 archive path/ -- store files uncompressed
  shar a3 archive path/ -- compress files with zstd level 3 (solid)

!!! atm only a directory can be added to archive
!!! last name without trailing slash is turned to "."
!!! because wildcard support is not implemented yet

  shar x archive base -- extract files/dirs from archive to base\
  shar x - base       -- extract from stdin to base\

Notes:

1. File/dir attributes, streams, security info, timestamps are not preserved.
Links/junctions are not preserved (added as what they point to).

2. Extracted files or archive file at "a" command are overwritten without questions.

3. shar2 archive format uses signatures to terminate the files and data streams.
Thus it's possible to edit the stored archive file, then still extract the archive.
