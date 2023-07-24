%{

#include <stdio.h>
#include <string.h>
int yylex();
int yyerror();


double vbltable[26];
char strvartable[26][100];
%}
%union {
double dval;
int vblno;
char strval[100];
}

%token <vblno> NAME
%token <vblno> SNAME
%token <dval> NUMBER
%token <strval> STR
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS
%type <dval> expression
%%
statement_list: statement '\n'
| statement_list statement '\n'
;
statement: NAME '=' expression { vbltable[$1] = $3; }
| expression { printf("= %g\n", $1); }
| SNAME '=' STR { strcpy(strvartable[$1],$3); }
| SNAME {printf("=%s\n",strvartable[$1]);}
| SNAME '+' SNAME { strcat(strvartable[$1],strvartable[$3]); printf("=%s\n",strvartable[$1]);}
;
expression: expression '+' expression { $$ = $1 + $3; }
| expression '-' expression { $$ = $1 - $3; }
| expression '*' expression { $$ = $1 * $3; }
| expression '/' expression { 
if($3 == 0.0)
yyerror("divide by zero");
else
$$ = $1 / $3;
}
| NUMBER
| NAME { $$ = vbltable[$1]; }
;
%%
extern FILE *yyin;
int main()
{
do
{
yyparse();
}
while (!feof(yyin));
return 0;
}
int yyerror(s)
char *s;
{

fprintf(stderr, "1%s\n", s);
}
