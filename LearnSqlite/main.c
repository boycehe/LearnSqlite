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



int main(int argc, const char * argv[]) {
    // insert code here...
    
  
    
 
    
    const char * sql = "/**/create table if not exists t_student(id integer primary key autoincrement, name text not null, score real default 0, sex text default '不明');";
  
    int len = 0;
    for (int i = 0; i < strlen(sql) ; i+=len) {
   
        
        int tokenType = 0;
        sql = &sql[i];
        printf("sql:%s\n",sql);
        len = sqlite3GetToken((unsigned char *)sql, &tokenType);
        i = 0;
      
        
        printf("len %d\n",len);
        printf("tokentype:%d\n",tokenType);
        
        
    }
   
    
    
   
    
    return 0;
}


