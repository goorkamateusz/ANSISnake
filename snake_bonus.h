#ifndef SNAKE_BONUS_H
#define SNAKE_BONUS_H

#include <stdlib.h>
#include <time.h>
#include "mati_bool.h"
#include "mati_listy.h"
#include "snake_draw.h"
#include "snake_config.h"

/* BONUS - struktura pojedynczych nagrod/kar ------------------------------ */
 typedef struct m_bonus {
   char type;   //rodzaj bonusu
   char x;
   char y;
   
 } bonus;

 void *bonustovoid( bonus n ){           //przydziela stale miejsce w pamieci
                                         //i konwertuje strukture bonus na void*
   bonus *new = (bonus*) malloc( sizeof( bonus ) );
   *new = n;

   return (void*) new;
 }
 //wartosc bonusa jako: bonus*
 bonus *vbonus( mlist *list ){ return (bonus*) list->val;  }

/* PRIZES - struktura listy wszystkich nagrod i kar ----------------------- */
 struct prizes {
   unsigned short int am;     //amount - ilosc
   unsigned short int maxam;  //maksymalna ilosc
   unsigned short int delay;  //opozienie tury
   
   mlist *list;
 };
/* --- FUNCTIONS: --- */
void defaultprizes( struct prizes *prizes ); //ustawia domyslne wartosci
bool randlocation( bonus *new, struct prizes *prizes, struct snake *snake ); //losuje pozycje dla bonusa
void addnewbonus( struct prizes *prizes, char type, struct snake *snake );
void bonusrandomly( struct prizes *prizes, struct snake *snake );
bool checkbouns( struct prizes *prizes, struct snake *snake );

/* --- DECLARATIONS: ---- */
 void defaultprizes( struct prizes *prizes ){
   prizes->am = 0;
   prizes->maxam = _PRIZES_MAX;
   prizes->delay = _PRIZES_DELAY;
   prizes->list = 0; //NULL
 }

 //losuje wspolzedne dla nowego bonusa:
 bool randalocation( bonus *new, struct prizes *prizes, struct snake *snake ){

   short int n=0;    //n pomaga w szybszym losowaniu roznych liczb
                    
 tryagain: // <-- tutaj
   srand( time( NULL ) + n );
   
   new->x = rand()%WIDTH;
   new->y = ( rand() + n )%HEIGHT;

   if( n > 10 ) return false;

   //sprawdzanie czy wspolzednych nie zajmuje snake:
   if( ! islocationfreesnake( snake, new->x, new->y ) ){
     n++;
     goto tryagain;
   }

   //sprawdzanie czy wspolzednych nie zajmuje inny bonus:
   if( prizes->list != NULL ){
     mlist *tmp = prizes->list;
     
     while( tmp != NULL ){
       if( vbonus( tmp )->x == new->x && vbonus( tmp )->y == new->y ){
	 n++;
	 goto tryagain;
       }
       tmp = tmp->next;
     }
   }

   return true;
 }

 //dodaje nowy bonus:
 void addnewbonus( struct prizes *prizes, char type, struct snake *snake ){
   bonus new;
   new.type = type;

   if( randalocation( &new, prizes, snake ) ){                   //losowanie wspolednych bonusu
     addnewtolist( & ( prizes->list ), bonustovoid( new ), 1 );  //dodanie nowego bonusa do listy

     prizes->am++;
   
     draw_object( vbonus( prizes->list )->x,
		  vbonus( prizes->list )->y,
		  vbonus( prizes->list )->type );
   }
 }

 void bonusrandomly( struct prizes *prizes, struct snake *snake ){
   
   if( prizes->am < prizes->maxam ){
     srand( time( 0 ) );

     unsigned short int r = rand()%100;
   
     if     ( r < 42 );
     else if( r < 58 ) addnewbonus( prizes, '$', snake );
     else if( r < 74 ) addnewbonus( prizes, '+', snake );
     else if( r < 80 ) addnewbonus( prizes, '-', snake );
     else if( r < 86 ) addnewbonus( prizes, 'f', snake );
     else if( r < 92 ) addnewbonus( prizes, 's', snake );
     else if( r < 94 ) addnewbonus( prizes, 'c', snake );
     else if( r < 96 ) addnewbonus( prizes, 'm', snake );
     else if( r < 98 ) addnewbonus( prizes, 'l', snake );
     else if( r < 100) addnewbonus( prizes, 'r', snake );
   }
 }

bool checkbonus( struct prizes *prizes, struct snake *snake ){

   if( prizes->am == 0 ) return false; //wyjscie jezeli nie ma bonusow na planszy
   
   //sprawdzanie czy snake najechal na bonus:
   mlist *tmp = prizes->list;

   while( tmp != NULL ){
     if( vbonus( tmp )->x == snake->pos.x && vbonus( tmp )->y == snake->pos.y ){

       switch( vbonus( tmp )->type ){
	 
       case '$':
	 snake->point += _BONUS_POINT;
	 
	 if( snake->type == CHAR_SNAKE )
	   set_communicate_numb( "punkty +%d !!!\n", _BONUS_POINT, 0 );
	 else
	   set_communicate_numb( "+%d pkt dla wroga :(\n", _BONUS_POINT, 0 );
	 break;
	 
       case '+':
	 snake->maxl += _BONUS_LENGTH;
	 
	 if( snake->type == CHAR_SNAKE )
	   set_communicate_numb( "Rosniesz o +%d!\n", _BONUS_LENGTH, 0 );
	 else
	   set_communicate_numb( "Wrog rosnie o +%d\n", _BONUS_LENGTH, 0 );
	 break;
	 
       case '-':
	 snake->maxl -= _BONUS_LENGTH;
	 
	 if( snake->type == CHAR_SNAKE )
	   set_communicate_numb( "Malejesz o -%d...\n", _BONUS_LENGTH, 0 );
	 else
	   set_communicate_numb( "Wrog maleje o -%d!\n", _BONUS_LENGTH, 0 );
	 break;
	 
       case 'f':
	 snake->delay -= _BONUS_DELAY;

	 if( snake->type == CHAR_SNAKE )
	   set_communicate_numb( "Twoja predkosc rosnie o +%d\n", _BONUS_DELAY,0);
	 else
	   set_communicate_numb( "Wrog jest coraz szybkszy +%d\n", _BONUS_DELAY,0);
	 break;
	 
       case 's':
	 snake->delay += _BONUS_DELAY;

	 if( snake->type == CHAR_SNAKE )
	   set_communicate_numb( "Predkosc weza -%d...\n", _BONUS_DELAY, 0 );
	 else
	   set_communicate_numb( "Wrog zwalnia!!! -%d\n", _BONUS_DELAY, 0 );
	 break;
	 
       case 'c':

	 clearlist( &( snake->tail ) );
	 snake->l = 0;
       	 clearlist( &( prizes->list ) );
	 prizes->am = 1;
	 
	 draw_border();

	 if( snake->type == CHAR_SNAKE )
	   set_communicate( "WYCZYSZCZONO POLE!\n" );
	 else
	   set_communicate( "Twoj wrog wyczyscil pole!\n" );
	 
	 break;
	 
       case 'm':
	 prizes->maxam += _BONUS_AMOUNT;
	 
	 set_communicate_numb( "ilosc bonusow +%d\n", _BONUS_AMOUNT, 0 );
	 break;
	 
       case 'l':
	 prizes->maxam -= _BONUS_AMOUNT;
	 
	 set_communicate_numb( "ilosc bonusow: -%d\n", _BONUS_AMOUNT, 0 );
	 break;
	 
       case 'r':
	 snake->maxl = _SNAKE_MAXLENGTH;
	 snake->delay = _SNAKE_DELAY;
	 prizes->maxam = _PRIZES_MAX;
	 prizes->delay = _PRIZES_DELAY;

	 if( snake->type == CHAR_SNAKE )
	   set_communicate( "zresetowano bonusy\n" );
	 else
	   set_communicate( "zresetowania bonusy twojego przeciwnika\n");

	 break;
	 
       default:	 fprintf( stderr, "nie ma takiego bonusu" );   break;
       };

       rmelementlist( &( prizes->list ), tmp ); //usuwanie najechanego elementu
       prizes->am--;
       
       return true;
     }

     tmp = tmp->next; //szukaj dalej
   }
   
   return false;
 }

#endif
