#ifndef skei_isaac_included
#define skei_isaac_included
//----------------------------------------------------------------------

// TODO: check, duble-check, triple-check..

/*
  readable.c: My random number generator, ISAAC.
  (c) Bob Jenkins, March 1996, Public Domain
  You may use this code in any way you wish, and it is free.  No warrantee.
  * May 2008 -- made it not depend on standard.h
*/

//----------

/*
  http://www.burtleburtle.net/bob/rand/isaacafa.html
  http://www.burtleburtle.net/bob/c/readable.c

  http://en.wikipedia.org/wiki/ISAAC_%28cipher%29

  It uses an array of 256 four-octet integers as the internal state,
  writing the results to another 256 four-octet integer array,
  from which they are read one at a time until empty,
  at which point they are recomputed.

  http://www.cs.rit.edu/~ark/spring2012/482/team/u3/presentation1.pdf
  http://www.cs.rit.edu/~cmd9625/CryptographyTeamReport.pdf
  http://stackoverflow.com/questions/14420754/isaac-cipher-in-php
  http://ccodearchive.net/info/isaac.html

*/

//----------

#include <stdio.h>
#include <stddef.h>
//#include "skei_types.h"   // uint32
//#include "skei_memory.h"  // SMemset

//----------------------------------------------------------------------

#define isaac_mix(a,b,c,d,e,f,g,h) {  \
  a^=b<<11; d+=a; b+=c;               \
  b^=c>>2;  e+=b; c+=d;               \
  c^=d<<8;  f+=c; d+=e;               \
  d^=e>>16; g+=d; e+=f;               \
  e^=f<<10; h+=e; f+=g;               \
  f^=g>>4;  a+=f; g+=h;               \
  g^=h<<8;  b+=g; h+=a;               \
  h^=a>>9;  c+=h; a+=b;               \
}

//----------

class SIsaac {
  private:
    // internal state
    uint32 mm[256];
    uint32 aa, bb, cc;
  protected:
    // external results
    uint32 randrsl[256];
    uint32 randcnt;

  public:

    uint32  getCount(void)       { return randcnt; }
    uint32* getResults(void)     { return randrsl; }
    uint32  getResult(int index) { return randrsl[index]; }

    uint32  getNext(void) {
      uint32 res = randrsl[randcnt];
      randcnt++;
      if (randcnt > 255) {
        process();
        randcnt = 0;
      }
      return res;
    }

  public:

    SIsaac() {
      //aa=0;
      //bb=0;
      //cc=0;
      //randcnt = 0;
      //SMemset(mm,0,256*sizeof(uint32));
      //SMemset(randrsl,0,256*sizeof(uint32));
      //uint32 i,j;
      aa = bb = cc = 0;
      for (int i=0; i<256; ++i) mm[i] = randrsl[i] = 0;
      init(1);
    }

    void process() {
      register uint32 i,x,y;
      cc = cc + 1;    /* cc just gets incremented once per 256 results */
      bb = bb + cc;   /* then combined with bb */
      for (i=0; i<256; ++i) {
        x = mm[i];
        switch (i%4) {
          case 0: aa = aa^(aa<<13); break;
          case 1: aa = aa^(aa>> 6); break;
          case 2: aa = aa^(aa<< 2); break;
          case 3: aa = aa^(aa>>16); break;
        }
        aa              = mm[(i+128)%256] + aa;
        mm[i]      = y  = mm[(x>> 2)%256] + aa + bb;
        randrsl[i] = bb = mm[(y>>10)%256] + x;
        /*
          Note that bits 2..9 are chosen from x but 10..17 are chosen from y.
          The only important thing here is that 2..9 and 10..17 don't overlap.
          2..9 and 10..17 were then chosen for speed in the optimized version (rand.c)
          See http://burtleburtle.net/bob/rand/isaac.html
          for further explanations and analysis.
        */
      }
    }

    void init(int flag) {
      int i;
      uint32 a,b,c,d,e,f,g,h;
      aa = bb = cc = 0;
      a = b = c = d = e = f = g = h = 0x9e3779b9; // the golden ratio
      // scramble it
      for (i=0; i<4; ++i) {
        isaac_mix(a,b,c,d,e,f,g,h);
      }
      // fill in mm[] with messy stuff
      for (i=0; i<256; i+=8) {
        if (flag) {
          // use all the information in the seed
          a+=randrsl[i  ]; b+=randrsl[i+1]; c+=randrsl[i+2]; d+=randrsl[i+3];
          e+=randrsl[i+4]; f+=randrsl[i+5]; g+=randrsl[i+6]; h+=randrsl[i+7];
        }
        isaac_mix(a,b,c,d,e,f,g,h);
        mm[i  ]=a; mm[i+1]=b; mm[i+2]=c; mm[i+3]=d;
        mm[i+4]=e; mm[i+5]=f; mm[i+6]=g; mm[i+7]=h;
      }
      if (flag) {
        // do a second pass to make all of the seed affect all of mm
        for (i=0; i<256; i+=8) {
          a+=mm[i  ]; b+=mm[i+1]; c+=mm[i+2]; d+=mm[i+3];
          e+=mm[i+4]; f+=mm[i+5]; g+=mm[i+6]; h+=mm[i+7];
          isaac_mix(a,b,c,d,e,f,g,h);
          mm[i  ]=a; mm[i+1]=b; mm[i+2]=c; mm[i+3]=d;
          mm[i+4]=e; mm[i+5]=f; mm[i+6]=g; mm[i+7]=h;
        }
      }
      process();        // fill in the first set of results
      //randcnt = 256;  // prepare to use the first set of results
      randcnt = 0;
    }


};

//----------------------------------------------------------------------
#endif


/*

// a ub4 is an unsigned 4-byte quantity
//typedef  unsigned long int  ub4;

//----------

// external results
uint32 randrsl[256];
uint32 randcnt;

// internal state
static uint32 mm[256];
static uint32 aa=0, bb=0, cc=0;

//----------

void isaac() {
  register uint32 i,x,y;
  cc = cc + 1;    // cc just gets incremented once per 256 results
  bb = bb + cc;   // then combined with bb
  for (i=0; i<256; ++i) {
    x = mm[i];
    switch (i%4) {
      case 0: aa = aa^(aa<<13); break;
      case 1: aa = aa^(aa>> 6); break;
      case 2: aa = aa^(aa<< 2); break;
      case 3: aa = aa^(aa>>16); break;
    }
    aa              = mm[(i+128)%256] + aa;
    mm[i]      = y  = mm[(x>> 2)%256] + aa + bb;
    randrsl[i] = bb = mm[(y>>10)%256] + x;
    // Note that bits 2..9 are chosen from x but 10..17 are chosen from y.
    // The only important thing here is that 2..9 and 10..17 don't overlap.
    // 2..9 and 10..17 were then chosen for speed in the optimized version (rand.c)
    // See http://burtleburtle.net/bob/rand/isaac.html
    // for further explanations and analysis.
  }
}

// if (flag!=0), then use the contents of randrsl[] to initialize mm[].

#define mix(a,b,c,d,e,f,g,h) {  \
  a^=b<<11; d+=a; b+=c;         \
  b^=c>>2;  e+=b; c+=d;         \
  c^=d<<8;  f+=c; d+=e;         \
  d^=e>>16; g+=d; e+=f;         \
  e^=f<<10; h+=e; f+=g;         \
  f^=g>>4;  a+=f; g+=h;         \
  g^=h<<8;  b+=g; h+=a;         \
  h^=a>>9;  c+=h; a+=b;         \
}

//----------

void randinit(int flag) {
  int i;
  uint32 a,b,c,d,e,f,g,h;
  aa = bb = cc = 0;
  a = b = c = d = e = f = g = h = 0x9e3779b9; // the golden ratio
  // scramble it
  for (i=0; i<4; ++i) {
    mix(a,b,c,d,e,f,g,h);
  }
  // fill in mm[] with messy stuff
  for (i=0; i<256; i+=8) {
    if (flag) {
      // use all the information in the seed
      a+=randrsl[i  ]; b+=randrsl[i+1]; c+=randrsl[i+2]; d+=randrsl[i+3];
      e+=randrsl[i+4]; f+=randrsl[i+5]; g+=randrsl[i+6]; h+=randrsl[i+7];
    }
    mix(a,b,c,d,e,f,g,h);
    mm[i  ]=a; mm[i+1]=b; mm[i+2]=c; mm[i+3]=d;
    mm[i+4]=e; mm[i+5]=f; mm[i+6]=g; mm[i+7]=h;
  }
  if (flag) {
    // do a second pass to make all of the seed affect all of mm
    for (i=0; i<256; i+=8) {
      a+=mm[i  ]; b+=mm[i+1]; c+=mm[i+2]; d+=mm[i+3];
      e+=mm[i+4]; f+=mm[i+5]; g+=mm[i+6]; h+=mm[i+7];
      mix(a,b,c,d,e,f,g,h);
      mm[i  ]=a; mm[i+1]=b; mm[i+2]=c; mm[i+3]=d;
      mm[i+4]=e; mm[i+5]=f; mm[i+6]=g; mm[i+7]=h;
    }
  }
  isaac();        // fill in the first set of results
  randcnt = 256;  // prepare to use the first set of results
}

*/

//----------

/*
int main() {
  uint32 i,j;
  aa = bb = cc = (uint32)0;
  for (i=0; i<256; ++i) mm[i] = randrsl[i] = (uint32)0;
  randinit(1);
  for (i=0; i<2; ++i) {
    isaac();
    for (j=0; j<256; ++j) {
      printf("%.8lx",randrsl[j]);
      if ((j&7)==7) printf("\n");
    }
  }
}
*/
