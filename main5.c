#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>



#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

int done = FALSE;

/** Prints an error message and exits the program. */
void fail(char * message) {
  printf("Error: %s\n", message);
  exit(1);
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


#define IMAGES_MAX 128
/** This is where we wil store all images we load. */
SDL_Surface * images[IMAGES_MAX];

/** Empties the image storage. */
void images_empty() {
  int index; 
  for(index = 0; index < IMAGES_MAX; index++) { 
    images[index] = NULL;
  }  
}

/** Loads an image in the image storage. */
SDL_Surface * image_load(char * filename, int index) {
  if (index < 0)            return NULL;
  if (index >= IMAGES_MAX)  return NULL;
  images[index] = image_load_colorkey(filename, 255, 0, 255);
  return images[index];
}

/** Gets the image from the image storage. */
SDL_Surface * image_get(int index) {
  if (index < 0)            return NULL;
  if (index >= IMAGES_MAX)  return NULL;
  return images[index];
}

/** Cleans up the image storage. */
void images_free() {
  int index; 
  for(index = 0; index < IMAGES_MAX; index++) { 
    SDL_FreeSurface(images[index]);
    images[index] = NULL;
  }  
}

/** Numbers to refer to the images with.  */
enum ImageIndex_ {
  PLAYER_1      = 0,  PLAYER_2,
  STAR1_1          ,  STAR1_2,
  BULLET1_1        ,  BULLET1_2,
  BULLET2_1        ,  BULLET2_2,
  FOE1_1           ,  FOE1_2,
};

/** Names of bitmaps to load, must match with list above */
char * image_names[] = { 
  "player_1.bmp" , "player_2.bmp" ,
  "star1_1.bmp"  , "star1_2.bmp"  ,
  "bullet1_1.bmp", "bullet1_2.bmp",
  "bullet2_1.bmp", "bullet2_2.bmp",
  "foe1_1.bmp"   , "foe1_2.bmp"   ,
  NULL // last image to load
};

/** Loads all needed images. */
int images_load() {
  int index = 0;
  for (index = 0; index < IMAGES_MAX; index ++) {
    char * name = image_names[index];
    if(!name) return 0; // end of list. 
    if(!image_load(name, index)) fail("Could not load sprite"); 
  }
  return 0;
}


enum SpriteType_ {
  SpriteNone        = 0,
  SpritePlayer      = 1,
  SpritePlayerBullet= 2,
  SpriteFoe         = 3,
  SpriteFoeBullet   = 4,
  SpriteExtra       = 5,
};


struct Sprite_ {
  SDL_Surface * image;
  SDL_Rect      box;
  int           type;
  int           health;
  int           active;
  int           speed_x;
  int           speed_y;
  Uint32        timer;
};
typedef struct Sprite_ Sprite;

#define SPRITES_MAX 128
Sprite sprites[SPRITES_MAX]; 

/** empties a sprite before use. */
Sprite * sprite_empty(Sprite * sprite) {
  sprite->image   = NULL;
  sprite->box     = (SDL_Rect) {0, 0, 0, 0};
  sprite->type    = SpriteNone  ;
  sprite->health  = 0;
  sprite->active  = FALSE;
  sprite->timer   = 0;
  sprite->speed_x = 0;
  sprite->speed_y = 0;
  return sprite;
}

/** Cleans up a sprite after use. */
Sprite * sprite_done(Sprite * sprite) {
  SDL_FreeSurface(sprite->image);
  return sprite_empty(sprite);
}

/** empties all sprites. */
void sprites_empty() {
  int index; 
  for(index = 0; index < SPRITES_MAX; index++) {
    Sprite * sprite = sprites + index;
    sprite_empty(sprite);
  }
}

/** Checks if a sprite is "enabled" that is ,if it can be drawn and updated. */
int sprite_enabled(Sprite * sprite) {
  if(!sprite)         return FALSE;
  if(!sprite->type)   return FALSE;
  if(!sprite->active) return FALSE; 
  if(!sprite->image)  return FALSE;
  return TRUE; 
}

/** Draw a single sprite to the screen. */
void sprite_draw(Sprite * sprite, SDL_Surface * screen) { 
  if(!sprite_enabled(sprite)) return;
  SDL_BlitSurface(sprite->image, NULL, screen, &sprite->box);
}

/** Draws all enabled sprites. */
void sprites_draw(SDL_Surface * screen) {
  int index; 
  for(index = 0; index < SPRITES_MAX; index++) {
     Sprite * sprite = sprites + index;
     sprite_draw(sprite, screen);
  }
}

#define SPRITE_FILENAME_MAX 1000

/** Loads a single sprite with full options. */
Sprite * sprite_loadfull(Sprite * sprite, int image, int type, 
                         int x, int y, int health) {
  char filename[SPRITE_FILENAME_MAX];   
  sprite->image     = image_get(image);
  if(!sprite->image) return NULL;
  sprite->box.x     = x;
  sprite->box.y     = y;
  sprite->box.w     = sprite->image->w;
  sprite->box.h     = sprite->image->h;  
  sprite->health    = health;
  sprite->type      = type;  
  sprite->active    = TRUE;
  return sprite;
}

// Loads the sprite into sprites at the given index.
Sprite * sprite_loadindex(int index, int image, int type, int x, int y, 
                          int health) {
  Sprite * sprite = sprites + index; 
  sprite_done(sprite); // Clean up sprite before using.
  return sprite_loadfull(sprite, image, type, x, y, health);
}


/** Updates the case of the player sprite. */
Sprite * sprite_player_update(Sprite * sprite, SDL_Surface * screen) {
  /** Player cannot leave screen */
  if(sprite->box.x < 0) sprite->box.x = 0;
  if(sprite->box.y < 0) sprite->box.y = 0;
  if(sprite->box.x + sprite->box.w > screen->w) { 
    sprite->box.x = screen->w - sprite->box.w;
  }  
  if(sprite->box.y + sprite->box.h > screen->h) { 
    sprite->box.y = screen->h - sprite->box.h;
  }
}

/** Updates the case of a bullet sprite. */
Sprite * sprite_bullet_update(Sprite * sprite, SDL_Surface * screen) {
  /** If bullet leaves screen, it is deactivated */
  if(sprite->box.x < 0) sprite->active = FALSE;
  if(sprite->box.y < 0) sprite->active = FALSE;
  if(sprite->box.x > screen->w) sprite->active = FALSE;  
  if(sprite->box.y > screen->h) sprite->active = FALSE; 
}
 

/** Updates the case of the foe sprite. */
Sprite * sprite_foe_update(Sprite * sprite, SDL_Surface * screen) {
  // do nothing yet
}

/** Updates the case of the extra sprite. */
Sprite * sprite_extra_update(Sprite * sprite, SDL_Surface * screen) {
  // do nothing yet
}


/** Updates a single sprite. */
Sprite * sprite_update(Sprite * sprite, SDL_Surface * screen) {
  if(!sprite_enabled(sprite)) return NULL;
  sprite->box.x += sprite->speed_x;
  sprite->box.y += sprite->speed_y;  
  /** Updates the case of the player sprite. */
  switch(sprite->type) { 
    case SpritePlayer: 
      return sprite_player_update(sprite, screen);
    case SpritePlayerBullet:  
      return sprite_bullet_update(sprite, screen);
    case SpriteFoeBullet:
      return sprite_bullet_update(sprite, screen);
    case SpriteFoe:
      return sprite_foe_update(sprite, screen);
    case SpriteExtra:
      return sprite_extra_update(sprite, screen);
    default:
      return sprite;
  }  
}

/** Updates all enabled sprites. */
void sprites_update(SDL_Surface * screen) {
  int index; 
  for(index = 0; index < SPRITES_MAX; index++) {
     Sprite * sprite = sprites + index;
     sprite_update(sprite, screen);
  }
}


int handle_input(Sprite * player) {
  SDL_Event event;
  /* Check for events. */
  /* Loop until there are no events left on the queue.    */
  while(SDL_PollEvent(&event)) { 
    switch(event.type) { /* Process the appropriate event type */
    case SDL_QUIT:  /* Handle a KEYDOWN event */
      done = TRUE;
    break;
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym) {
        case SDLK_UP: 
          player->speed_y = -2;
        break;
        case SDLK_LEFT: 
          player->speed_x = -2;
        break;
        case SDLK_RIGHT: 
          player->speed_x = 2;
        break;
        case SDLK_DOWN: 
          player->speed_y = 2;
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
          player->speed_y = 0;
        break;
        case SDLK_LEFT: 
          player->speed_x = 0;
        break;
        case SDLK_RIGHT: 
          player->speed_x = 0;
        break;
        case SDLK_DOWN: 
          player->speed_y = 0;
        break;
        default:;
      }

    break;
    default: /* Report an unhandled event */
      ;
      // printf("I don't know what this event is!\n");
    }
  }
}


#define PLAYER 0


int main(void) {
  SDL_Surface * screen    = NULL;
  Sprite      * player    = NULL;
  SDL_Rect      player_rect;
  Uint32        colorkey, black;
  sprites_empty();
     
  // Start SDL
  SDL_Init(SDL_INIT_EVERYTHING);
  screen  = SDL_SetVideoMode(640, 480, 0, SDL_HWSURFACE | SDL_ANYFORMAT);
  black   = SDL_MapRGB(screen->format, 0, 0, 0);
  
  // Load all images
  images_load();  
  // Set up the player's sprite.    
  player  = sprite_loadindex(PLAYER, PLAYER_1, SpritePlayer, 320, 400, 3);
  
  if(!player) fail("Could not load player sprite.");  
  
  while(!done) {
    /* Handle the input, which modifies the player's state. */
    handle_input(player);
    /* Update all sprites. */
    sprites_update(screen);
    /* Clear the screen.  */
    SDL_FillRect(screen, NULL, black);
    /* Draw all sprites. */
    sprites_draw(screen);
    /* And show the changes. */
    SDL_Flip(screen);
  }
 
  // Free loaded images.
  images_free(); 
  // Quit SDL
  SDL_Quit(); 
  return 0;
}


