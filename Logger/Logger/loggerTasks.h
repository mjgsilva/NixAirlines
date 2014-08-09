int createPipe();
void outputs(pInbound inAux, char *filename);
int writetxtFile(char *line, char *filename);
int writePipe(pOutbound out);
int alertServerUp();
void shutdownLog();
void writeTemp(char *filename);
void writeLastLine(char *filename);