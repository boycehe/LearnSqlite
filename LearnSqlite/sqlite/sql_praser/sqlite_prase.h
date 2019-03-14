//
//  sqlite_prase.h
//  LearnSqlite
//
//  Created by heboyce on 2019/3/14.
//  Copyright © 2019 heboyce. All rights reserved.
//

#ifndef sqlite_prase_h
#define sqlite_prase_h

#include <stdio.h>
#include "sqlite_token.h"
#include "sqlite_common.h"

#define sqlite3ParserCTX_PDECL ,Parse *pParse
#define sqlite3ParserCTX_STORE yypParser->pParse=pParse;
#define sqlite3ParserCTX_PARAM ,pParse


typedef struct yyParser yyParser;

typedef struct Parse Parse;
typedef struct Token Token;

struct Parse {
    sqlite3 *db;         /* The main database structure */
    char *zErrMsg;       /* An error message */
    Vdbe *pVdbe;         /* An engine for executing database bytecode */
    int rc;              /* Return code from execution */
    const char *zTail;            /* All SQL text past the last semicolon parsed */
    Table *pNewTable;         /* A table being constructed by CREATE TABLE */
    Trigger *pNewTrigger;     /* Trigger under construct by a CREATE TRIGGER */
    ynVar nVar;               /* Number of '?' variables seen in the SQL so far */
    VList *pVList;            /* Mapping between variable names and numbers */
    Token sLastToken;       /* The last token parsed */
    int nErr;            /* Number of errors seen */
    u8 nested;           /* Number of nested calls to the parser/code generator */
#ifndef SQLITE_OMIT_SHARED_CACHE
    int nTableLock;        /* Number of locks in aTableLock */
    TableLock *aTableLock; /* Required table locks for shared-cache mode */
#endif
    With *pWith;              /* Current WITH clause, or NULL */
    With *pWithToFree;        /* Free this WITH object at the end of the parse */
    AutoincInfo *pAinc;  /* Information about AUTOINCREMENT counters */
    Table *pZombieTab;        /* List of Table objects to delete after code gen */
#ifndef SQLITE_OMIT_VIRTUALTABLE
    Token sArg;               /* Complete text of a module argument */
    Table **apVtabLock;       /* Pointer to virtual tables needing locking */
#endif
    u8 eParseMode;            /* PARSE_MODE_XXX constant */
};

struct yyParser {
    yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
    int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
    int yyerrcnt;                 /* Shifts left before out of the error */
#endif
    sqlite3ParserARG_SDECL                /* A place to hold %extra_argument */
    sqlite3ParserCTX_SDECL                /* A place to hold %extra_context */
#if YYSTACKDEPTH<=0
    int yystksz;                  /* Current side of the stack */
    yyStackEntry *yystack;        /* The parser's stack */
    yyStackEntry yystk0;          /* First stack entry */
#else
    yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
    yyStackEntry *yystackEnd;            /* Last entry in the stack */
#endif
};

#endif /* sqlite_prase_h */
