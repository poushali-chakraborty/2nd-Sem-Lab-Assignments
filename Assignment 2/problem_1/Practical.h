void DieWithError(char *errorMessage); /* Error handling function */
void HandleTCPClient(int clntSocket);
void DieWithUserMessage(const char *msg, const char *detail);
void DieWithSystemMessage(const char *msg);
unsigned long ResolveName(char name[]);

