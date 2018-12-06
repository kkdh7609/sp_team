#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define PORT 55001
#define MAXLEN 512
int main(int argc, char *argv[]){
  int serv_sock;
  int clnt_sock;

  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_sz;

  int addrlen, datalen;

  char buf[MAXLEN];
  int nbytes;

  pid_t pid;

  serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(serv_sock == -1){
    perror("socket() error!\n");
    exit(0);
  }

  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORT);

  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
    perror("bind() error\n");
    exit(0);
  }

  if(listen(serv_sock, 3) == -1){
    perror("listen() error\n");
    exit(0);
  }

  clnt_addr_sz = sizeof(clnt_addr);
  clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
  if(clnt_sock == -1){
    perror("accept() error\n");
    exit(0);
  }

  if((pid=fork())==-1){
    close(clnt_sock);
    perror("fork() error\n");
    exit(0);
  }
  else if(pid==0){
    while(1){
      fgets(buf,sizeof(buf),stdin);
      nbytes = strlen(buf);
      write(clnt_sock, buf, MAXLEN -1);
      if(strncmp(buf, "exit", 4) == 0){
        puts("exit program");
	exit(0);
      }
    }
  }
  else if(pid>0){
    while(1){
      if((nbytes == read(clnt_sock,buf,MAXLEN-1)) < 0){
        perror("read() error\n");
	exit(0);
      }
      printf("%s\n", buf);
      if(strncmp(buf, "exit", 4) == 0){
        puts("exit program");
	exit(0);
      }
    }
  }

  close(clnt_sock);
  close(serv_sock);
  return 0;
}
