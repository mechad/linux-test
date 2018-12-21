//gcc main.c -o sqlite3_search -lpthread -lsqlite3

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "sqlite3.h"


int search_by_stmt(const char* db_name, const char* sql_cmd)
{
    sqlite3 *db=NULL;
    sqlite3_stmt* stmt = 0;
    int ncolumn = 0;
    const char *column_name;
    int vtype , i;
    int rc;

    if(access(db_name, 0) == -1)
    {
        fprintf(stderr, "%s not found\n", db_name);
        return -1;
    }

    rc = sqlite3_open(db_name, &db);

    if( rc != SQLITE_OK)
    {
        fprintf(stderr, "%s open failed: %s\n", db_name,sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    //查询数据
    rc = sqlite3_prepare_v2(db, sql_cmd, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "%s %s: %s\n", db_name,sql_cmd, sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    ncolumn = sqlite3_column_count(stmt);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        for(i = 0 ; i < ncolumn ; i++ )
        {
            vtype = sqlite3_column_type(stmt , i);
            column_name = sqlite3_column_name(stmt , i);
            switch( vtype )
            {
              case SQLITE_NULL:
                  fprintf(stdout, "%s: null\n", column_name);
                break;
              case SQLITE_INTEGER:
                  fprintf(stdout, "%s: %d\n", column_name,sqlite3_column_int(stmt,i));
                break;
              case SQLITE_FLOAT:
                  fprintf(stdout, "%s: %f\n", column_name,sqlite3_column_double(stmt,i));
                break;
              case SQLITE_BLOB: /* arguably fall through... */
                  fprintf(stdout, "%s: BLOB\n", column_name);
                break;
              case SQLITE_TEXT:
                  fprintf(stdout, "%s: %s\n", column_name,sqlite3_column_text(stmt,i));
                break;
              default:
                  fprintf(stdout, "%s: ERROR [%s]\n", column_name, sqlite3_errmsg(db));
                break;
            }
        }
    }

    sqlite3_finalize(stmt);

    //关闭数据库
    sqlite3_close(db);
    return 0;

}



int main(int argc, char* argv[])
{
    int i=0;
    if(argc != 3)
    {
        fprintf(stderr, "usage: %s <db_name> <sql_cmd>\r\n", argv[0]);
        return -1;
    }

    struct timeval time1, time2;

    gettimeofday(&time1, NULL);
    search_by_stmt(argv[1],argv[2]);
    gettimeofday(&time2, NULL);
	int elapsed_time = (time2.tv_sec - time1.tv_sec) * 1000000 + (time2.tv_usec - time1.tv_usec) / 1;
	printf("-------------------------------------\n");
	if (elapsed_time > 1000000) printf("Elapsed time:%.2fs\n", elapsed_time/1000000.0);
	else if (elapsed_time > 1000) printf("Elapsed time:%.2fms\n", elapsed_time/1000.0);
	else printf("Elapsed time:%dus\n", elapsed_time);
	printf("-------------------------------------\n");


    return 0;
}
