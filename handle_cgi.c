#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void handle_cgi(char* path, char* query_string, char* method, FILE* request)
{
	char* post_data = "\n";
	int content_length = 0;

	// Setter opp en pipe fil
	int fd[2];
	pipe(fd);

	if(strcmp(method, "POST") == 0)
	{
		char* txt = NULL;
		size_t len;
		while(getline(&txt, &len, request) > 1); // Looper til newline eller EOF

		// Finner content_length
		long header_length = ftell(request);
		fseek(request, 0L, SEEK_END);
		long request_length = ftell(request);
		content_length = (int)request_length - (int)header_length;
		fseek(request, header_length, SEEK_SET);

		// Leser body inn i post_data
		post_data = malloc(content_length);
		while(getline(&txt, &len, request) != -1)
		{
			strcat(post_data, txt);
		}
	}
	else if(strcmp(method, "GET") != 0)
	{
		// Error if not get or post
		error(405);
		return;
	}

	char* argv[] = {path, 0};

	// Konstruerer envp
	char* envp[4];
	envp[0] = malloc(strlen(query_string) + 14);
	envp[1] = malloc(strlen(method) + 16);
	envp[2] = malloc(32);
	envp[3] = malloc(1);


	sprintf(envp[0], "QUERY_STRING=%s", query_string);
	sprintf(envp[1], "REQUEST_METHOD=%s", method);
	sprintf(envp[2], "CONTENT_LENGTH=%d", content_length);
	envp[3] = (char*)0;

	// Send statuskode ok
	printf("HTTP/1.1 200 OK\n");

	// Forker og sender post_data
	if(fork() == 0)
	{
		close(0);
		dup2(fd[0], 0);
		execve(path, &argv[0], envp);
		dprintf(2, "%s. Path: %s\n", strerror(errno), path);
	}

	dprintf(fd[1], post_data);
	dprintf(2,"200 OK\n");
	exit(0);
}
