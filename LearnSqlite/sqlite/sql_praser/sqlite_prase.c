//
//  sqlite_prase.c
//  LearnSqlite
//
//  Created by heboyce on 2019/3/14.
//  Copyright © 2019 heboyce. All rights reserved.
//

#include "sqlite_prase.h"
#include <stdarg.h>

SQLITE_PRIVATE void *sqlite3Malloc(u64);
SQLITE_PRIVATE void *sqlite3ParserAlloc(void*(*)(u64), Parse*);
SQLITE_PRIVATE int   analyzeWindowKeyword(const unsigned char *z);
SQLITE_PRIVATE int   analyzeOverKeyword(const unsigned char *z, int lastToken);
SQLITE_PRIVATE int   analyzeFilterKeyword(const unsigned char *z, int lastToken);
SQLITE_PRIVATE int   getToken(const unsigned char **pz);
SQLITE_PRIVATE void  sqlite3OomFault(sqlite3 *db);
SQLITE_PRIVATE void  sqlite3ErrorMsg(Parse*, const char*, ...);
SQLITE_API     void  sqlite3_free(void *p);
SQLITE_PRIVATE void  sqlite3Parser(void*, int, Token);
SQLITE_PRIVATE char *sqlite3MPrintf(sqlite3 *db, const char *zFormat, ...);
SQLITE_PRIVATE void  sqlite3ParserFree(void *p,void (*freeProc)(void*));
SQLITE_PRIVATE const char *sqlite3ErrStr(int rc);
SQLITE_API     void sqlite3_log(int iErrCode, const char *zFormat, ...);
SQLITE_PRIVATE void sqlite3DeleteTable(sqlite3 *db, Table *pTable);
SQLITE_PRIVATE void sqlite3VdbeDelete(Vdbe *p);
SQLITE_PRIVATE void sqlite3DbFree(sqlite3 *db, void *p);
SQLITE_PRIVATE void sqlite3DeleteTrigger(sqlite3 *db, Trigger *pTrigger);
SQLITE_PRIVATE void sqlite3WithDelete(sqlite3 *db, With *pWith);
SQLITE_PRIVATE void sqlite3DbFreeNN(sqlite3 *db, void *p);
static YYACTIONTYPE yy_find_shift_action(YYCODETYPE iLookAhead, YYACTIONTYPE stateno);
static YYACTIONTYPE yy_reduce( yyParser *yypParser,unsigned int yyruleno,int yyLookahead,sqlite3ParserTOKENTYPE yyLookaheadToken sqlite3ParserCTX_PDECL);
static void yy_shift(yyParser *yypParser,YYACTIONTYPE yyNewState,YYCODETYPE yyMajor,sqlite3ParserTOKENTYPE yyMinor);
static void yy_accept(yyParser *yypParser);
static void yy_syntax_error(yyParser *yypParser,int yymajor,sqlite3ParserTOKENTYPE yyminor);
static void yy_destructor(yyParser *yypParser,YYCODETYPE yymajor,YYMINORTYPE *yypminor);
static void yy_parse_failed(yyParser *yypParser);
SQLITE_PRIVATE void sqlite3ParserInit(void *yypRawParser sqlite3ParserCTX_PDECL);
SQLITE_PRIVATE int sqlite3ParserFallback(int iToken);



SQLITE_PRIVATE int sqlite3RunParser(Parse *pParse, const char *zSql, char **pzErrMsg){
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
    
    
    
SQLITE_PRIVATE void sqlite3Parser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  sqlite3ParserTOKENTYPE yyminor       /* The value for the token */
  sqlite3ParserARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  YYACTIONTYPE yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser = (yyParser*)yyp;  /* The parser */
  sqlite3ParserCTX_FETCH
  sqlite3ParserARG_STORE

  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif

  yyact = yypParser->yytos->stateno;


  do{
    assert( yyact==yypParser->yytos->stateno );
    yyact = yy_find_shift_action((YYCODETYPE)yymajor,yyact);
    if( yyact >= YY_MIN_REDUCE ){
      yyact = yy_reduce(yypParser,yyact-YY_MIN_REDUCE,yymajor,
                        yyminor sqlite3ParserCTX_PARAM);
    }else if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,(YYCODETYPE)yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      break;
    }else if( yyact==YY_ACCEPT_ACTION ){
      yypParser->yytos--;
      yy_accept(yypParser);
      return;
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif

#ifdef YYERRORSYMBOL
  
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) > YY_MAX_SHIFTREDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
      if( yymajor==YYNOCODE ) break;
      yyact = yypParser->yytos->stateno;
#elif defined(YYNOERRORRECOVERY)
    
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      break;
#else  /* YYERRORSYMBOL is not defined */
    
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      break;
#endif
    }
  }while( yypParser->yytos>yypParser->yystack );

  return;
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
    
SQLITE_PRIVATE void *sqlite3Malloc(u64 n){
   void *p;
  
   return p;
}
    
SQLITE_PRIVATE void sqlite3OomFault(sqlite3 *db){
  if( db->mallocFailed==0 && db->bBenignMalloc==0 ){
      db->mallocFailed = 1;
     if( db->nVdbeExec>0 ){
         db->u1.isInterrupted = 1;
       }
         db->lookaside.bDisable++;
     }
}

SQLITE_PRIVATE void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...){
    
}

/*
 ** Print into memory obtained from sqliteMalloc().  Use the internal
 ** %-conversion extensions.
*/
SQLITE_PRIVATE char *sqlite3MPrintf(sqlite3 *db, const char *zFormat, ...){
  va_list ap;
  char *z;
    /*
  va_start(ap, zFormat);
  z = sqlite3VMPrintf(db, zFormat, ap);
  va_end(ap);
     */
  return z;

}

SQLITE_PRIVATE void sqlite3ParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
        if( p==0 ) return;
#endif
      //  sqlite3ParserFinalize(p);
        (*freeProc)(p);
}

SQLITE_PRIVATE const char *sqlite3ErrStr(int rc){
  
        const char *zErr = "unknown error";
    
        return zErr;
}

SQLITE_API void sqlite3_log(int iErrCode, const char *zFormat, ...){
    /*
      va_list ap;
      if( sqlite3GlobalConfig.xLog ){
            va_start(ap, zFormat);
            renderLogMsg(iErrCode, zFormat, ap);
            va_end(ap);
        }
    */
}

SQLITE_PRIVATE void sqlite3DeleteTable(sqlite3 *db, Table *pTable){
  /* Do not delete the table until the reference count reaches zero. */
  
}

SQLITE_PRIVATE void sqlite3VdbeDelete(Vdbe *p){
 
}

SQLITE_PRIVATE void sqlite3DbFree(sqlite3 *db, void *p){
  
}

SQLITE_PRIVATE void sqlite3DeleteTrigger(sqlite3 *db, Trigger *pTrigger){
 
}

SQLITE_PRIVATE void sqlite3WithDelete(sqlite3 *db, With *pWith){
  
}

SQLITE_PRIVATE void sqlite3DbFreeNN(sqlite3 *db, void *p){
  
}

static YYACTIONTYPE yy_find_shift_action(YYCODETYPE iLookAhead, YYACTIONTYPE stateno){
    
   int i;
   return i;
    
}

static YYACTIONTYPE yy_reduce( yyParser *yypParser,unsigned int yyruleno,int yyLookahead,sqlite3ParserTOKENTYPE yyLookaheadToken sqlite3ParserCTX_PDECL){
  return 1;
}

static void yy_shift(yyParser *yypParser,YYACTIONTYPE yyNewState,YYCODETYPE yyMajor,sqlite3ParserTOKENTYPE yyMinor){
 
}    


static void yy_accept(yyParser *yypParser){
 
}

static void yy_syntax_error(yyParser *yypParser,int yymajor,sqlite3ParserTOKENTYPE yyminor){

}

static void yy_destructor(yyParser *yypParser,YYCODETYPE yymajor,YYMINORTYPE *yypminor){

}
static void yy_parse_failed(yyParser *yypParser){

}

SQLITE_PRIVATE void sqlite3ParserInit(void *yypRawParser sqlite3ParserCTX_PDECL){
    
}

SQLITE_PRIVATE int sqlite3ParserFallback(int iToken){
    
    return 1;
}
