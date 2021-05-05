/* naglowek zawierajacy definicje list jednokierunkowych
   Mateusz Gorka

   funckcje nazwane jako:
         typ-zwracany nazwa-po-angl "list(" {arg} ")"
*/

#ifndef MATI_LISTY_H
#define MATI_LISTY_H

#include <stdlib.h>
#include "mati_bool.h"

//LISTA  -----------------------------------------
typedef struct m_list {
  
  void * val;
  struct m_list *next;
  
} mlist;

//FUNCIONS  -------------------------------------
//dodawanie nowych elementow
char addnewtolist( mlist** lista, void* wartoscNowegoElementu, char czyNaPoczatek ); //char <-> bool
void addtobeginlist( mlist** lista, mlist* nowyElement );
void addtoendlist( mlist** lista, mlist* nowyElement );

//funkcje sprawdzajace
char isemptylist( mlist *lista_lub_element );
 //czy element jest rowny zero

//funkcje zwracajace
mlist *lastelementlist( mlist **lista );
mlist *firstelementlist( mlist **lista );
void  *valueoflist( mlist *elementListy );
 //zwraca wskaznik void na wartosc elementu

//funkcje czyszczace         (rm - remove )
mlist rmlastelementlist( mlist **lista );
mlist rmfirstelementlist( mlist **lista );
bool rmelementlist( mlist **lista, mlist *elementListy );
 //zwraca 1 jak usuwanie sie uda, 0 jak nie
void clearlist( mlist **lista );
 //czysci cala liste

//poruszanie sie po liscie
mlist *nextlist( mlist *lista );
mlist *mvlist( mlist *lista, char czy_od_poczatku );
 //UWAGA KORZYSTA Z STATIC!
 //zwraca wartosc elementu pierwszego i przesuwa na kolejny
 //jezeli dojdzie do konca zwraca NULLx
void *valmvlist( mlist *list, char czy_od_poczatku );
 //to co mvlist, ale zwraca wartosc el. listy lub NULL



//DEFINITIONS ----------------------------------
//FUNCTIONS ADD
void addtobeginlist( mlist **list, mlist *new ){
  new->next = (*list);
  (*list) = new;
}

void addtoendlist( mlist **list, mlist *new ){

  while( (*list) != NULL )         //leci na sam koniec
    list = & ( (*list)->next );    //

  new->next = NULL; 
  (*list) = new;
}

char addnewtolist( mlist **list, void *nval, char begin ){ //(char <-> bool)
  
  mlist *new = ( mlist* ) malloc( sizeof( mlist ) );

  if( new == NULL ) return 0;  //blad f. malloc
                               //brak przydzielonego miejsca RAM

  new->val = nval;             //przypisanie obiektu wskaznikowi void *val

  if( (*list) == NULL ){       //dla pustych list
    new->next = NULL;
    (*list) = new;
  }
  else
    if( begin ) addtobeginlist( list, new );
    else        addtoendlist  ( list, new );
  
  return 1;
}

//BOOL FUNCTION
char isemptylist( mlist *list ){ //(char / bool )
  if( list == NULL ) return 1;
  return 0;
}

//RETURN FUNCTION
mlist *lastelementlist( mlist **list ){
  if( isemptylist( *list ) ) return NULL;
  
  do{
    list = & (*list)->next;
  }while( (*list)->next != NULL );
  
  return *list;
}

mlist *firstelementlist( mlist **list ){
  if( isemptylist( *list ) ) return NULL;
  return (*list);
}

void *valueoflist( mlist *list ){ return list->val; }

//REMOVE FUNCTION
mlist rmlastelementlist( mlist **list ){
  
  mlist *prelast, *wsk = *list, tmp;

  while( wsk->next != NULL ){
    prelast = wsk;
    wsk = wsk->next;
    
    tmp = *wsk;
  }

  free( wsk );
  prelast->next = NULL;
  
  return tmp;
}

bool rmelementlist( mlist **list, mlist *el ){

  if( el == NULL || *list == NULL )
    return false;
  
  if( (*list) != el ){
    mlist *wsk = *list;
    
    while( wsk->next != el )
      wsk = wsk->next;

    wsk->next = el->next;
  }
  else
    (*list) = el->next;    
 
  free( el );
  return true;
}

mlist rmfirstelementlist( mlist **list ){ //TODO
  mlist tmp = **list;
  rmelementlist( list, *list );
  return tmp;
}


void clearlist( mlist **list ){
  while( rmelementlist( list, *list ) );
}

//SERCHING FUNCION
mlist *nextlist( mlist *list ){
  return list != NULL ? list->next : NULL;
}

mlist *mvlist( mlist *list, char reset ){
  static mlist *current;
  static char start = 1;

  if( start || reset ){
    if( list != NULL ){  current = list->next;
                         start = 0; }
    else start = 1;
  }
  else if( current != NULL ){
    list = current;
    current = current->next;
  }
  else{
    list = current = NULL;
    start = 1;
  }
  
  return list;
}

void *valmvlist( mlist *list, char reset ){
  list = mvlist( list, reset );
  return list != NULL ? list->val : NULL;
}

#endif
