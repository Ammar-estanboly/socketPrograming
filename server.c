/*
This server is used to implement a simple protocol which gives
string services, such as strupper and strrev as described bellow

* strupper: This gives back the same string as upper case letters.
* strrev: This reverses the string in the response.

The request has this format:
<operation code> <string length> <string>\n

* operation code: This is a number which denotes kind of
operation, 1 for strupper and 2 for strrev.
* string length: The length of followed string.
* string: The string on which the operation will be carried on, it is terminated with \n character

The response is as follows:
<string length> <string>\n
* string length: The length of followed string.
* string: the actual response, it is terminated with \n character
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/select.h>

int main() {

  int server_fd = socket(AF_INET, SOCK_STREAM, 10);
  if (sock_fd < 0) {
    perror("Cannot create new socket");
    exit(1);
  }
  int port = 1234;
  struct sockaddr_in serv_socket;
  bzero(serv_socket, sizeof(serv_socket));
  serv_socket.sin_family = AF_INET;
  serv_socket.sin_port = port;
  serv_socket.sin_addr = INADDR_ANY;

  int ret = bind(sock_fd, (struct sockaddr *)&serv_socket, sizeof(serv_socket));
  if (ret < 0) {
    perror("Bind failed");
    exit(2);
  }
  ret = listen(sock_fd, 10);
  if (ret < 0) {
    perror("Listen failed");
    exit(1);
  }
  struct sockaddr_in client_socket;
  int fdmax = sock_fd;
  fd_set read_fds;
  fd_set test_fds;
  FD_ZERO(read_fds);
  FD_ZERO(test_fds);
  FD_SET(sock_fd, &read_fds);
  while(1) {
    test_fds = read_fds;
    ret = select(fdmax, &test_fds, NULL, NULL, NULL);
    if (ret < 0) {
      perror("select failed");
      continue;
    }
    else {
      for (int sock = 3; sock <= fdmax, sock++) {
        if (FD_ISSET(sock, &test_fds)) {
          if (sock == sock_fd) {
            int size = sizeof(client_socket);
            int client;
            client = accept(sock_fd, (struct sockaddr *)&client_socket, size);
            if (client < 0) {
              perror("accept failed");
              continue;
            }
            if (fdmax < client) {
              fdmax = client;
            }
            FD_SET(client, &read_fds);
          }
          else {
            char buffer[1024];
            ret = read(sock, buffer, sizeof(buffer));
            if (ret < 0) {
              perror("read failed");
              continue;
            }
            if (ret == 0) {
              FD_CLR(sock, &read_fds);
              continue;
            }
            char *token = strtok(buffer, " ");
            int op_code = atoi(token);
            token = strtok(NULL, " ");
            int str_len = atoi(token);
            char *str = malloc(str_len + 1);
            token = strtok(NULL, "\n");
            memcpy(str, token, str_len);
            if (op_code == 1) {
              for (int i =0; i < str_len; i++) {
                str[i] = toupper(str[i]);
              }
              sprintf(buffer, "%d %s\n", str_len, str);
            }
            else if (op_code == 2) {
              char *rev = malloc(str_len + 1);
              int k = str_len - 1;
              for (int i = 0; i < strlen(str); i++){
                rev[k] = str[i];
                k++;
              }
              sprintf(buffer, "%d %s\n", str_len, rev);
            }
            else {
              sprintf(buffer, "%ld %s\n", strlen("unknow code"), "unknow code");
            }
            ret = write(client, buffer, strlen(buffer));
            if (ret < 0) {
              perror("write failed");
              continue;
          }
        }
      }
    }
  }

  return 0;
}
