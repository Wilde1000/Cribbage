#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define DICE_COUNT 6
#define bline  "     "
#define lline "o    "
#define dline "o   o"
#define cline  "  o  "
#define rline  "    o"
#define WINNING_SCORE 10000
#define MIN_INITIAL_BANK 500 // Minimum score required for the initial bank

int playerScore[2] = {0};
int player = 0;
int roundScore = 0;
int dice[6];
int hold[6];
int remainingDice = DICE_COUNT;
int msg = 0;
int msgVar;
int err = 0;
int errVar;

void board();
int calculateScore(int roll[], int num);
void getChoices();
int showDice();
void msgCtr(int mess);
void rollDice(int roll[], int num);
void checkWin();
void resetDice();

int main() {
    srand(time(NULL));
    resetDice();
    
    char ans[20];
    char choice;
    
    while (1) {
        int rollScore = 0;
        err = 7;
        msg = 3;
        rollDice(dice, remainingDice);
        rollScore = calculateScore(dice, remainingDice);
        
        if (rollScore == 0) {  // Check for farkle (no points)
            err = 4;
            board();
            roundScore = 0;  // Reset round score
            player = (player + 1) % 2;  // Switch players
            resetDice();  // Reset dice for the next player's turn
        } else {
            roundScore += rollScore; // Accumulate rollScore in roundScore
            errVar = rollScore;
            msgVar = roundScore;

            // Check if "hot dice" is triggered (all dice scored)
            if (remainingDice == 0) {  
                remainingDice = DICE_COUNT;  // Reset to 6 dice for hot dice
                msg = 10; // Display "Hot Dice! Roll again."
                board();
            } else {
                do {
                    err = 7;
                    msg = 8;
                    board();
                    scanf("%s", ans);
                    choice = ans[0];
                    if (choice > 96) choice -= 32;
                    
                    // Check if the player has met the minimum initial bank score
                    if (choice == 'B' && (playerScore[player] + roundScore < MIN_INITIAL_BANK)) {
                        err = 11;  // Custom error message for insufficient points
                        choice = 'R';  // Reset choice to continue rolling
                    }
                    
                } while (choice != 'B' && choice != 'R');
                
                if (choice == 'B') {  // Bank score
                    playerScore[player] += roundScore;
                    checkWin();
                    roundScore = 0;
                    player = (player + 1) % 2;  // Switch players
                    resetDice();
                } else {
                    getChoices();  // Continue rolling with held dice
                }
            }
        }
        board();
    }
}

void resetDice() {
    for (int x = 0; x < DICE_COUNT; x++) {
        hold[x] = 0;
        dice[x] = rand() % 6 + 1;
    }
    remainingDice = DICE_COUNT;
}

void board(){
    system("cls");
    printf("\t\tC Farkle\n");
    printf("\t\t R Wilde\n\n");
    printf("Player 1: %5d\t\tPlayer 2: %5d\n", playerScore[0], playerScore[1]);
    showDice();
    msgCtr(err);
    msgCtr(msg);
    printf("Command: ");
}

int calculateScore(int roll[], int num){
    int score = 0;
    int counts[6] = {0};
    for (int x = 0; x < num; x++) {
        counts[roll[x] - 1]++;
    }
    for (int x = 0; x < 6; x++) {
        if (counts[x] >= 3) {
            score += (x == 0) ? 1000 : (x + 1) * 100;
            counts[x] -= 3;
        }
    }
    score += counts[0] * 100;
    score += counts[4] * 50;

    // Check if all dice scored
    if (num == DICE_COUNT && score > 0) {
        remainingDice = 0;  // Indicate hot dice
    }
    
    return score;
}

void checkWin(){
    if (playerScore[0] >= WINNING_SCORE) {
        msg = 5;
        board();
    } else if (playerScore[1] >= WINNING_SCORE) {
        msg = 6;
        board();
    }
}

void getChoices(){
    int choice = 1;
    for (int x = 0; x < DICE_COUNT; x++) hold[x] = 0;
    while (choice) {
        err = 7;
        msg = 3;
        board();
        scanf("%d", &choice);
        if (choice) {
           hold[choice - 1] = 1;
        }
        board();
    }
    int rdice[DICE_COUNT] = {0};
    int sdice[DICE_COUNT] = {0};
    int rnum = 0;
    int snum = 0;
    for (int x = 0; x < remainingDice; x++) {
        if (hold[x]) {
            rdice[rnum] = dice[x];
            rnum++;
        } else {
            sdice[snum] = dice[x];
            snum++;
        }
    }
    for (int x = 0; x < remainingDice; x++) dice[x] = 0;
    roundScore += calculateScore(sdice, snum);
    remainingDice = 0;
    for (int x = 0; x < rnum; x++) {
        hold[x] = 0;
        dice[remainingDice] = rdice[x];
        remainingDice++;
    }
    board();
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
            printf("Select dice to roll <1-%d> R to roll\n", remainingDice);
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
        case 7:
            printf("Roll Score is %d\tRound Score is %d\n",errVar, msgVar);
            break;
        case 8:
            printf("Bank or Roll (B/R)\n");
            break;
        case 9:
            printf("Round Score is %d\n", msgVar);
            break;
        case 10:
            printf("Hot Dice! Roll all six dice again.\n");
            break;
        case 11:
            printf("You need at least 500 points to bank your score for the first time.\n");
            break;
    }
}

void rollDice(int roll[], int num){
    for (int x = 0; x < num; x++) {
         // Only reroll dice not held
            dice[x] = rand() % 6 + 1;
        
    }
}

int showDice(){
    for (int x = 0; x < remainingDice; x++) {
        printf("%c%c%c%c%c%c%c", 218, 196, 196, 196, 196, 196, 191);
    }
    printf("\n");
    for (int x = 0; x < remainingDice; x++){
        if (dice[x] == 1) printf("%c%s%c", 179, bline, 179);
        else if (dice[x] < 4) printf("%c%s%c", 179, lline, 179);
        else printf("%c%s%c", 179, dline, 179);
    }
    printf("\n");
    for (int x = 0; x < remainingDice; x++){
        if (dice[x] == 6) printf("%c%s%c", 179, dline, 179);
        else if (dice[x] % 2) printf("%c%s%c", 179, cline, 179);
        else printf("%c%s%c", 179, bline, 179);
    }    
    printf("\n");
    for (int x = 0; x < remainingDice; x++){
        if (dice[x] == 1) printf("%c%s%c", 179, bline, 179);
        else if (dice[x] < 4) printf("%c%s%c", 179, rline, 179);
        else printf("%c%s%c", 179, dline, 179);
    }
    printf("\n");
    for (int x = 0; x < remainingDice; x++){
        printf("%c%c%c%c%c%c%c", 192, 196, 196, 196, 196, 196, 217);
    }        
    printf("\n");
    for (int x = 1; x <= remainingDice; x++){
        if (hold[x - 1]) printf(" %c%c%d%c%c ", 219, 219, x, 219, 219);
        else printf(" < %d > ", x);
    }
    printf("\n");
}
