#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void handle_cgi(char* path, char* query_string, char* method, FILE* request)
{
	char* post_data = "\n";
	char* content_length_str = get_header("Content-Length");
	if (content_length_str==NULL) content_length_str="0";
	int content_length = atoi(content_length_str);

	// Setter opp en pipe fil
	int fd[2];
	pipe(fd);

	if(strcmp(method, "POST") == 0)
	{
		// Leser body inn i post_data
		post_data = malloc(content_length);
		fread(post_data, content_length, 1, request);

		post_data[content_length+1] = 0;
	}
	else if(strcmp(method, "GET") != 0)
	{
		// Error if not get or post
		error(405);
		return;
	}

	char* argv[] = {path, 0};
	char* cookie = get_header("Cookie");

	// Konstruerer envp
	char* envp[5];
	envp[0] = malloc(strlen(query_string) + 14);
	envp[1] = malloc(strlen(method) + 16);
	envp[2] = malloc(strlen(content_length_str) + 16);
	envp[3] = malloc(strlen(cookie) + 8);
	envp[4] = malloc(1);


	sprintf(envp[0], "QUERY_STRING=%s", query_string);
	sprintf(envp[1], "REQUEST_METHOD=%s", method);
	sprintf(envp[2], "CONTENT_LENGTH=%d", content_length);
	sprintf(envp[3], "COOKIE=%s", cookie);
	envp[4] = (char*)0;

	// Send statuskode ok
	printf("HTTP/1.1 200 OK\n");
	fflush(stdout);

	// Forker og sender post_data
	if(fork() == 0)
	{
		close(0);
		dup2(fd[0], 0);
		execve(path, &argv[0], envp);
		dprintf(2, "%s. Path: %s\n", strerror(errno), path);
	}

	dprintf(fd[1], "%s", post_data);
	dprintf(2,"200 OK\n");
	exit(0);
}
