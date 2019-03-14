//
//  sqlite_common.h
//  LearnSqlite
//
//  Created by heboyce on 2019/3/14.
//  Copyright © 2019 heboyce. All rights reserved.
//

#ifndef sqlite_common_h
#define sqlite_common_h

#include <assert.h>

#define UINT32_TYPE unsigned int
#define UINT16_TYPE unsigned short int
#define INT16_TYPE short int
#define UINT8_TYPE unsigned char
#define INT8_TYPE signed char

#define SQLITE_OK           0   /* Successful result */
/* beginning-of-error-codes */
#define SQLITE_ERROR        1   /* Generic error */
#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
#define SQLITE_PERM         3   /* Access permission denied */
#define SQLITE_ABORT        4   /* Callback routine requested an abort */
#define SQLITE_BUSY         5   /* The database file is locked */
#define SQLITE_LOCKED       6   /* A table in the database is locked */
#define SQLITE_NOMEM        7   /* A malloc() failed */
#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
#define SQLITE_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
#define SQLITE_FULL        13   /* Insertion failed because database is full */
#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
#define SQLITE_EMPTY       16   /* Internal use only */
#define SQLITE_SCHEMA      17   /* The database schema changed */
#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
#define SQLITE_MISMATCH    20   /* Data type mismatch */
#define SQLITE_MISUSE      21   /* Library used incorrectly */
#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
#define SQLITE_AUTH        23   /* Authorization denied */
#define SQLITE_FORMAT      24   /* Not used */
#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define SQLITE_NOTADB      26   /* File opened that is not a database file */
#define SQLITE_NOTICE      27   /* Notifications from sqlite3_log() */
#define SQLITE_WARNING     28   /* Warnings from sqlite3_log() */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */

#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
#define SQLITE_LIMIT_WORKER_THREADS           11
#define SQLITE_LIMIT_SQL_LENGTH                1
#define SQLITE_NOMEM_BKPT SQLITE_NOMEM


#define sqlite3ParserARG_SDECL
#define sqlite3ParserARG_PDECL
#define sqlite3ParserARG_PARAM
#define sqlite3ParserARG_FETCH
#define sqlite3ParserARG_STORE
#define sqlite3ParserCTX_SDECL Parse *pParse;

#define YYCODETYPE unsigned short int
#define YYNOCODE 277
#define YYACTIONTYPE unsigned short int
#define YYWILDCARD 91
#define sqlite3ParserTOKENTYPE Token

#define PARSE_MODE_NORMAL        0
#define PARSE_MODE_DECLARE_VTAB  1
#define PARSE_MODE_RENAME_COLUMN 2
#define PARSE_MODE_RENAME_TABLE  3

#define IN_SPECIAL_PARSE (pParse->eParseMode!=PARSE_MODE_NORMAL)

typedef UINT32_TYPE u32;           /* 4-byte unsigned integer */
typedef UINT16_TYPE u16;           /* 2-byte unsigned integer */
typedef INT16_TYPE i16;            /* 2-byte signed integer */
typedef UINT8_TYPE u8;             /* 1-byte unsigned integer */
typedef INT8_TYPE i8;              /* 1-byte signed integer */


#define SQLITE_N_LIMIT (SQLITE_LIMIT_WORKER_THREADS+1)
#define SQLITE_API
#define YYMALLOCARGTYPE  u64


typedef struct sqlite3 sqlite3;
typedef struct Db Db;
typedef struct Table Table;
typedef struct Trigger Trigger;
typedef struct Vdbe Vdbe;
typedef struct TableLock TableLock;
typedef struct With With;
typedef struct AutoincInfo AutoincInfo;
typedef struct yyStackEntry yyStackEntry;
typedef struct Lookaside Lookaside;
typedef struct LookasideSlot LookasideSlot;
typedef unsigned long long int sqlite_uint64;
typedef sqlite_uint64 sqlite3_uint64;
typedef sqlite3_uint64 u64;       /* 8-byte unsigned integer */
typedef i16 ynVar;
typedef int VList;
#define IN_RENAME_OBJECT 0


struct Lookaside {
    u32 bDisable;           /* Only operate the lookaside when zero */
    u16 sz;                 /* Size of each buffer in bytes */
    u8 bMalloced;           /* True if pStart obtained from sqlite3_malloc() */
    u32 nSlot;              /* Number of lookaside slots allocated */
    u32 anStat[3];          /* 0: hits.  1: size misses.  2: full misses */
    LookasideSlot *pInit;   /* List of buffers not previously used */
    LookasideSlot *pFree;   /* List of available buffers */
    void *pStart;           /* First byte of available memory space */
    void *pEnd;             /* First byte past end of available space */
};

typedef union {
    int yyinit;
   
} YYMINORTYPE;

struct sqlite3 {
    int aLimit[SQLITE_N_LIMIT];   /* Limits */
    int nVdbeActive;              /* Number of VDBEs currently running */
    union {
        volatile int isInterrupted; /* True if sqlite3_interrupt has been called */
        double notUsed1;            /* Spacer */
    } u1;

    int nVdbeExec;                /* Number of nested calls to VdbeExec() */
    u8 enc;                       /* Text encoding */
    u8 autoCommit;                /* The auto-commit flag. */
    u8 temp_store;                /* 1: file 2: memory 0: default */
    u8 mallocFailed;              /* True if we have seen a malloc failure */
    u8 bBenignMalloc;             /* Do not require OOMs if true */
    u8 dfltLockMode;              /* Default locking-mode for attached dbs */
    Lookaside lookaside;          /* Lookaside malloc configuration */
    u8 suppressErr;               /* Do not issue error messages if true */
    u8 vtabOnConflict;            /* Value to return for s3_vtab_on_conflict() */
    u8 isTransactionSavepoint;    /* True if the outermost savepoint is a TS */
    u8 mTrace;                    /* zero or more SQLITE_TRACE flags */
    u8 noSharedCache;             /* True if no shared-cache backends */
    u8 nSqlExec;                  /* Number of pending OP_SqlExec opcodes */
};

struct Token {
    const char *z;     /* Text of the token.  Not NULL-terminated! */
    unsigned int n;    /* Number of characters in this token */
};

struct Vdbe {
    sqlite3 *db;
    
};

struct TableLock {
    int iDb;               /* The database containing the table to be locked */
    int iTab;              /* The root page of the table to be locked */
    u8 isWriteLock;        /* True for write lock.  False for a read lock */
    const char *zLockName; /* Name of the table */
};

struct With {
    int nCte;                       /* Number of CTEs in the WITH clause */
    With *pOuter;                   /* Containing WITH clause, or NULL */
   
};

struct AutoincInfo {
    AutoincInfo *pNext;   /* Next info block in a list of them all */
    Table *pTab;          /* Table this info block refers to */
    int iDb;              /* Index in sqlite3.aDb[] of database holding pTab */
    int regCtr;           /* Memory register holding the rowid counter */
};


struct Table {
    Table *pNextZombie;  /* Next on the Parse.pZombieTab list */
};

struct Trigger {
    
};

struct yyStackEntry {
    YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
    YYCODETYPE major;      /* The major token value.  This is the code
                            ** number for the token at this stack level */
    YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                            ** is the value of the token  */
};

struct LookasideSlot {
    LookasideSlot *pNext;    /* Next buffer in the list of free buffers */
};




#endif /* sqlite_common_h */
