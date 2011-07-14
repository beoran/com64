#include <SDL.h>
#include <stdio.h>

#define MAX_SPRITES 64

int main(void) {
  SDL_Surface * screen;
  SDL_Surface * sprites[MAX_SPRITES];
  // Friendly message.
  puts("Hello world!");
  // Start SDL
  SDL_Init(SDL_INIT_EVERYTHING);
  
  // Quit SDL
  SDL_Quit(); 
  return 0;
}


