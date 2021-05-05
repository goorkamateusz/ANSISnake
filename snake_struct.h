#ifndef SNAKE_STRUCT_H
#define SNAKE_STRUCT_H

#include <stdlib.h>
#include "mati_listy.h"
#include "mati_bool.h"
#include "snake_config.h"

#define V_CLASS char //typ wspolzednych w wektorach

/* strcut m_vector => mvector -------------------------------------------- */
 typedef struct m_vector {
   V_CLASS x;
   V_CLASS y;
 } mvector;

/* --- FUNCTIONS --------------------------------------------------------- */
void set_vector( mvector *vector, V_CLASS x, V_CLASS y ); 
void *mvectortovoid( mvector vektor );
mvector *vpos( mlist *element_listy );    //zwraca wartosc wektora z listy

/* struct snake --------------------------------------------------------- */
 struct snake {
   struct m_vector pos;     //pozycja
   struct m_vector v;       //vektor

   char type;

   unsigned int delay;      //opoznienie wykonania w turach pentli

   mlist *tail;
   short int l;             //dlugosc snake
   short int maxl;          //maksymalna dlugosc ogona

   short int point;         //ilosc punktow
 };

/* --- FUNCTIONS --------------------------------------------------------- */
void defaultsnake( struct snake *snake ); //ustawienia poczatkowe snake
void rubber( mlist element_listy );       //przekazuje do draw w celu wymazania
void snake_move( struct snake *snake );   //przesuwa snaka
bool snake_turn( struct snake *snake, V_CLASS x, V_CLASS y ); //sprawdza czy obrot jest mozliwy
bool islocationfreesnake( struct snake *snake, int x, int y ); //sprawdza czy lokalizacja nie jest zajeta przez snake
bool hitintail( struct snake *snake ); //sprawdza czy snake nie udezyl w ogon
bool gooutside( int x, int y );        //sprawdza punkt nie jest poza plansza
                                       //zwraca 1 jezeli jest poza plansza
                                       //zwraca 0 jezeli nie


/* --- DEFINITIONS --- mvector ------------------------------------------ */
 void set_vector( struct m_vector * v, V_CLASS x, V_CLASS y ){
   v->x = x;
   v->y = y;
 }

 void *mvectortovoid( mvector n ){
   mvector *new = (mvector*) malloc( sizeof( mvector ) );
   *new = n;

   return (void*) new;
 }
 mvector *vpos( mlist *list ){  return (mvector*) list->val;  }


/* --- DEFINITIONS --- struct snake ------------------------------------- */
 void defaultsnake( struct snake *snake ){
   snake->pos.x = WIDTH/2;
   snake->pos.y = HEIGHT/2;

   snake->type  = CHAR_SNAKE;
   
   srand( (long) snake );
   switch( rand()%4 ){
   case 0:  snake->v.x = 0;  snake->v.y = 1;  break;
   case 1:  snake->v.x = 1;  snake->v.y = 0;  break;
   case 2:  snake->v.x = 0;  snake->v.y = -1; break;
   case 3:  snake->v.x = -1; snake->v.y = 0;  break;
   }

   snake->delay = _SNAKE_DELAY;

   snake->tail = 0; //NULL
   snake->l = 0;
   snake->maxl = _SNAKE_MAXLENGTH;

   snake->point = 0;
 }

 void rubber( mlist list ){      //przekazuje do draw by wymazalo
   draw_rubber( ((mvector* )list.val)->x, ((mvector*) list.val)->y );
 }

 void snake_move( struct snake *snake ){
   snake->pos.x += snake->v.x; //przesuniecie snake o v
   snake->pos.y += snake->v.y;

   //dodawaj nowe segmenty na poczatek ogona:
   addnewtolist( &( snake->tail ),
		 mvectortovoid( snake->pos ),
		 1 );

   //wymazuje ogon:
   if( snake->l >= snake->maxl ){
     rubber( rmlastelementlist( &(snake->tail) ) );

     if( snake->l > snake->maxl ){
       rubber( rmlastelementlist( &(snake->tail) ) );
       snake->l--;
     }
   }
   else
     snake->l++;        
 }

 bool snake_turn( struct snake * snake, V_CLASS x, V_CLASS y ){
   
   if( x == -1 * snake->v.x ) return false; //nie pozwala na obrocenie o 180*
   if( y == -1 * snake->v.y ) return false; //-..-

   snake->v.x = x;
   snake->v.y = y;
   
   return true;
 }

 bool islocationfreesnake( struct snake *snake, int x, int y ){
   
   if( snake->tail != NULL ){
     mlist *tmp = snake->tail;

     while( tmp != NULL ){
       if( vpos( tmp )->x == x && vpos( tmp )->y == y ) return false;
       tmp = tmp->next;
     }
   }
   return true;
 }

 bool hitintail( struct snake *snake ){
   // ( najpierwsz przypisuje next, aby pominac 1. element )
   // potem porownuje z NULL (poniewaz 1. element jest rowny aktualnej pozycji)
   
   if( snake->l <= 2 ) return false;
     
   mlist *tmp = snake->tail;

   while( (tmp = tmp->next) != NULL )
     if( vpos( tmp )->x == snake->pos.x && vpos( tmp )->y == snake->pos.y ) return true; //jezeli pozycja = pozycji ogona

   return false;
 }

 bool gooutside( int x, int y ){
   if( ( x < 0 || x >= WIDTH ) || ( y < 0 || y >= HEIGHT) ) return true;
   else return false;
 }

#endif
