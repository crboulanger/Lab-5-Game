/*
 * Author: Christopher Boulanger
 * Created: 4 November 2013
 * Description: This program is a game played using the LCD and buttons 1-4.
 * It prints an '*' and moves and then clears the '*' based upon corresponding button presses.
 * 2-second delay waits for a button press. If the player fails to move within 2 seconds
 * The LCD prints "GAME OVER!" Winning the game prompts "YOU WON!"
 * B Functionality includes land mines which end the game when hit.
 * Any button press will prompt a new game.
 * Documentation: Worked a lot with C2C Mossing. He is responsible for helping me
 * with the lab requirements and B functionality.
 */
#include <msp430.h>
#include "game.h"
#include "button.h" //Captain Branchflower
#include "LCD.h" //from last lab
#include "clkSpeed.h" //makes clock speed
#include "rand.h"

void init_timer();
void init_buttons();
void moveProperPlayer(char buttonToTest);
void Reset(char buttonToTest);
void testAndRespondToButtonPush(char buttonToTest);
void newGame();
void gameOver();


char player=0; //initialize flags to 0
char gameover=0;
char btnPush=0;
char timerCount=0;
unsigned char mines[2];
unsigned int seed;


void clearTimer(){	//clear timer at the beginning of the game and after button push
	timerCount=0;
	TACTL |= TACLR;
}

int main(void)
{
        WDTCTL = (WDTPW|WDTHOLD);

        player = initPlayer();//beginning of the game
        initSPI();
        initLCD();
        clearLCD();

        seed = prand(1234);
        newGame();

        init_timer();
        init_buttons();
        __enable_interrupt();//enable locals and globals, enable maskable interrupts

        while(1)
        {
             if (player==0xC7){		//winning condition
            	 TACTL &= ~TAIE;
            	 clearLCD();
            	 line1Cursor();
            	 writeString("YOU");
            	 line2Cursor();
            	 writeString("WON!");
            	 gameover=1;
            	 _delay_cycles(100000);
             }
             if(didPlayerHitMine(player, mines) && gameover == 0){ //gameover == 0 because I want this to show once, and not to alternate between game over and kaboom
                 TACTL &= ~TAIE;
                 clearLCD();
                 line1Cursor();
                 writeString("BOOM!");
                 line2Cursor();
                 writeString("Hit Mine");
                 _delay_cycles(200000);
                 gameOver();
             }
             if (timerCount>=4){	//losing condition
            	 TACTL &= ~TAIE;
            	 gameOver();
             }
        }

        return 0;
}

void init_timer()
{
        TACTL &= ~(MC1|MC0); //stop timer

        TACTL |= TACLR;	//clear TAR

        setspeed_1MHz();
        TACTL |=TASSEL1;	// configure for SMCLK frequency roughly 1MHz
        TACTL |=ID1|ID0;	//divide clock by 8
        TACTL &= ~TAIFG; 	// clear interrupt flag

        TACTL |=MC1;		// set count mode to continuous
        TACTL |=TAIE;		// enable interrupt
}

void init_buttons()
{
		configureP2PinAsButton(BIT2);// pins on chip to interface with buttons
	    configureP2PinAsButton(BIT3);
	    configureP2PinAsButton(BIT4);
	    configureP2PinAsButton(BIT5);
        P2IES |= BIT2|BIT3|BIT4|BIT5;
        P2IFG &= ~BIT2|BIT3|BIT4|BIT5;
        P2IE |= BIT2|BIT3|BIT4|BIT5;
}
void newGame(){
        gameover = 0;
        clearLCD();
        player = initPlayer();
        printPlayer(player);
        seed = generateMines(mines, seed);
        printMines(mines);
}
void gameOver(){
        clearLCD();
        line1Cursor();
        writeString("Game");
        line2Cursor();
        writeString("Over!");
        gameover = 1;
        _delay_cycles(100000);
}
void testAndRespondToButtonPush(char buttonToTest){
	if (buttonToTest & P2IFG){
		if(buttonToTest & P2IES){
			moveProperPlayer(buttonToTest);
			clearTimer(); //timer countdown starts again as soon as button is released
		}
		else{
			debounce();
		}
		P2IES ^=buttonToTest;
		P2IFG &= ~buttonToTest;
	}
}

void moveProperPlayer(char buttonToTest){
	switch(buttonToTest){
	case BIT3:
		player=movePlayer(player,RIGHT);
		break;
	case BIT4:
		player=movePlayer(player, LEFT);
		break;
	case BIT5:
		player=movePlayer(player, UP);
		break;
	case BIT2:
		player=movePlayer(player, DOWN);
	}
}

void Reset(char buttonToTest){
	if(buttonToTest &P2IFG){
		if(buttonToTest & P2IES){
			gameover=0;
			clearLCD();
			player=initPlayer();
			printPlayer(player);
			clearTimer();
			TACTL|= TAIE;
		}
		else{
			debounce();
		}
		P2IES ^= buttonToTest;
		P2IFG &= ~buttonToTest;
	}
}

#pragma vector= TIMER0_A1_VECTOR //interrupt vectors
__interrupt void TIMER0_A1_ISR(){  //timer interrupt
	TACTL &= ~TAIFG;
	timerCount++;
}

#pragma vector = PORT2_VECTOR	//button push interrupt vector
__interrupt void PORT_2_ISR(){
	if(gameover==0){
		testAndRespondToButtonPush(BIT2);
		testAndRespondToButtonPush(BIT3);
		testAndRespondToButtonPush(BIT4);
		testAndRespondToButtonPush(BIT5);
	}
	else{
		Reset(BIT2);
		Reset(BIT3);
		Reset(BIT4);
		Reset(BIT5);
	}
}

/////PsuedoCode for main.c
/*
                 * while (game is on)
                 * {
                 *                 check if button is pushed (you don't want to block here, so don't poll!)
                 *                 if button is pushed:
                 *                         clear current player marker
                 *                         update player position based on direction
                 *                         print new player
                 *                         clear two second timer
                 *                         wait for button release (you can poll here)
                 * }
                 *
                 * print game over or you won, depending on game result
                 *
                 * wait for button press to begin new game (you can poll here)
                 * wait for release before starting again
                 */

