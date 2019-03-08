//
//  sqlite_token.h
//  LearnSqlite
//
//  Created by heboyce on 2019/1/12.
//  Copyright © 2019 heboyce. All rights reserved.
//

#ifndef sqlite_token_h
#define sqlite_token_h

# define SQLITE_PRIVATE static

#include <stdio.h>

int sqlite3GetToken(const unsigned char *z, int *tokenType);

#endif /* sqlite_token_h */
