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
        // do timer initialization work
}

void init_buttons()
{
        // do button initialization work
}
