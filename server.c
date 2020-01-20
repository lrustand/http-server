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
#include "validate_request.c"

#define WEBROOT "./www"
#define MIMEFILE "./etc/mime.types"
#define LOGFILE "/var/log/httpd.log"
#define GID 65534
#define UID 65534

#define LOKAL_PORT 80
#define BAK_LOGG 10 // Størrelse på for kø ventende forespørsler

int main ()
{

	struct sockaddr_in	lok_adr;
	int sd, ny_sd;

	int logfile = open(LOGFILE, O_WRONLY | O_APPEND | O_CREAT);
	FILE* mimefile = fopen(MIMEFILE, "r");

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

	// Stenger stdout og stderr
	close(1);
	close(2);


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
		dprintf(logfile, "Prosess %d er knyttet til port %d.\n", getpid(), LOKAL_PORT);
	else {
		dprintf(logfile, "Kunne ikke binde port\n");
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
			char* txt = NULL;
			FILE* request = fdopen(ny_sd, "r");
			size_t len;
			getline(&txt, &len, request);
			fclose(request);

			// Remove escape characters \r and \n
			while(strchr(txt, '\r') != NULL)
			{
				strchr(txt, '\r')[0] = '\0';
			}

			while(strchr(txt, '\n') != NULL)
			{
				strchr(txt, '\n')[0] = '\0';
			}


			switch(validate_request(txt))
			{
				case 200:
				{
					// henter path fra linjen
					char* saveptr = NULL;
					strtok_r(txt, " ", &saveptr);
					char path[256];
					strcpy(path, strtok_r(NULL, " ", &saveptr));

					// Logger requesten til konsollen
					dprintf(logfile, "%s forespør %s\n", inet_ntoa(client_addr.sin_addr), path);

					// Sjekker om path er /
					if (strcmp(path,"/")==0){
						printf("HTTP/1.1 200 OK\n");
						printf("Content-Type: text/plain\n");
						printf("\n");
						directory_listing(path);
					}
					// Hvis fila eksisterer, send den
					else if (access( path, F_OK ) != -1){
						char* mime = get_mime(path, mimefile);

						// Hvis filtypen ikke gjenkjennes, gi feilmelding
						if (mime==NULL){
							printf("HTTP/1.1 415 Unsupported Media Type\n");
							printf("Content-Type: text/html\n");
							printf("\n");
							printf("<h1>415 Unsupported Media Type</h1>");
						}
						else {
							printf("HTTP/1.1 200 OK\n");
							printf("Content-Type: %s\n", mime);
							printf("\n");
							fflush(stdout); // nødvendig for å få riktig rekkefølge
							print_file(path);
						}
					}
					// Hvis ikke, send 404
					else {
						printf("HTTP/1.1 404 Not Found\n");
						printf("Content-Type: text/html\n");
						printf("\n");
						printf("<h1>404 File not found</h1>");
					}
					break;
				}
				case 400:
					printf("HTTP/1.1 400 Bad Request\n");
					printf("Content-Type: text/html\n");
					printf("\n");
					printf("<h1>400 Bad Request</h1>");
					break;

				case 403:
					printf("HTTP/1.1 403 Forbidden\n");
					printf("Content-Type: text/html\n");
					printf("\n");
					printf("<h1>403 Forbidden</h1>");
					break;

				default:
					printf("HTTP/1.1 400 Bad Request\n");
					printf("Content-Type: text/html\n");
					printf("\n");
					printf("<h1>400 Bad Request</h1>");
					break;
			}

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
