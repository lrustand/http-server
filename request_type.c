#include <string.h>

char* request_type(char* line){
	char* req_type;

	// Kopier det første ordet fra linjen
	int ind = strchr(line, ' ') - line;
	req_type = malloc(ind);
	strncpy(req_type, line, ind);

	// Nullterminere stringen
	req_type[ind] = 0;

	return req_type;
}
