#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t players_pid_array[4];


int main(int argc, char *argv[])
{
  int i, status;
  pid_t co_judge;
  int playerNum = 0;
  char player[10];
  float min,max;


  //handling rounds//////////////////////////////////////////////////////////////////////
  int rounds = 5;

  if (argc == 2 ){
      rounds = atoi(argv[1]);
  } 
  printf("Rounds: %d\n",rounds);
  printf("My process ID is # %d\n", getpid());


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
  sleep(1);

  //Sending The teams PIDS to the co-judge/////////////////////////////////////////////////////////
  FILE *teams;
  teams = fopen("teams.txt", "w");
  fprintf(range,"%d\n%d\n%d\n%d\n", players_pid_array[0], players_pid_array[1], players_pid_array[2], players_pid_array[3]);
  fclose(teams);
  sleep(1);

 
  // //private fifo-creation.//////////////////////////////////////////////////////////////////////
  // char fifoName[B_SIZ];
  // char msg[B_SIZ];
  // sprintf(fifoName, "/tmp/fifo%d", getpid());
  // if (mknod(fifoName, S_IFIFO | 0666, 0) < 0)
  // {
  //     perror("Error Creating fifo\n");
  //     exit(1);
  // }

  // int parentfifo = open(fifoName,O_RDONLY);
  // for (i = 0; i < 2; i++)
  // {
  //   while ( read(parentfifo, (char *) &msg, sizeof(msg)) > 0 ) {
  //     printf(msg);
  //   }
  // }
  
  for (i = 0; i < 4; i++ ) {
    if (players_pid_array[i] = wait(&status)) {
	  printf("Process ID %d has terminated\n", players_pid_array[i]);
    }
  }
  
  if (co_judge = wait(&status)) {
	  printf("Process ID %d has terminated\n", co_judge);
  }

  return(0);
}


