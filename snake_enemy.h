#ifndef SNAKE_ENEMY_H
#define SNAKE_ENEMY_H

/* naglowek odpowiedzialny za wroga snaka'e
   Mateusz Gorka

- ai pojawia sie tylko od krawedzi
TODO: - ai moze zamiast znakow byc zbudowany z liter "mateusz"

PLAN DZIALANIA STEROWANIA AI:
  1  ai wybiera losowy kierunek ruchu

  2  if    n kratek do przodu jest bezpieczny
  idzie do 6
  3  else  wybiera nastepny kierunek od losowego

  4  if    skonczyly sie kierunki:  n--
  5  wraca do 2.

  6  ...
*/
#include <stdlib.h>
#include <time.h>
#include "mati_bool.h"
#include "mati_listy.h"
#include "snake_config.h"
#include "snake_struct.h"

#define AMOUNT_TRY 10 
#define DISTANS_TRY 4 /* odleglosc jak do przodu enemy bedzie sprawdzal droge */
#define MIN_LENGTH 10  /* minimalna dlugosc snaka wroga */

/* FUNCTIONS --------------------------------------------------- */
bool create_enemy( struct snake **enemy, struct snake *snake );
/* tworzy snake'a wroga */
bool move_enemy( struct snake **enemy, struct snake *snake );
/* w 'inteligenty' sposob steruje wrogiem */
void dead_enemy( struct snake **enemy );
/* unicestwia obiekt enemy i zeruje *enemy */
/* */

/* DECLARATIONS ------------------------------------------------- */
bool create_enemy( struct snake **enemy, struct snake *snake ){

  short int i = 0, j, tmp;
  bool ok;
  mvector pos, tmppos, v;

  int aa;
  
  do{  //losowanie pozycji poczatkowej i ustawianie wektora
    tmp = rand();
    srand( time( 0 ) + i );
    ok = true;
    
    switch( aa = rand()%4 ){
    case 0: //od gory
      pos.x = tmp%WIDTH;  pos.y = 0;
        v.x = 0;            v.y = 1;
      break;
      
    case 1: //od prawej
      pos.x = WIDTH;  pos.y = tmp%HEIGHT;
        v.x = -1;       v.y = 0;
      break;
    case 2: //od dolu
      pos.x = tmp%WIDTH;  pos.y = HEIGHT;
        v.x = 0;            v.y = -1;
      break;
    case 4: //od lewej
      pos.x = 0;  pos.y = tmp%HEIGHT;
        v.x = 1;    v.y = 0;
    }
    
    /* sprawdza czy DISTANS_TRY razy ma mozliwosc ruchu z tego miejsca */
    tmppos = pos;

    for( j=0; j<DISTANS_TRY; j++ ){ 
      tmppos.x += v.x;
      tmppos.y += v.y;

      if( ! islocationfreesnake( snake, tmppos.x, tmppos.y ) ){
	ok = false;
	break;
      }
    }

    i++;
  } while( !ok && i<AMOUNT_TRY );

  if( !ok ) return false;
  
  /* tworzenie enemy pod wskaznikiem *enemy */
  (*enemy) = (struct snake*) malloc( sizeof( struct snake ) ); //TU 
    
  if( *enemy == NULL ){
    fprintf( stderr, "BRAK MIEJSCA RAM DLA ENEMY\n" );
    return false;
  }

  (*enemy)->pos = pos;
  (*enemy)->v = v;

  (*enemy)->type = 'e';
      
  (*enemy)->l = 0;
  (*enemy)->maxl = ( rand()%(_SNAKE_MAXLENGTH-MIN_LENGTH) )+MIN_LENGTH;
      
  (*enemy)->delay = _SNAKE_DELAY;
  (*enemy)->point = 0;
      
  (*enemy)->tail = 0; //NULL

  set_communicate("Nadchodzi wrog!\n");
  
  return true;
}

bool move_enemy( struct snake **enemy, struct snake *snake ){

  bool ok;
  short int j, i, rand_dir;
  mvector tmppos, new_v = (*enemy)->v;

  short int n = DISTANS_TRY;
  
  /* Wybieranie drogi */  
  do{

    new_v  = (*enemy)->v;   //pierwotny kierunek enemy
    
    srand( time( 0 ) + n );
    rand_dir = rand();      //losowanie drugiego kroku
    
    j = 4;

    do{   //sprawdzanie pozostalych kierunkow

      tmppos = (*enemy)->pos;   //pierwotne wspolzedne
      ok = true;                //zalozenie ze sie uda

      for( i=0; i<n; i++ ){ //sprawdzanie n krokow do przodu

	tmppos.x += new_v.x;
	tmppos.y += new_v.y;

	if(   gooutside( tmppos.x, tmppos.y ) ||
	      ! islocationfreesnake( snake, tmppos.x, tmppos.y ) ||
	      hitintail( *enemy )
	      ){
	  
	  ok = false;       //jezeli nie da sie tam isc
	  break;  
	}
      } //end for

      if( ok ) break;       //wyjscie z petli do while( j ) 
      else{
	
	do{
	  switch( (rand_dir+j)%4 ){
           case 0:  new_v.x =  0;  new_v.y = -1;  break; //gora
	   case 1:  new_v.x =  1;  new_v.y =  0;  break; //prawa
	   case 2:  new_v.x =  0;  new_v.y =  1;  break; //dol
	   case 3:  new_v.x = -1;  new_v.y =  0;  break; //lewa
	  }

	  j--;

	  /* jezeli vektor jest przeciwny od pierwotnego kierunku */
	  if( new_v.x + (*enemy)->v.x == 0 && new_v.y + (*enemy)->v.y == 0 )
	    continue;
	  else
	  /* jezeli vektor jest ten sam co pierwotny kierunek */
	  if( new_v.x == (*enemy)->v.x && new_v.y == (*enemy)->v.y )
	    continue;
	  else
	    break;
	  
	} while( j > 0 );
      }
    } while( j > 0 );
    
    n--;

  } while( n > 0 && ! ok );

  if( ! ok ){
    dead_enemy( enemy );
    
    return false;
  }

  /* nowy kierunek */
  (*enemy)->v = new_v;

  /* przesuniecie */
  snake_move( *enemy );
  
  /* dorysowanie */
  draw_object( (*enemy)->pos.x, (*enemy)->pos.y, (*enemy)->type );

  return true;
}

void dead_enemy( struct snake **enemy ){

  mlist *tmp = (*enemy)->tail;

  while( tmp ){
    draw_rubber( vpos( tmp )->x, vpos( tmp )->y );
    tmp = tmp->next;
  }

  clearlist( &(*enemy)->tail );
  
  free( *enemy );
  *enemy = NULL;

  set_communicate( "Wrog zginal!!!\n" );
}

#endif
