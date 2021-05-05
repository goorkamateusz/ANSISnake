#ifndef SNAKE_DRAW_H
#define SNAKE_DRAW_H

#include "snake_config.h"

#define SET_CHANGE change = 1;

//GLOBAL:
char gfield[ HEIGHT_ALL ][ WIDTH_ALL ]; //pole gry
char *communicate;                      //komunikat
int gnumb0, gnumb1; 

char change = 0; //przechowuje dane o tym czy zostala zarejestrowana zmiana
unsigned short int reset_com = 0; //sluzy do odliczania do resetu komunikatu

/* FUNCTIONS:   --- */
void draw_border(void);
void draw_object( int x, int y, char znak );
void draw_rubber( int x, int y ); // znak == CHAR_BACKGROUND
void draw_snake( int x, int y );  // znak == CHAR_SNAKE

void set_comunicate_numb( char *txt, int numb0, int num1 );
void set_comunnicate( char *txt );
void reset_communicate_numb( char *txt, int numb0, int numb1 );
void reset_communicate( char *txt );

void draw( void );
void drawif( void );

/* DEFINITIONS: --- */
//DRAWING A FIELD:
 void draw_border( void ){
   for( short int yy = 0; yy < HEIGHT_ALL; yy++ ){

     if( ( yy < BORDER && yy >= 0 ) ||
	 ( yy >= HEIGHT_ALL-BORDER && yy < HEIGHT_ALL )
       ){
       
       for( short int xx = 0; xx < WIDTH_ALL; xx++ )
	 gfield[ yy ][ xx ] = CHAR_BORDER;          
     }
     else
       for( short int xx = 0; xx < WIDTH_ALL; xx++ ){

	 if( xx < BORDER )
	   gfield[ yy ][ xx ] = CHAR_BORDER; //lewa strona
	
	 else if( xx > WIDTH_ALL-BORDER-2 )  
	   gfield[ yy ][ xx ] = CHAR_BORDER; //prawa strona
	
	 else
	   gfield[ yy ][ xx ] = CHAR_BACKGROUND;
       }
    
     gfield[yy][WIDTH_ALL-1] = '\n';
   }
   SET_CHANGE
 }

 void draw_object( int x, int y, char c ){ // x, y - position of object 
   gfield[ y+BORDER ][ x+BORDER ] = c;
   SET_CHANGE
 }
 void draw_rubber( int x, int y ){ draw_object( x, y, CHAR_BACKGROUND ); }
 void draw_snake( int x, int y ){ draw_object( x, y, CHAR_SNAKE ); }

//COMMUNICATE - ustawianie komunikatu
 void set_communicate_numb( char *txt, int numb0, int numb1 ){
   communicate = txt;
   gnumb0 = numb0;
   gnumb1 = numb1;
   SET_CHANGE
 }
 void set_communicate( char *txt ){ set_communicate_numb( txt, 0, 0 ); }

 void reset_communicate_numb( char *txt, int numb0, int numb1 ){
   if( reset_com > _RESET_COMMUNICATE ){
     set_communicate_numb( txt, numb0, numb1 );
     reset_com = 0;
   }
   else reset_com++;
 }
 void reset_communicate( char *txt ){ reset_communicate_numb( txt, 0, 0 ); }

//DRAW
void draw(void){

  system( "clear" );   //czyszczenie okna
  printf( *gfield );     //wyswietlanie pola
  printf( communicate, gnumb0, gnumb1 ); //wyswietlanie komunikatu
}

void drawif(void){  
  if( change ){
    draw();
    change = 0;
  }
}

#endif
