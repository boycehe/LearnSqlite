#sqlite中的sql语句的词法分析

### static int keywordCode(const char *z, int n, int *pType); 
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
