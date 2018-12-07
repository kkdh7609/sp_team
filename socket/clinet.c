#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAXLEN 512
#define PORT 55001
int main(int argc, char* argv[]){
  int sock;
  struct sockaddr_in serv_addr;

  char buf[MAXLEN];
  int nbytes;

  pid_t pid;

  if(argc != 2){
    printf("Usage : %s <IP>\n", argv[0]);
    exit(1);
  }

  sock = socket(PF_INET,SOCK_STREAM,0);
  if(sock == -1){
    perror("socket() error\n");
    exit(0);
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(PORT);

  if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
    perror("connect() error");
    exit(0);
  }

  if((pid=fork()) == -1){
    perror("fork() error\n");
    exit(0);
  }
  else if(pid == 0){
    while(1){
      fgets(buf, sizeof(buf), stdin);
      nbytes = strlen(buf);
      write(sock, buf, MAXLEN-1);
      if(strncmp(buf,"exit",4)==0){
        puts("exit program");
	exit(0);
      }
    }
  }
  else if(pid>0){
    while(1){
      if((nbytes = read(sock, buf, MAXLEN-1))<0){
        perror("read() error\n");
	exit(0);
      }
      printf("%s\n", buf);
      if(strncmp(buf,"exit",4)==0){
        puts("exit program");
	exit(0);
      }
  }
  }
  close(sock);
  return 0;
}
