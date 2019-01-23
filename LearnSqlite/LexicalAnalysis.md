#sqlite中的sql语句的词法分析 使用了状态机技术。

### static int keywordCode(const char *z, int n, int *pType); 
```

    static int keywordCode(const char *z, int n, int *pType){
    int i, j;
    const char *zKW;
    if( n>=2 ){

        i = ((charMap(z[0])*4) ^ (charMap(z[n-1])*3) ^ n) % 127;
        for(i=((int)aKWHash[i])-1; i>=0; i=((int)aKWNext[i])-1){
    
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
        *pType = aKWCode[i];
        break;
        }
    }
return n;
}

```
charMap(z[0])这里将对应的大写字母转换为小写字母，如果不是字符则ascii不变
aKWHash这个数组存放的是各个关键词的hash值，
aKWNext这个数组主要是为了解决多个关键词可能存在的hash冲突
aKWLen存放的是关键词的长度
这里的是通过hash算法代替字符串匹配，提升效率。
1.i = ((charMap(z[0])*4) ^ (charMap(z[n-1])*3) ^ n) % 127; 通过这个公式计算得到i,通过这个i获取aKWHash对应的下标
2.如果aKWHash的获得新的i
3.然后用这个i通过aKWLen数组获取关键词的长度，如果长度不相等，说明出现hash冲突，通过aKWNext获取i的下一个值
4.如果通过aKWLen校验没有发生冲突，那么遍历z，如果与zw前n个字符是否一样，如果一样,则关键词识别成功，如果不是则通过aKWNext获取i的下一个值，重复上边的步骤

这里作者精心设计了这么个算法就是为了提升词法分析的效率

这里可以看做一个对关键词取hash的index，然后aKWHash数组里边返回的值-1，得到新的index，这个
index到


### static int sqlite3GetToken(const unsigned char *z, int *tokenType);
这个方法主要是获取z中的第一个toekn，返回的是token长度，同时给tokenType赋值，告诉token类型
aiClass存放的是ASCII编码，如果使用的是EBCDIC，则就是EBCDIC对应的编码，两者只会支持一种
下边我们用ASCII举例，数组下标就是ascii码的十进制对应数字，比如<对应的是60，他在sql中属于 CC_LT，因此
aiClass[60]的值应该是CC_LT，也就是12
下边的aiClass[*z]取的就是z第一个字符对应ascii 如果 create table .... 取的是第一个字符c，他所对应ascii
99 所以相当于aiClass[99],所以c开头的可能是关键字或者是个标识符
case CC_SPACE:
这里主要是处理空格等分隔符包括了\t','\n','\f','\r',如果遇到多余的分隔符游标i会不断向前，直到遇到非分隔符
CC_SLASH这个是处理注释逻辑

```
aiClass
```
这里数组存放ascii编码对应的可能的token类型，有些token长度为1，那么很好判断，但是有些token它的长度大于1，那么可能是keword 也可能是个identifier或者其他，所以这里只是做第一步的推导，根据aiClass返回的类型做相应的处理

```
case CC_SPACE: {
      testcase( z[0]==' ' );
      testcase( z[0]=='\t' );
      testcase( z[0]=='\n' );
      testcase( z[0]=='\f' );
      testcase( z[0]=='\r' );
      for(i=1; sqlite3Isspace(z[i]); i++){}
      *tokenType = TK_SPACE;
      return i;
    }
```
这个主要是判断分隔符，同时也处理有多个分隔符的情况，这里最关键的是理解sqlite3Isspace这个这个是将如果z[i]是分隔符，然后将这个分隔符与0x01进行与运算，space的ascii值为32，sqlite3CtypeMap[32]对应的是1,所对应的二进制为01，与0x01进行与运算后值为1,这里可能会有很多人疑问，有没有不是分隔符他在sqlite3CtypeMap返回的值与0x01与运算后也是1呢，这个后边有空在研究他在数学上或者在设计上的原理。如何避免碰撞。

```
case CC_MINUS: {
if( z[1]=='-' ){
for(i=2; (c=z[i])!=0 && c!='\n'; i++){}
*tokenType = TK_SPACE;   /* IMP: R-22934-25134 */
return i;
}
*tokenType = TK_MINUS;
return 1;
}
```
这里处理‘-’符号和sql的单行注释 --；
如果属于单行注释，那么获取该行注释的长度 返回一个TK_SPACE类型，如果不属于则返回TK_MINUS类型
```
case CC_SLASH: {
if( z[1]!='*' || z[2]==0 ){
*tokenType = TK_SLASH;
return 1;
}
for(i=3, c=z[2]; (c!='*' || z[i]!='/') && (c=z[i])!=0; i++){}
if( c ) i++;
*tokenType = TK_SPACE;   /* IMP: R-22934-25134 */
return i;
}
```
首先判断他是不是注释 如果不是注释直接范围 1的长度 并且tokentype为TK_SLASH
如果他是注释就会找到注释的长度，如果注释没有结束符号则会吧整个字符串作为注释。
sqlite把注释归属空格token，
for循环中 (c!='*' || z[i]!='/') 是判断是否遇到了'*/'结尾，如果遇到这个结尾则代表注释结束
返回token长度

```
case CC_DIGIT: {
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
```
这里主要是处理数字，数字除了整数还包含了16进制和浮点数如果第一个数是0第二个数是x或者X那么就是16进制，那么开始获取16进制长度
sqlite3Isxdigit 这个宏主要判断数字是不是16进制的。如果不是16进制就开始获取数字长度，通过一个宏来判断sqlite3Isdigit是否为十进制数字
，获取十进制数字长度，直到遇到一个非数字的符号，如果这个非数字符号是一个.那么进入处理浮点数的流程。获取浮点数长度。如果遇到非数字字符
为e，那么首先判断这个格式是否正确，如果正确继续处理浮点数的情况，如果不正确，那么就会归属于非法字符

```
case CC_VARALPHA: {
    int n = 0;
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
```
这个case处理sql的变量，sql变量。我这里只在网上查到了@后边跟着变量名称。#:$没有查到
下边for循环获取变量长度。for循环直到遇到0结束。如果字符c是个标识符那么n++;如果不是标识符
那么看着该字符串是不是(或者是:如果是(则需要等到他的)的出现，如果不是这两个符号，那么继续进行下一个循环
。循环结束后如果n = 0则是非法token，返回这个token长度

```
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
}

```
这个处理二进制token，如果是x\开头的则是二进制对象，然后获取二进制的长度。
如果结尾不是'\' 或者长度不是偶数则是非法的token 最后获取准确长度+1.返回长度

```
while( IdChar(z[i]) ){ i++; }
*tokenType = TK_ID;
```
这个是在case中命中的非TK_ID后，发现是TK_ID，获取identifier长度

```
static int getToken(const unsigned char **pz)
```
获取token

