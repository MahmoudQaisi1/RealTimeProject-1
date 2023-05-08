#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>


char fifoName[1024];
char msg[1024];
int privatefifo;
float min,max;
float value;
void sendNumbers(int);

int main(int argc, char *argv[]){

sleep(1);
//Reading Range from File//////////////////////////////////////////////////////////////////////////
FILE *range;
range = fopen("range.txt", "r");
fscanf(range,"%f %f", &max,&min);
fclose(range);
printf("Player #%s Ready!\n", argv[1]);
srand(getpid());

//Adjusting The signal/////////////////////////////////////////////////////////////////////////////

if (signal(SIGUSR1, sendNumbers) == SIG_ERR) //SIG_ERR = -1
{
    perror("Sigset can not set SIGUSR1");
    exit(SIGUSR1);
}

//Player Private fifo Creation/////////////////////////////////////////////////////////////////////
sprintf(fifoName, "/tmp/fifo%d", getpid());
if (mknod(fifoName,S_IFIFO | 0666, 0) < 0)
{
    perror("Error Creating fifo\n");
    exit(1);
}

while (1);

}

void sendNumbers(int the_signal){
    //Save Number in private fifo and send signal to parent/////////////////////////////////////////
    value = ((float)rand() / (float)RAND_MAX) * (max - min) + min;
    printf("My number: %f\n",value);
    if ( (privatefifo = open(fifoName, O_WRONLY)) == -1 ) {
      perror(fifoName);
      exit(3);
    }
    sprintf(msg,"%.3f",value);
    write(privatefifo, msg, sizeof(msg));
    close(privatefifo);
    sleep(2);
    kill(getppid(),SIGUSR2);
}