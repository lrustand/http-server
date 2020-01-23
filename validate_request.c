#include <stdio.h>
#include <string.h>

int validate_request(char* request)
{
	int type;
	// Check if request starts with GET or POST
	if(strncmp(request, "GET ", 4) == 0)
	{
		type = 1;
	}
	else if(strncmp(request, "POST ", 5) == 0)
	{
		type = 2;
	}
	else
	{
		return 405;
	}

	// Return 400 if request doesn't end with "HTTP_._"
	if(!(strcmp(&request[strlen(request) - 9], " HTTP/1.0") == 0
		|| strcmp(&request[strlen(request) - 9], " HTTP/1.1") == 0
		|| strcmp(&request[strlen(request) - 9], " HTTP/2.0") == 0))
	{
		return 400;
	}

	// check for ..
	if(strstr(request, "..") != NULL)
	{
		return 403;
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

	// Return 1 or 2 if there's exactly 2 spaces
	if(count == 2)
	{
		return type;
	}
	else
	{
		return 400;
	}
}
