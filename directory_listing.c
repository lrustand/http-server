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
		if((strcmp(ent->d_name, ".") != 0)
			&& (strcmp(ent->d_name, "..") != 0))
		{
			printf ("%o\t\t", stat_buffer.st_mode & 0777 );
			printf ("%d\t",	 stat_buffer.st_uid);
			printf ("%d\t",	 stat_buffer.st_gid);
			printf ("%s\n",	 ent->d_name);
		}
	}

	closedir (dir);
}

