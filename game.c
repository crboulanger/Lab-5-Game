/*Christopher Boulanger
 */

#include <msp430g2553.h>
#include "game.h"
#include "LCD.h"

unsigned char initPlayer()
{
        return 0x80;/// player starts at address 0x80 corresponding to top left LCD square
}

void printPlayer(unsigned char player)
{
        writeCommandByte(player);
        writeDataByte('*');
}

void clearPlayer(unsigned char player)
{
        writeCommandByte(player);
        writeDataByte(' ');
}

unsigned char movePlayer(unsigned char player, unsigned char direction)
{
	clearPlayer(player);
        switch (direction) {
        		case UP:
        			player &= ~BIT6;
        			break;
        		case DOWN:
        			player |=BIT6;
        			break;
        		case RIGHT:
        			if (player<0x87 || player>=0xC0){// doesn't allow player to go out of bounds
        				player ++;					///// could add functionality here that prompts game over for out of bounds
        			}								///// could add functionality here that loops the boundaries ex) at location 0x87... right... print new location 0xC0
        			break;
        		case LEFT:

        			if ((player & 0x0f)>0){
        				player --;
        			}
        }
       printPlayer(player);
       return player;
}

char didPlayerWin(unsigned char player)
{
        return player == 0xC7;/// winning square is bottom right/// could add functionality to print a square around the finish line square
}
