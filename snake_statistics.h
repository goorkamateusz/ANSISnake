/* naglowek do gry snake
   Gorka Mateusz

budowa pliku snake_stat.txt
budowa lini (MBNF)
  r "." m "." d " " h ":" m nick " p:" liczba " l:" liczba " s" liczba " b:" liczba

po kolei rekordy z dziedzin w liniach:
  0 max point
  1 max length
  2 max speed
  3 max amount  
*/

#ifndef SNAKE_STATISTICS_H
#define SNAKE_STATISTICS_H

#include <stdio.h>
#include <time.h>
#include "mati_bool.h"
#include "mati_listy.h"
#include "snake_config.h"

//STRUCT record ------------------------------
typedef struct record{
  unsigned int year, mon, day;
  unsigned int hour, min;
  
  char nick[100];

  int point, l, speed;
  int p_am;
} record;

//FUNCTIONS ----------------------------------
void *recordtovoid( record record );
void printrecord( FILE *plik, record * record );
void importrecord( FILE *plik, mlist **lista );

void save_record( record *nowy, mlist *lista ); //zapisuje nowy rekord do pliku

//do wykorzystania w funkcji glownej:
void check_stat( struct snake *snake, struct prizes *prizes );
 //sprawdza czy wynik gry wpisac do statystyk
void show_stat( void );
 //wyswietla statystyki


//DEFINITIONS --------------------------------
void *recordtovoid( record n ){
  record *new = (record*) malloc( sizeof( record ) );
  *new = n;

  return (void*) new;
}

record *valrecord( mlist *listel ){
  return listel != NULL ? (record*) listel->val : NULL;
}

void printrecord( FILE *stream, record *r ){
 
  fprintf( stream, "%u.%u.%u %u:%u ",     //zapisywanie czasu
	   r->year, r->mon, r->day,
	   r->hour, r->min );

  fprintf( stream, "%s ", r->nick );      //zapisywanie nicku

  fprintf( stream, "p:%d l:%d s:%d ",     //zapisywanie weza
	   r->point, r->l, r->speed  );

  fprintf( stream, "b:%d\n", r->p_am );   //zapisywanie bonusow	   
}

void importrecord( FILE *file, mlist **list ){
  record n;

  while( ! feof( file ) )
    if(   fscanf( file, "%u.%u.%u %u:%u ",
		  &(n.year), &(n.mon), &(n.day),
		  &(n.hour), &(n.min) )
	+ fscanf( file, "%s ", (n.nick) )
        + fscanf( file, "p:%d l:%d s:%d ",
		  &(n.point), &(n.l), &(n.speed) )
	+ fscanf( file, "b:%d\n", &(n.p_am) )
	  
	  == 5+1+3+1 //sprawdza czy wszystkie dane sa dobrze wczytane
      ){
      addnewtolist( list, recordtovoid( n ), 0 );
    }
}

void save_record( record *new, mlist *list ){

  #define amount_record 4

  FILE *file = fopen( "snake_stat.txt", "w" );
  if( file == NULL ){
    fprintf( stderr, "ERR file null save_record");
    return; //error
  }
  record *old;
  char i = 0;

  if( list == NULL ){ //jezeli plik jest pusty, nie istnial, zapelnianie got
    printf( "Jestes pierwszym zarejestrowanym graczem!\n"
	    "Podaj nick: " );
    
    scanf( "%s", new->nick ); //dodawanie nicku
    
    for(; i < amount_record; i++ )
      printrecord( file, new );
  }
  else{
    printf( "WOW! Pobiles rekord chcemy cie wpisac na liste!\n"
	    "Podaj nick: " );
      
    scanf( "%s", new->nick ); //dodawanie nicku
    
    valmvlist( NULL, 1 ); //reset static mvlist
    
    while( ( old = (record*) valmvlist( list, 0 ) ) != NULL && i<amount_record){
      
      switch( i ){ //dodawanie plikow zgodnie z kolejka
      case 0:  if( new->point > old->point ) old = NULL;  break;
      case 1:  if( new->l > old->l )         old = NULL;  break;
      case 2:  if( new->speed > old->speed ) old = NULL;  break;
      case 3:  if( new->p_am > old->p_am )   old = NULL;  break;
      }
    
      if( old == NULL ) printrecord( file, new );
      else              printrecord( file, old );
      
      i++;
    }
  }

  fclose( file );
  clearlist( & list );
}

void check_stat( struct snake *snake, struct prizes *prizes ){

  //ladowanie/tworzenie pliku:
  FILE *file = fopen( "snake_stat.txt", "r" );

  record new;
  { //tworzenie strukut nowego rekordu
    time_t rawtime;         //data:
    time( &rawtime );

    struct tm *t = localtime( &rawtime );

    new.year = t->tm_year;  //nowy rekord: (Bez nicku)
    new.mon = t->tm_mon;
    new.day = t->tm_mday;
    new.hour = t->tm_hour;
    new.min = t->tm_min;
  
    new.point = snake->point;
    new.l = snake->l;
    new.speed = speedpersecond( snake->delay );
  
    new.p_am = prizes->am;
  }

  if( file == NULL ){ //jezeli nie ma jeszcze pliku
    
    save_record( &new, NULL );
  }
  else{              //odczyt z pliku
    mlist *list = NULL, *current, *tmp;
	  
    //import rekordow:
    importrecord( file, &list );
    fclose( file );

    //sprawdzanie czy jest rekordem:
    
    current = list;
    if( new.point > valrecord(current)->point ){
      save_record( &new, list );
      return;
    }
    
    if( (tmp = nextlist( current )) != NULL ) current = tmp;
    if( new.l > valrecord(current)->l ){
      save_record( &new, list );
      return;
    }

    if( (tmp = nextlist( current )) != NULL ) current = tmp;
    if( new.speed > valrecord(current)->speed ){
      save_record( &new, list );
      return;
    }

    if( (tmp = nextlist( current )) != NULL ) current = tmp;
    if( new.p_am > valrecord(current)->p_am ) save_record( &new, list );
  }
}

void show_stat(){

  printf("\n\n--- rekordy: ---\n\n");

  //ladowanie pliku:
  FILE *file = fopen( "snake_stat.txt", "r" );
  if( file == NULL ){
    printf( "blad: brak pliku" );    
    return;
  }

  mlist *list, *tmp;
  
  //import rekordow
  importrecord( file, &list );

  //wyswietlanie rekordow:
  printf( "--- Punktowy rekord zdobyl(a):  %-20s  czyli:        %-3d  pkt!!!\n",
	  valrecord(list)->nick, valrecord(list)->point );

  if( (tmp = nextlist( list )) != NULL ) list = tmp;
  printf( "--- Najdluzszego weza mial(a):  %-20s  o dlugosci:   %-3d  kratek\n",
	  valrecord(list)->nick, valrecord(list)->l );
      
  if( (tmp = nextlist( list )) != NULL ) list = tmp;
  printf( "--- Najszybszybciej pelza:      %-20s  z szybkoscia: %-3d  kratek/s\n",
	  valrecord(list)->nick, valrecord(list)->speed );
      
  if( (tmp = nextlist( list )) != NULL ) list = tmp;
  printf( "--- Najwiecej bonusow mial:     %-20s  to znaczy:    %-3d  bonusow na planszy\n",
	  valrecord(list)->nick, valrecord(list)->p_am );

  fclose( file );
  //czyszczenie listy sie nie uda bo jest przesunieta na koniec
  //i czy czysznie listy jest wgl potrzebne tutaj?
}

#endif
