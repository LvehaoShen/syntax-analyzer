#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype
{
    ID = 256, // 标识符
    NUMBER,   // 整数
    STRING,   //字符串
    INT,      // int
    STR,      // str
    VOID,     // void
    CONST,    // const
    IF,       // if
    ELSE,     // else
    WHILE,    // while
    BREAK,    // break
    CONTINUE, // continue
    RETURN,   // return
    ADD,      // +
    SUB,      // -
    MUL,      // *
    DIV,      // /
    MODULO,   // %
    LESS,     // <
    LESSEQ,   // <=
    GREAT,    // >
    GREATEQ,  // >=
    NOTEQ,    // !=
    EQ,       // ==
    NOT,      // !
    AND,      // &&
    OR,       // ||
    ASSIGN,   // =
    LPAR,     // (
    RPAR,     // )
    LBRACKET, // {
    RBRACKET, // }
    LSQUARE,  // [
    RSQUARE,  // ]
    COMMA,    // ,
    SEMICOLON // ;
};

extern int yylex();
extern int yylval;
extern char *yytext;
extern FILE *yyin;
void advance();
int compunit();
int compunit_();
int decl();
int constdecl();
int btype();
int constdef();
int constinitval();
int vardecl();
int vardef();
int initval();
int funcdef();
int functype();
int funcfparams();
int funcfparam();
int block();
int blockitem();
int stmt();
int expression();
int cond();
int lval();
int primaryexp();
int number();
int unaryexp();
int unaryop();
int funcrparams();
int mulexp();
int mulexp_();
int addexp();
int addexp_();
int relexp();
int relexp_();
int eqexp();
int eqexp_();
int landexp();
int landexp_();
int lorexp();
int lorexp_();
int constexp();
void grammarerror();
int tokbuffer[100000];
int tok = 0;
void advance()
{
    if (tokbuffer[tok] == 0)
    {
        tokbuffer[tok] = yylex();
        printf("tok: %s\n", yytext);
    }
    tok++;
}
/*
CompUnit	→ [ CompUnit ] ( Decl | FuncDef )
*/
int compunit()
{
    if (decl())
    {
        if (compunit_())
        {
            return 1;
        }
        else
        {
            grammarerror();
            return 0;
        }
    }
    else if (funcdef())
    {
        if (compunit_())
        {
            return 1;
        }
        else
        {
            grammarerror();
            return 0;
        }
    }
    else
    {
        grammarerror();
        return 0;
    }
}
int compunit_()
{
    int temptok = tok;
    if (decl())
    {
        if (compunit_())
        {
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else if (funcdef())
    {
        if (compunit_())
        {
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {
        if (tokbuffer[tok - 1] == 0)
        {
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
}
/*
Decl	→ ConstDecl | VarDecl
*/

int decl()
{
    int temptok = tok;
    if (constdecl())
    {
        return 1;
    }
    else if (vardecl())
    {
        return 1;
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
ConstDecl	→ 'const' BType ConstDef { ',' ConstDef } ';'
*/
int constdecl()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == CONST)
    {

        advance();
        if (btype())
        {
            if (constdef())
            {
                while (tokbuffer[tok - 1] != SEMICOLON)
                {
                    if (tokbuffer[tok - 1] == COMMA)
                    {
                        advance();
                        if (constdef())
                        {
                            continue;
                        }
                        else
                        {

                            tok = temptok;
                            return 0;
                        }
                    }
                    else
                    {

                        tok = temptok;
                        return 0;
                    }
                }
                advance();
                return 1;
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
BType	→ 'int'
*/
int btype()
{
    if (tokbuffer[tok - 1] == INT)
    {
        advance();
        return 1;
    }
    else
    {

        return 0;
    }
}
/*
ConstDef	→ Ident { '[' ConstExp ']' } '=' ConstInitVal
*/
int constdef()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == ID)
    {

        advance();
        while (tokbuffer[tok - 1] != ASSIGN)
        {
            if (tokbuffer[tok - 1] == LSQUARE)
            {
                advance();
                if (constexp())
                {
                    if (tokbuffer[tok - 1] == RSQUARE)
                    {
                        advance();
                    }
                    else
                    {

                        tok = temptok;
                        return 0;
                    }
                }
                else
                {

                    tok = temptok;
                    return 0;
                }
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        advance();
        if (constinitval())
        {
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
ConstInitVal	→ ConstExp	
	| '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
*/
int constinitval()
{
    int temptok = tok;
    if (constexp())
    {
        return 1;
    }
    else if (tokbuffer[tok - 1] == LBRACKET)
    {

        advance();
        if (tokbuffer[tok - 1] == RBRACKET)
        {
            advance();
            return 1;
        }
        else
        {
            if (constinitval())
            {
                while (tokbuffer[tok - 1] != RBRACKET)
                {
                    if (tokbuffer[tok - 1] == COMMA)
                    {
                        advance();
                        if (constinitval())
                        {
                            continue;
                        }
                        else
                        {

                            tok = temptok;
                            return 0;
                        }
                    }
                    else
                    {

                        tok = temptok;
                        return 0;
                    }
                }
                advance();
                return 1;
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
VarDecl	→ BType VarDef { ',' VarDef } ';'
*/
int vardecl()
{
    int temptok = tok;
    if (btype())
    {
        if (vardef())
        {
            while (tokbuffer[tok - 1] != SEMICOLON)
            {
                if (tokbuffer[tok - 1] == COMMA)
                {
                    advance();
                    if (vardef())
                    {
                        continue;
                    }
                    else
                    {

                        tok = temptok;
                        return 0;
                    }
                }
                else
                {

                    tok = temptok;
                    return 0;
                }
            }
            advance();
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
VarDef	→ Ident { '[' ConstExp ']' }
	| Ident { '[' ConstExp ']' } '=' InitVal

*/
int vardef()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == ID)
    {
        advance();
        while (tokbuffer[tok - 1] != ASSIGN)
        {
            if (tokbuffer[tok - 1] == SEMICOLON || tokbuffer[tok - 1] == COMMA)
            {
                return 1;
            }
            else if (tokbuffer[tok - 1] == LSQUARE)
            {
                advance();
                if (constexp())
                {
                    if (tokbuffer[tok - 1] == RSQUARE)
                    {
                        advance();
                    }
                    else
                    {

                        tok = temptok;
                        return 0;
                    }
                }
                else
                {

                    tok = temptok;
                    return 0;
                }
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        advance();
        if (initval())
        {
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
InitVal	→ Exp | '{' [ InitVal { ',' InitVal } ] '}'
*/
int initval()
{
    int temptok = tok;
    if (expression())
    {
        return 1;
    }

    else if (tokbuffer[tok - 1] == LBRACKET)
    {
        advance();
        if (tokbuffer[tok - 1] == RBRACKET)
        {
            advance();
            return 1;
        }
        else
        {
            if (initval())
            {
                while (tokbuffer[tok - 1] != RBRACKET)
                {
                    if (tokbuffer[tok - 1] == COMMA)
                    {
                        advance();
                        if (initval())
                        {
                            continue;
                        }
                        else
                        {

                            tok = temptok;
                            return 0;
                        }
                    }
                    else
                    {

                        tok = temptok;
                        return 0;
                    }
                }
                advance();
                return 1;
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
    }

    else
    {

        tok = temptok;
        return 0;
    }
}
/*
FuncDef	→ FuncType Ident '(' [FuncFParams] ')' Block
*/
int funcdef()
{
    int temptok = tok;
    if (functype())
    {
        if (tokbuffer[tok - 1] == ID)
        {
            advance();
            if (tokbuffer[tok - 1] == LPAR)
            {
                advance();
                if (tokbuffer[tok - 1] == RPAR)
                {
                    advance();
                    if (block())
                    {
                        return 1;
                    }
                    else
                    {

                        tok = temptok;
                        return 0;
                    }
                }
                else
                {
                    if (funcfparams())
                    {
                        if (tokbuffer[tok - 1] == RPAR)
                        {
                            advance();
                            if (block())
                            {
                                return 1;
                            }
                            else
                            {

                                tok = temptok;
                                return 0;
                            }
                        }
                        else
                        {

                            tok = temptok;
                            return 0;
                        }
                    }
                    else
                    {

                        tok = temptok;
                        return 0;
                    }
                }
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
FuncType	→ 'void' | 'int'
*/
int functype()
{
    if (tokbuffer[tok - 1] == VOID || tokbuffer[tok - 1] == INT)
    {
        advance();
        return 1;
    }
    else
    {

        return 0;
    }
}
/*
FuncFParams	→ FuncFParam { ',' FuncFParam }
*/
int funcfparams()
{
    int temptok = tok;
    if (funcfparam())
    {
        while (tokbuffer[tok - 1] != RPAR)
        {
            if (tokbuffer[tok - 1] == COMMA)
            {
                advance();
                if (funcfparam())
                {
                    continue;
                }
                else
                {

                    tok = temptok;
                    return 0;
                }
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        return 1;
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
FuncFParam	→ BType Ident ['[' ']' { '[' Exp ']' }]
*/
int funcfparam()
{
    int temptok = tok;
    if (btype())
    {
        if (tokbuffer[tok - 1] == ID)
        {
            advance();
            if (tokbuffer[tok - 1] == LSQUARE)
            {
                advance();
                if (tokbuffer[tok - 1] == RSQUARE)
                {
                    advance();
                    while (tokbuffer[tok - 1] != COMMA && tokbuffer[tok - 1] != RPAR)
                    {
                        if (tokbuffer[tok - 1] == LSQUARE)
                        {
                            advance();
                            if (expression())
                            {
                                if (tokbuffer[tok - 1] == RSQUARE)
                                {
                                    advance();
                                }
                                else
                                {

                                    tok = temptok;
                                    return 0;
                                }
                            }
                            else
                            {

                                tok = temptok;
                                return 0;
                            }
                        }
                        else
                        {

                            tok = temptok;
                            return 0;
                        }
                    }
                    return 1;
                }
                else
                {

                    tok = temptok;
                    return 0;
                }
            }
            else if (tokbuffer[tok - 1] == COMMA)
            {
                return 1;
            }
            else if (tokbuffer[tok - 1] == RPAR)
            {
                return 1;
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
Block	→ '{' { BlockItem } '}'
*/
int block()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == LBRACKET)
    {
        advance();
        while (tokbuffer[tok - 1] != RBRACKET)
        {
            if (blockitem())
            {
                continue;
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        advance();
        return 1;
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
BlockItem	→ Decl | Stmt	
*/
int blockitem()
{
    int temptok = tok;
    if (decl())
    {
        return 1;
    }
    else if (stmt())
    {
        return 1;
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
Stmt	→ LVal '=' Exp ';' | [Exp] ';'  | Block
	| 'if' '( Cond ')' Stmt [ 'else' Stmt ]
	| 'while' '(' Cond ')' Stmt
	| 'break' ';'	| 'continue' ';'
	| 'return' [Exp] ';'

*/
int stmt()
{
    int temptok = tok;
    if (lval())
    {
        if (tokbuffer[tok - 1] == ASSIGN)
        {
            advance();
            if (expression())
            {
                if (tokbuffer[tok - 1] == SEMICOLON)
                {
                    advance();
                    return 1;
                }
                else
                {

                    tok = temptok;
                    return 0;
                }
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else if (expression())
    {
        if (tokbuffer[tok - 1] == SEMICOLON)
        {
            advance();
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }

    else if (tokbuffer[tok - 1] == SEMICOLON)
    {
        advance();
        return 1;
    }
    else if (block())
    {
        return 1;
    }
    else if (tokbuffer[tok - 1] == IF)
    {
        advance();
        if (tokbuffer[tok - 1] == LPAR)
        {
            advance();
            if (cond())
            {
                if (tokbuffer[tok - 1] == RPAR)
                {
                    advance();
                    if (stmt())
                    {
                        if (tokbuffer[tok - 1] == ELSE)
                        {
                            advance();
                            if (stmt())
                            {
                                return 1;
                            }
                            else
                            {

                                tok = temptok;
                                return 0;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {

                        tok = temptok;
                        return 0;
                    }
                }
                else
                {

                    tok = temptok;
                    return 0;
                }
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else if (tokbuffer[tok - 1] == WHILE)
    {
        advance();
        if (tokbuffer[tok - 1] == LPAR)
        {
            advance();
            if (cond())
            {
                if (tokbuffer[tok - 1] == RPAR)
                {
                    advance();
                    if (stmt())
                    {
                        return 1;
                    }
                    else
                    {

                        tok = temptok;
                        return 0;
                    }
                }
                else
                {

                    tok = temptok;
                    return 0;
                }
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else if (tokbuffer[tok - 1] == BREAK)
    {
        advance();
        if (tokbuffer[tok - 1] == SEMICOLON)
        {
            advance();
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else if (tokbuffer[tok - 1] == CONTINUE)
    {
        advance();
        if (tokbuffer[tok - 1] == SEMICOLON)
        {
            advance();
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else if (tokbuffer[tok - 1] == RETURN)
    {
        advance();
        if (tokbuffer[tok - 1] == SEMICOLON)
        {
            advance();
            return 1;
        }
        else if (expression())
        {
            if (tokbuffer[tok - 1] == SEMICOLON)
            {
                advance();
                return 1;
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
Exp	→ AddExp
*/
int expression()
{
    int temptok = tok;
    if (addexp())
    {
        return 1;
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
Cond	→ LOrExp
*/
int cond()
{
    int temptok = tok;
    if (lorexp())
    {
        return 1;
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
LVal	→ Ident {'[' Exp ']'}
*/
int lval()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == ID)
    {
        advance();
        while (tokbuffer[tok - 1] == LSQUARE)
        {
            advance();
            if (expression())
            {
                if (tokbuffer[tok - 1] == RSQUARE)
                {
                    advance();
                }
                else
                {

                    tok = temptok;
                    return 0;
                }
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        if (tokbuffer[tok - 1] == LPAR)
        {

            tok = temptok;
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
PrimaryExp	→ '(' Exp ')' | LVal | Number
*/
int primaryexp()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == LPAR)
    {
        advance();
        if (expression())
        {
            if (tokbuffer[tok - 1] == RPAR)
            {
                return 1;
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else if (lval())
    {
        return 1;
    }
    else if (number())
    {
        return 1;
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
Number	→ IntConst	
*/
int number()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == NUMBER)
    {
        advance();
        return 1;
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
UnaryExp	→ PrimaryExp | Ident '(' [FuncRParams] ')'
	| UnaryOp UnaryExp

*/
int unaryexp()
{
    int temptok = tok;
    if (primaryexp())
    {
        return 1;
    }
    else if (tokbuffer[tok - 1] == ID)
    {
        advance();
        if (tokbuffer[tok - 1] == LPAR)
        {
            advance();
            if (tokbuffer[tok - 1] == RPAR)
            {
                advance();
                return 1;
            }
            else if (funcrparams())
            {
                if (tokbuffer[tok - 1] == RPAR)
                {
                    advance();
                    return 1;
                }
                else
                {

                    tok = temptok;
                    return 0;
                }
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }

    else if (unaryop())
    {
        if (unaryexp())
        {
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
UnaryOp	→ '+' | '−' | '!'
*/
int unaryop()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == ADD || tokbuffer[tok - 1] == SUB || tokbuffer[tok - 1] == NOT)
    {
        advance();
        return 1;
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
FuncRParams	→ Exp { ',' Exp }
*/
int funcrparams()
{
    int temptok = tok;
    if (expression())
    {
        while (tokbuffer[tok - 1] != RPAR)
        {
            if (tokbuffer[tok - 1] == COMMA)
            {
                advance();
                if (expression())
                {
                    continue;
                }
                else
                {

                    tok = temptok;
                    return 0;
                }
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        return 1;
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
/*
MulExp	→ UnaryExp | MulExp ('*' | '/' | '%') UnaryExp
*/
int mulexp()
{
    int temptok = tok;
    if (unaryexp())
    {
        if (mulexp_())
        {
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
int mulexp_()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == MUL||tokbuffer[tok - 1] == MODULO||tokbuffer[tok - 1] == DIV)
    {
        advance();
        if (unaryexp())
        {
            if (mulexp_())
            {
                return 1;
            }
            else
            {
                tok = temptok;
                return 0;
            }
        }
        else
        {
            tok = temptok;
            return 0;
        }
    }
    else
    {
        return 1;
    }
}
/*
AddExp	→ MulExp | AddExp ('+' | '−') MulExp
*/
int addexp()
{
    int temptok = tok;
    if (mulexp())
    {
        if (addexp_())
        {
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
int addexp_()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == ADD||tokbuffer[tok - 1] == SUB)
    {
        advance();
        if (mulexp())
        {
            if (addexp_())
            {
                return 1;
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {
        return 1;
    }
}
/*
RelExp	→ AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp
*/
int relexp()
{
    int temptok = tok;
    if (addexp())
    {
        if (relexp_())
        {
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {
        tok = temptok;
        return 0;
    }
}
int relexp_()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == LESS||tokbuffer[tok - 1] == GREAT||tokbuffer[tok - 1] == LESSEQ||tokbuffer[tok - 1] == GREATEQ)
    {
        advance();
        if (addexp())
        {
            if (relexp_())
            {
                return 1;
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {
        return 1;
    }
}
/*
EqExp	→ RelExp | EqExp ('==' | '!=') RelExp
*/
int eqexp()
{
    int temptok = tok;
    if (relexp())
    {
        if (eqexp_())
        {
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
int eqexp_()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == EQ||tokbuffer[tok - 1] == NOTEQ)
    {
        advance();
        if (relexp())
        {
            if (eqexp_())
            {
                return 1;
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {
        return 1;
    }
}
/*
LAndExp	→ EqExp | LAndExp '&&' EqExp
*/
int landexp()
{

    int temptok = tok;
    if (eqexp())
    {
        if (landexp_())
        {
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
int landexp_()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == AND)
    {
        advance();
        if (eqexp())
        {
            if (landexp_())
            {
                return 1;
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {
        return 1;
    }
}
/*
LOrExp	→ LAndExp | LOrExp '||' LAndExp
*/
int lorexp()
{

    int temptok = tok;
    if (landexp())
    {
        if (lorexp_())
        {
            return 1;
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
int lorexp_()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == OR)
    {
        advance();
        if (landexp())
        {
            if (lorexp_())
            {
                return 1;
            }
            else
            {

                tok = temptok;
                return 0;
            }
        }
        else
        {

            tok = temptok;
            return 0;
        }
    }
    else
    {
        return 1;
    }
}
/*
ConstExp	→ AddExp
*/
int constexp()
{
    int temptok = tok;
    if (addexp())
    {
        return 1;
    }
    else
    {

        tok = temptok;
        return 0;
    }
}
void grammarerror()
{
    printf("grammar error.\n");
    exit(0);
}

int main(int argc, char **argv)
{
    if(argc!=2)
    {
        printf("error.\n");
    }
    else
    {
        setbuf(stdout, NULL);
        yyin = fopen(argv[1], "r");
        advance();
        int rr = compunit();
    }
    return 0;
}
