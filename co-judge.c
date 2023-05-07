#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int main(){
    int team1[2];
    int team2[2];
    printf("I am the co-judge My PID = %d\n", getpid());
    sleep(1);

    //Acquiring the PIDs of the players///////////////////////////////////////////////////////////

    FILE *teams;
    teams = fopen("teams.txt", "r");
    fscanf(teams,"%d %d %d %d",&team1[0],&team1[1],&team2[0],&team2[1]);
    fclose(teams);
    printf("Teams are selected!\n Team 1: %d %d\n Team 2: %d %d\n",team1[0],team1[1],team2[0],team2[1]);

// //private fifo-creation.//////////////////////////////////////////////////////////////////////
//   sprintf(msg.fifo_name, "/tmp/fifo%d", getpid());
//   if (mknod(msg.fifo_name, S_IFIFO | 0666, 0) < 0)
//   {
//         perror("Private");
//         exit(-8);
//   }

return 0;
}


