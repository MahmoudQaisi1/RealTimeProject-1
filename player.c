#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
int min,max;
char line[5];

FILE *range;
range = fopen("range.txt", "r");

fgets(line,5,range);
max = atoi(line);
fgets(line,5,range);
min = atoi(line);

printf("I am player #%s here my pid: %d\n I have the range %d %d\n ", argv[1], getpid(),max,min);

fclose(range);




return 0;
}