//
//  main.c
//  LearnSqlite
//
//  Created by heboyce on 2019/1/10.
//  Copyright © 2019 heboyce. All rights reserved.
//

#include <stdio.h>
#include "sqlite3.h"

sqlite3 *newDBconnection;

int main(int argc, const char * argv[]) {
    // insert code here...
    
  
    
    const char *path = "/Users/heboyce/Desktop/demo.db";
    //参数2:指向数据库变量的指针的地址
    //返回值:数据库操作结果
    int ret = sqlite3_open(path, &newDBconnection);
    //判断执行结果
    
    if (ret == SQLITE_OK) {
        printf("create db success");
    }
   
    
    const char * sql = "CREATE TABLE IF NOT EXISTS t_Student(id integer PRIMARY KEY AUTOINCREMENT, name text NOT NULL, score real DEFAULT 0, sex text DEFAULT '不明');";
    
  
    int ret1 = sqlite3_exec(newDBconnection, sql, NULL, NULL, NULL);
    
 
    
   
    
    return 0;
}

int openDb(){
    
    const char *path = "";
    //参数2:指向数据库变量的指针的地址
    //返回值:数据库操作结果
    int ret = sqlite3_open(path, &newDBconnection);
    //判断执行结果
    return (ret == SQLITE_OK);
    
}
