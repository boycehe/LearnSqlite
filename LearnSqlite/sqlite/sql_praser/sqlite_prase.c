//
//  sqlite_prase.c
//  LearnSqlite
//
//  Created by heboyce on 2019/3/14.
//  Copyright © 2019 heboyce. All rights reserved.
//

#include "sqlite_prase.h"
SQLITE_API void sqlite3_free(void *p);

SQLITE_PRIVATE void *sqlite3Malloc(u64);
SQLITE_PRIVATE   void *sqlite3ParserAlloc(void*(*)(u64), Parse*);
static int analyzeWindowKeyword(const unsigned char *z);
static int analyzeOverKeyword(const unsigned char *z, int lastToken);
static int analyzeFilterKeyword(const unsigned char *z, int lastToken);
static int getToken(const unsigned char **pz);
SQLITE_API void sqlite3_free(void *p);

static int sqlite3RunParser(Parse *pParse, const char *zSql, char **pzErrMsg){
    int nErr = 0;                   /* Number of errors encountered */
    void *pEngine;                  /* The LEMON-generated LALR(1) parser */
    int n = 0;                      /* Length of the next token token */
    int tokenType;                  /* type of the next token */
    int lastTokenParsed = -1;       /* type of the previous token */
    sqlite3 *db = pParse->db;       /* The database connection */
    int mxSqlLen;                   /* Max length of an SQL string */
#ifdef sqlite3Parser_ENGINEALWAYSONSTACK
    yyParser sEngine;    /* Space to hold the Lemon-generated Parser object */
#endif
    
    assert( zSql!=0 );
    mxSqlLen = db->aLimit[SQLITE_LIMIT_SQL_LENGTH];
    if( db->nVdbeActive==0 ){
        db->u1.isInterrupted = 0;
    }
    pParse->rc = SQLITE_OK;
    pParse->zTail = zSql;
    assert( pzErrMsg!=0 );
    /* sqlite3ParserTrace(stdout, "parser: "); */
#ifdef sqlite3Parser_ENGINEALWAYSONSTACK
    pEngine = &sEngine;
    sqlite3ParserInit(pEngine, pParse);
#else
    pEngine = sqlite3ParserAlloc(sqlite3Malloc, pParse);
    if( pEngine==0 ){
        sqlite3OomFault(db);
        return SQLITE_NOMEM_BKPT;
    }
#endif
    assert( pParse->pNewTable==0 );
    assert( pParse->pNewTrigger==0 );
    assert( pParse->nVar==0 );
    assert( pParse->pVList==0 );
    while( 1 ){
        n = sqlite3GetToken((u8*)zSql, &tokenType);
        mxSqlLen -= n;
        if( mxSqlLen<0 ){
            pParse->rc = SQLITE_TOOBIG;
            break;
        }
#ifndef SQLITE_OMIT_WINDOWFUNC
        if( tokenType>=TK_WINDOW ){
            assert( tokenType==TK_SPACE || tokenType==TK_OVER || tokenType==TK_FILTER
                   || tokenType==TK_ILLEGAL || tokenType==TK_WINDOW
                   );
#else
            if( tokenType>=TK_SPACE ){
                assert( tokenType==TK_SPACE || tokenType==TK_ILLEGAL );
#endif /* SQLITE_OMIT_WINDOWFUNC */
                if( db->u1.isInterrupted ){
                    pParse->rc = SQLITE_INTERRUPT;
                    break;
                }
                if( tokenType==TK_SPACE ){
                    zSql += n;
                    continue;
                }
                if( zSql[0]==0 ){
                    /* Upon reaching the end of input, call the parser two more times
                     ** with tokens TK_SEMI and 0, in that order. */
                    if( lastTokenParsed==TK_SEMI ){
                        tokenType = 0;
                    }else if( lastTokenParsed==0 ){
                        break;
                    }else{
                        tokenType = TK_SEMI;
                    }
                    n = 0;
#ifndef SQLITE_OMIT_WINDOWFUNC
                }else if( tokenType==TK_WINDOW ){
                    assert( n==6 );
                    tokenType = analyzeWindowKeyword((const u8*)&zSql[6]);
                }else if( tokenType==TK_OVER ){
                    assert( n==4 );
                    tokenType = analyzeOverKeyword((const u8*)&zSql[4], lastTokenParsed);
                }else if( tokenType==TK_FILTER ){
                    assert( n==6 );
                    tokenType = analyzeFilterKeyword((const u8*)&zSql[6], lastTokenParsed);
#endif /* SQLITE_OMIT_WINDOWFUNC */
                }else{
                    sqlite3ErrorMsg(pParse, "unrecognized token: \"%.*s\"", n, zSql);
                    break;
                }
            }
            pParse->sLastToken.z = zSql;
            pParse->sLastToken.n = n;
            sqlite3Parser(pEngine, tokenType, pParse->sLastToken);
            lastTokenParsed = tokenType;
            zSql += n;
            if( pParse->rc!=SQLITE_OK || db->mallocFailed ) break;
        }
        assert( nErr==0 );
#ifdef YYTRACKMAXSTACKDEPTH
        sqlite3_mutex_enter(sqlite3MallocMutex());
        sqlite3StatusHighwater(SQLITE_STATUS_PARSER_STACK,
                               sqlite3ParserStackPeak(pEngine)
                               );
        sqlite3_mutex_leave(sqlite3MallocMutex());
#endif /* YYDEBUG */
#ifdef sqlite3Parser_ENGINEALWAYSONSTACK
        sqlite3ParserFinalize(pEngine);
#else
        sqlite3ParserFree(pEngine, sqlite3_free);
#endif
        if( db->mallocFailed ){
            pParse->rc = SQLITE_NOMEM_BKPT;
        }
        if( pParse->rc!=SQLITE_OK && pParse->rc!=SQLITE_DONE && pParse->zErrMsg==0 ){
            pParse->zErrMsg = sqlite3MPrintf(db, "%s", sqlite3ErrStr(pParse->rc));
        }
        assert( pzErrMsg!=0 );
        if( pParse->zErrMsg ){
            *pzErrMsg = pParse->zErrMsg;
            sqlite3_log(pParse->rc, "%s in \"%s\"",
                        *pzErrMsg, pParse->zTail);
            pParse->zErrMsg = 0;
            nErr++;
        }
        pParse->zTail = zSql;
        if( pParse->pVdbe && pParse->nErr>0 && pParse->nested==0 ){
            sqlite3VdbeDelete(pParse->pVdbe);
            pParse->pVdbe = 0;
        }
#ifndef SQLITE_OMIT_SHARED_CACHE
        if( pParse->nested==0 ){
            sqlite3DbFree(db, pParse->aTableLock);
            pParse->aTableLock = 0;
            pParse->nTableLock = 0;
        }
#endif
#ifndef SQLITE_OMIT_VIRTUALTABLE
        sqlite3_free(pParse->apVtabLock);
#endif
        
        if( !IN_SPECIAL_PARSE ){
            /* If the pParse->declareVtab flag is set, do not delete any table
             ** structure built up in pParse->pNewTable. The calling code (see vtab.c)
             ** will take responsibility for freeing the Table structure.
             */
            sqlite3DeleteTable(db, pParse->pNewTable);
        }
        if( !IN_RENAME_OBJECT ){
            sqlite3DeleteTrigger(db, pParse->pNewTrigger);
        }
        
        if( pParse->pWithToFree ) sqlite3WithDelete(db, pParse->pWithToFree);
        sqlite3DbFree(db, pParse->pVList);
        while( pParse->pAinc ){
            AutoincInfo *p = pParse->pAinc;
            pParse->pAinc = p->pNext;
            sqlite3DbFreeNN(db, p);
        }
        while( pParse->pZombieTab ){
            Table *p = pParse->pZombieTab;
            pParse->pZombieTab = p->pNextZombie;
            sqlite3DeleteTable(db, p);
        }
        assert( nErr==0 || pParse->rc!=SQLITE_OK );
        return nErr;
    }
    
    
    
SQLITE_PRIVATE void *sqlite3ParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE) sqlite3ParserCTX_PDECL){
    yyParser *yypParser;
    yypParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
    if( yypParser ){
        sqlite3ParserCTX_STORE
        sqlite3ParserInit(yypParser sqlite3ParserCTX_PARAM);
    }
        return (void*)yypParser;
}
    
static int analyzeWindowKeyword(const unsigned char *z){
      int t;
      t = getToken(&z);
      if( t!=TK_ID ) return TK_ID;
      t = getToken(&z);
      if( t!=TK_AS ) return TK_ID;
      return TK_WINDOW;
}
static int analyzeOverKeyword(const unsigned char *z, int lastToken){
    if( lastToken==TK_RP ){
            int t = getToken(&z);
            if( t==TK_LP || t==TK_ID ) return TK_OVER;
     }
      return TK_ID;
}
    
static int analyzeFilterKeyword(const unsigned char *z, int lastToken){
     if( lastToken==TK_RP && getToken(&z)==TK_LP ){
          return TK_FILTER;
      }
        return TK_ID;
}
    
static int getToken(const unsigned char **pz){
        const unsigned char *z = *pz;
        int t;                          /* Token type to return */
        do {
            z += sqlite3GetToken(z, &t);
        }while( t==TK_SPACE );
        if( t==TK_ID
           || t==TK_STRING
           || t==TK_JOIN_KW
           || t==TK_WINDOW
           || t==TK_OVER
           || sqlite3ParserFallback(t)==TK_ID
           ){
            t = TK_ID;
        }
        *pz = z;
        return t;
    }
    
    
    
SQLITE_API void sqlite3_free(void *p){
        //todo
}
