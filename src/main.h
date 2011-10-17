#ifndef MAIN_H_
#define MAIN_H_

#define VERSION "0.1"
#define DEFAULT_PORT 2051
#define DEFAULT_BASE "./"
#define BUFFER_LENGTH 100

// LibHTTPD internal function
void _httpd_catFile(request *r, char *path);

// Dummy, nodogsplash function
void _debug(char *filename, int line, int level, char *format, ...);

void null();
void help();

#endif /* MAIN_H_ */
