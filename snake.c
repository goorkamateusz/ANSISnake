/* GRA SNAKE
   Mateusz Gorka

   kompilacja:
              gcc -o snake snake.c -Wall -pedantic

   lista plikow:
                  mati_getch.h - ustawienia terminala odpowiadajace funkcji getch (z conio.h) 
		               - funkcje std_input()
		  mati_ascii.h - kody zwracane przez funkcje w-w

		  mati_listy.h - definicje listy jednokierunkowej wskaznikow void*

		  snake_draw.h - funkcje sterujace wyswietlaniem gry na terminalu
		  snake_struct.h - defunicje struktury mvector, snake i funkcje
		  snake_bonus.h - definicje strukruy prizes i bonus
		  snake_config.h - ustawienia konfiguracj gry
		  snake_statistics.h - zapis rekordow graczy
*/

#include <stdio.h>
#include <stdlib.h>
#include "mati_getch.h"
#include "mati_ascii.h"
#include "snake_draw.h"
#include "snake_struct.h"
#include "snake_bonus.h" 
#include "snake_config.h"
#include "snake_statistics.h"
#include "snake_enemy.h"

//FUNCTION ---------------------------------------------------
void welcome( void );  //komunikat przywitania

//MAIN --------------------------------------------------------
int main(){
  
//PRZYGOTOWANIE
  int c;                  //znak do odczytu z stdin
  unsigned int delay = 0; //zmienna zliczajaca obroty petli
  bool keyable = true;    //blokowanie klawiszy
  
  /*deklaracja podstawowych struktur:*/
  struct snake snake;
  defaultsnake( & snake );

  struct prizes prizes;
  defaultprizes( & prizes );

  struct snake *enemy = NULL;

  /*funckje przygotowywujace terminal:*/
  m_setTermios();

  draw_border();
  set_communicate( "Powodzenia!!! :D ~Mati\n" );

  /*przywitanie:*/
  welcome();
  show_stat();

  c = getchar();
  fflush( stdin );
  fflush( stdout );
  
//GRA
  while( c != 'q' ){   //petla gry

    if( keyable && m_nacisniecie() ){     //sterowanie

      c = m_stdinput();        //input

      switch( c ){
      
      case keyUP:    snake_turn( & snake,  0, -1 );  break;
      case keyRIGHT: snake_turn( & snake,  1,  0 );  break;
      case keyDOWN:  snake_turn( & snake,  0,  1 );  break;
      case keyLEFT:  snake_turn( & snake, -1,  0 );  break;
      case 'q':      goto exitgame;
      }

      keyable = false;
    }

    //ruch i rendering:
      if( delay % snake.delay == 0 ){
	
	//przesuniecie snak'a
        snake_move( & snake );     
	//sprawdzanie czy snake najechal na bonus (w snake_bonus.h)
      	checkbonus( & prizes, & snake );
	
	draw_snake( snake.pos.x, snake.pos.y );
	keyable = true;
      }

    //enemy
      if( enemy )
	if( delay % enemy->delay == 0 ){
	  checkbonus( & prizes, enemy );
	  move_enemy( &enemy, &snake ); //UWAGA moze zerowac wskaznik enemy 
	}

      if( !enemy )
	create_enemy( &enemy, &snake );
      
    //punkty i bonusy - prizes:
      if( delay % prizes.delay == 0 )
	bonusrandomly( & prizes, & snake );

    //dodawanie punktow
      if( delay % _GAME_POINT == 0 )
	snake.point++;

    //koniec gry:
      if( gooutside( snake.pos.x, snake.pos.y ) ||
	  hitintail( & snake ) ||
	  ! islocationfreesnake( enemy, snake.pos.x, snake.pos.y )
         ) break;
      
    //reset do wyswietlania punktow:
      reset_communicate_numb( "pkt:  %d  waz:  %d\n", snake.point, snake.l );

    //aktulizacja planszy:
      drawif();             //rysuje jezeli sa zmiany

    /* delay */
      usleep( _GAME_DELAY );   //opoznienie petli gry
      delay++;
  }

//ZAKONCZENIE
 exitgame:
  m_resetTermios();

  { /* end of game */
    printf( "\nKONIEC GRY!\n\n" );
    printf( "Zdobyles  %d  pkt\n", snake.point );
    printf( "Wychodowales weza  %d  dlugosc\n", snake.l );
    printf( "A gdy przegrales na planszy bylo  %d  bonusow\n", prizes.am );
    printf( "\nAle gratulujemy ;)\n\n" );
  }
  
  check_stat( &snake, &prizes );
  printf("\n");

  return 0;
}

//DEFINICJE FUNKCJI:
void welcome( void ){
  system( "clear" );
  
  printf( "- - - - - - - - - - - - - - SNAKE GAME - - - - - - - - - - - - - - -\n\n" );
  printf( "by Mati Gorka\n" );
  printf( "--------------------------------------------------------------------\n\n" );
  printf( "~ STEROWANIE: ~\n" );
  printf( "strzalki - sterowanie wezem\n" );
  printf( "q        - wyjscie z gry w dowolnym momencie\n" );
  printf( "ESC      - zatrzymanie gry\n\n" );
  printf( "--------------------------------------------------------------------\n\n" );
  printf( "Zapraszam do emocjonujacej gry!\n");
  printf( "                                  nasicij dowolny przycisk by zaczac" );
}
