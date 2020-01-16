#include <stdio.h>
#include <string.h>

int validate_request(char* request)
{
	// Return 400 if request doesn't start with "GET "
	if(strncmp(request, "GET ", 4))
	{
		return 400;
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

	// Retrun 200 if there's exactly 2 spaces
	if(count == 2)
	{
		return 200;
	}
	else
	{
		return 400;
	}
}
