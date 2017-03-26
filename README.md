# sqltocrud
Simple/crude SQL to C generator.
Reads one or more .sql files and generates matching .h and .c files.
Each table will get a matching struct and creat, read, update and
delete functions. Three extra methods are created for each struct. One
that allocates a new struct, and initializes any pointers to NULL, one
that frees all pointers and and one that destroys the struct (frees
any pointers first).

# Usage

```
$ ./generate.pl path/to/files/*.sql
```

All new files will end up in the same directory as the SQL files
resides in.

## Command line options

* -v enable verbose mode.
* -i run indent(1) on the created .h and .c files. This require write
  permisson to $CWD as indent(1) creates temporary files in it. These
  files are removed by the script.

