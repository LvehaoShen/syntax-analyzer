%{
enum yytokentype{
	ID=256,		 // 标识符
	NUMBER,	 // 整数
	STRING,	//字符串
	INT,		 // int
	STR,		// str
	VOID,		 // void
	CONST,		 // const
	IF,			 // if
	ELSE,		 // else
	WHILE,		 // while
	BREAK,		 // break
	CONTINUE,	 // continue
	RETURN,		 // return
	ADD,		 // +
	SUB,		 // -
	MUL,		 // *
	DIV,		 // /
	MODULO,		 // %
	LESS,		 // <
	LESSEQ,		 // <=
	GREAT,		 // >
	GREATEQ,	 // >=
	NOTEQ,		 // !=
	EQ,		 	 // ==
	NOT,		 // !
	AND, 		 // &&
	OR,			 // ||
	ASSIGN,		 // =
	LPAR,		 // (
	RPAR,		 // )
	LBRACKET,	 // {
	RBRACKET,	 // }
	LSQUARE,	 // [
	RSQUARE,	 // ]
	COMMA,		 // ,
	SEMICOLON	 // ;
};
int yylval;
%}
STRING \"(.*)\"
DIGIT [0-9] 
ID [a-zA-Z][a-zA-Z0-9|_]*
%%
[ |\t|\n]+	{ /* ignore whitespace */ }
{DIGIT}+    {yylval = atoi(yytext); return NUMBER;}
"int"       {return INT;}
"str"		{return STR;}
"void"      {return VOID;}
"const"     {return CONST;}
"if"        {return IF;}
"else"      {return ELSE;}
"while"     {return WHILE;}
"break"     {return BREAK;}
"continue"  {return CONTINUE;}
"return"    {return RETURN;}

{STRING}		{return STRING;}
{ID}        {return ID;}

"+"         {return ADD;}
"-"         {return SUB;}
"*"         {return MUL;}
"/"         {return DIV;}
"%"         {return MODULO;}
"<"         {return LESS;}
"<="        {return LESSEQ;}
">"         {return GREAT;}
">="        {return GREATEQ;}
"="         {return ASSIGN;}
"=="        {return EQ;}
"!="        {return NOTEQ;}
"!"         {return NOT;}
"&&"        {return AND;}
"||"        {return OR;}
"("         {return LPAR;}
")"         {return RPAR;}
"{"         {return LBRACKET;}
"}"         {return RBRACKET;}
"["         {return LSQUARE;}
"]"         {return RSQUARE;}
","         {return COMMA;}
";"         {return SEMICOLON;}
%%
int yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}

int yywrap()
{
	return 1;
}

