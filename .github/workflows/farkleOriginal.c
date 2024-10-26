/*****************************************
*             Included Libraries         *
*****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>



/*****************************************
*             Macros                     *
*****************************************/
#define numDice 6
#define bline  "     "
#define lline "o    "
#define dline "o   o"
#define cline  "  o  "
#define rline  "    o"
#define WINNING_SCORE 10000
/*****************************************
*             Global Variables           *
*****************************************/
int comp_score;
int play1_score=0;
int play2_score=1000;
int dice[6];
int hold[6];

int msg=0;
int err=0;
/*****************************************
*             Function Prototypes        *
*****************************************/
void board();
int calculateScore(int roll[], int num);
void play1();
void play2();
int showDice();
void turn(int num);
void msgCtr(int mess);
void rollDice(int roll[], int num);
void checkWin();
	
	


int main(){
	srand(time(NULL));
	for(int x=0; x<numDice; x++){
		hold[x]=0;
		dice[x]=rand()%6+1;
	}
	turn(0);
	
	
}





/*****************************************
*             Functions                  *
*****************************************/
void board(){
	system("cls");
	printf("\t\tC Farkle\n");
	printf("\t\t R Wilde\n\n");
	printf("Player 1: %5d\t\tPlayer 2: %5d\n", play1_score, play2_score);
	showDice();
	msgCtr(err);
	msgCtr(msg);
	printf("Command: ");
}

int calculateScore(int roll[], int num){
	int score=0;
	int counts[6]={0};
	for(int x=0; x < num; x++){
		counts[roll[x]-1]++;
	}
	for(int x=0; x<num;x++){
		if(counts[x] >=3){
			if(x==0){
				score+=1000;
			}else{
			    score += (x+1)*100;
			}
			counts[x]-=3;
		}
		
	}
	score += counts[0]*100;
	score += counts[4]*50;
	return score;
}

void checkWin(){
	if(play1_score>=WINNING_SCORE ){
	   msg=5;
	   board();
	}else if(play2_score>=WINNING_SCORE){
		msg=6;
		board();
	}
}


void msgCtr(int mess){
    switch(mess){
		case 0:
			printf("\n");
			break;
		case 1:
			printf("Player 1 turn - R to roll\n");
			break;
		case 2:
			printf("Player 2 turn - R to roll\n");
			break;
		case 3:
			printf("Select dice to hold <1-6> R to roll\n");
			break;
		case 4:
			printf("Farkle - No Points\n");
			break;
		case 5:
			printf("Player 1 wins\n");
			break;
		case 6:
			printf("Player 2 wins\n");
			break;
	}
}

/* The turn function accept a player number num which
determines whose turn it is */
void turn(int num){
	char ans[10];
	int reroll[6];
	int rerollNum=0;
	int valid=0;
	int flag=1;
	int turn_score=0;
	if(num) msg=2;
	else msg=1;
	for(int x=0; x<6 ;x++)hold[x]=0;
	rollDice(dice, 6);
	for(int x=0; x<numDice; x++){
		reroll[x]=dice[x];
	}
	rerollNum=6;
	while (flag){
		if(calculateScore(reroll, rerollNum)){
			turn_score=calculateScore(reroll, rerollNum);
			for(int x=0; x<rerollNum; x++) reroll[x]=0;
			rerollNum=0;
			do{
				msg=3;
				board();
				scanf("%s",ans);
				if(atoi(ans)){
					int nDice=strlen(ans);
					for(int x=0; x<nDice; x++){
						if(hold[ans[x]-48]==0) hold[ans[x]-48]=1;
						else if(hold[ans[x]-48]=1) hold[ans[x]-48]=0;
					}
				}else if(ans[0]=='R' || ans[0]=='r'){
					for(int x=0; x<=numDice; x++){
						if(hold[x]==0){
							reroll[rerollNum]=x;
							rerollNum++;
						}else hold[x]++;
						board();
					}
					rollDice(reroll, rerollNum);
					
					board();
					
				}
			}while (!valid );
			if(num==0){
				play1_score+=turn_score;
				
			}else{
				play2_score+=turn_score;
			}
			valid=1;
		}else{
			msg=4;
			board();
			flag=0;
		}
	}
	return;
	
}


void play1(){
	int roll[6];
	int dieNo=0;
	int scored;
	msg=1;
	board();
	char ans[25];
	scanf("%s", ans);
	rollDice(dice, 6);
	//Dice are rolled - need to pick held dice 
	do{
		rollDice(dice, 6);
		scored=0;
		msg=3;
		board();
		scanf("%s", ans);
		
		
		
	}while (scored && ans>0);
}

void rollDice(int roll[], int num){
	for(int x=0; x<numDice; x++){
		if(!hold[x]){
			dice[x]=rand()%6+1;
		}
	}
}



int showDice(){
	for(int x=0; x<numDice; x++){
		//First Line
		printf("%c%c%c%c%c%c%c",218,196,196,196,196,196,191);
	}
	printf("\n");
	//Second Line
	for(int x=0; x<numDice; x++){	
		if(dice[x]==1) printf("%c%s%c",179,bline,179);
		else if(dice[x]<4) printf("%c%s%c",179,lline,179);
		else printf("%c%s%c",179,dline,179);
	}
	printf("\n");
	//Third Line
	for(int x=0; x<numDice; x++){
		if(dice[x]==6)printf("%c%s%c",179,dline,179);
		else if(dice[x]%2) printf("%c%s%c",179,cline,179);
		else printf("%c%s%c",179,bline,179);
	}	
		printf("\n");
		//Fourth Line
	for(int x=0; x<numDice; x++){	
		if(dice[x]==1) printf("%c%s%c",179,bline,179);
		else if(dice[x]<4) printf("%c%s%c",179,rline,179);
		else printf("%c%s%c",179,dline,179);
	}
	printf("\n");
	for(int x=0; x<numDice; x++){	
		//Fifth Line
		printf("%c%c%c%c%c%c%c",192,196,196,196,196,196,217);
	}		
	printf("\n");
	for(int x=0; x<numDice; x++){
		if(hold[x])printf(" %c%c%d%c%c ",219,219,x,219,219);
		else printf(" < %d > ",x);
		
	}
	printf("\n");
}

