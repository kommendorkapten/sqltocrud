#!/bin/sh

set -u
set -e

GEN=../generate.pl
SQL=../testfiles
CC=cc
TGT=test
LFLAGS="-lsqlite3"
DBFILE=test.db

cleanup() {
    if [ -e ${DBFILE} ]; then
        rm ${DBFILE}
    fi
    if [ -e ${DBFILE}-journal ]; then
        rm ${DBFILE}-journal
    fi    

    if [ -e ${TGT} ]; then
        rm ${TGT}
    fi

    rm -rf *.o
}

trap cleanup 2 EXIT

if [ ${CC} == gcc ]; then
    CFLAGS="-std=c89 -Wconversion -Wno-sign-conversion -Wextra -Wall -Werror -pedantic"
else
    CFLAGS="-std=c89 -pedantic"
fi

echo Generate c files...
${GEN} ${SQL}/*.sql
echo Make sure they are buildable...

for c in `ls ${SQL}/*.c`; do
    echo ${CC} ${CFLAGS} -c ${c}
    ${CC} ${CFLAGS} -c ${c}
done

echo Generate database schema
sqlite3 ${DBFILE} < ${SQL}/manager.sql
sqlite3 ${DBFILE} < ${SQL}/employee.sql
sqlite3 ${DBFILE} < ${SQL}/contract.sql

${CC} ${CFLAGS} manager.o employee.o contract.o test.c -o ${TGT} ${LFLAGS}
./${TGT}

