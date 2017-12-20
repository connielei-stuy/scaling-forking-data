#include "pipe_networking.h"
#include <signal.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("wkp");
    exit(0);
  }
}

int main() {
  while(1){

    signal(SIGINT,sighandler);
    int from_client = server_setup();

    if(from_client){

      int f = fork();
      if(!f){
	subserver(from_client);
      }
    }
  }
}

void subserver(int from_client) {

  int to_client = server_connect(from_client);

  char buffer[BUFFER_SIZE];

  while(read(from_client, buffer, sizeof(buffer))){

    process(buffer);
    printf("[server] %s\n", buffer);
    write(to_client, buffer, sizeof(buffer));
    
  }

  exit(0);
  
}

void process(char * s) {
  //rot 13

  int i = 0;

  for(; s[i] != "\0"; i++){
    if ( * (s + i) >= "a" && * (s + i) < "n"){
      *( s + i ) += 13;
    }
    else if( * ( s + i ) >= "n" && * ( s + i ) <= "z"){
      * (s + i) -= 13;
    }
  }
}
