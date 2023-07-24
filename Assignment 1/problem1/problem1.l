%{
/*
* The definition section, introduces any initial C program code we want
copied into the final program.
Lex copies the material between %{ and %} directly to the generated C file.
*/
# include "y.tab.h" /*token codes from parser */

# define LOOKUP 0 /* default - not a defined word type. */
/**extra**/

int state;
int add_word(int type, char *word);
int lookup_word(char *word);
%}
/*The %% marks the end of this section.*/
/*The next section is the rules section. Each rule is made up of two parts: a pattern and an
action, separated by whitespace. The lexer that lex generates will execute the action when
it recognizes the pattern.*/
%% 
\n { state = LOOKUP; } /* end of line, return to default state */
\.\n {state =LOOKUP;
	return 0; //end of statement
	}

^verb { state = VERB; } /*The caret, ^, at the beginning of the pattern makes the pattern match only at the beginning of an input line*/
^adj { state = ADJ; }
^adv { state = ADV; }
^noun { state = NOUN; }
^prep { state = PREP; }
^pron { state = PRON; }
^conj { state = CONJ; }


^noun_or_verb {state = NOUN_OR_VERB; }

[a-zA-Z]+ {
/* a normal word, define it or look it up */
if(state != LOOKUP) {
/* define the current word */
add_word(state, yytext);
}
 else {
switch(lookup_word(yytext)) {
case VERB: printf("%s: verb\n", yytext); return VERB;
case ADJ: printf("%s: adjective\n", yytext); return ADJ;
case ADV: printf("%s: adverb\n", yytext); return ADV;
case NOUN: printf("%s: noun\n", yytext); return NOUN;
case PREP: printf("%s: preposition\n", yytext); return PREP;
case PRON: printf("%s: pronoun\n", yytext); return PRON;
case CONJ: printf("%s: conjunction\n", yytext); return CONJ;

/**extra**/
case NOUN_OR_VERB: printf("%s: noun or verb\n", yytext); return NOUN_OR_VERB;

default:
printf("%s: don't recognize\n", yytext);
break;
}
}
}

%%

/* define a linked list of words and types */
struct word {
char *word_name;
int word_type;
struct word *next;
};
struct word *word_list; /* first element in word list */
extern void *malloc() ;
int add_word(int type, char *word)
{
struct word *wp;
if(lookup_word(word) != LOOKUP) {
printf("!!! warning: word %s already defined \n", word);
return 0;
}
/* word not there, allocate a new entry and link it on the list */
wp = (struct word *) malloc(sizeof(struct word));
wp->next = word_list;
/* have to copy the word itself as well */
wp->word_name = (char *) malloc(strlen(word)+1);
strcpy(wp->word_name, word);
wp->word_type = type;
word_list = wp;
return 1; /* it worked */
}
int lookup_word(char *word)
{
struct word *wp = word_list;
/* search down the list looking for the word */
for(; wp; wp = wp->next) {
if(strcmp(wp->word_name, word) == 0)
return wp->word_type;
}
return LOOKUP; /* not found */
}
