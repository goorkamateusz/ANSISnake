/* naglowek z funkcja getch i ustawieniami termios
   Gorka Mateusz

   Przezonaczona na systemy Linux.

   !!! UWAGI:
      - NIE RADZI SOBIE Z WCZYTYWANIEM POJEDYNCZYCH ESC
      - NIE CZYTA KLAWISZY 3 BAJTOWYCH 
*/
#ifndef MATI_GETCH_H
#define MAIT_GETCH_H

#include <termios.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include "mati_ascii.h"

// GLOBAL -------------------------------------------------
static struct termios tOld, tNew; //stuktury Termios

// FUNCTIONS ----------------------------------------------
void m_setTermios( void );   //ustawia strukture termios zgodnie z wymaganiami
void m_resetTermios( void ); //resetuje strukture termos do pierwonych ustawien

char m_nacisniecie( void );  //sprawdza czy nacisniety guzik

char m_getch( void );
int  m_stdinput( void );

// DECLARATIONS -------------------------------------------
//ustawienia struktury Termios
void m_setTermios( void ){
  
  tcgetattr( 0, & tOld );
  tNew = tOld;                //kopia settings
  
  //nowe znaki ukladane w wiersze, nadanie wartosci dla znakow scierajacych
  tNew.c_lflag &= ~ICANON;

  //wylaczenie funkcji echo
  tNew.c_lflag &= ~ECHO;

  //ustawienie szybkosci transmisji danych
  //  tNew.c_lflag |= B9600;

  //automatyczna zmiana wielkich liter na male
  //  tNew.c_iflag |= IUCLC;
  
  tcsetattr( 0, TCSANOW, & tNew ); //zaladowanie nowej struktury 
}

void m_resetTermios(){
  tcsetattr( 0, TCSANOW, & tOld ); //reset ustawienia struktury termios
}


//zamiennik dla kbhit
char m_nacisniecie(){
  int byteswaiting;
  
  ioctl( 0, FIONREAD, & byteswaiting );
  
  return byteswaiting;
}

//funkcja getch
char m_getch(){
  char c;
  
  m_setTermios();
  
  c = getchar();
  
  m_resetTermios();
  
  return c;
}

int m_stdinput(){
  int c;

  if( ( c = getchar() ) == ESC ){
    if( ( c = getchar() ) == '[' ){
      
      c = getchar() + 256;
      fflush( stdin ); // dla znakow dluzszych by wyeliminowac ~ 

      return c;
    }
    else
      return ESC;
  }
  else return c;
}

#endif
