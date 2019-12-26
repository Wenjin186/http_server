#include "header/create_socket.h"


//Reference: I learned from helpful links at the end of Programming Assignment #2 to help me build the connection between server and client.
//http://www.binarytides.com/server-client-example-c-sockets-linux/


void sigchld_handler(int signo)
{
    pid_t pid;
    pid = waitpid(-1, NULL, WNOHANG);
    printf("Child process has been recycle\n");
}

int main(int argc , char *argv[])
{
	int socket_desc;
	if (argv[1] != NULL){
		socket_desc  = set_listener(atoi(argv[1]));
	}else{
		socket_desc = set_listener(8888);
	}
	int client_sock, c;
	struct sockaddr_in client;
	int parent = 1;
	int status;

	signal(SIGCHLD, sigchld_handler);

	while(1 && parent){	
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return -1;
	}

	pid_t pid;
	pid = fork();
	if (pid < 0){
		perror("fork error");
		exit(-1);
	}else if (pid == 0){
		parent = 0;
	}
//--------------------------------------------------------------	
	
	if (!parent){
		char header[BUFFSIZE] = {0};
		char hlist[HLISTSIZE][BUFFSIZE];
		char response[BUFFSIZE] = {0};
		memset(hlist, 0, HLISTSIZE*BUFFSIZE);
		char path[200] = "..";
		char req_path[200] = {0};
		char file_read_buf[BUFFSIZE] = {0};
		char file_ext[200] = {0};
		char type_name[200] = {0};


		recv(client_sock, header, BUFFSIZE, 0);
		request_header_operation(header, BUFFSIZE, hlist, HLISTSIZE, BUFFSIZE);
		
		printf("******************************************\n");
		printf("header = %s\n", header);

		//get file path name
		get_request_path(req_path, hlist[GET]);
		strcat(path, req_path);
		printf("file path = %s\n", path);

		//get file extension name
		get_file_ext(file_ext, path);
		printf("file extension name = %s\n", file_ext);

		//get typename
		int ret = get_type_name(type_name, file_ext);
		printf("file type name = %s\n", type_name);

		FILE *fp = fopen(path, "r");
		if (fp==NULL||ret==-1){
			build_response(response, BUFFSIZE, FAIL, "", 0, 0);
		}else{
			long fsize = get_file_size(fp);
			build_response(response, BUFFSIZE, SUCCESS, type_name, 0, fsize);
			send(client_sock, response, strlen(response), 0);
			int ret = 0;
			do{
				ret = fread(file_read_buf, 1, BUFFSIZE, fp);
				send(client_sock, file_read_buf, ret, 0);
			}while(ret > 0);
			fclose(fp);
		}

		close(client_sock);
		close(socket_desc);
		printf("The child process's client sock has been closed\n");
		printf("******************************************\n");
		exit(0);
	}
	
	close(client_sock);		

	}
	close(socket_desc);
	return 0;
}
