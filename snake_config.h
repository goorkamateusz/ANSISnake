#ifndef SNAKE_CONFIG_H
#define SNAKE_CONFIG_H

/*GAME: */
#define _GAME_DELAY 10000 /* 1/100 s */
#define _GAME_POINT 200   /* pkt +1 co 2 s. */

/*DRAW: */
#define HEIGHT 20
#define WIDTH 40

#define BORDER 1

#define HEIGHT_ALL 22 /* HEIGHT + BORDER*2 */
#define WIDTH_ALL 43 /* WIDTH +  BORDER*2 + 1 (\n) */

#define CHAR_BORDER '#'

#define CHAR_ENDLINE '\n'
#define CHAR_BACKGROUND ' '

#define CHAR_SNAKE 'x'
#define CHAR_SNAKE_HEAD 'X'

#define _RESET_COMMUNICATE 200 /*co 2 s. */

/*DEFAULT SETTINGS: */
#define _SNAKE_MAXLENGTH 20
#define _SNAKE_DELAY     20
#define _SNAKE_VECTOR    1, 0

#define _PRIZES_MAX   12
#define _PRIZES_DELAY 120

/*PRIZES: */
#define _BONUS_POINT  5
#define _BONUS_LENGTH 4
#define _BONUS_AMOUNT 4
#define _BONUS_DELAY  3

/* ENEMY: */
#define DISTANS_TRY 4  /* odleglosc jak do przodu enemy bedzie sprawdzal droge */
#define MIN_LENGTH 10  /* minimalna dlugosc snaka wroga */

int speedpersecond( int delay ){
  return ( 1000000 / ( _GAME_DELAY * delay ) );
}

#endif
