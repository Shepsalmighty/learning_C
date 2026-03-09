gerp (not grep) can search a file for multiple patterns and write them to multiple outfiles in one pass unlike the ancient, outdated, no good, disgusting, useless grep.

example:

```cat file | gerp -e pattern1 -e pattern2 -o outfile_1 -e pattern3 -v -o outfile_2```

would read from file, search for lines matching pattern_1 or 2 and write those lines to outfile_1
and lines not matching pattern3 (-v non matching) written to outfile_2

much wow
big impress
