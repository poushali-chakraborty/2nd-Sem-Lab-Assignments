%{
//definition section
#include <stdio.h>
unsigned charCount =0, wordCount=0, lineCount=0;
%}
word [^ \t\n][a-zA-Z\-\']+
eol \n

//rule section
/*In a pattern, lex replaces the
name inside the braces {} with substitution, the actual regular expression in the definition
section. Our example increments the number of words and characters after the lexer
has recognized a complete word.*/
%%
{word} { wordCount++; charCount += yyleng; } // lex internal variable yyleng which contains the length of the string 
{eol} { charCount++; lineCount++; }
[a-zA-Z0-9]+ {}
%%

int main(){
yylex();
printf("\n%d %d %d\n",lineCount, wordCount, charCount);

/*
It first calls the lexer’s entry point yylex() and then calls printf() to print the results of
this run. Note that our code doesn’t do anything fancy; it doesn’t accept commandline arguments, doesn’t open any files, but uses the lex default to read from the standard
input.
*/
}

