/*
 * Mitar <mitar@tnode.com>
 * http://mitar.tnode.com/
 * In kind public domain.
 *
 * $Id$
 * $Revision$
 * $HeadURL$
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <httpd.h>

#include "main.h"

char *program_name;
char *host = NULL;
int port = DEFAULT_PORT;
char *base = DEFAULT_BASE;
char *response_file = NULL;
char *index_file = DEFAULT_INDEX;
httpd *server;
request *req;

void _debug(char *filename, int line, int level, char *format, ...) {
	// Dummy, nodogsplash function
}

void null() {
	if (response_file != NULL) {
		char *suffix;
		struct stat sbuf;

		if (stat(response_file, &sbuf) == 0) {
			suffix = rindex(response_file, '.');
			if (suffix != NULL) {
				if (strcasecmp(suffix,".gif") == 0) {
					httpdSetContentType(req, "image/gif");
				}
				else if (strcasecmp(suffix,".jpg") == 0) {
					httpdSetContentType(req, "image/jpeg");
				}
				else if (strcasecmp(suffix,".png") == 0) {
					httpdSetContentType(req, "image/png");
				}
				else if (strcasecmp(suffix,".css") == 0) {
					httpdSetContentType(req, "text/css");
				}
				else if (strcasecmp(suffix,".js") == 0) {
					httpdSetContentType(req, "text/javascript");
				}
			}

			char length[BUFFER_LENGTH];
			snprintf(length, BUFFER_LENGTH, "Content-Length: %lld", sbuf.st_size);
			httpdAddHeader(req, length);
			httpdSendHeaders(req);
			_httpd_catFile(req, response_file);
			return;
		}
	}

	httpdAddHeader(req, "Content-Length: 0");
	httpdOutput(req, "");
}

void help() {
	fprintf(stderr, "%s [-h <host>] [-p <port>] [-b <base>] [-r <response>] [-i <index>]\n", program_name);
	fprintf(stderr, "  -h <host>      to which <host> to bind (default: all available)\n");
	fprintf(stderr, "  -p <port>      to which <port> to bind (default: %d)\n", DEFAULT_PORT);
	fprintf(stderr, "  -b <base>      <base> directory to use (default: %s)\n", DEFAULT_BASE);
	fprintf(stderr, "  -r <response>  send <response> file as a response (default: empty response)\n");
	fprintf(stderr, "  -i <index>     send <index> file as a directory response (default: %s)\n", DEFAULT_INDEX);
	fprintf(stderr, "\n");
	fprintf(stderr, "  <host>      a single IP address\n");
	fprintf(stderr, "  <port>      TCP port on which the server will listen (integer)\n");
	fprintf(stderr, "  <base>      directory for files\n");
	fprintf(stderr, "  <response>  file with a response content (system path)\n");
	fprintf(stderr, "  <index>     filename for a directory response content file\n");
}

int main(int argc, char *argv[]) {
	program_name = argv[0];

	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			i++;
			if ((i < argc) && (argv[i][0] != '\0')) {
				host = argv[i];
			}
			else {
				fprintf(stderr, "Missing parameter for -h argument.\n\n");
				help();
				return 1;
			}
		}
		else if (strcmp(argv[i], "-p") == 0) {
			i++;
			if ((i < argc) && (argv[i][0] != '\0')) {
				char *end;
				port = strtol(argv[i], &end, 10);
				if (*end != '\0') {
					fprintf(stderr, "Invalid parameter '%s' for -p argument.\n\n", argv[i]);
					help();
					return 1;
				}
			}
			else {
				fprintf(stderr, "Missing parameter for -p argument.\n\n");
				help();
				return 1;
			}
		}
		else if (strcmp(argv[i], "-b") == 0) {
			i++;
			if ((i < argc) && (argv[i][0] != '\0')) {
				base = argv[i];
			}
			else {
				fprintf(stderr, "Missing parameter for -b argument.\n\n");
				help();
				return 1;
			}
		}
		else if (strcmp(argv[i], "-r") == 0) {
			i++;
			if ((i < argc) && (argv[i][0] != '\0')) {
				response_file = argv[i];
			}
			else {
				fprintf(stderr, "Missing parameter for -r argument.\n\n");
				help();
				return 1;
			}
		}
		else if (strcmp(argv[i], "-i") == 0) {
			i++;
			if ((i < argc) && (argv[i][0] != '\0')) {
				index_file = argv[i];
			}
			else {
				fprintf(stderr, "Missing parameter for -i argument.\n\n");
				help();
				return 1;
			}
		}
		else {
			fprintf(stderr, "Invalid argument '%s'.\n\n", argv[i]);
			help();
			return 1;
		}
	}

	server = httpdCreate(host, port);
	if (server == NULL) {
		fprintf(stderr, "Could not create HTTP server on %s:%d: %s.\n", (host == NULL ? "*" : host), port, strerror(errno));
		return 2;
	}

	httpdSetFileBase(server, base);

	httpdAddWildcardContent(server, "/", NULL, "");
	httpdAddFileContent(server, "/", index_file, HTTP_TRUE, NULL, index_file);
	httpdAddC404Content(server, null);

	while ((req = httpdGetConnection(server, NULL)) != NULL) {
		if (httpdReadRequest(server, req) < 0) {
			fprintf(stderr, "Error reading request.\n");
		}
		else {
			httpdProcessRequest(server, req);
		}
		httpdEndRequest(req);
	}

	return 0;
}
