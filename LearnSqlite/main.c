//
//  main.c
//  LearnSqlite
//
//  Created by heboyce on 2019/1/10.
//  Copyright © 2019 heboyce. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "sqlite_token.h"
#include "sqlite3.h"




int main(int argc, const char * argv[]) {
    // insert code here...
    
  /*
    sqlite3 *_db;
    const char *dbPath = "/Users/heboyce/Desktop/sqlitedemo.db";
    const char *t_sql = "create table if not exists t_student (id integer primary key autoincrement, name text, age integer);";
    if (sqlite3_open(dbPath, &_db) == SQLITE_OK) {
        printf("数据库建立成功");
    }else{
        printf("数据库建立失败");
    }

    char *t_errorMesg = NULL; // 用来存储错误信息
    int t_result = sqlite3_exec(_db, t_sql, NULL, NULL, &t_errorMesg);

    
    return 1;
   */
 
  // const char *keywords = "DESC REINDEX INDEXED INDEX ESCAPE EACH CHECK KEY BEFORE FOREIGN FOR IGNORE REGEXP EXPLAIN INSTEAD ADD DATABASE AS SELECT TABLE LEFT THEN END DEFERRABLE ELSE EXCEPT TRANSACTION ACTION ON NATURAL ALTER RAISE EXCLUSIVE EXISTS SAVEPOINT INTERSECT TRIGGER REFERENCES CONSTRAINT INTO OFFSET OF SET TEMPORARY TEMP OR UNIQUE QUERY WITHOUT WITH OUTER RELEASE ATTACH HAVING GROUP UPDATE BEGIN INNER RANGE BETWEEN NOTHING GLOB BY CASCADE ASC DELETE CASE COLLATE CREATE CURRENT_DATE DETACH IMMEDIATE JOIN INSERT LIKE MATCH PLAN ANALYZE PRAGMA ABORT VALUES VIRTUAL LIMIT WHEN NOTNULL NOT NO NULL WHERE RECURSIVE AFTER RENAME AND DEFAULT AUTOINCREMENT TO IN CAST COLUMN COMMIT CONFLICT CROSS CURRENT_TIMESTAMP CURRENT_TIME CURRENT PARTITION DEFERRED DISTINCT IS DROP PRECEDING FAIL FILTER REPLACE FOLLOWING FROM FULL IF ISNULL ORDER RESTRICT OVER RIGHT ROLLBACK ROWS ROW UNBOUNDED UNION USING VACUUM VIEW WINDOW DO INITIALLY ALL PRIMARY";
    const char *keywords = "seconds";//task cancel upload happen times seconds
   // const char *keywords = "create table if not exists t_student (id integer primary key autoincrement, name text, age integer);";
   // UNIQUE UPDATE CURRENT_DATE CURRENT_TIME
   // const char *keywords = "SELECT INSERT";
   
   // const char * sql = "create task  name = 'ff' where count(x) >0x233for upload * from dogger where value1 = 'defgh' and time > '2019-09-19 10:23' happen 5 times in 60 seconds";

    int len = 0;
    for (int i = 0; i < strlen(keywords) ; i+=len) {
   
        
        int tokenType = 0;
        keywords = &keywords[i];
       // printf("sql:%s\n",keywords);
        len = sqlite3GetToken((unsigned char *)keywords, &tokenType);
        i   = 0;
        if (tokenType != 172) {
           printf("%d\n",tokenType);
        }
      
      /*
        printf("len %d\n",len);
        
        printf("token:");
        for (int m = 0; m < len; m++) {
            printf("%c",keywords[m]);
        }
        
        printf("\n");
        printf("tokentype:%d\n",tokenType);
        */
        
    }
   
    
    
   
    
    return 0;
}


