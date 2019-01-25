//
//  sqlite_token.c
//  LearnSqlite
//
//  Created by heboyce on 2019/1/12.
//  Copyright © 2019 heboyce. All rights reserved.
//

#include "sqlite_token.h"

# define testcase(X)


#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
    0,  /*          $ => nothing */
    0,  /*       SEMI => nothing */
    59,  /*    EXPLAIN => ID */
    59,  /*      QUERY => ID */
    59,  /*       PLAN => ID */
    59,  /*      BEGIN => ID */
    0,  /* TRANSACTION => nothing */
    59,  /*   DEFERRED => ID */
    59,  /*  IMMEDIATE => ID */
    59,  /*  EXCLUSIVE => ID */
    0,  /*     COMMIT => nothing */
    59,  /*        END => ID */
    59,  /*   ROLLBACK => ID */
    59,  /*  SAVEPOINT => ID */
    59,  /*    RELEASE => ID */
    0,  /*         TO => nothing */
    0,  /*      TABLE => nothing */
    0,  /*     CREATE => nothing */
    59,  /*         IF => ID */
    0,  /*        NOT => nothing */
    0,  /*     EXISTS => nothing */
    59,  /*       TEMP => ID */
    0,  /*         LP => nothing */
    0,  /*         RP => nothing */
    0,  /*         AS => nothing */
    59,  /*    WITHOUT => ID */
    0,  /*      COMMA => nothing */
    59,  /*      ABORT => ID */
    59,  /*     ACTION => ID */
    59,  /*      AFTER => ID */
    59,  /*    ANALYZE => ID */
    59,  /*        ASC => ID */
    59,  /*     ATTACH => ID */
    59,  /*     BEFORE => ID */
    59,  /*         BY => ID */
    59,  /*    CASCADE => ID */
    59,  /*       CAST => ID */
    59,  /*   CONFLICT => ID */
    59,  /*   DATABASE => ID */
    59,  /*       DESC => ID */
    59,  /*     DETACH => ID */
    59,  /*       EACH => ID */
    59,  /*       FAIL => ID */
    0,  /*         OR => nothing */
    0,  /*        AND => nothing */
    0,  /*         IS => nothing */
    59,  /*      MATCH => ID */
    59,  /*    LIKE_KW => ID */
    0,  /*    BETWEEN => nothing */
    0,  /*         IN => nothing */
    0,  /*     ISNULL => nothing */
    0,  /*    NOTNULL => nothing */
    0,  /*         NE => nothing */
    0,  /*         EQ => nothing */
    0,  /*         GT => nothing */
    0,  /*         LE => nothing */
    0,  /*         LT => nothing */
    0,  /*         GE => nothing */
    0,  /*     ESCAPE => nothing */
    0,  /*         ID => nothing */
    59,  /*   COLUMNKW => ID */
    59,  /*         DO => ID */
    59,  /*        FOR => ID */
    59,  /*     IGNORE => ID */
    59,  /*  INITIALLY => ID */
    59,  /*    INSTEAD => ID */
    59,  /*         NO => ID */
    59,  /*        KEY => ID */
    59,  /*         OF => ID */
    59,  /*     OFFSET => ID */
    59,  /*     PRAGMA => ID */
    59,  /*      RAISE => ID */
    59,  /*  RECURSIVE => ID */
    59,  /*    REPLACE => ID */
    59,  /*   RESTRICT => ID */
    59,  /*        ROW => ID */
    59,  /*       ROWS => ID */
    59,  /*    TRIGGER => ID */
    59,  /*     VACUUM => ID */
    59,  /*       VIEW => ID */
    59,  /*    VIRTUAL => ID */
    59,  /*       WITH => ID */
    59,  /*    CURRENT => ID */
    59,  /*  FOLLOWING => ID */
    59,  /*  PARTITION => ID */
    59,  /*  PRECEDING => ID */
    59,  /*      RANGE => ID */
    59,  /*  UNBOUNDED => ID */
    59,  /*    REINDEX => ID */
    59,  /*     RENAME => ID */
    59,  /*   CTIME_KW => ID */
};
#endif /* YYFALLBACK */

/************** End of hash.h ************************************************/
/************** Continuing where we left off in sqliteInt.h ******************/
/************** Include parse.h in the middle of sqliteInt.h *****************/
/************** Begin file parse.h *******************************************/
#define TK_SEMI                             1
#define TK_EXPLAIN                          2
#define TK_QUERY                            3
#define TK_PLAN                             4
#define TK_BEGIN                            5
#define TK_TRANSACTION                      6
#define TK_DEFERRED                         7
#define TK_IMMEDIATE                        8
#define TK_EXCLUSIVE                        9
#define TK_COMMIT                          10
#define TK_END                             11
#define TK_ROLLBACK                        12
#define TK_SAVEPOINT                       13
#define TK_RELEASE                         14
#define TK_TO                              15
#define TK_TABLE                           16
#define TK_CREATE                          17
#define TK_IF                              18
#define TK_NOT                             19
#define TK_EXISTS                          20
#define TK_TEMP                            21
#define TK_LP                              22
#define TK_RP                              23
#define TK_AS                              24
#define TK_WITHOUT                         25
#define TK_COMMA                           26
#define TK_ABORT                           27
#define TK_ACTION                          28
#define TK_AFTER                           29
#define TK_ANALYZE                         30
#define TK_ASC                             31
#define TK_ATTACH                          32
#define TK_BEFORE                          33
#define TK_BY                              34
#define TK_CASCADE                         35
#define TK_CAST                            36
#define TK_CONFLICT                        37
#define TK_DATABASE                        38
#define TK_DESC                            39
#define TK_DETACH                          40
#define TK_EACH                            41
#define TK_FAIL                            42
#define TK_OR                              43
#define TK_AND                             44
#define TK_IS                              45
#define TK_MATCH                           46
#define TK_LIKE_KW                         47
#define TK_BETWEEN                         48
#define TK_IN                              49
#define TK_ISNULL                          50
#define TK_NOTNULL                         51
#define TK_NE                              52
#define TK_EQ                              53
#define TK_GT                              54
#define TK_LE                              55
#define TK_LT                              56
#define TK_GE                              57
#define TK_ESCAPE                          58
#define TK_ID                              59
#define TK_COLUMNKW                        60
#define TK_DO                              61
#define TK_FOR                             62
#define TK_IGNORE                          63
#define TK_INITIALLY                       64
#define TK_INSTEAD                         65
#define TK_NO                              66
#define TK_KEY                             67
#define TK_OF                              68
#define TK_OFFSET                          69
#define TK_PRAGMA                          70
#define TK_RAISE                           71
#define TK_RECURSIVE                       72
#define TK_REPLACE                         73
#define TK_RESTRICT                        74
#define TK_ROW                             75
#define TK_ROWS                            76
#define TK_TRIGGER                         77
#define TK_VACUUM                          78
#define TK_VIEW                            79
#define TK_VIRTUAL                         80
#define TK_WITH                            81
#define TK_CURRENT                         82
#define TK_FOLLOWING                       83
#define TK_PARTITION                       84
#define TK_PRECEDING                       85
#define TK_RANGE                           86
#define TK_UNBOUNDED                       87
#define TK_REINDEX                         88
#define TK_RENAME                          89
#define TK_CTIME_KW                        90
#define TK_ANY                             91
#define TK_BITAND                          92
#define TK_BITOR                           93
#define TK_LSHIFT                          94
#define TK_RSHIFT                          95
#define TK_PLUS                            96
#define TK_MINUS                           97
#define TK_STAR                            98
#define TK_SLASH                           99
#define TK_REM                            100
#define TK_CONCAT                         101
#define TK_COLLATE                        102
#define TK_BITNOT                         103
#define TK_ON                             104
#define TK_INDEXED                        105
#define TK_STRING                         106
#define TK_JOIN_KW                        107
#define TK_CONSTRAINT                     108
#define TK_DEFAULT                        109
#define TK_NULL                           110
#define TK_PRIMARY                        111
#define TK_UNIQUE                         112
#define TK_CHECK                          113
#define TK_REFERENCES                     114
#define TK_AUTOINCR                       115
#define TK_INSERT                         116
#define TK_DELETE                         117
#define TK_UPDATE                         118
#define TK_SET                            119
#define TK_DEFERRABLE                     120
#define TK_FOREIGN                        121
#define TK_DROP                           122
#define TK_UNION                          123
#define TK_ALL                            124
#define TK_EXCEPT                         125
#define TK_INTERSECT                      126
#define TK_SELECT                         127
#define TK_VALUES                         128
#define TK_DISTINCT                       129
#define TK_DOT                            130
#define TK_FROM                           131
#define TK_JOIN                           132
#define TK_USING                          133
#define TK_ORDER                          134
#define TK_GROUP                          135
#define TK_HAVING                         136
#define TK_LIMIT                          137
#define TK_WHERE                          138
#define TK_INTO                           139
#define TK_NOTHING                        140
#define TK_FLOAT                          141
#define TK_BLOB                           142
#define TK_INTEGER                        143
#define TK_VARIABLE                       144
#define TK_CASE                           145
#define TK_WHEN                           146
#define TK_THEN                           147
#define TK_ELSE                           148
#define TK_INDEX                          149
#define TK_ALTER                          150
#define TK_ADD                            151
#define TK_WINDOW                         152
#define TK_OVER                           153
#define TK_FILTER                         154
#define TK_TRUEFALSE                      155
#define TK_ISNOT                          156
#define TK_FUNCTION                       157
#define TK_COLUMN                         158
#define TK_AGG_FUNCTION                   159
#define TK_AGG_COLUMN                     160
#define TK_UMINUS                         161
#define TK_UPLUS                          162
#define TK_TRUTH                          163
#define TK_REGISTER                       164
#define TK_VECTOR                         165
#define TK_SELECT_COLUMN                  166
#define TK_IF_NULL_ROW                    167
#define TK_ASTERISK                       168
#define TK_SPAN                           169
#define TK_END_OF_FILE                    170
#define TK_UNCLOSED_STRING                171
#define TK_SPACE                          172
#define TK_ILLEGAL                        173

#define CC_X          0    /* The letter 'x', or start of BLOB literal */
#define CC_KYWD       1    /* Alphabetics or '_'.  Usable in a keyword */
#define CC_ID         2    /* unicode characters usable in IDs */
#define CC_DIGIT      3    /* Digits */
#define CC_DOLLAR     4    /* '$' */
#define CC_VARALPHA   5    /* '@', '#', ':'.  Alphabetic SQL variables */
#define CC_VARNUM     6    /* '?'.  Numeric SQL variables */
#define CC_SPACE      7    /* Space characters */
#define CC_QUOTE      8    /* '"', '\'', or '`'.  String literals, quoted ids */
#define CC_QUOTE2     9    /* '['.   [...] style quoted ids */
#define CC_PIPE      10    /* '|'.   Bitwise OR or concatenate */
#define CC_MINUS     11    /* '-'.  Minus or SQL-style comment */
#define CC_LT        12    /* '<'.  Part of < or <= or <> */
#define CC_GT        13    /* '>'.  Part of > or >= */
#define CC_EQ        14    /* '='.  Part of = or == */
#define CC_BANG      15    /* '!'.  Part of != */
#define CC_SLASH     16    /* '/'.  / or c-style comment */
#define CC_LP        17    /* '(' */
#define CC_RP        18    /* ')' */
#define CC_SEMI      19    /* ';' */
#define CC_PLUS      20    /* '+' */
#define CC_STAR      21    /* '*' */
#define CC_PERCENT   22    /* '%' */
#define CC_COMMA     23    /* ',' */
#define CC_AND       24    /* '&' */
#define CC_TILDA     25    /* '~' */
#define CC_DOT       26    /* '.' */
#define CC_ILLEGAL   27    /* Illegal character */
#define CC_NUL       28    /* 0x00 */

static const char zKWText[613] = {
    'R','E','I','N','D','E','X','E','D','E','S','C','A','P','E','A','C','H',
    'E','C','K','E','Y','B','E','F','O','R','E','I','G','N','O','R','E','G',
    'E','X','P','L','A','I','N','S','T','E','A','D','D','A','T','A','B','A',
    'S','E','L','E','C','T','A','B','L','E','F','T','H','E','N','D','E','F',
    'E','R','R','A','B','L','E','L','S','E','X','C','E','P','T','R','A','N',
    'S','A','C','T','I','O','N','A','T','U','R','A','L','T','E','R','A','I',
    'S','E','X','C','L','U','S','I','V','E','X','I','S','T','S','A','V','E',
    'P','O','I','N','T','E','R','S','E','C','T','R','I','G','G','E','R','E',
    'F','E','R','E','N','C','E','S','C','O','N','S','T','R','A','I','N','T',
    'O','F','F','S','E','T','E','M','P','O','R','A','R','Y','U','N','I','Q',
    'U','E','R','Y','W','I','T','H','O','U','T','E','R','E','L','E','A','S',
    'E','A','T','T','A','C','H','A','V','I','N','G','R','O','U','P','D','A',
    'T','E','B','E','G','I','N','N','E','R','A','N','G','E','B','E','T','W',
    'E','E','N','O','T','H','I','N','G','L','O','B','Y','C','A','S','C','A',
    'D','E','L','E','T','E','C','A','S','E','C','O','L','L','A','T','E','C',
    'R','E','A','T','E','C','U','R','R','E','N','T','_','D','A','T','E','D',
    'E','T','A','C','H','I','M','M','E','D','I','A','T','E','J','O','I','N',
    'S','E','R','T','L','I','K','E','M','A','T','C','H','P','L','A','N','A',
    'L','Y','Z','E','P','R','A','G','M','A','B','O','R','T','V','A','L','U',
    'E','S','V','I','R','T','U','A','L','I','M','I','T','W','H','E','N','O',
    'T','N','U','L','L','W','H','E','R','E','C','U','R','S','I','V','E','A',
    'F','T','E','R','E','N','A','M','E','A','N','D','E','F','A','U','L','T',
    'A','U','T','O','I','N','C','R','E','M','E','N','T','C','A','S','T','C',
    'O','L','U','M','N','C','O','M','M','I','T','C','O','N','F','L','I','C',
    'T','C','R','O','S','S','C','U','R','R','E','N','T','_','T','I','M','E',
    'S','T','A','M','P','A','R','T','I','T','I','O','N','D','E','F','E','R',
    'R','E','D','I','S','T','I','N','C','T','D','R','O','P','R','E','C','E',
    'D','I','N','G','F','A','I','L','F','I','L','T','E','R','E','P','L','A',
    'C','E','F','O','L','L','O','W','I','N','G','F','R','O','M','F','U','L',
    'L','I','F','I','S','N','U','L','L','O','R','D','E','R','E','S','T','R',
    'I','C','T','O','V','E','R','I','G','H','T','R','O','L','L','B','A','C',
    'K','R','O','W','S','U','N','B','O','U','N','D','E','D','U','N','I','O',
    'N','U','S','I','N','G','V','A','C','U','U','M','V','I','E','W','I','N',
    'D','O','W','I','N','I','T','I','A','L','L','Y','P','R','I','M','A','R',
    'Y',
};
/* aKWHash[i] is the hash value for the i-th keyword */
static const unsigned char aKWHash[127] = {
    74, 109, 124,  72, 106,  45,   0,   0,  81,   0,  76,  61,   0,
    42,  12,  77,  15,   0, 123,  84,  54, 118, 125,  19,   0,   0,
    130,   0, 128, 121,   0,  22,  96,   0,   9,   0,   0, 115,  69,
    0,  67,   6,   0,  48,  93, 136,   0, 126, 104,   0,   0,  44,
    0, 107,  24,   0,  17,   0, 131,  53,  23,   0,   5,  62, 132,
    99,   0,   0, 135, 110,  60, 134,  57, 113,  55,   0,  94,   0,
    103,  26,   0, 102,   0,   0,   0,  98,  95, 100, 105, 117,  14,
    39, 116,   0,  80,   0, 133, 114,  92,  59,   0, 129,  79, 119,
    86,  46,  83,   0,   0,  97,  40, 122, 120,   0, 127,   0,   0,
    29,   0,  89,  87,  88,   0,  20,  85, 111,  56,
};
/* aKWNext[] forms the hash collision chain.  If aKWHash[i]==0
 ** then the i-th keyword has no more hash collisions.  Otherwise,
 ** the next keyword with the same hash is aKWHash[i]-1. */
static const unsigned char aKWNext[136] = {
    0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   2,   0,   0,   0,   0,   0,   0,  13,   0,   0,   0,   0,
    0,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,  33,   0,  21,   0,   0,   0,   0,   0,  50,
    0,  43,   3,  47,   0,   0,  32,   0,   0,   0,   0,   0,   0,
    0,   1,  64,   0,   0,  65,   0,  41,   0,  38,   0,   0,   0,
    0,   0,  49,  75,   0,   0,  30,   0,  58,   0,   0,   0,  31,
    63,  16,  34,  10,   0,   0,   0,   0,   0,   0,   0,  11,  70,
    91,   0,   0,   8,   0, 108,   0, 101,  28,  52,  68,   0, 112,
    0,  73,  51,   0,  90,  27,  37,   0,  71,  36,  82,   0,  35,
    66,  25,  18,   0,   0,  78,
};
/* aKWLen[i] is the length (in bytes) of the i-th keyword */
static const unsigned char aKWLen[136] = {
    7,   7,   5,   4,   6,   4,   5,   3,   6,   7,   3,   6,   6,
    7,   7,   3,   8,   2,   6,   5,   4,   4,   3,  10,   4,   6,
    11,   6,   2,   7,   5,   5,   9,   6,   9,   9,   7,  10,  10,
    4,   6,   2,   3,   9,   4,   2,   6,   5,   7,   4,   5,   7,
    6,   6,   5,   6,   5,   5,   5,   7,   7,   4,   2,   7,   3,
    6,   4,   7,   6,  12,   6,   9,   4,   6,   4,   5,   4,   7,
    6,   5,   6,   7,   5,   4,   7,   3,   2,   4,   5,   9,   5,
    6,   3,   7,  13,   2,   2,   4,   6,   6,   8,   5,  17,  12,
    7,   9,   8,   8,   2,   4,   9,   4,   6,   7,   9,   4,   4,
    2,   6,   5,   8,   4,   5,   8,   4,   3,   9,   5,   5,   6,
    4,   6,   2,   9,   3,   7,
};
/* aKWOffset[i] is the index into zKWText[] of the start of
 ** the text for the i-th keyword. */
static const unsigned short int aKWOffset[136] = {
    0,   2,   2,   8,   9,  14,  16,  20,  23,  25,  25,  29,  33,
    36,  41,  46,  48,  53,  54,  59,  62,  65,  67,  69,  78,  81,
    86,  91,  95,  96, 101, 105, 109, 117, 122, 128, 136, 142, 152,
    159, 162, 162, 165, 167, 167, 171, 176, 179, 184, 184, 188, 192,
    199, 204, 209, 212, 218, 221, 225, 230, 236, 242, 245, 247, 248,
    252, 258, 262, 269, 275, 287, 293, 302, 304, 310, 314, 319, 321,
    328, 333, 338, 344, 350, 355, 358, 358, 358, 361, 365, 368, 377,
    381, 387, 389, 396, 398, 400, 409, 413, 419, 425, 433, 438, 438,
    438, 454, 463, 470, 471, 478, 481, 490, 494, 499, 506, 515, 519,
    523, 525, 531, 535, 543, 546, 551, 559, 559, 563, 572, 577, 582,
    588, 591, 594, 597, 602, 606,
};

static const unsigned char aiClass[] = {

    /*         x0  x1  x2  x3  x4  x5  x6  x7  x8  x9  xa  xb  xc  xd  xe  xf */
    /* 0x 16*/   28, 27, 27, 27, 27, 27, 27, 27, 27,  7,  7, 27,  7,  7, 27, 27,
    /* 1x 32*/   27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
    /* 2x 48*/    7, 15,  8,  5,  4, 22, 24,  8, 17, 18, 21, 20, 23, 11, 26, 16,
    /* 3x 64*/    3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  5, 19, 12, 14, 13,  6,
    /* 4x 80*/    5,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* 5x 96*/    1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  9, 27, 27, 27,  1,
    /* 6x 112*/    8,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* 7x */    1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1, 27, 10, 27, 25, 27,
    /* 8x */    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    /* 9x */    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    /* Ax */    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    /* Bx */    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    /* Cx */    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    /* Dx */    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    /* Ex */    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    /* Fx */    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2

};

SQLITE_PRIVATE const unsigned char sqlite3UpperToLower[] = {
#ifdef SQLITE_ASCII
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
    54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 97, 98, 99,100,101,102,103,
    104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,
    122, 91, 92, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,
    108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,
    126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
    144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,
    162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,
    180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,
    198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,
    216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,
    234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,
    252,253,254,255
#endif
#ifdef SQLITE_EBCDIC
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, /* 0x */
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, /* 1x */
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, /* 2x */
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, /* 3x */
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, /* 4x */
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, /* 5x */
    96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111, /* 6x */
    112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127, /* 7x */
    128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143, /* 8x */
    144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159, /* 9x */
    160,161,162,163,164,165,166,167,168,169,170,171,140,141,142,175, /* Ax */
    176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191, /* Bx */
    192,129,130,131,132,133,134,135,136,137,202,203,204,205,206,207, /* Cx */
    208,145,146,147,148,149,150,151,152,153,218,219,220,221,222,223, /* Dx */
    224,225,162,163,164,165,166,167,168,169,234,235,236,237,238,239, /* Ex */
    240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255, /* Fx */
#endif
};

SQLITE_PRIVATE const unsigned char sqlite3CtypeMap[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 00..07    ........ */
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,  /* 08..0f    ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 10..17    ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 18..1f    ........ */
    0x01, 0x00, 0x80, 0x00, 0x40, 0x00, 0x00, 0x80,  /* 20..27     !"#$%&' */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 28..2f    ()*+,-./ */
    0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,  /* 30..37    01234567 */
    0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 38..3f    89:;<=>? */
    
    0x00, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x02,  /* 40..47    @ABCDEFG */
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,  /* 48..4f    HIJKLMNO */
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,  /* 50..57    PQRSTUVW */
    0x02, 0x02, 0x02, 0x80, 0x00, 0x00, 0x00, 0x40,  /* 58..5f    XYZ[\]^_ */
    0x80, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x22,  /* 60..67    `abcdefg */
    0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,  /* 68..6f    hijklmno */
    0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,  /* 70..77    pqrstuvw */
    0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,  /* 78..7f    xyz{|}~. */
    
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 80..87    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 88..8f    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 90..97    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* 98..9f    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* a0..a7    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* a8..af    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* b0..b7    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* b8..bf    ........ */
    
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* c0..c7    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* c8..cf    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* d0..d7    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* d8..df    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* e0..e7    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* e8..ef    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  /* f0..f7    ........ */
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40   /* f8..ff    ........ */
};

/* aKWCode[i] is the parser symbol code for the i-th keyword */
static const unsigned char aKWCode[136] = {
    TK_REINDEX,    TK_INDEXED,    TK_INDEX,      TK_DESC,       TK_ESCAPE,
    TK_EACH,       TK_CHECK,      TK_KEY,        TK_BEFORE,     TK_FOREIGN,
    TK_FOR,        TK_IGNORE,     TK_LIKE_KW,    TK_EXPLAIN,    TK_INSTEAD,
    TK_ADD,        TK_DATABASE,   TK_AS,         TK_SELECT,     TK_TABLE,
    TK_JOIN_KW,    TK_THEN,       TK_END,        TK_DEFERRABLE, TK_ELSE,
    TK_EXCEPT,     TK_TRANSACTION,TK_ACTION,     TK_ON,         TK_JOIN_KW,
    TK_ALTER,      TK_RAISE,      TK_EXCLUSIVE,  TK_EXISTS,     TK_SAVEPOINT,
    TK_INTERSECT,  TK_TRIGGER,    TK_REFERENCES, TK_CONSTRAINT, TK_INTO,
    TK_OFFSET,     TK_OF,         TK_SET,        TK_TEMP,       TK_TEMP,
    TK_OR,         TK_UNIQUE,     TK_QUERY,      TK_WITHOUT,    TK_WITH,
    TK_JOIN_KW,    TK_RELEASE,    TK_ATTACH,     TK_HAVING,     TK_GROUP,
    TK_UPDATE,     TK_BEGIN,      TK_JOIN_KW,    TK_RANGE,      TK_BETWEEN,
    TK_NOTHING,    TK_LIKE_KW,    TK_BY,         TK_CASCADE,    TK_ASC,
    TK_DELETE,     TK_CASE,       TK_COLLATE,    TK_CREATE,     TK_CTIME_KW,
    TK_DETACH,     TK_IMMEDIATE,  TK_JOIN,       TK_INSERT,     TK_LIKE_KW,
    TK_MATCH,      TK_PLAN,       TK_ANALYZE,    TK_PRAGMA,     TK_ABORT,
    TK_VALUES,     TK_VIRTUAL,    TK_LIMIT,      TK_WHEN,       TK_NOTNULL,
    TK_NOT,        TK_NO,         TK_NULL,       TK_WHERE,      TK_RECURSIVE,
    TK_AFTER,      TK_RENAME,     TK_AND,        TK_DEFAULT,    TK_AUTOINCR,
    TK_TO,         TK_IN,         TK_CAST,       TK_COLUMNKW,   TK_COMMIT,
    TK_CONFLICT,   TK_JOIN_KW,    TK_CTIME_KW,   TK_CTIME_KW,   TK_CURRENT,
    TK_PARTITION,  TK_DEFERRED,   TK_DISTINCT,   TK_IS,         TK_DROP,
    TK_PRECEDING,  TK_FAIL,       TK_FILTER,     TK_REPLACE,    TK_FOLLOWING,
    TK_FROM,       TK_JOIN_KW,    TK_IF,         TK_ISNULL,     TK_ORDER,
    TK_RESTRICT,   TK_OVER,       TK_JOIN_KW,    TK_ROLLBACK,   TK_ROWS,
    TK_ROW,        TK_UNBOUNDED,  TK_UNION,      TK_USING,      TK_VACUUM,
    TK_VIEW,       TK_WINDOW,     TK_DO,         TK_INITIALLY,  TK_ALL,
    TK_PRIMARY,
};


# define sqlite3Toupper(x)  ((x)&~(sqlite3CtypeMap[(unsigned char)(x)]&0x20))
# define sqlite3Isspace(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x01)
# define sqlite3Isalnum(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x06)
# define sqlite3Isalpha(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x02)
# define sqlite3Isdigit(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x04)
# define sqlite3Isxdigit(x)  (sqlite3CtypeMap[(unsigned char)(x)]&0x08)
# define sqlite3Tolower(x)   (sqlite3UpperToLower[(unsigned char)(x)])
# define sqlite3Isquote(x)   (sqlite3CtypeMap[(unsigned char)(x)]&0x80)

#ifndef UINT8_TYPE
# ifdef HAVE_UINT8_T
#  define UINT8_TYPE uint8_t
# else
#  define UINT8_TYPE unsigned char
# endif
#endif

typedef UINT8_TYPE u8;

SQLITE_PRIVATE int sqlite3IsIdChar(u8);
#define IdChar(C)  ((sqlite3CtypeMap[(unsigned char)C]&0x46)!=0)
#define charMap(X) sqlite3UpperToLower[(unsigned char)X]

static int keywordCode(const char *z, int n, int *pType);


int sqlite3GetToken(const unsigned char *z, int *tokenType){
    int i, c;
    switch( aiClass[*z] ){  /* Switch on the character-class of the first byte
                             ** of the token. See the comment on the CC_ defines
                             ** above. */
        case CC_SPACE: {
            testcase( z[0]==' ' );
            testcase( z[0]=='\t' );
            testcase( z[0]=='\n' );
            testcase( z[0]=='\f' );
            testcase( z[0]=='\r' );
           
            for(i=1; sqlite3Isspace(z[i]); i++){
               
                
            }
            *tokenType = TK_SPACE;
            return i;
        }
        case CC_MINUS: {
            if( z[1]=='-' ){
                for(i=2; (c=z[i])!=0 && c!='\n'; i++){}
                *tokenType = TK_SPACE;   /* IMP: R-22934-25134 */
                return i;
            }
            *tokenType = TK_MINUS;
            return 1;
        }
        case CC_LP: {
            *tokenType = TK_LP;
            return 1;
        }
        case CC_RP: {
            *tokenType = TK_RP;
            return 1;
        }
        case CC_SEMI: {
            *tokenType = TK_SEMI;
            return 1;
        }
        case CC_PLUS: {
            *tokenType = TK_PLUS;
            return 1;
        }
        case CC_STAR: {
            *tokenType = TK_STAR;
            return 1;
        }
        case CC_SLASH: {
            if( z[1]!='*' || z[2]==0 ){
                *tokenType = TK_SLASH;
                return 1;
            }
            for(i=3, c=z[2]; (c!='*' || z[i]!='/') && (c=z[i])!=0; i++){
                printf("CC_SLASH %c\n",z[i]);
            }
            if( c ) i++;
            *tokenType = TK_SPACE;   /* IMP: R-22934-25134 */
            return i;
        }
        case CC_PERCENT: {
            *tokenType = TK_REM;
            return 1;
        }
        case CC_EQ: {
            *tokenType = TK_EQ;
            return 1 + (z[1]=='=');
        }
        case CC_LT: {
            if( (c=z[1])=='=' ){
                *tokenType = TK_LE;
                return 2;
            }else if( c=='>' ){
                *tokenType = TK_NE;
                return 2;
            }else if( c=='<' ){
                *tokenType = TK_LSHIFT;
                return 2;
            }else{
                *tokenType = TK_LT;
                return 1;
            }
        }
        case CC_GT: {
            if( (c=z[1])=='=' ){
                *tokenType = TK_GE;
                return 2;
            }else if( c=='>' ){
                *tokenType = TK_RSHIFT;
                return 2;
            }else{
                *tokenType = TK_GT;
                return 1;
            }
        }
        case CC_BANG: {
            if( z[1]!='=' ){
                *tokenType = TK_ILLEGAL;
                return 1;
            }else{
                *tokenType = TK_NE;
                return 2;
            }
        }
        case CC_PIPE: {
            if( z[1]!='|' ){
                *tokenType = TK_BITOR;
                return 1;
            }else{
                *tokenType = TK_CONCAT;
                return 2;
            }
        }
        case CC_COMMA: {
            *tokenType = TK_COMMA;
            return 1;
        }
        case CC_AND: {
            *tokenType = TK_BITAND;
            return 1;
        }
        case CC_TILDA: {
            *tokenType = TK_BITNOT;
            return 1;
        }
        case CC_QUOTE: {
            int delim = z[0];
            testcase( delim=='`' );
            testcase( delim=='\'' );
            testcase( delim=='"' );
            for(i=1; (c=z[i])!=0; i++){
                if( c==delim ){
                    if( z[i+1]==delim ){
                        i++;
                    }else{
                        break;
                    }
                }
            }
            if( c=='\'' ){
                *tokenType = TK_STRING;
                return i+1;
            }else if( c!=0 ){
                *tokenType = TK_ID;
                return i+1;
            }else{
                *tokenType = TK_ILLEGAL;
                return i;
            }
        }
        case CC_DOT: {
#ifndef SQLITE_OMIT_FLOATING_POINT
            if( !sqlite3Isdigit(z[1]) )
#endif
            {
                *tokenType = TK_DOT;
                return 1;
            }
            /* If the next character is a digit, this is a floating point
             ** number that begins with ".".  Fall thru into the next case */
        }
        case CC_DIGIT: {
            testcase( z[0]=='0' );  testcase( z[0]=='1' );  testcase( z[0]=='2' );
            testcase( z[0]=='3' );  testcase( z[0]=='4' );  testcase( z[0]=='5' );
            testcase( z[0]=='6' );  testcase( z[0]=='7' );  testcase( z[0]=='8' );
            testcase( z[0]=='9' );
            *tokenType = TK_INTEGER;
#ifndef SQLITE_OMIT_HEX_INTEGER
            if( z[0]=='0' && (z[1]=='x' || z[1]=='X') && sqlite3Isxdigit(z[2]) ){
                for(i=3; sqlite3Isxdigit(z[i]); i++){}
                return i;
            }
#endif
            for(i=0; sqlite3Isdigit(z[i]); i++){}
#ifndef SQLITE_OMIT_FLOATING_POINT
            if( z[i]=='.' ){
                i++;
                while( sqlite3Isdigit(z[i]) ){ i++; }
                *tokenType = TK_FLOAT;
            }
            if( (z[i]=='e' || z[i]=='E') &&
               ( sqlite3Isdigit(z[i+1])
                || ((z[i+1]=='+' || z[i+1]=='-') && sqlite3Isdigit(z[i+2]))
                )
               ){
                i += 2;
                while( sqlite3Isdigit(z[i]) ){ i++; }
                *tokenType = TK_FLOAT;
            }
#endif
            while( IdChar(z[i]) ){
                *tokenType = TK_ILLEGAL;
                i++;
            }
            return i;
        }
        case CC_QUOTE2: {
            for(i=1, c=z[0]; c!=']' && (c=z[i])!=0; i++){}
            *tokenType = c==']' ? TK_ID : TK_ILLEGAL;
            return i;
        }
        case CC_VARNUM: {
            *tokenType = TK_VARIABLE;
            for(i=1; sqlite3Isdigit(z[i]); i++){}
            return i;
        }
        case CC_DOLLAR:
        case CC_VARALPHA: {
            int n = 0;
            testcase( z[0]=='$' );  testcase( z[0]=='@' );
            testcase( z[0]==':' );  testcase( z[0]=='#' );
            *tokenType = TK_VARIABLE;
            for(i=1; (c=z[i])!=0; i++){
                if( IdChar(c) ){
                    n++;
#ifndef SQLITE_OMIT_TCL_VARIABLE
                }else if( c=='(' && n>0 ){
                    do{
                        i++;
                    }while( (c=z[i])!=0 && !sqlite3Isspace(c) && c!=')' );
                    if( c==')' ){
                        i++;
                    }else{
                        *tokenType = TK_ILLEGAL;
                    }
                    break;
                }else if( c==':' && z[i+1]==':' ){
                    i++;
#endif
                }else{
                    break;
                }
            }
            if( n==0 ) *tokenType = TK_ILLEGAL;
            return i;
        }
        case CC_KYWD: {
            for(i=1; aiClass[z[i]]<=CC_KYWD; i++){}
            if( IdChar(z[i]) ){
                /* This token started out using characters that can appear in keywords,
                 ** but z[i] is a character not allowed within keywords, so this must
                 ** be an identifier instead */
                i++;
                break;
            }
            *tokenType = TK_ID;
            return keywordCode((char*)z, i, tokenType);
        }
        case CC_X: {
#ifndef SQLITE_OMIT_BLOB_LITERAL
            testcase( z[0]=='x' ); testcase( z[0]=='X' );
            if( z[1]=='\'' ){
                *tokenType = TK_BLOB;
                for(i=2; sqlite3Isxdigit(z[i]); i++){}
                if( z[i]!='\'' || i%2 ){
                    *tokenType = TK_ILLEGAL;
                    while( z[i] && z[i]!='\'' ){ i++; }
                }
                if( z[i] ) i++;
                return i;
            }
#endif
            /* If it is not a BLOB literal, then it must be an ID, since no
             ** SQL keywords start with the letter 'x'.  Fall through */
        }
        case CC_ID: {
            i = 1;
            break;
        }
        case CC_NUL: {
            *tokenType = TK_ILLEGAL;
            return 0;
        }
        default: {
            *tokenType = TK_ILLEGAL;
            return 1;
        }
    }
    while( IdChar(z[i]) ){ i++; }
    *tokenType = TK_ID;
    return i;
}


static int keywordCode(const char *z, int n, int *pType){
    int i, j;
    const char *zKW;
    if( n>=2 ){
        
        i = ((charMap(z[0])*4) ^ (charMap(z[n-1])*3) ^ n) % 127;
        
        printf("keywordCode->%s\n",z);
        printf("keywordCode->n:%d\n",n);
        printf("keywordCode->z[0]:%c\tz[n-1]:%c\n",z[0],z[n-1]);
        printf("keywordCode->i:%d\n",i);
        for(i=((int)aKWHash[i])-1; i>=0; i=((int)aKWNext[i])-1){
            printf("hash->i:%d\n",i);
            if( aKWLen[i]!=n ) continue;
            j = 0;
            zKW = &zKWText[aKWOffset[i]];
#ifdef SQLITE_ASCII
            while( j<n && (z[j]&~0x20)==zKW[j] ){ j++; }
#endif
#ifdef SQLITE_EBCDIC
            while( j<n && toupper(z[j])==zKW[j] ){ j++; }
#endif
            if( j<n ) continue;
            testcase( i==0 ); /* REINDEX */
            testcase( i==1 ); /* INDEXED */
            testcase( i==2 ); /* INDEX */
            testcase( i==3 ); /* DESC */
            testcase( i==4 ); /* ESCAPE */
            testcase( i==5 ); /* EACH */
            testcase( i==6 ); /* CHECK */
            testcase( i==7 ); /* KEY */
            testcase( i==8 ); /* BEFORE */
            testcase( i==9 ); /* FOREIGN */
            testcase( i==10 ); /* FOR */
            testcase( i==11 ); /* IGNORE */
            testcase( i==12 ); /* REGEXP */
            testcase( i==13 ); /* EXPLAIN */
            testcase( i==14 ); /* INSTEAD */
            testcase( i==15 ); /* ADD */
            testcase( i==16 ); /* DATABASE */
            testcase( i==17 ); /* AS */
            testcase( i==18 ); /* SELECT */
            testcase( i==19 ); /* TABLE */
            testcase( i==20 ); /* LEFT */
            testcase( i==21 ); /* THEN */
            testcase( i==22 ); /* END */
            testcase( i==23 ); /* DEFERRABLE */
            testcase( i==24 ); /* ELSE */
            testcase( i==25 ); /* EXCEPT */
            testcase( i==26 ); /* TRANSACTION */
            testcase( i==27 ); /* ACTION */
            testcase( i==28 ); /* ON */
            testcase( i==29 ); /* NATURAL */
            testcase( i==30 ); /* ALTER */
            testcase( i==31 ); /* RAISE */
            testcase( i==32 ); /* EXCLUSIVE */
            testcase( i==33 ); /* EXISTS */
            testcase( i==34 ); /* SAVEPOINT */
            testcase( i==35 ); /* INTERSECT */
            testcase( i==36 ); /* TRIGGER */
            testcase( i==37 ); /* REFERENCES */
            testcase( i==38 ); /* CONSTRAINT */
            testcase( i==39 ); /* INTO */
            testcase( i==40 ); /* OFFSET */
            testcase( i==41 ); /* OF */
            testcase( i==42 ); /* SET */
            testcase( i==43 ); /* TEMPORARY */
            testcase( i==44 ); /* TEMP */
            testcase( i==45 ); /* OR */
            testcase( i==46 ); /* UNIQUE */
            testcase( i==47 ); /* QUERY */
            testcase( i==48 ); /* WITHOUT */
            testcase( i==49 ); /* WITH */
            testcase( i==50 ); /* OUTER */
            testcase( i==51 ); /* RELEASE */
            testcase( i==52 ); /* ATTACH */
            testcase( i==53 ); /* HAVING */
            testcase( i==54 ); /* GROUP */
            testcase( i==55 ); /* UPDATE */
            testcase( i==56 ); /* BEGIN */
            testcase( i==57 ); /* INNER */
            testcase( i==58 ); /* RANGE */
            testcase( i==59 ); /* BETWEEN */
            testcase( i==60 ); /* NOTHING */
            testcase( i==61 ); /* GLOB */
            testcase( i==62 ); /* BY */
            testcase( i==63 ); /* CASCADE */
            testcase( i==64 ); /* ASC */
            testcase( i==65 ); /* DELETE */
            testcase( i==66 ); /* CASE */
            testcase( i==67 ); /* COLLATE */
            testcase( i==68 ); /* CREATE */
            testcase( i==69 ); /* CURRENT_DATE */
            testcase( i==70 ); /* DETACH */
            testcase( i==71 ); /* IMMEDIATE */
            testcase( i==72 ); /* JOIN */
            testcase( i==73 ); /* INSERT */
            testcase( i==74 ); /* LIKE */
            testcase( i==75 ); /* MATCH */
            testcase( i==76 ); /* PLAN */
            testcase( i==77 ); /* ANALYZE */
            testcase( i==78 ); /* PRAGMA */
            testcase( i==79 ); /* ABORT */
            testcase( i==80 ); /* VALUES */
            testcase( i==81 ); /* VIRTUAL */
            testcase( i==82 ); /* LIMIT */
            testcase( i==83 ); /* WHEN */
            testcase( i==84 ); /* NOTNULL */
            testcase( i==85 ); /* NOT */
            testcase( i==86 ); /* NO */
            testcase( i==87 ); /* NULL */
            testcase( i==88 ); /* WHERE */
            testcase( i==89 ); /* RECURSIVE */
            testcase( i==90 ); /* AFTER */
            testcase( i==91 ); /* RENAME */
            testcase( i==92 ); /* AND */
            testcase( i==93 ); /* DEFAULT */
            testcase( i==94 ); /* AUTOINCREMENT */
            testcase( i==95 ); /* TO */
            testcase( i==96 ); /* IN */
            testcase( i==97 ); /* CAST */
            testcase( i==98 ); /* COLUMN */
            testcase( i==99 ); /* COMMIT */
            testcase( i==100 ); /* CONFLICT */
            testcase( i==101 ); /* CROSS */
            testcase( i==102 ); /* CURRENT_TIMESTAMP */
            testcase( i==103 ); /* CURRENT_TIME */
            testcase( i==104 ); /* CURRENT */
            testcase( i==105 ); /* PARTITION */
            testcase( i==106 ); /* DEFERRED */
            testcase( i==107 ); /* DISTINCT */
            testcase( i==108 ); /* IS */
            testcase( i==109 ); /* DROP */
            testcase( i==110 ); /* PRECEDING */
            testcase( i==111 ); /* FAIL */
            testcase( i==112 ); /* FILTER */
            testcase( i==113 ); /* REPLACE */
            testcase( i==114 ); /* FOLLOWING */
            testcase( i==115 ); /* FROM */
            testcase( i==116 ); /* FULL */
            testcase( i==117 ); /* IF */
            testcase( i==118 ); /* ISNULL */
            testcase( i==119 ); /* ORDER */
            testcase( i==120 ); /* RESTRICT */
            testcase( i==121 ); /* OVER */
            testcase( i==122 ); /* RIGHT */
            testcase( i==123 ); /* ROLLBACK */
            testcase( i==124 ); /* ROWS */
            testcase( i==125 ); /* ROW */
            testcase( i==126 ); /* UNBOUNDED */
            testcase( i==127 ); /* UNION */
            testcase( i==128 ); /* USING */
            testcase( i==129 ); /* VACUUM */
            testcase( i==130 ); /* VIEW */
            testcase( i==131 ); /* WINDOW */
            testcase( i==132 ); /* DO */
            testcase( i==133 ); /* INITIALLY */
            testcase( i==134 ); /* ALL */
            testcase( i==135 ); /* PRIMARY */
            *pType = aKWCode[i];
            break;
        }
    }
    return n;
}

SQLITE_PRIVATE int sqlite3ParserFallback(int iToken){
#ifdef YYFALLBACK
    if( iToken<(int)(sizeof(yyFallback)/sizeof(yyFallback[0])) ){
        return yyFallback[iToken];
    }
#else
    (void)iToken;
#endif
    return 0;
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
