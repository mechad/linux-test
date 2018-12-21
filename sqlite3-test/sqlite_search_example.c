/*
 * @file: sqlite_search_example.c
 * @create time: 2018-11-21 09:30:50
 * @last modified: 2018-11-21 09:30:50
 * @description:
 */

//gcc sqlite_search_example.c ./sqlite-autoconf-3250300/sqlite3.c -o sqlite3_search -lpthread -ldl

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>   
#include <string.h>  
#include "sqlite3.h"   
  
//  
//typedef int (*sqlite3_callback)(   
//    void* data,       /* Data provided in the 4th argument of sqlite3_exec() */   
//    int ncols,        /* The number of columns in row */   
//    char** values,    /* An array of strings representing fields in the row */   
//    char** headers    /* An array of strings representing column names */   
//);   
  
int callback(void* data, int ncols, char** values, char** headers)  
{  
    int i;  
    int len =0;  
    int ll=0;  
    for(i=0; i < ncols; i++)  
    {  
        if(strlen(headers[i])>len)  
            len = strlen(headers[i]);  
    }  
      
    for(i=0; i < ncols; i++)   
    {  
        ll = len-strlen(headers[i]);  
        while(ll)  
        {  
            fprintf(stdout," ");  
            --ll;  
        }  
        fprintf(stdout, "%s: %s\n", headers[i], values[i]);  
    }  
  
    fprintf(stdout, "\n");  
    return 0;  
}  
  
int search_by_callback(const char* db_name, const char* sql_cmd)   
{    
    int i = 0 ;    
    int j = 0 ;    
    int nrow = 0, ncolumn = 0;    
    char **azResult; //二维数组存放结果    
    sqlite3 *db=NULL;    
    char *zErrMsg = 0;   
    int rc;  
    int len=0;  
  
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
    rc = sqlite3_exec( db,sql_cmd, callback, NULL, &zErrMsg );  
    if( rc != SQLITE_OK)  
    {    
        fprintf(stderr, "%s %s: %s\n", db_name,sql_cmd, sqlite3_errmsg(db));    
        if(zErrMsg)  
        {  
            fprintf(stderr,"ErrMsg = %s \n", zErrMsg);   
            sqlite3_free(zErrMsg);  
        }  
        sqlite3_close(db);    
        return -1;    
    }  
      
    if(zErrMsg)  
    {  
        sqlite3_free(zErrMsg);  
    }  
  
    //关闭数据库    
    sqlite3_close(db);   
    return 0;    
  
}  
  
int search_by_table(const char* db_name, const char* sql_cmd)   
{    
    int i = 0 ;    
    int j = 0 ;    
    int nrow = 0, ncolumn = 0;    
    char **azResult; //二维数组存放结果    
    sqlite3 *db=NULL;    
    char *zErrMsg = 0;   
    int rc;  
    int len=0;  
  
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
    rc = sqlite3_get_table( db , sql_cmd, &azResult , &nrow , &ncolumn , &zErrMsg );    
    if( rc != SQLITE_OK)  
    {    
        fprintf(stderr, "%s %s: %s\n", db_name,sql_cmd, sqlite3_errmsg(db));    
        if(zErrMsg)  
            fprintf(stderr,"ErrMsg = %s \n", zErrMsg);   
        sqlite3_free_table( azResult );   
        sqlite3_close(db);    
        return -1;    
    }    
  
    for(j=0; j < ncolumn; j++)  
    {  
        if(strlen(azResult[j])>len)  
            len = strlen(azResult[j]);  
    }  
  
    //从第0索引到第 nColumn - 1索引都是字段名称  
    //从第 nColumn 索引开始，后面都是字段值  
    for( i = 0 ; i < nrow; i++ )  
    {  
        for(j=0; j < ncolumn; j++)  
        {  
            int ll = (len- strlen(azResult[j]));  
            while(ll)  
            {  
                printf(" ");  
                --ll;  
            }  
            printf( "%s: %s\n", azResult[j], azResult[(i+1)*ncolumn+j]);  
        }  
        printf("\n");  
    }  
  
#if 0     
    for( j = 0 ; j < ncolumn; j++ )  
    {  
        printf( "%s ", azResult[j]);  
    }  
    printf( "\n");    
  
    for( i=ncolumn ; i<( nrow + 1 ) * ncolumn ; i++ )  
    {  
        if(((i-ncolumn+1)%ncolumn) ==0)  
            printf( "%s\n",azResult[i]);  
        else  
            printf( "%s ",azResult[i] );   
    }  
#endif  
    //与sqlite3_get_table对应，释放掉  azResult 的内存空间    
    sqlite3_free_table( azResult );   
    //关闭数据库    
    sqlite3_close(db);   
    return 0;    
  
}   
  
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
    if(argc != 4)  
    {  
        fprintf(stderr, "usage: %s [table/callback/stmt] <db_name> <sql_cmd>\r\n", argv[0]);  
        return -1;  
    }  
  
    if((strcmp(argv[1],"table") ==0))  
        search_by_table(argv[2],argv[3]);  
    else if((strcmp(argv[1],"callback") ==0))  
        search_by_callback(argv[2],argv[3]);  
    else  
        search_by_stmt(argv[2],argv[3]);  
    return 0;  
}  
