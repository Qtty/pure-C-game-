#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_ttf.h"
#include "jeu.c"
#include "menu.c"
#include "save.c"

void main()
{
  SDL_Surface *ecran = NULL;
  time_t seed;

  seed = time(NULL);
  srand(seed);

  init(&ecran,340,510);
  menu(&ecran);
  SDL_Quit();
}
