#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define GET 0
#define BUFFSIZE 1024
#define HOST 1
#define HLISTSIZE 3


#define SUCCESS 1
#define FAIL 0

int set_listener();
int set_listener(int port);
int get_active_socket(int socket_desc);
int close_server_socket(int socket_desc);
int request_header_operation(char *buffer, int size, char (*hlist)[1024], int list_acc, int list_length);
int build_response(char *buf, int bsize, int error, const char *type, int csize, int fsize);
void get_request_path(char *buf, const char *req);
long get_file_size(FILE *fp);
void get_file_ext(char *buf, const char *path);
int get_type_name(char *buf, char *file_ext);
