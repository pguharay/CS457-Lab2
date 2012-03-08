#ifndef UTIL_H_
#define UTIL_H_

#define SUCCESS 0
#define FAILURE -1

#define error(msg) fprintf(stderr,"%s",msg)
#define info(msg) fprintf(stderr,"%s",msg)
#define debug(msg, ...) fprintf(stdout, msg, __VA_ARGS__)


void validateStartArguments(int argc, char** argv);
void displayErrorAndExit(const char *message);

#endif
