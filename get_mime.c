#include <string.h>
#include <stdio.h>


char* get_mime(char* path, FILE* mimefile)
{
	rewind(mimefile);
	// get file name
	const char* file = strrchr(path, '/') + 1;

	// get file extension
	const char* dot = strrchr(file, '.');
	if(!dot || dot == file) dot = " ";
	dot++;

	// open mime file
	char* line = NULL;
	size_t len = 0;

	// read mime file
	while(getline(&line, &len, mimefile) != -1)
	{
		if((strstr(line, "#") == NULL) && (strcmp(line, "\n") != 0)
			&& (strstr(line, "\t") != NULL))
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
					return mimetype;
				}
			}
		}
	}
	return NULL;
}
