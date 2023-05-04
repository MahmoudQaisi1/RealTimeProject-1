#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  int i, status;
  pid_t players_pid_array[4];
  pid_t co_judge;
  int playerNum = 0;
  char player[10];
  printf("My process ID is # %d\n", getpid());
  
  //players generation
  for ( i = 0; i < 4; i++ ) {
    playerNum++;
    snprintf(player,10,"%d",playerNum);
    players_pid_array[playerNum - 1] = fork();
    if ( players_pid_array[playerNum - 1] == 0 ) {
      execlp("/home/mah/4330/proj1/player","player",player,NULL); //path to projet file
      printf("Error!! generating player %d",playerNum);
    }
  }

  co_judge = fork();
  if(co_judge == 0){
    execlp("/home/mah/4330/proj1/co-judge","co-judge",NULL);
    printf("Error!! generating co-judge");
  }


  //while( 1 ) {
    for ( i = 0; i < 4; i++ ) {
      if (players_pid_array[i] = wait(&status)) {
	printf("Process ID %d has terminated\n", players_pid_array[i]);
      }
    }
  //}
  
  return(0);
}
