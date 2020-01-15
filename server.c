#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "print_file.c"
#include "get_mime.c"
#include "directory_listing.c"
#include "validate_request.c"

#define LOKAL_PORT 80
#define BAK_LOGG 10 // Størrelse på for kø ventende forespørsler

int main ()
{

	struct sockaddr_in	lok_adr;
	int sd, ny_sd;

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
		fprintf(stderr, "Prosess %d er knyttet til port %d.\n", getpid(), LOKAL_PORT);
	else {
		dprintf(2,"Kunne ikke binde port\n");
		exit(1);
	}
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

			if(validate_request(txt))
			{
				// henter path fra linjen
				char* saveptr = NULL;
				strtok_r(txt, " ", &saveptr);
				char path[256];
				strcpy(path, strtok_r(NULL, " ", &saveptr));

				char absolute_path[512] = "";
				strcat(absolute_path, "/home/da-nan/prosjekt/http-server/www");
				strcat(absolute_path, path);
				// Logger requesten til konsollen
				dprintf(2,"%s forespør %s\n", inet_ntoa(client_addr.sin_addr), path);

				// Sjekker om path er /
				if (strcmp(path,"/")==0){
					printf("HTTP/1.1 200 OK\n");
					printf("Content-Type: text/plain\n");
					printf("\n");
					directory_listing(absolute_path);
				}
				// Hvis fila eksisterer, send den
				else if (access( absolute_path, F_OK ) != -1){
					char* mime = get_mime(absolute_path);

					// Hvis filtypen ikke gjenkjennes, gi feilmelding
					if (mime==NULL){
						printf("HTTP/1.1 415 Unsupported Media Type");
						printf("\n");
						printf("<h1>Unsupported Media Type</h1>");
					}
					else {
						printf("HTTP/1.1 200 OK\n");
						printf("Content-Type: %s\n", mime);
						printf("\n");
						fflush(stdout); // nødvendig for å få riktig rekkefølge
						print_file(absolute_path);
					}
				}
				// Hvis ikke, send 404
				else {
					printf("HTTP/1.1 404 Not Found\n");
					printf("Content-Type: text/html\n");
					printf("\n");
					printf("<h1>404 File not found</h1>");
				}
			}
			else
			{
				printf("HTTP/1.1 400 Bad Request\n");
				printf("Content-Type: text/html\n");
				printf("\n");
				printf("<h1>400 Bad Request</h1>");
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
