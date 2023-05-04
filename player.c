#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){

printf("I am player #%s here my pid: %d\n", argv[1], getpid());

return 0;
}