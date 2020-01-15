#include <string.h>
#include <stdio.h>

#define MIMEFILE "./etc/mime.types"

char* get_mime(char* path)
{
	// get file name
	const char* file = strrchr(path, '/') + 1;

	// get file extension
	const char* dot = strrchr(file, '.');
	if(!dot || dot == file) dot = " ";
	dot++;

	// open mime file
	FILE* mimes = fopen(MIMEFILE, "r");
	char* line = NULL;
	size_t len = 0;

	// read mime file
	while(getline(&line, &len, mimes) != -1)
	{
		char tokens [64];
		strcpy(tokens, strrchr(line, '\t') + 1);
		char* token;
		char* saveptr;
		for(token = strtok_r(tokens, " \n", &saveptr); token != NULL; token = strtok_r(NULL, " \n", &saveptr))
		{
			if(strcmp(token, dot) == 0)
			{
				char* mimetype = strtok(line, "\t");
				fclose(mimes);
				return mimetype;
			}
		}
	}
	fclose(mimes);
	return NULL;
}
