#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SPRITES 64

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

int done = FALSE;
  
/* A function that passes it's argument by value . */ 
int by_value(int a) { 
  a = a + 10; 
  return a; 
}
   
/* A function that passes it's argument by address . */
int by_address(int * a) { 
  // (*a) = (*a) + 10;
  printf("In by_address: a: %p, (*a): %i\n", a, *a);
  (*a) = 10; 
  return (*a); 
}   
   

int main(void) {
  int a = 2;
  int b = 0;
  SDL_Surface * screen        = NULL;
  SDL_Surface * sprites[MAX_SPRITES];
  SDL_Surface * player_sprite = NULL;
  int jones; 
  int * pointer_to_jones; 
  jones            = 127; 
  pointer_to_jones = &jones;
  
  printf("jones: %d, pointer to jones: %p, follow pointer_to_jones: %d\n", jones, pointer_to_jones, (*pointer_to_jones));
   
  // Friendly message.
  puts("Hello world!");
  b = by_value(a);
  printf("a: %d, b:%d, address of a: %p, address of b: %p\n", a, b, &a, &b);
  b = by_address(&a);
  b = by_address(&b);
  printf("a: %d, b:%d \n", a, b);  
  
  
  // Start SDL
  SDL_Init(SDL_INIT_EVERYTHING);
  screen = SDL_SetVideoMode(640, 480, 0, SDL_HWSURFACE | SDL_ANYFORMAT);
  SDL_Delay(1000);
  
  while(!done) {
    SDL_Event event;
    /* Check for events */
    /* Loop until there are no events left on the queue */
    while(SDL_PollEvent(&event)) { 
      switch(event.type) { /* Process the appropriate event type */
      case SDL_QUIT:  /* Handle a KEYDOWN event */
        done = TRUE;
      break;
      default: /* Report an unhandled event */
        ;
        // printf("I don't know what this event is!\n");
      }
    }
  }
 
  
  // Quit SDL
  SDL_Quit(); 
  return 0;
}


