/* Lex program to search a word in a file */
  
%{
 /* Definition section */
#include<string.h>
#include<time.h>

void check(char *);
%}
  
/* Rule Section */
%%
[a-zA-Z]+ check(yytext);
%%
char **fileList;
int currentFile = 0;

int nFiles;
unsigned argc;
char **argv;
// driver code
int main(unsigned argc0,char **argv0)
{

	argc=argc0;
	argv=argv0;
    // The function that starts the analysis 
    yylex();

    return 0;
}
void check(char *str)
{
      /* fp as pointer 
    of File type */
    int flag=0;
    FILE *fp;
    int i=0;
    char temp[100];
    clock_t t;
    t=clock();
    
    
     fileList = argv+1;
     nFiles = argc-1;
     
     for(i=0;i<argc-1;i++) {
     	flag=0;
     
     	currentFile = i+1;
	fp = fopen(argv[i+1], "r");
	if (!fp) {
	fprintf(stderr,"could not open %s\n",argv[i+1]);
	exit(1);
	}
	else{
	/* fp points to the file input.txt 
    and opens it in read mode */
    printf("search file %s\n",fileList[currentFile-1]);
    while(fgets(temp,sizeof(temp),fp))
    {
    
        
        if(strstr(temp,str))
        {
        printf("FOUND\n");
            printf("%s\n",temp);
            flag=1;
        }
        
         
    }
    if(!flag)
    printf("NOT FOUND\n");
        
	}

     }
        t=clock()-t;
	double time_taken= (((double)t)/CLOCKS_PER_SEC)* 1000000000;
	printf("\ntime taken %g ns to execute\n",time_taken);
     return;
       
    
}
