/*
 * Mitar <mitar@tnode.com>
 * http://mitar.tnode.com/
 * In kind public domain.
 *
 * $Id$
 * $Revision$
 * $HeadURL$
 */

#ifndef MAIN_H_
#define MAIN_H_

#define VERSION "0.1"
#define DEFAULT_PORT 2051
#define DEFAULT_BASE "./"
#define DEFAULT_INDEX "index.html"
#define DEFAULT_PID "/var/run/nullhttpd.pid"
#define BUFFER_LENGTH 100

// LibHTTPD internal function
void _httpd_catFile(request *r, char *path);

// Dummy, nodogsplash function
void _debug(char *filename, int line, int level, char *format, ...);

void null();
void help();

#endif /* MAIN_H_ */
