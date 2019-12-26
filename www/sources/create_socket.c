#include "header/create_socket.h"

void get_request_path(char *buf, const char *req){
	
	unsigned long i = 0;
	for(i = 0; i<strlen(req); i++){
		if (req[i] !=' ') strncpy(buf+i, req+i, 1);
		else break;
	}
	if (strcmp(buf, "/") == 0){
		strcpy(buf, "/index.html");
		return;
	}
}

long get_file_size(FILE *fp){
	fseek(fp, 0L, SEEK_END);
	long result  = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return result;
}

void get_file_ext(char *buf, const char *path){
	int len = strlen(path);
	int i = 0;
	int j = 0;
	char temp[200] = {0};

	for(i = len - 1; i>=0; i--){
		if (path[i] != '.') temp[j] = path[i];
		else break;
		j++;
	}
	j = 0;
	for(i = strlen(temp)-1 ; i>=0; i--){
		buf[j] = temp[i];	
		j++;
	}
}

int get_type_name(char *buf, char *file_ext){
	if (strcmp(file_ext, "jpg") == 0){
		strcpy(buf, "image/jpg");
	}else if (strcmp(file_ext, "jpeg") == 0){
		strcpy(buf, "image/jpg");
	}else if (strcmp(file_ext, "html") == 0){
		strcpy(buf, "text/html");
	}else if (strcmp(file_ext, "txt") == 0){
		strcpy(buf, "text/plain");
	}else if (strcmp(file_ext, "png") == 0){
		strcpy(buf, "image/png");
	}else if (strcmp(file_ext, "gif") == 0){
		strcpy(buf, "image/gif");
	}else if (strcmp(file_ext, "css") == 0){
		strcpy(buf, "text/css");
	}else if (strcmp(file_ext, "js") == 0){
		strcpy(buf, "application/javascript");
	}
	else return -1;
	return 0;
}

int set_listener(int port){
	
	int socket_desc;
	struct sockaddr_in server;

	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( port );	
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("bind failed. Error");
		return -1;
	}
	listen(socket_desc , 15);

	return socket_desc;
}

int get_active_socket(int socket_desc){
	int client_sock, c;
	struct sockaddr_in client;

	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return -1;
	}
	
	return client_sock;
}

int close_server_socket(int socket_desc){
	close(socket_desc);
	return 0;
}

int request_header_operation(char *header, int size, char (*hlist)[BUFFSIZE], int list_acc, int list_length){
	
	char buffer[BUFFSIZE] = {0};

	int i;
	int pos = -1;
	int j = 0;
	int copy = 0;
	for (i = 0; i < size; i++){
		if (header[i] == ' ' && copy == 0){
			if (strcmp(buffer, "GET") == 0){
				pos = GET;
			}else if (strcmp(buffer, "Host:")==0){
				pos = HOST;
			}else{
				pos = -1;
			}
			memset(buffer, 0, BUFFSIZE);
			j = 0;
			copy = 1;
			continue;

		}

		if (header[i] != '\n'){
			buffer[j] = header[i];
			j ++; 
		}
		else {
			if (pos >= 0) {
				strcpy((char *)hlist+pos*list_length, buffer);
			}
			copy = 0;
			memset(buffer, 0, BUFFSIZE);
			j = 0;
		}
			
	}	

	return 0;
}

int build_response(char *buf, int bsize, int error, const char *type, int csize, int fsize){
	char ch_csize[20] = {0};
	char ch_fsize[20] = {0};
	char ch_error[20];
	
	if (error == 1){
		strcpy(ch_error, "200 OK");
	}else{
		strcpy(ch_error, "404 FAIL");
	}

	sprintf(ch_csize, "%d", csize);
	sprintf(ch_fsize, "%d", fsize);

	char response[BUFFSIZE] = {0};
	strcpy(response, "HTTP/1.1 ");
	strcat(response, ch_error);
	strcat(response, "\r\n");
	strcat(response, "Content-Type: ");
	strcat(response, type);
	strcat(response, "\r\n");
	strcat(response, "Content-Length: ");
	strcat(response, ch_fsize);
	strcat(response, "\r\n\r\n");

	strcpy(buf, response);

	return 0;
}
