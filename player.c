#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "local.h"

int main(int argc, char *argv[]){
float min,max;
sleep(1);

FILE *range;
range = fopen("range.txt", "r");

fscanf(range,"%f %f", &max,&min);
fclose(range);
printf("I am player #%s here my pid: %d\nI have the range %.3f %.3f\n", argv[1], getpid(),max,min);

return 0;
}