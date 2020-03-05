struct header {
	char name[100];
	char value[100];
};


struct header headers[20];

// Funksjon for å lese inn headers fra requesten. Kjøres en gang per req
void read_headers(FILE* request)
{
	char* line;
	int i = 0;
	size_t len;

	// Tell antall linjer
	while(getline(&line, &len, request) > 2){
		char* sep = strchr(line, ':');
		sep[0] = 0;
		sep++;
		while(sep[0]==' '){sep++;}
		strcpy(headers[i].name, line);
		strcpy(headers[i].value, sep);
		i++;
	}; // Looper til newline eller EOF
}

// Funksjon for å hente verdien av en spesifikk header
char* get_header(char* header)
{
	size_t n = sizeof(headers)/sizeof(headers[0]);
	for (int i=0; i<n; i++)
	{
		if (headers[i].name[0]==0){
			break;
		}
		if (strcmp(headers[i].name,header)==0){
			return headers[i].value;
		}
	}
	return "";
}
