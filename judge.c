#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>
#include <stdbool.h>


void playGame(int);
void call_co_jud(int);
pid_t players_pid_array[4];
pid_t co_judge;
int rounds = 5;
int roundsPlayed = 0;
char fifoName[1024];
char msg[1024];
int playersResp = 0;
int i, status;
float sumArr[2];
int parentfifo;
char pfifoname[1024];
char pmsg[1024];
char numbers[1024] = "";
int playerfifo;
int points1 = 0;
int points2 = 0;

int main(int argc, char *argv[])
{
  
  int playerNum = 0;
  char player[10];
  int team1points = 0;
  int team2points = 0;
  float min,max;

  //Adjsting the signal.////////////////////////////////////////////////////////////////////////
  if (signal(SIGUSR1, playGame) == SIG_ERR) //SIG_ERR = -1
  {
    perror("Sigset can not set SIGUSR1");
    exit(SIGUSR1);
  }

  if (signal(SIGUSR2, call_co_jud) == SIG_ERR) //SIG_ERR = -1
  {
    perror("Sigset can not set SIGUSR1");
    exit(SIGUSR2);
  }


  //handling rounds//////////////////////////////////////////////////////////////////////
  
  if (argc == 2 ){
      rounds = atoi(argv[1]);
  } 
  
  printf("I am the judge..The game is starting..\n");
  printf("Rounds: %d\n",rounds);

  //range generation and writing it in file./////////////////////////////////////////////
  FILE *range;
  range = fopen("range.txt", "w");
  srand(getpid());
  max =  ((float)rand() / (float)RAND_MAX) * 100.00;
  min =  ((float)rand() / (float)RAND_MAX) * max;
  fprintf(range,"%.3f\n%.3f\n", max,min);
  printf("The range is : max = %.3f min = %.3f\n",max,min);
  fclose(range);


  //players generation.//////////////////////////////////////////////////////////////////
  for ( i = 0; i < 4; i++ ) {
    sleep(1);
    playerNum++;
    snprintf(player,10,"%d",playerNum);
    players_pid_array[playerNum - 1] = fork();
    if ( players_pid_array[playerNum - 1] == 0 ) {
      execlp("/4330/proj1/player","player",player,NULL); //path to projet file
      printf("Error!! generating player %d",playerNum);
    }

  }
  sleep(2);
  //co-judge generation./////////////////////////////////////////////////////////////////
  co_judge = fork();
  if(co_judge == 0){
    execlp("/4330/proj1/co-judge","co-judge",NULL);
    printf("Error!! generating co-judge");
  }

  //private fifo-creation.//////////////////////////////////////////////////////////////////////
  sprintf(fifoName, "/tmp/fifo%d", getpid());
  if (mknod(fifoName, S_IFIFO | 0666, 0) < 0)
  {
    perror("Error Creating fifo\n");
    exit(1);
  }

  
  
  while (1);
}

void playGame(int the_signal){
  int i;
  if (roundsPlayed == 0)
  {
    printf("Baby Let The Games Begin!!!\n");
    sleep(0.5);
  } else {
    printf("Reading The sums:\n");
    parentfifo = open(fifoName,O_RDONLY);
    read(parentfifo,&msg, sizeof(msg));
    close(parentfifo);

    printf("jusdge recived: %s\n",msg);

    sscanf(msg,"%f/%f",&sumArr[0],&sumArr[1]);
    sleep(1);
    if(sumArr[0] > sumArr[1]){
      printf("Team 1 won this round with (%f > %f)\n",sumArr[0],sumArr[1]);
      points1++;
      
    }else{
      printf("Team 2 won this round with (%f < %f)\n",sumArr[0],sumArr[1]);
      points2++;
    }
    
  } //process the result////////
  
  if (roundsPlayed != rounds)
  {
    printf("Round %d\n",roundsPlayed+1);
    sprintf(numbers,"");
    sleep(0.5);
    //Calling the players to send their numbers//////////////////////////////////////////////////////////////////////////
    for (i = 0; i < 4; i++ ) {
    kill(players_pid_array[i],SIGUSR1);
    sprintf(pfifoname,"/tmp/fifo%d",players_pid_array[i]);
    playerfifo = open(pfifoname,O_RDONLY);
    read(playerfifo,&pmsg,sizeof(pmsg));
    close(playerfifo);
    sprintf(numbers,"%s%s/",numbers,pmsg);
    sleep(1);
    }
    roundsPlayed++;
  }else{ //declare the winner/////////////
    printf("GameOver\n");
    printf("Team 1: %d points || Team 2: %d points\n",points1,points2);
    if(points1 > points2){
      printf("Team 1 won!! with %d points.\n",points1);
    } else if(points1 < points2){
      printf("Team 2 won!! with %d points.\n",points2);
    }else{
      printf("Draw!!\n");
    }
    for (i = 0; i < 4; i++ ) {
    kill(players_pid_array[i],SIGKILL);
    printf("Process ID %d has terminated\n", players_pid_array[i]);
    sprintf(pfifoname,"/tmp/fifo%d",players_pid_array[i]);
    unlink(pfifoname);
    printf("%s was deleted\n",pfifoname);
    }
    kill(co_judge,SIGKILL);
    printf("Process ID %d has terminated\n", co_judge);
    unlink(fifoName);
    printf("%s was deleted\n",fifoName);
    printf("Exiting...\n");
    sleep(1);
    exit(0);
  }
}



void call_co_jud(int the_signal){
  playersResp++;
  printf("Players responded: %d\n",playersResp);
  if(playersResp == 4){
    sleep(3);
    kill(co_judge,SIGUSR1);
    sleep(1);
    printf("%s\n",numbers);
    parentfifo = open(fifoName,O_WRONLY);
    write(parentfifo,numbers,sizeof(numbers));
    close(parentfifo);
    sleep(1);
    playersResp = 0;
  }
}
 