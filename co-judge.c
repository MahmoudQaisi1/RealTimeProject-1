#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>


void result(int);


char fifoName[1024];
char msg[1024];
int counter = 0;
int parentFifo;

int main(){
    
    
    printf("co-judge is Ready!!\n");

    //making the process sensitive to SIGUSR1//////////////////////////////////////////////////////
    if (signal(SIGUSR1,result) == SIG_ERR) //SIG_ERR = -1
    {
        perror("Sigset can not set SIGUSR1");
        exit(SIGUSR1);
    }
    sleep(1);
    //Opening the parents private fifo/////////////////////////////////////////////////////////////
    sprintf(fifoName,"/tmp/fifo%d",getppid());

    //Sending Game  Start Singal///////////////////////////////////////////////////////////////////
    kill(getppid(),SIGUSR1);
    sleep(1);
    while (1);
return 0;
}

float num[4];

void result(int the_signal){
    printf("co-judge claculating the sums\n");
    sleep(1);
    parentFifo = open(fifoName,O_RDONLY);
    read(parentFifo,&msg,sizeof(msg));
    close(parentFifo);
    sscanf(msg, "%f/%f/%f/%f", &num[0], &num[1], &num[2], &num[3]);
    float sum1 = num[0] + num[1];
    float sum2 = num[2] + num[3];
    printf("Sum of first two numbers: %f\n", sum1);
    printf("Sum of second two numbers: %f\n", sum2);
    sleep(2);
    sprintf(msg,"%.3f/%.3f",sum1,sum2);
    kill(getppid(),SIGUSR1);
    parentFifo = open(fifoName,O_WRONLY);
    write(parentFifo,msg, sizeof(msg));
    close(parentFifo);
    
}