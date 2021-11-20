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
typedef struct _ast ast;
typedef struct _ast *past;
struct _ast
{
    char *nodetype;
    char *content;
    past left;
    past right;
};
int tokbuffer[100000];
char* wordbuffer[100000];
int tok = 0;
void advance();
void showast(past node, int nest);
past newastnode(char *nodetype, char *content, past left, past right);
past astcompunit();
past astcompunit_();
past astdecl();
past astconstdecl();
past astbtype();
past astconstdef();
past astconstinitval();
past astvardecl();
past astvardef();
past astinitval();
past astfuncdef();
past astfunctype();
past astfuncfparams();
past astfuncfparam();
past astblock();
past astblockitem();
past aststmt();
past astexpression();
past astcond();
past astlval();
past astprimaryexp();
past astnumber();
past astunaryexp();
past astunaryop();
past astfuncrparams();
past astmulexp();
past astmulexp_();
past astaddexp();
past astaddexp_();
past astrelexp();
past astrelexp_();
past asteqexp();
past asteqexp_();
past astlandexp();
past astlandexp_();
past astlorexp();
past astlorexp_();
past astconstexp();
void grammarerror();
void advance()
{
    if (tokbuffer[tok] == 0)
    {
        tokbuffer[tok] = yylex();
        wordbuffer[tok] = strdup(yytext);
        printf("tok: %s\n", yytext);
    }
    tok++;
}
void showast(past node, int nest)
{
    if (node == NULL)
        return;

    int i = 0;
    for (i = 0; i < nest; i++)
        printf("  ");
    printf("%s %s\n", node->nodetype, node->content);
    showast(node->left, nest + 1);
    showast(node->right, nest + 1);
}
past newastnode(char *nodetype, char *content, past left, past right)
{
    past node = malloc(sizeof(ast));
    if (node == NULL)
    {
        printf("run out of memory.\n");
        exit(0);
    }
    memset(node, 0, sizeof(ast));
    node->nodetype = nodetype;
    node->content = content;
    node->left = left;
    node->right = right;
    return node;
}
/*
CompUnit	→ [ CompUnit ] ( Decl | FuncDef )
*/
past astcompunit()
{
    past root = newastnode("compunit", "", NULL, NULL);
    if (root->left = astdecl())
    {
        if (root->left->right = astcompunit_())
        {
            return root;
        }
        else
        {
            grammarerror();
            return NULL;
        }
    }
    else if (root->left = astfuncdef())
    {
        if (root->left->right = astcompunit_())
        {
            return root;
        }
        else
        {
            grammarerror();
            return NULL;
        }
    }
    else
    {
        grammarerror();
        return NULL;
    }
}
past astcompunit_()
{
    int temptok = tok;
    past root = newastnode("compunit_", "", NULL, NULL);
    if (root->left = astdecl())
    {
        if (root->left->right = astcompunit_())
        {
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else if (root->left = astfuncdef())
    {
        if (root->left->right = astcompunit_())
        {
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        if (tokbuffer[tok - 1] == 0)
        {
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
}
/*
Decl	→ ConstDecl | VarDecl
*/
past astdecl()
{
    int temptok = tok;
    past root = newastnode("decl", "", NULL, NULL);
    if (root->left = astconstdecl())
    {
        return root;
    }
    else if (root->left = astvardecl())
    {
        return root;
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
ConstDecl	→ 'const' BType ConstDef { ',' ConstDef } ';'
*/
past astconstdecl()
{
    int temptok = tok;
    past root = newastnode("constdecl", "", NULL, NULL);
    if (tokbuffer[tok - 1] == CONST)
    {
        root->left = newastnode("const", "", NULL, NULL);
        advance();
        if (root->left->right = astbtype())
        {
            if (root->left->right->right = astconstdef())
            {
                past temp = root->left->right->right;
                while (tokbuffer[tok - 1] != SEMICOLON)
                {
                    if (tokbuffer[tok - 1] == COMMA)
                    {
                        advance();
                        if (temp->right = astconstdef())
                        {
                            temp = temp->right;
                        }
                        else
                        {
                            tok = temptok;
                            return NULL;
                        }
                    }
                    else
                    {
                        tok = temptok;
                        return NULL;
                    }
                }
                advance();
                return root;
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
BType	→ 'int'
*/
past astbtype()
{ 
    if (tokbuffer[tok - 1] == INT)
    {
        past root = newastnode("btype", "int", NULL, NULL);
        advance();
        return root;
    }
    else
    {
        return NULL;
    }
}
/*
ConstDef	→ Ident { '[' ConstExp ']' } '=' ConstInitVal
*/
past astconstdef()
{
    int temptok = tok;
    past root = newastnode("constdef", "", NULL, NULL);
    if (tokbuffer[tok - 1] == ID)
    {
        root->left = newastnode("ident", strdup(wordbuffer[tok-1]), NULL, NULL);
        advance();
        past temp = root->left;
        int flag = 0;
        while (tokbuffer[tok - 1] != ASSIGN)
        {
            if (tokbuffer[tok - 1] == LSQUARE)
            {
                advance();
                if (temp->right = astconstexp())
                {
                    temp = temp->right;
                    if (tokbuffer[tok - 1] == RSQUARE)
                    {
                        advance();
                        flag++;
                    }
                    else
                    {
                        tok = temptok;
                        return NULL;
                    }
                }
                else
                {
                    tok = temptok;
                    return NULL;
                }
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        if (flag > 0)
        {
            temp->right = newastnode("array", "", NULL, NULL);
            temp = temp->right;
        }
        advance();
        if (temp->right = astconstinitval())
        {
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
ConstInitVal	→ ConstExp	
	| '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
*/
past astconstinitval()
{
    int temptok = tok;
    past root = newastnode("constinitval", "", NULL, NULL);
    if (root->left = astconstexp())
    {
        return root;
    }
    else if (tokbuffer[tok - 1] == LBRACKET)
    {
        root->left = newastnode("constinitlist", "", NULL, NULL);
        advance();
        if (tokbuffer[tok - 1] == RBRACKET)
        {
            advance();
            return root;
        }
        else
        {
            if (root->left->right = astconstinitval())
            {
                past temp = root->left->right;
                while (tokbuffer[tok - 1] != RBRACKET)
                {
                    if (tokbuffer[tok - 1] == COMMA)
                    {
                        advance();
                        if (temp->right = astconstinitval())
                        {
                            temp = temp->right;
                        }
                        else
                        {
                            tok = temptok;
                            return NULL;
                        }
                    }
                    else
                    {
                        tok = temptok;
                        return NULL;
                    }
                }
                advance();
                return root;
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
VarDecl	→ BType VarDef { ',' VarDef } ';'
*/
past astvardecl()
{
    int temptok = tok;
    past root = newastnode("vardecl", "", NULL, NULL);
    if (root->left = astbtype())
    {
        if (root->left->right = astvardef())
        {
            past temp = root->left->right;
            while (tokbuffer[tok - 1] != SEMICOLON)
            {
                if (tokbuffer[tok - 1] == COMMA)
                {
                    advance();
                    if (temp->right = astvardef())
                    {
                        temp = temp->right;
                    }
                    else
                    {
                        tok = temptok;
                        return NULL;
                    }
                }
                else
                {
                    tok = temptok;
                    return NULL;
                }
            }
            advance();
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
VarDef	→ Ident { '[' ConstExp ']' }
	| Ident { '[' ConstExp ']' } '=' InitVal

*/
past astvardef()
{
    int temptok = tok;
    past root = newastnode("vardef", "", NULL, NULL);
    if (tokbuffer[tok - 1] == ID)
    {
        root->left = newastnode("ident", strdup(wordbuffer[tok-1]), NULL, NULL);
        advance();
        past temp = root->left;
        int flag = 0;
        while (tokbuffer[tok - 1] != ASSIGN)
        {
            if (tokbuffer[tok - 1] == SEMICOLON || tokbuffer[tok - 1] == COMMA)
            {
                if (flag > 0)
                {
                    temp->right = newastnode("array", "", NULL, NULL);
                }
                return root;
            }
            else if (tokbuffer[tok - 1] == LSQUARE)
            {
                advance();
                if (temp->right = astconstexp())
                {
                    temp = temp->right;
                    if (tokbuffer[tok - 1] == RSQUARE)
                    {
                        advance();
                        flag++;
                    }
                    else
                    {
                        tok = temptok;
                        return NULL;
                    }
                }
                else
                {
                    tok = temptok;
                    return NULL;
                }
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        if (flag > 0)
        {
            temp->right = newastnode("array", "", NULL, NULL);
            temp = temp->right;
        }
        advance();
        if (temp->right = astinitval())
        {
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
InitVal	→ Exp | '{' [ InitVal { ',' InitVal } ] '}'
*/
past astinitval()
{
    int temptok = tok;
    past root = newastnode("initval", "", NULL, NULL);
    if (root->left = astexpression())
    {
        return root;
    }

    else if (tokbuffer[tok - 1] == LBRACKET)
    {
        root->left->right = newastnode("initlist", "", NULL, NULL);
        past temp = root->left->right;
        advance();
        if (tokbuffer[tok - 1] == RBRACKET)
        {
            advance();
            return root;
        }
        else
        {
            if (temp->right = astinitval())
            {
                temp = temp->right;
                while (tokbuffer[tok - 1] != RBRACKET)
                {
                    if (tokbuffer[tok - 1] == COMMA)
                    {
                        advance();
                        if (temp->right = astinitval())
                        {
                            temp = temp->right;
                        }
                        else
                        {
                            tok = temptok;
                            return NULL;
                        }
                    }
                    else
                    {
                        tok = temptok;
                        return NULL;
                    }
                }
                advance();
                return root;
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
    }

    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
FuncDef	→ FuncType Ident '(' [FuncFParams] ')' Block
*/
past astfuncdef()
{
    int temptok = tok;
    past root = newastnode("funcdef", "", NULL, NULL);
    if (root->left = astfunctype())
    {
        if (tokbuffer[tok - 1] == ID)
        {
            root->left->right = newastnode("ident", strdup(wordbuffer[tok-1]), NULL, NULL);
            past temp = root->left->right;
            advance();
            if (tokbuffer[tok - 1] == LPAR)
            {
                temp->right = newastnode("funcfparamslist", "", NULL, NULL);
                temp = temp->right;
                advance();
                if (tokbuffer[tok - 1] == RPAR)
                {
                    advance();
                    if (temp->right = astblock())
                    {
                        return root;
                    }
                    else
                    {

                        tok = temptok;
                        return NULL;
                    }
                }
                else
                {
                    if (temp->right = astfuncfparams())
                    {
                        temp = temp->right;
                        if (tokbuffer[tok - 1] == RPAR)
                        {
                            advance();
                            if (temp->right = astblock())
                            {
                                return root;
                            }
                            else
                            {

                                tok = temptok;
                                return NULL;
                            }
                        }
                        else
                        {
                            tok = temptok;
                            return NULL;
                        }
                    }
                    else
                    {
                        tok = temptok;
                        return NULL;
                    }
                }
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
FuncType	→ 'void' | 'int'
*/
past astfunctype()
{

    if (tokbuffer[tok - 1] == VOID || tokbuffer[tok - 1]==INT)
    {
        past root = newastnode("functype",strdup(wordbuffer[tok-1]), NULL, NULL);
        advance();
        return root;
    }
    else
    {
        return NULL;
    }
}
/*
FuncFParams	→ FuncFParam { ',' FuncFParam }
*/
past astfuncfparams()
{
    int temptok = tok;
    past root = newastnode("funcfparams", "", NULL, NULL);
    if (root->left = astfuncfparam())
    {
        past temp = root->left;
        while (tokbuffer[tok - 1] != RPAR)
        {
            if (tokbuffer[tok - 1] == COMMA)
            {
                advance();
                if (temp->right = astfuncfparam())
                {
                    temp = temp->right;
                }
                else
                {
                    tok = temptok;
                    return NULL;
                }
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        return root;
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
FuncFParam	→ BType Ident ['[' ']' { '[' Exp ']' }]
*/
past astfuncfparam()
{
    int temptok = tok;
    past root = newastnode("funcfparm", "", NULL, NULL);
    if (root->left = astbtype())
    {
        if (tokbuffer[tok - 1] == ID)
        {
            root->left->right = newastnode("ident", strdup(wordbuffer[tok-1]), NULL, NULL);
            advance();
            past temp = root->left->right;
            int flag = 0;
            if (tokbuffer[tok - 1] == LSQUARE)
            {
                advance();
                if (tokbuffer[tok - 1] == RSQUARE)
                {
                    temp->right = newastnode("array", "", NULL, NULL);
                    temp = temp->right;
                    advance();
                    while (tokbuffer[tok - 1] != COMMA && tokbuffer[tok - 1] != RPAR)
                    {
                        if (tokbuffer[tok - 1] == LSQUARE)
                        {
                            advance();
                            if (temp->right = astexpression())
                            {
                                temp = temp->right;
                                if (tokbuffer[tok - 1] == RSQUARE)
                                {
                                    advance();
                                }
                                else
                                {
                                    tok = temptok;
                                    return NULL;
                                }
                            }
                            else
                            {
                                tok = temptok;
                                return NULL;
                            }
                        }
                        else
                        {
                            tok = temptok;
                            return NULL;
                        }
                    }
                    return root;
                }
                else
                {
                    tok = temptok;
                    return NULL;
                }
            }
            else if (tokbuffer[tok - 1] == COMMA)
            {
                return root;
            }
            else if (tokbuffer[tok - 1] == RPAR)
            {
                return root;
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
Block	→ '{' { BlockItem } '}'
*/
past astblock()
{
    int temptok = tok;
    past root = newastnode("block", "", NULL, NULL);
    if (tokbuffer[tok - 1] == LBRACKET)
    {
        past temp = root;
        int flag = 0;
        advance();
        while (tokbuffer[tok - 1] != RBRACKET)
        {
            if (flag == 0)
            {
                if (temp->left = astblockitem())
                {
                    flag++;
                    temp = temp->left;
                }
                else
                {
                    tok = temptok;
                    return NULL;
                }
            }
            else
            {
                if (temp->right = astblockitem())
                {
                    flag++;
                    temp = temp->right;
                }
                else
                {
                    tok = temptok;
                    return NULL;
                }
            }
        }
        advance();
        return root;
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
BlockItem	→ Decl | Stmt	
*/
past astblockitem()
{
    int temptok = tok;
    past root = newastnode("blockitem", "", NULL, NULL);
    if (root->left = astdecl())
    {
        return root;
    }
    else if (root->left = aststmt())
    {
        return root;
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
Stmt	→ LVal '=' Exp ';' | [Exp] ';'  | Block
	| 'if' '( Cond ')' Stmt [ 'else' Stmt ]
	| 'while' '(' Cond ')' Stmt
	| 'break' ';'	| 'continue' ';'
	| 'return' [Exp] ';'

*/
past aststmt()
{
    int temptok = tok;
    past root = newastnode("stmt", "", NULL, NULL);
    if (root->left = astlval())
    {
        if (tokbuffer[tok - 1] == ASSIGN)
        {
            root->left->right = newastnode("assignstmt", "", NULL, NULL);
            advance();
            if (root->left->right->right = astexpression())
            {
                if (tokbuffer[tok - 1] == SEMICOLON)
                {
                    advance();
                    return root;
                }
                else
                {
                    tok = temptok;
                    return NULL;
                }
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else if (root->left = astexpression())
    {
        if (tokbuffer[tok - 1] == SEMICOLON)
        {
            advance();
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }

    else if (tokbuffer[tok - 1] == SEMICOLON)
    {
        root->left = newastnode("nullstmt", "", NULL, NULL);
        advance();
        return root;
    }
    else if (root->left = astblock())
    {
        return root;
    }
    else if (tokbuffer[tok - 1] == IF)
    {
        root->left = newastnode("ifstmt", "", NULL, NULL);
        advance();
        if (tokbuffer[tok - 1] == LPAR)
        {
            advance();
            if (root->left->right = astcond())
            {
                if (tokbuffer[tok - 1] == RPAR)
                {
                    advance();
                    if (root->left->right->right = aststmt())
                    {
                        if (tokbuffer[tok - 1] == ELSE)
                        {
                            root->left->right->right->right = newastnode("elsestmt", "", NULL, NULL);
                            advance();
                            if (root->left->right->right->right->right = aststmt())
                            {
                                return root;
                            }
                            else
                            {
                                tok = temptok;
                                return NULL;
                            }
                        }
                        else
                        {
                            return root;
                        }
                    }
                    else
                    {
                        tok = temptok;
                        return NULL;
                    }
                }
                else
                {
                    tok = temptok;
                    return NULL;
                }
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else if (tokbuffer[tok - 1] == WHILE)
    {
        root->left = newastnode("whilestmt", "", NULL, NULL);
        advance();
        if (tokbuffer[tok - 1] == LPAR)
        {
            advance();
            if (root->left->right = astcond())
            {
                if (tokbuffer[tok - 1] == RPAR)
                {
                    advance();
                    if (root->left->right->right = aststmt())
                    {
                        return root;
                    }
                    else
                    {
                        tok = temptok;
                        return NULL;
                    }
                }
                else
                {
                    tok = temptok;
                    return NULL;
                }
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else if (tokbuffer[tok - 1] == BREAK)
    {
        advance();
        if (tokbuffer[tok - 1] == SEMICOLON)
        {
            root->left = newastnode("breakstmt", "", NULL, NULL);
            advance();
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else if (tokbuffer[tok - 1] == CONTINUE)
    {
        advance();
        if (tokbuffer[tok - 1] == SEMICOLON)
        {
            root->left = newastnode("continuestmt", "", NULL, NULL);
            advance();
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else if (tokbuffer[tok - 1] == RETURN)
    {
        root->left = newastnode("returnstmt", "", NULL, NULL);
        advance();
        if (tokbuffer[tok - 1] == SEMICOLON)
        {
            advance();
            return root;
        }
        else if (root->left->right = astexpression())
        {
            if (tokbuffer[tok - 1] == SEMICOLON)
            {
                advance();
                return root;
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
Exp	→ AddExp
*/
past astexpression()
{
    int temptok = tok;
    past root = newastnode("exp", "", NULL, NULL);
    if (root->left = astaddexp())
    {
        return root;
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
Cond	→ LOrExp
*/
past astcond()
{
    int temptok = tok;
    past root = newastnode("condexp", "", NULL, NULL);
    if (root->left = astlorexp())
    {
        return root;
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
LVal	→ Ident {'[' Exp ']'}
*/
past astlval()
{
    int temptok = tok;
    past root = newastnode("lval", "", NULL, NULL);
    if (tokbuffer[tok - 1] == ID)
    {
        root->left = newastnode("ident", strdup(wordbuffer[tok-1]), NULL, NULL);
        advance();
        int flag = 0;
        past temp = root;
        while (tokbuffer[tok - 1] == LSQUARE)
        {
            advance();
            if (temp->right = astexpression())
            {
                temp = temp->right;
                if (tokbuffer[tok - 1] == RSQUARE)
                {
                    advance();
                    flag++;
                }
                else
                {

                    tok = temptok;
                    return NULL;
                }
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        if (tokbuffer[tok - 1] == LPAR)
        {
            tok = temptok;
            return NULL;
        }
        else
        {
            if (flag > 0)
            {
                temp->right = newastnode("array", "", NULL, NULL);
            }
            return root;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
PrimaryExp	→ '(' Exp ')' | LVal | Number
*/
past astprimaryexp()
{
    int temptok = tok;
    past root = newastnode("primaryexp", "", NULL, NULL);
    if (tokbuffer[tok - 1] == LPAR)
    {
        advance();
        if (root->left = astexpression())
        {
            if (tokbuffer[tok - 1] == RPAR)
            {
                return root;
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else if (root->left = astlval())
    {
        return root;
    }
    else if (root->left = astnumber())
    {
        return root;
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
Number	→ IntConst	
*/
past astnumber()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == NUMBER)
    {
        past root = newastnode("number", strdup(wordbuffer[tok-1]), NULL, NULL);
        advance();
        return root;
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
UnaryExp	→ PrimaryExp | Ident '(' [FuncRParams] ')'
	| UnaryOp UnaryExp

*/
past astunaryexp()
{
    int temptok = tok;
    past root = newastnode("unaryexp", "", NULL, NULL);
    if (root->left = astprimaryexp())
    {
        return root;
    }
    else if (tokbuffer[tok - 1] == ID)
    {
        root->left = newastnode("ident", strdup(wordbuffer[tok-1]), NULL, NULL);
        advance();
        if (tokbuffer[tok - 1] == LPAR)
        {
            advance();
            if (tokbuffer[tok - 1] == RPAR)
            {
                advance();
                return root;
            }
            else if (root->left->right = astfuncrparams())
            {
                if (tokbuffer[tok - 1] == RPAR)
                {
                    advance();
                    return root;
                }
                else
                {
                    tok = temptok;
                    return NULL;
                }
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else if (root->left = astunaryop())
    {
        if (root->left->right = astunaryexp())
        {
            return root;
        }
        else
        {

            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
UnaryOp	→ '+' | '−' | '!'
*/
past astunaryop()
{
    int temptok = tok;
    if (tokbuffer[tok - 1] == ADD || tokbuffer[tok - 1] == SUB || tokbuffer[tok - 1] == NOT)
    {
        past root = newastnode("unaryop",strdup(wordbuffer[tok-1]), NULL, NULL);
        advance();
        return root;
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
FuncRParams	→ Exp { ',' Exp }
*/
past astfuncrparams()
{
    int temptok = tok;
    past root = newastnode("funcrparams", "", NULL, NULL);
    if (root->left = astexpression())
    {
        past temp = root;
        while (tokbuffer[tok - 1] != RPAR)
        {
            if (tokbuffer[tok - 1] == COMMA)
            {
                advance();
                if (temp->right = astexpression())
                {
                    temp = temp->right;
                }
                else
                {
                    tok = temptok;
                    return NULL;
                }
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        return root;
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
/*
MulExp	→ UnaryExp | MulExp ('*' | '/' | '%') UnaryExp
*/
past astmulexp()
{
    int temptok = tok;
    past root = newastnode("mulexp", "", NULL, NULL);
    if (root->left = astunaryexp())
    {
        if (root->left->right = astmulexp_())
        {
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
past astmulexp_()
{
    int temptok = tok;
    past root = newastnode("mulexp_", "", NULL, NULL);
    if (tokbuffer[tok - 1] == MUL || tokbuffer[tok - 1] == MODULO || tokbuffer[tok - 1] == DIV)
    {
        root->left = newastnode("operator", strdup(wordbuffer[tok-1]), NULL, NULL);
        advance();
        if (root->left->right = astunaryexp())
        {
            if (root->left->right->right = astmulexp_())
            {
                return root;
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        return root;
    }
}
/*
AddExp	→ MulExp | AddExp ('+' | '−') MulExp
*/
past astaddexp()
{
    int temptok = tok;
    past root = newastnode("addexp", "", NULL, NULL);
    if (root->left = astmulexp())
    {
        if (root->left->right = astaddexp_())
        {
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
past astaddexp_()
{
    int temptok = tok;
    past root = newastnode("addexp_", "", NULL, NULL);
    if (tokbuffer[tok - 1] == ADD || tokbuffer[tok - 1] == SUB)
    {
        root->left = newastnode("operator", strdup(wordbuffer[tok-1]), NULL, NULL);
        advance();
        if (root->left->right = astmulexp())
        {
            if (root->left->right->right = astaddexp_())
            {
                return root;
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        return root;
    }
}
/*
RelExp	→ AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp
*/
past astrelexp()
{
    int temptok = tok;
    past root = newastnode("relexp", "", NULL, NULL);
    if (root->left = astaddexp())
    {
        if (root->left->right = astrelexp_())
        {
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
past astrelexp_()
{
    int temptok = tok;
    past root = newastnode("relexp_", "", NULL, NULL);
    if (tokbuffer[tok - 1] == LESS || tokbuffer[tok - 1] == GREAT || tokbuffer[tok - 1] == LESSEQ || tokbuffer[tok - 1] == GREATEQ)
    {
        root->left = newastnode("operator", strdup(wordbuffer[tok-1]), NULL, NULL);
        advance();
        if (root->left->right = astaddexp())
        {
            if (root->left->right->right = astrelexp_())
            {
                return root;
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        return root;
    }
}
/*
EqExp	→ RelExp | EqExp ('==' | '!=') RelExp
*/
past asteqexp()
{

    int temptok = tok;
    past root = newastnode("eqexp", "", NULL, NULL);
    if (root->left = astrelexp())
    {
        if (root->left->right = asteqexp_())
        {
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
past asteqexp_()
{
    int temptok = tok;
    past root = newastnode("eqexp_", "", NULL, NULL);
    if (tokbuffer[tok - 1] == EQ || tokbuffer[tok - 1] == NOTEQ)
    {
        root->left = newastnode("operator",strdup(wordbuffer[tok-1]), NULL, NULL);
        advance();
        if (root->left->right = astrelexp())
        {
            if (root->left->right->right = asteqexp_())
            {
                return root;
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        return root;
    }
}
/*
LAndExp	→ EqExp | LAndExp '&&' EqExp
*/
past astlandexp()
{
    int temptok = tok;
    past root = newastnode("landexp", "", NULL, NULL);
    if (root->left = asteqexp())
    {
        if (root->left->right = astlandexp_())
        {
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
past astlandexp_()
{
    int temptok = tok;
    past root = newastnode("landexp_", "", NULL, NULL);
    if (tokbuffer[tok - 1] == AND)
    {
        root->left = newastnode("operator", strdup(wordbuffer[tok-1]), NULL, NULL);
        advance();
        if (root->left->right = asteqexp())
        {
            if (root->left->right->right = astlandexp_())
            {
                return root;
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        return root;
    }
}
/*
LOrExp	→ LAndExp | LOrExp '||' LAndExp
*/
past astlorexp()
{
    int temptok = tok;
    past root = newastnode("lorexp", "", NULL, NULL);
    if (root->left = astlandexp())
    {
        if (root->left->right = astlorexp_())
        {
            return root;
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        tok = temptok;
        return NULL;
    }
}
past astlorexp_()
{
    int temptok = tok;
    past root = newastnode("lorexp_", "", NULL, NULL);
    if (tokbuffer[tok - 1] == OR)
    {
        root->left = newastnode("operator", "", NULL, NULL);
        advance();
        if (root->left->right = astlandexp())
        {
            if (root->left->right->right = astlorexp_())
            {
                return root;
            }
            else
            {
                tok = temptok;
                return NULL;
            }
        }
        else
        {
            tok = temptok;
            return NULL;
        }
    }
    else
    {
        return root;
    }
}
/*
ConstExp	→ AddExp
*/
past astconstexp()
{
    int temptok = tok;
    past root = newastnode("constexp", "", NULL, NULL);
    if (root->left = astaddexp())
    {
        return root;
    }
    else
    {
        tok = temptok;
        return NULL;
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
        past root = astcompunit();
        showast(root, 0);
    }
    return 0;
}
