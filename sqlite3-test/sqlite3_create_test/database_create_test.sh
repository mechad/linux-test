#!/bin/bash
# ********************************************************
# @file: createdb.sh
# @author: Lin, Chao
# @create time: 2018-12-21 12:42:39
# @last modified: 2018-12-21 12:42:39
# @description:
# ********************************************************

sql_file="createtable.sql"
execfile="sqlite3_search"
rows=100000
CC=gcc

# Set Bash color
ECHO_PREFIX_INFO="\033[1;32;40mINFO...\033[0;0m"
ECHO_PREFIX_ERROR="\033[1;31;40mError...\033[0;0m"

# Try & echo command  for test command result.
function try_echo_command {
	echo -e $ECHO_PREFIX_INFO $@
	echo -e $ECHO_PREFIX_INFO "-----------------------------------"
    "$@"
    status=$?
    if [ $status -ne 0 ]; then
        echo -e $ECHO_PREFIX_ERROR "ERROR with \"$@\", Return status $status."
        exit $status
    fi
    return $status
}


if [ ! -f $execfile -a -f main.c ]; then
    try_echo_command $CC -o $execfile main.c -lpthread -lsqlite3
    try_echo_command chmod +x $execfile
fi

try_echo_command sqlite3 --version

echo "BEGIN TRANSACTION;" > $sql_file
echo "CREATE TABLE IF NOT EXISTS JiaQian(EDID INTEGER PRIMARY KEY, value text);" >> $sql_file

cnt=1
while(($cnt != $rows)); do
    echo "INSERT INTO JiaQian VALUES($cnt, 'this is $cnt row!');" >> $sql_file
    cnt=$[$cnt + 1]
done

echo "COMMIT TRANSACTION;" >> $sql_file

try_echo_command rm test.db 2>1& > /dev/null

try_echo_command sqlite3 test.db < $sql_file
try_echo_command ./$execfile test.db "select value from JiaQian where EDID=1932;"
try_echo_command sqlite3 test.db "select value from JiaQian where EDID=1932;"
