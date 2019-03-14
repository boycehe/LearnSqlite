//
//  sqlite_common.h
//  LearnSqlite
//
//  Created by heboyce on 2019/3/14.
//  Copyright © 2019 heboyce. All rights reserved.
//

#ifndef sqlite_common_h
#define sqlite_common_h
#include <pthread.h>

#define UINT32_TYPE unsigned int
#define UINT16_TYPE unsigned short int
#define INT16_TYPE short int
#define UINT8_TYPE unsigned char
#define INT8_TYPE signed char

typedef long long int sqlite_int64;
typedef unsigned long long int sqlite_uint64;


typedef sqlite_int64 i64;          /* 8-byte signed integer */
typedef sqlite_uint64 u64;         /* 8-byte unsigned integer */
typedef UINT32_TYPE u32;           /* 4-byte unsigned integer */
typedef UINT16_TYPE u16;           /* 2-byte unsigned integer */
typedef INT16_TYPE i16;            /* 2-byte signed integer */
typedef UINT8_TYPE u8;             /* 1-byte unsigned integer */
typedef INT8_TYPE i8;              /* 1-byte signed integer */

typedef struct sqlite3_io_methods sqlite3_io_methods;
typedef struct sqlite3_file sqlite3_file;
typedef struct sqlite3_mutex sqlite3_mutex;
typedef sqlite_int64 sqlite3_int64;
typedef struct sqlite3_vfs sqlite3_vfs;
typedef struct CollSeq CollSeq;
typedef struct Db Db;
typedef struct Btree Btree;
typedef struct sqlite3 sqlite3;
typedef struct BtShared BtShared;

struct Btree {
    sqlite3 *db;       /* The database connection holding this btree */
    BtShared *pBt;     /* Sharable content of this btree */
    u8 inTrans;        /* TRANS_NONE, TRANS_READ or TRANS_WRITE */
    u8 sharable;       /* True if we can share pBt with another db */
    u8 locked;         /* True if db currently has pBt locked */
    u8 hasIncrblobCur; /* True if there are one or more Incrblob cursors */
    int wantToLock;    /* Number of nested calls to sqlite3BtreeEnter() */
    int nBackup;       /* Number of backup operations reading this btree */
    u32 iDataVersion;  /* Combines with pBt->pPager->iDataVersion */
    Btree *pNext;      /* List of other sharable Btrees from the same db */
    Btree *pPrev;      /* Back pointer of the same list */
#ifndef SQLITE_OMIT_SHARED_CACHE
    BtLock lock;       /* Object used to lock page 1 */
#endif
};

struct Db {
    char *zDbSName;      /* Name of this database. (schema name, not filename) */
    Btree *pBt;          /* The B*Tree structure for this database file */
    u8 safety_level;     /* How aggressive at syncing data to disk */
    u8 bSyncSet;         /* True if "PRAGMA synchronous=N" has been run */
    Schema *pSchema;     /* Pointer to database schema (possibly shared) */
};

struct CollSeq {
    char *zName;          /* Name of the collating sequence, UTF-8 encoded */
    u8 enc;               /* Text encoding handled by xCmp() */
    void *pUser;          /* First argument to xCmp() */
    int (*xCmp)(void*,int, const void*, int, const void*);
    void (*xDel)(void*);  /* Destructor for pUser */
};

struct sqlite3_file {
    const struct sqlite3_io_methods *pMethods;  /* Methods for an open file */
};

struct sqlite3_io_methods {
    int iVersion;
    int (*xClose)(sqlite3_file*);
    int (*xRead)(sqlite3_file*, void*, int iAmt, sqlite3_int64 iOfst);
    int (*xWrite)(sqlite3_file*, const void*, int iAmt, sqlite3_int64 iOfst);
    int (*xTruncate)(sqlite3_file*, sqlite3_int64 size);
    int (*xSync)(sqlite3_file*, int flags);
    int (*xFileSize)(sqlite3_file*, sqlite3_int64 *pSize);
    int (*xLock)(sqlite3_file*, int);
    int (*xUnlock)(sqlite3_file*, int);
    int (*xCheckReservedLock)(sqlite3_file*, int *pResOut);
    int (*xFileControl)(sqlite3_file*, int op, void *pArg);
    int (*xSectorSize)(sqlite3_file*);
    int (*xDeviceCharacteristics)(sqlite3_file*);
    /* Methods above are valid for version 1 */
    int (*xShmMap)(sqlite3_file*, int iPg, int pgsz, int, void volatile**);
    int (*xShmLock)(sqlite3_file*, int offset, int n, int flags);
    void (*xShmBarrier)(sqlite3_file*);
    int (*xShmUnmap)(sqlite3_file*, int deleteFlag);
    /* Methods above are valid for version 2 */
    int (*xFetch)(sqlite3_file*, sqlite3_int64 iOfst, int iAmt, void **pp);
    int (*xUnfetch)(sqlite3_file*, sqlite3_int64 iOfst, void *p);
    /* Methods above are valid for version 3 */
    /* Additional methods may be added in future releases */
};

struct sqlite3_vfs {
    int iVersion;            /* Structure version number (currently 3) */
    int szOsFile;            /* Size of subclassed sqlite3_file */
    int mxPathname;          /* Maximum file pathname length */
    sqlite3_vfs *pNext;      /* Next registered VFS */
    const char *zName;       /* Name of this virtual file system */
    void *pAppData;          /* Pointer to application-specific data */
    int (*xOpen)(sqlite3_vfs*, const char *zName, sqlite3_file*,
                 int flags, int *pOutFlags);
    int (*xDelete)(sqlite3_vfs*, const char *zName, int syncDir);
    int (*xAccess)(sqlite3_vfs*, const char *zName, int flags, int *pResOut);
    int (*xFullPathname)(sqlite3_vfs*, const char *zName, int nOut, char *zOut);
    void *(*xDlOpen)(sqlite3_vfs*, const char *zFilename);
    void (*xDlError)(sqlite3_vfs*, int nByte, char *zErrMsg);
    void (*(*xDlSym)(sqlite3_vfs*,void*, const char *zSymbol))(void);
    void (*xDlClose)(sqlite3_vfs*, void*);
    int (*xRandomness)(sqlite3_vfs*, int nByte, char *zOut);
    int (*xSleep)(sqlite3_vfs*, int microseconds);
    int (*xCurrentTime)(sqlite3_vfs*, double*);
    int (*xGetLastError)(sqlite3_vfs*, int, char *);
    /*
     ** The methods above are in version 1 of the sqlite_vfs object
     ** definition.  Those that follow are added in version 2 or later
     */
    int (*xCurrentTimeInt64)(sqlite3_vfs*, sqlite3_int64*);
    /*
     ** The methods above are in versions 1 and 2 of the sqlite_vfs object.
     ** Those below are for version 3 and greater.
     */
    int (*xSetSystemCall)(sqlite3_vfs*, const char *zName, sqlite3_syscall_ptr);
    sqlite3_syscall_ptr (*xGetSystemCall)(sqlite3_vfs*, const char *zName);
    const char *(*xNextSystemCall)(sqlite3_vfs*, const char *zName);
    /*
     ** The methods above are in versions 1 through 3 of the sqlite_vfs object.
     ** New fields may be appended in future versions.  The iVersion
     ** value will increment whenever this happens.
     */
};

struct sqlite3_mutex {
    pthread_mutex_t mutex;     /* Mutex controlling the lock */
#if SQLITE_MUTEX_NREF || defined(SQLITE_ENABLE_API_ARMOR)
    int id;                    /* Mutex type */
#endif
#if SQLITE_MUTEX_NREF
    volatile int nRef;         /* Number of entrances */
    volatile pthread_t owner;  /* Thread that is within this mutex */
    int trace;                 /* True to trace changes */
#endif
};

struct sqlite3 {
    sqlite3_vfs *pVfs;            /* OS Interface */
    struct Vdbe *pVdbe;           /* List of active virtual machines */
    CollSeq *pDfltColl;           /* The default collating sequence (BINARY) */
    sqlite3_mutex *mutex;         /* Connection mutex */
    Db *aDb;                      /* All backends */
    int nDb;                      /* Number of backends currently in use */
    u32 mDbFlags;                 /* flags recording internal state */
    u64 flags;                    /* flags settable by pragmas. See below */
    i64 lastRowid;                /* ROWID of most recent insert (see above) */
    i64 szMmap;                   /* Default mmap_size setting */
    u32 nSchemaLock;              /* Do not reset the schema when non-zero */
    unsigned int openFlags;       /* Flags passed to sqlite3_vfs.xOpen() */
    int errCode;                  /* Most recent error code (SQLITE_*) */
    int errMask;                  /* & result codes with this before returning */
    int iSysErrno;                /* Errno value from last system error */
    u16 dbOptFlags;               /* Flags to enable/disable optimizations */
    u8 enc;                       /* Text encoding */
    u8 autoCommit;                /* The auto-commit flag. */
    u8 temp_store;                /* 1: file 2: memory 0: default */
    u8 mallocFailed;              /* True if we have seen a malloc failure */
    u8 bBenignMalloc;             /* Do not require OOMs if true */
    u8 dfltLockMode;              /* Default locking-mode for attached dbs */
    signed char nextAutovac;      /* Autovac setting after VACUUM if >=0 */
    u8 suppressErr;               /* Do not issue error messages if true */
    u8 vtabOnConflict;            /* Value to return for s3_vtab_on_conflict() */
    u8 isTransactionSavepoint;    /* True if the outermost savepoint is a TS */
    u8 mTrace;                    /* zero or more SQLITE_TRACE flags */
    u8 noSharedCache;             /* True if no shared-cache backends */
    u8 nSqlExec;                  /* Number of pending OP_SqlExec opcodes */
    int nextPagesize;             /* Pagesize after VACUUM if >0 */
    u32 magic;                    /* Magic number for detect library misuse */
    int nChange;                  /* Value returned by sqlite3_changes() */
    int nTotalChange;             /* Value returned by sqlite3_total_changes() */
    int aLimit[SQLITE_N_LIMIT];   /* Limits */
    int nMaxSorterMmap;           /* Maximum size of regions mapped by sorter */
    struct sqlite3InitInfo {      /* Information used during initialization */
        int newTnum;                /* Rootpage of table being initialized */
        u8 iDb;                     /* Which db file is being initialized */
        u8 busy;                    /* TRUE if currently initializing */
        unsigned orphanTrigger : 1; /* Last statement is orphaned TEMP trigger */
        unsigned imposterTable : 1; /* Building an imposter table */
        unsigned reopenMemdb : 1;   /* ATTACH is really a reopen using MemDB */
    } init;
    int nVdbeActive;              /* Number of VDBEs currently running */
    int nVdbeRead;                /* Number of active VDBEs that read or write */
    int nVdbeWrite;               /* Number of active VDBEs that read and write */
    int nVdbeExec;                /* Number of nested calls to VdbeExec() */
    int nVDestroy;                /* Number of active OP_VDestroy operations */
    int nExtension;               /* Number of loaded extensions */
    void **aExtension;            /* Array of shared library handles */
    int (*xTrace)(u32,void*,void*,void*);     /* Trace function */
    void *pTraceArg;                          /* Argument to the trace function */
    void (*xProfile)(void*,const char*,u64);  /* Profiling function */
    void *pProfileArg;                        /* Argument to profile function */
    void *pCommitArg;                 /* Argument to xCommitCallback() */
    int (*xCommitCallback)(void*);    /* Invoked at every commit. */
    void *pRollbackArg;               /* Argument to xRollbackCallback() */
    void (*xRollbackCallback)(void*); /* Invoked at every commit. */
    void *pUpdateArg;
    void (*xUpdateCallback)(void*,int, const char*,const char*,sqlite_int64);
#ifdef SQLITE_ENABLE_PREUPDATE_HOOK
    void *pPreUpdateArg;          /* First argument to xPreUpdateCallback */
    void (*xPreUpdateCallback)(   /* Registered using sqlite3_preupdate_hook() */
                               void*,sqlite3*,int,char const*,char const*,sqlite3_int64,sqlite3_int64
                               );
    PreUpdate *pPreUpdate;        /* Context for active pre-update callback */
#endif /* SQLITE_ENABLE_PREUPDATE_HOOK */
#ifndef SQLITE_OMIT_WAL
    int (*xWalCallback)(void *, sqlite3 *, const char *, int);
    void *pWalArg;
#endif
    void(*xCollNeeded)(void*,sqlite3*,int eTextRep,const char*);
    void(*xCollNeeded16)(void*,sqlite3*,int eTextRep,const void*);
    void *pCollNeededArg;
    sqlite3_value *pErr;          /* Most recent error message */
    union {
        volatile int isInterrupted; /* True if sqlite3_interrupt has been called */
        double notUsed1;            /* Spacer */
    } u1;
    Lookaside lookaside;          /* Lookaside malloc configuration */
#ifndef SQLITE_OMIT_AUTHORIZATION
    sqlite3_xauth xAuth;          /* Access authorization function */
    void *pAuthArg;               /* 1st argument to the access auth function */
#endif
#ifndef SQLITE_OMIT_PROGRESS_CALLBACK
    int (*xProgress)(void *);     /* The progress callback */
    void *pProgressArg;           /* Argument to the progress callback */
    unsigned nProgressOps;        /* Number of opcodes for progress callback */
#endif
#ifndef SQLITE_OMIT_VIRTUALTABLE
    int nVTrans;                  /* Allocated size of aVTrans */
    Hash aModule;                 /* populated by sqlite3_create_module() */
    VtabCtx *pVtabCtx;            /* Context for active vtab connect/create */
    VTable **aVTrans;             /* Virtual tables with open transactions */
    VTable *pDisconnect;          /* Disconnect these in next sqlite3_prepare() */
#endif
    Hash aFunc;                   /* Hash table of connection functions */
    Hash aCollSeq;                /* All collating sequences */
    BusyHandler busyHandler;      /* Busy callback */
    Db aDbStatic[2];              /* Static space for the 2 default backends */
    Savepoint *pSavepoint;        /* List of active savepoints */
    int busyTimeout;              /* Busy handler timeout, in msec */
    int nSavepoint;               /* Number of non-transaction savepoints */
    int nStatement;               /* Number of nested statement-transactions  */
    i64 nDeferredCons;            /* Net deferred constraints this transaction. */
    i64 nDeferredImmCons;         /* Net deferred immediate constraints */
    int *pnBytesFreed;            /* If not NULL, increment this in DbFree() */
#ifdef SQLITE_ENABLE_UNLOCK_NOTIFY
    /* The following variables are all protected by the STATIC_MASTER
     ** mutex, not by sqlite3.mutex. They are used by code in notify.c.
     **
     ** When X.pUnlockConnection==Y, that means that X is waiting for Y to
     ** unlock so that it can proceed.
     **
     ** When X.pBlockingConnection==Y, that means that something that X tried
     ** tried to do recently failed with an SQLITE_LOCKED error due to locks
     ** held by Y.
     */
    sqlite3 *pBlockingConnection; /* Connection that caused SQLITE_LOCKED */
    sqlite3 *pUnlockConnection;           /* Connection to watch for unlock */
    void *pUnlockArg;                     /* Argument to xUnlockNotify */
    void (*xUnlockNotify)(void **, int);  /* Unlock notify callback */
    sqlite3 *pNextBlocked;        /* Next in list of all blocked connections */
#endif
#ifdef SQLITE_USER_AUTHENTICATION
    sqlite3_userauth auth;        /* User authentication information */
#endif
};


#endif /* sqlite_common_h */
