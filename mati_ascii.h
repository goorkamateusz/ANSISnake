/* naglowek zawierajacy symbole ASCI 'po polskiemu'
   Mateusz Gorka

   UWAGI:
          - nie czyta pojedynczego ESC, ale zatrzymuje sie na nim
*/

#ifndef MATI_ASCII_H
#define MATI_ASCII_H

/*
  stale poprzedzone _ oznaczaja ze skladaja sie z kilku bajtow
  '_'  - przed:  ESC [
  '__' - przed:  ESC [
       - po:     ~
*/

#define ESC     27
#define TAB     9
#define ENTER   10

#define _UP    65
#define _DOWN  66
#define _RIGHT 67
#define _LEFT  68

#define _HOME  72
#define __PGUP 53
#define __PGDN  54
#define _END    70

#define __DELETE   51
#define BACKSPACCE 127

//stale dla m_stdinput():

#define keyUP    321 //_UP + 256 
#define keyDOWN  322
#define keyRIGHT 323
#define keyLEFT  324

#define keyHOME  328 // + 256
#define keyPGUP  309
#define keyPGDN  310
#define keyEND   326

#define keyDELETE 307 // + 256


#endif
