#include <stdio.h>
#include <stdlib.h>
void DieWithUserMessage(const char *msg, const char *detail) {
puts("error:");
puts(msg);
//puts(stderr);
//puts(detail, stderr);
//putc('\n', stderr);
exit(1);
}
void DieWithSystemMessage(const char *msg) {
perror(msg);
exit(1);
}
