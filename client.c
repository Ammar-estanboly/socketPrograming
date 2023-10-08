#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {

  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd < 0) {
    perror("socket failed");
    exit(1);
  }
  struct sockaddr_in server;
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET4;
  server.sin_port = 1235;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  int ret = connect(client_fd, (struct sockaddr *)&server, 0);
  if (ret < 0) {
    perror("connect failed");
    exit(2);
  }
  char buffer[1024];
  sprintf(buffer, "%d %d %s\n", 1, 7, "network");
  ret = Write(client_fd, buffer, strlen(buffer));
  if (ret < 0) {
    perror("write failed");
    exit(3);
  }
  ret = Read(client_fd, buffer, sizeof(buffer));
  if (ret < 0) {
    perror("read failed");
    exit(4);
  }
  buffer[ret] = 0;
  printf("read: %s\n", buffer);
  close(client_fd);
  return 0;
}
