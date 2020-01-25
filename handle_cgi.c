#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void handle_cgi(char* path, char* method, FILE* request)
{
	char post_data[1024] = "";
	int content_length;

	// Initierer query_string med innhold bak ? i path, og separerer path og query_string med \0
	char* query_string = strchr(path, '?');
	if(query_string == NULL)
	{
		query_string = "";
	}
	else
	{
		query_string++;
		strchr(path, '?')[0] = '\0';
	}

	// Setter opp en pipe fil
	int fd[2];
	pipe(fd);

	if(strcmp(method, "POST") == 0)
	{
		char* txt = NULL;
		size_t len;
		while(getline(&txt, &len, request) > 1); // Looper til newline eller EOF

		// Leser body inn i post_data
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


	content_length = (int) strlen(post_data);
	sprintf(envp[0], "QUERY_STRING=%s", query_string);
	sprintf(envp[1], "REQUEST_METHOD=%s", method);
	sprintf(envp[2], "CONTENT_LENGTH=%d", content_length);
	envp[3] = "\0";

	// Forker og sender post_data
	if(fork() == 0)
	{
		close(0);
		dup2(fd[0], 0);
		execve(path, &argv[0], &envp[0]);
		dprintf(2, "%s. Path: %s\n", strerror(errno), path);
	}

	dprintf(fd[1], post_data);
	return;
}
