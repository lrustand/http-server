void error(int errorcode){
	char* errormessage;
	char* headers = NULL;
	switch(errorcode)
	{
		case 400:
			errormessage = "Bad Request";
			break;

		case 403:
			errormessage = "Forbidden";
			break;

		case 404:
			errormessage = "Not Found";
			break;

		case 405:
			errormessage = "Method Not Allowed";
			headers = "Allow: GET, POST\n";
			break;

		case 415:
			errormessage = "Unsupported Media Type";
			break;
	}
	printf("HTTP/1.1 %d %s\n", errorcode, errormessage);
	printf("Content-Type: text/html\n");

	if(headers != NULL)
		printf(headers);

	printf("\n");
	printf("<h1>%d %s</h1>", errorcode, errormessage);
}
