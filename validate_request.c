#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool validate_request(char* request)
{
	// Return false if request doesn't start with "GET "
	if(strncmp(request, "GET ", 4))
	{
		return false;
	}

	// Return false if request doesn't end with "HTTP_._"
	if(!(strncmp(&request[strlen(request) - 11], " HTTP/1.0", 9) == 0
		|| strncmp(&request[strlen(request) - 11], " HTTP/1.1", 9) == 0
		|| strncmp(&request[strlen(request) - 11], " HTTP/2.0", 9) == 0))
	{
		return false;
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

	// Retrun true if there's exactly 2 spaces
	if(count == 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}
