%{
#include "y.tab.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
//extern double vbltable[26];

%}
%%
([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?) {
yylval.dval = atof(yytext); return NUMBER;
}
[ \t] ; /* ignore whitespace */
[a-z] { yylval.vblno = yytext[0] - 'a'; return NAME; }
[A-Z] { yylval.vblno = yytext[0] - 'A'; return SNAME; }

[\"][a-zA-Z0-9]+[\"] {
for (int i = 0, j; yytext[i] != '\0'; ++i) {

      // enter the loop if the character is not an alphabet
      // and not the null character
      while (!(yytext[i] >= 'a' && yytext[i] <= 'z') && !(yytext[i] >= 'A' && yytext[i] <= 'Z') && !(yytext[i] >= '0' && yytext[i] <= '9') && !(yytext[i] == '\0')) {
         for (j = i; yytext[j] != '\0'; ++j) {

            // if jth element of line is not an alphabet,
            // assign the value of (j+1)th element to the jth element
            yytext[j] = yytext[j + 1];
         }
         yytext[j] = '\0';
      }
   }

strcpy(yylval.strval,yytext);
//printf("\nstr= %s\n",yylval.strval);
return STR; 
}
"$" { return 0; /* end of input */ }
\n |
. return yytext[0];
%%
int yywrap()

{

return 1;

}
