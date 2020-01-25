#include <stdio.h>
#include <string.h>

void validate_request(char* request)
{
	// Check if request starts with GET or POST
	if(strncmp(request, "GET ", 4) != 0
		&& strncmp(request, "POST ", 5) != 0)
	{
		error(405);
	}

	// Return 400 if request doesn't end with "HTTP_._"
	if(!(strcmp(&request[strlen(request) - 9], " HTTP/1.0") == 0
		|| strcmp(&request[strlen(request) - 9], " HTTP/1.1") == 0
		|| strcmp(&request[strlen(request) - 9], " HTTP/2.0") == 0))
	{
		error(400);
	}

	// check for ..
	if(strstr(request, "..") != NULL)
	{
		error(403);
	}

	// count spaces
	int count = 0;
	for(int i = 0; i < strlen(request); i++)
	{
		if(request[i] == ' ')
		{
			count++;
		}
	}

	// Check if there are 2 spaces
	if(count != 2)
	{
		error(400);
	}
}
