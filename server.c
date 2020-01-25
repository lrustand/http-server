#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "print_file.c"
#include "get_mime.c"
#include "directory_listing.c"
#include "error.c"
#include "validate_request.c"
#include "handle_cgi.c"
#include "request_type.c"

#define WEBROOT "./www"
#define MIMEFILE "/etc/mime.types"
#define LOGFILE "/var/log/httpd.log"
#define GID 65534
#define UID 65534

#define LOKAL_PORT 80
#define BAK_LOGG 10 // Størrelse på for kø ventende forespørsler

int main ()
{

	struct sockaddr_in	lok_adr;
	int sd, ny_sd;

	// Åpner mimefil
	FILE* mimefile = fopen(MIMEFILE, "r");

	// Binder stderr til loggfila
	close(2);
	open(LOGFILE, O_WRONLY | O_APPEND | O_CREAT);

	// Chroot til webroten
	chdir(WEBROOT);
	chroot(".");

	// Forker og lukker foreldreprosessen
	if (fork()!=0){
	        exit(0);
	}

	// Setter til sesjonsleder
	setsid();

	// Ignorerer SIGHUP
	signal(SIGHUP, SIG_IGN);

	// Forker og lukker foreldreprosessen
	if (fork()!=0){
	        exit(0);
	}

	// Stenger stdout
	close(1);


	// Setter opp socket-strukturen
	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// For at operativsystemet ikke skal holde porten reservert etter tjenerens død
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));

	// Initierer lokal adresse
	lok_adr.sin_family			= AF_INET;
	lok_adr.sin_port				= htons((u_short)LOKAL_PORT);
	lok_adr.sin_addr.s_addr = htonl(				 INADDR_ANY);

	// Kobler sammen socket og lokal adresse
	if ( 0==bind(sd, (struct sockaddr *)&lok_adr, sizeof(lok_adr)) )
		dprintf(2, "Prosess %d er knyttet til port %d.\n", getpid(), LOKAL_PORT);
	else {
		dprintf(2, "Kunne ikke binde port\n");
		exit(1);
	}

	// Endrer bruker og gruppe til nobody
	setgid(GID);
	setuid(UID);

	// Venter på forespørsel om forbindelse
	listen(sd, BAK_LOGG);
	while(1){

		// Aksepterer mottatt forespørsel og tar vare på ip-adressen til klienten
		struct sockaddr_in client_addr;
		socklen_t slen = sizeof(client_addr);
		ny_sd = accept(sd, (struct sockaddr *)&client_addr, &slen);


		if(0==fork()) {

			dup2(ny_sd, 1); // redirigerer socket til standard utgang

			// åpner socket og leser første linje inn i variabel
			char* line = NULL;
			FILE* request = fdopen(ny_sd, "r");
			size_t len;
			getline(&line, &len, request);
			fclose(request);

			// Remove escape characters \r and \n
			while(strchr(line, '\r') != NULL)
			{
				strchr(line, '\r')[0] = '\0';
			}

			while(strchr(line, '\n') != NULL)
			{
				strchr(line, '\n')[0] = '\0';
			}

			// henter url fra linjen
			char* saveptr = NULL;
			char* line_copy = malloc(strlen(line));
			strcpy(line_copy, line);
			strtok_r(line_copy, " ", &saveptr);
			char* url = malloc(strlen(line));
			strcpy(url, strtok_r(NULL, " ", &saveptr));

			// Logger requesten til konsollen
			dprintf(2, "%s - %s - ", inet_ntoa(client_addr.sin_addr), line);

			// Finner path fra url ved å terminere på ? og #
			char* path = malloc(strlen(url));
			strcpy(path, url);
			char* query_ptr = strchr(path, '?');
			if(query_ptr != NULL)
			{
				query_ptr[0] = '\0';
			}

			query_ptr = strchr(path, '#');
			if(query_ptr != NULL)
			{
				query_ptr[0] = '\0';
			}

			if (validate_request(line))
			{
				char* req_type = request_type(line);

				if (strcmp(req_type, "GET") == 0){
					// Sjekker om url er /
					if (strcmp(url,"/")==0){
						printf("HTTP/1.1 200 OK\n");
						printf("Content-Type: text/plain\n");
						printf("\n");
						directory_listing(url);
					}

					// Hvis fila eksisterer, send den
					else if (access( path, F_OK ) != -1){
						char* mime = get_mime(url, mimefile);

						// Hvis filtypen ikke gjenkjennes, gi feilmelding
						if(strncmp(url, "/cgi-bin/", 9) == 0)
						{
							handle_cgi(url, req_type, request);
						}
						else if (mime==NULL){
							error(415);
						}
						else {
							printf("HTTP/1.1 200 OK\n");
							printf("Content-Type: %s\n", mime);
							printf("\n");
							fflush(stdout); // nødvendig for å få riktig rekkefølge
							print_file(url);
						}
					}
					// Hvis ikke, send 404
					else {
						error(404);
					}
				}

				else if (strcmp(req_type, "POST") == 0){
					if(strncmp(url, "/cgi-bin/", 9))
					{
						handle_cgi(url, req_type, request);
					}
					else
					{
						error(405);
					}
				}
			}

			dprintf(2, "200 OK\n");
			fflush(stdout);

			// Sørger for å stenge socket for skriving og lesing
			// NB! Frigjør ingen plass i fildeskriptortabellen
			shutdown(ny_sd, SHUT_RDWR);
			exit(0);
		}

		else {
			close(ny_sd);
		}
	}
	return 0;
}
