#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void directory_listing(char *path){

	struct stat     stat_buffer;
	struct dirent  *ent;
	DIR            *dir;

	dir = opendir (path);
	if (dir == NULL) {
		perror (""); exit(1); }

	printf ("\nKatalogen %s:\n", path);
	printf ("------------------------------------\n");
	printf ("Rettigheter\tUID\tGID\tNavn\n");
	printf ("------------------------------------\n");

	while ((ent = readdir (dir)) != NULL) {

		stat (ent->d_name, &stat_buffer);

		if((strcmp(ent->d_name, ".") != 0)
			&& (strcmp(ent->d_name, "..") != 0))
		{
			printf ("%o\t\t", stat_buffer.st_mode & 0777 );	// Retigheter
			printf ("%d\t",	 stat_buffer.st_uid);			// Eier bruker (id)
			printf ("%d\t",	 stat_buffer.st_gid);			// Eier gruppe (id)
			printf ("%s\n",	 ent->d_name);					// filnavn
		}
	}

	closedir (dir);
}

