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
int by_address(int * pointer) { 
  // (*a) = (*a) + 10;
  printf("In by_address: pointer: %p, (*pointer): %i\n", pointer, *pointer);
  (*pointer) = 10; 
  return (*pointer); 
}   

SDL_Surface* image_load_colorkey(char * filename, int r, int g, int b)  {
  Uint32 colorkey        = 0;
  SDL_Surface* image     = NULL;
  SDL_Surface* optimized = NULL;
  image                  = SDL_LoadBMP(filename);
  if(!image) return NULL;
  optimized              = SDL_DisplayFormat(image);
  SDL_FreeSurface(image);
  colorkey               = SDL_MapRGB(optimized->format, r, g, b);
  SDL_SetColorKey(optimized, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
  return optimized;
}


int main(void) {
  int           a = 2;
  int           b = 0;
  SDL_Surface * screen         = NULL;
  SDL_Surface * sprites[MAX_SPRITES];
  SDL_Surface * player_sprite  = NULL;
  int           player_speed_x = 0;
  int           player_speed_y = 0;
  SDL_Rect      player_rect;
  Uint32        colorkey, black; 
  int           jones; 
  int         * pointer_to_jones;
   
  jones            = 127; 
  pointer_to_jones = &jones;  
  printf("jones: %d, pointer to jones: %p, follow pointer_to_jones: %d\n", 
          jones    , pointer_to_jones    , (*pointer_to_jones));
  
  // Friendly message.
  puts("Hello world!");
  b = by_value(a);
  printf("a: %d, b:%d, address of a: %p, address of b: %p\n", a, b, &a, &b);
  b = by_address(&a);
  b = by_address(&b);
  printf("a: %d, b:%d \n", a, b);  
  
  
  // Start SDL
  SDL_Init(SDL_INIT_EVERYTHING);
  screen        = SDL_SetVideoMode(640, 480, 0, 
                  SDL_FULLSCREEN | SDL_HWSURFACE | SDL_ANYFORMAT);
  black         = SDL_MapRGB(screen->format, 0, 0, 0);
  
  player_sprite = image_load_colorkey("player_1.bmp", 255, 0, 255);
/*player_sprite = SDL_LoadBMP("player_1.bmp");
  if(!player_sprite) {
    puts("Could not load player sprite!");
    return 1;
  }
  colorkey   = SDL_MapRGB(player_sprite->format, 255, 0, 255 );
  SDL_SetColorKey(player_sprite, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey);
*/
  player_rect.x = 320;
  player_rect.y = 240;
  player_rect.w = player_sprite->w;
  player_rect.h = player_sprite->h;
  
  while(!done) {
    SDL_Event event;
    /* Check for events */
    /* Loop until there are no events left on the queue */
    while(SDL_PollEvent(&event)) { 
      switch(event.type) { /* Process the appropriate event type */
      case SDL_QUIT:  /* Handle a KEYDOWN event */
        done = TRUE;
      break;
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
          case SDLK_UP: 
            player_speed_y = -2;
          break;
          case SDLK_LEFT: 
            player_speed_x = -2;
          break;
          case SDLK_RIGHT: 
            player_speed_x = 2;
          break;
          case SDLK_DOWN: 
            player_speed_y = 2;
          break;
          case SDLK_q:
          case SDLK_ESCAPE:
            done = TRUE;
          break;  
          default:;
        }
        
      break;
      case SDL_KEYUP:
       switch(event.key.keysym.sym) {
          case SDLK_UP: 
            player_speed_y = 0;
          break;
          case SDLK_LEFT: 
            player_speed_x = 0;
          break;
          case SDLK_RIGHT: 
            player_speed_x = 0;
          break;
          case SDLK_DOWN: 
            player_speed_y = 0;
          break;
          default:;
        }

      break;
      default: /* Report an unhandled event */
        ;
        // printf("I don't know what this event is!\n");
      }
    }
    /* Update the player's position. */
    player_rect.x += player_speed_x;
    player_rect.y += player_speed_y;
    if(player_rect.x < 0) player_rect.x = 0;
    if(player_rect.y < 0) player_rect.y = 0;
    if(player_rect.x  + player_rect.w > screen->w) 
      player_rect.x = screen->w - player_rect.w;
    if(player_rect.y  + player_rect.h > screen->h) 
      player_rect.y = screen->h - player_rect.h;
      

    /* Clear the screen.  */
    SDL_FillRect(screen, NULL, black);
    /* Now draw the player. */
    SDL_BlitSurface(player_sprite, NULL, screen, &player_rect);
    /* And show the changes. */
    SDL_Flip(screen);
  }
 
  // Free player sprite 
  SDL_FreeSurface(player_sprite); 
  // Quit SDL
  SDL_Quit(); 
  return 0;
}


