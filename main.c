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
  screen = SDL_SetVideoMode(640, 480, 0, SDL_HWSURFACE | SDL_ANYFORMAT);
  
  // Quit SDL
  SDL_Quit(); 
  return 0;
}


