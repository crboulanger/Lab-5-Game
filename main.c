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
#include "button.h"
#include "LCD.h"

void init_timer();
void init_buttons();

char btnPush=0;
char timerCount=0;

int main(void)
{
        WDTCTL = (WDTPW|WDTHOLD);

        unsigned char player = initPlayer();

        init_timer();
        init_buttons();
        __enable_interrupt();

        while(1)
        {
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
        }

        return 0;
}

//
// YOUR TIMER A ISR GOES HERE
//

void init_timer()
{
        TACTL &= ~(PC1|MC0);

        TACTL |= TACLR;

        setspeed_1MHz();
        TACTL |=TASSEL1;
        TACTL |=ID1|ID0;
        TACTL &= ~TAIFG;

        TACTL |=MC1;
        TACTL |=TAIE;
}

void init_buttons()
{
        configureP2PinAsButton(2);
        configureP2PinAsButton(3);
        configureP2PinAsButton(4);
        configureP2PinAsButton(5);
        P2IES |= BIT2|BIT3|BIT4|BIT5;
        P2IFG &= BIT2|BIT3|BIT4|BIT5;
        P2IE |= BIT2|BIT3|BIT4|BIT5;
}

#pragma vector= TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(){
	TACTL &= TAIFG;
	count++;
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT_2_ISR(){
	if(P2IFG,BIT3){
		btnPush = RIGHT;
	}
}
