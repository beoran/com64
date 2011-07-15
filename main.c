#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SPRITES 64

int main(void) {
  SDL_Surface * screen        = NULL;
  SDL_Surface * sprites[MAX_SPRITES];
  SDL_Surface * player_sprite = NULL;
  // Friendly message.
  puts("Hello world!");
  // Start SDL
  SDL_Init(SDL_INIT_EVERYTHING);
  screen = SDL_SetVideoMode(640, 480, 0, SDL_HWSURFACE | SDL_ANYFORMAT);
  SDL_Delay(1000);
  
  // Quit SDL
  SDL_Quit(); 
  return 0;
}


