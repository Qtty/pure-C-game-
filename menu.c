
typedef struct opt //a sturct that contains the surfaces of the options in two colors:black and blue
{
  SDL_Surface *t[12];
}opt;

typedef struct game //a struct that contains the infos of a round
{
  field gr;
  choices T;
  int scr;
  int Gtime;
}game;

opt st_menu(SDL_Surface **ecran) //draws the start menu containing the game modes,scoreboard and quit ^___^!
{
  opt options;
  char opti[6][20] = {"New Game","Continue","Skynet","Man VS Machine","Best Scores","Quit"};
  SDL_Rect pos;
  TTF_Font *police = NULL;
  SDL_Color black = {255,255,255},blue = {140,140,140};

  TTF_Init();

  police = TTF_OpenFont("Fonts/DejaVuSerif.ttf",30);
  TTF_SetFontStyle(police,TTF_STYLE_ITALIC);
  pos.x = (*ecran)->w / 2 - 100;
  pos.y = 100;

  for(size_t i=0;i<6;i++)
  {
    options.t[i] = TTF_RenderText_Blended(police,opti[i],black);
    options.t[i+6] = TTF_RenderText_Blended(police,opti[i],blue);
    SDL_BlitSurface(options.t[i],NULL,*ecran,&pos);
    pos.y += 50;
  }

  SDL_Flip(*ecran);
  return options;

}

void best_scores(SDL_Surface **ecran) //shows the top 5 players in the scoreboard
{
  TTF_Font *police = NULL;
  SDL_Color black = {255,255,255},blue = {140,140,140};
  SDL_Surface *tmp[2],*swp,*back = NULL;
  SDL_Rect pos,p;
  SDL_Event event;
  int c = 1,w0,w,h;

  p.x = 0;
  p.y = 0;

  TTF_Init();

  back = IMG_Load("sprites/back.png");
  SDL_BlitSurface(back,NULL,*ecran,&p);

  police = TTF_OpenFont("Fonts/DejaVuSerif.ttf",20);
  TTF_SetFontStyle(police,TTF_STYLE_ITALIC);
  pos.x = 20;
  pos.y = 150;

  tmp[0] = TTF_RenderText_Blended(police,"The Top Best 5 Scores So Far!",black);
  SDL_BlitSurface(tmp[0],NULL,*ecran,&pos);
  pos.y = 50;
  pos.x = 20;


  load_score(ecran);


  swp = SDL_CreateRGBSurface(SDL_HWSURFACE,340,510,32,0,0,0,0);
  SDL_BlitSurface(*ecran,NULL,swp,&p);

  tmp[0] = TTF_RenderText_Blended(police,"Return to Main Menu",black);
  tmp[1] = TTF_RenderText_Blended(police,"Return to Main Menu",blue);
  SDL_BlitSurface(tmp[0],NULL,*ecran,&pos);
  w = tmp[0]->w;
  h = tmp[0]->h;

  SDL_Flip(*ecran);

  while (c)
  {
    SDL_WaitEvent(&event);

    switch (event.type)
    {

      case SDL_QUIT:
        c = 0;
        break;

      case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == SDL_BUTTON_LEFT)
        {

          if((check(event.button.y,50,50 + h))&&(check(event.button.x,20,20 + w)))
          {
            menu(ecran);
            c = 0;
            break;
          }

        }
        break;

      case SDL_MOUSEMOTION:
        pos.x = 0;
        pos.y = 0;
        SDL_BlitSurface(swp,NULL,*ecran,&pos);
        pos.x += 20;
        pos.y += 50;

        if(check(event.motion.x,20,20 + w) && check(event.motion.y,50,50 + h)) SDL_BlitSurface(tmp[1],NULL,*ecran,&pos);
        else SDL_BlitSurface(tmp[0],NULL,*ecran,&pos);
        SDL_Flip(*ecran);

        break;
    }
  }
  SDL_FreeSurface(tmp[0]);
  SDL_FreeSurface(tmp[1]);
  TTF_CloseFont(police);
  TTF_Quit();
}

void menu(SDL_Surface **ecran) //handles the changes in the start menu(selection,mouse motion,...)
{
    SDL_Surface *tmp,*tmp1 = NULL;
    opt options;
    field grille;
    SDL_Rect pos,p,p1;
    SDL_Event event;
    TTF_Font *police = NULL;
    SDL_Color black = {255,255,255},blue = {140,140,140};
    game gm;
    int c = 1,w = (*ecran)->w / 2 - 100,i,ch = -1,bst;

    TTF_Init();

    tmp = IMG_Load("sprites/back.png");
    p.x = 0;
    p.y = 0;
    SDL_BlitSurface(tmp,NULL,*ecran,&p);

    options = st_menu(ecran);

    bst = best();

    while(c)
    {
      SDL_WaitEvent(&event);

      switch (event.type)
      {
        case SDL_QUIT:
          c = 0;
          break;

        case SDL_MOUSEMOTION:
          SDL_BlitSurface(tmp,NULL,*ecran,&p);
          pos.x = event.motion.x;
          pos.y = event.motion.y;

          for(i=0;i<6;i++)
          {
            tmp1 = options.t[i];
            p1.x = w;
            p1.y = 100 + i*50;

            if(check(pos.x,w,w + options.t[i]->w) && check(pos.y,100 + i * 50 ,100 + i*50 + options.t[i]->h)) tmp1 = options.t[i+6];

            SDL_BlitSurface(tmp1,NULL,*ecran,&p1);
          }
          SDL_Flip(*ecran);

          break;
        case SDL_MOUSEBUTTONUP:
          pos.x = event.button.x;
          pos.y = event.button.y;

          for(i=0;i<6;i++)
          {
            if(check(pos.x,w,w + options.t[i]->w) && check(pos.y,100 + i * 50 ,100 + i*50 + options.t[i]->h))
            {
              c = 0;
              ch = i;
              break;
            }

          }

          break;


      }

    }
    switch (ch)
    {
      case 0:
        init(ecran,510,510);
        play(ecran,&(gm.gr),&(gm.T),&(gm.scr),&(gm.Gtime),bst,1,0);
        break;
      case 1:
        init(ecran,510,510);
        c = load(ecran,&gm);
        if(c) play(ecran,&(gm.gr),&(gm.T),&(gm.scr),&(gm.Gtime),bst,0,0);
        else play(ecran,&(gm.gr),&(gm.T),&(gm.scr),&(gm.Gtime),bst,1,0);
        break;
      case 2:
        init(ecran,510,510);
        play(ecran,&(gm.gr),&(gm.T),&(gm.scr),&(gm.Gtime),bst,1,1);
      case 4:
        init(ecran,340,510);
        best_scores(ecran);
        break;


    }
    SDL_FreeSurface(tmp);
    SDL_FreeSurface(tmp1);
    TTF_CloseFont(police);
    TTF_Quit();
}

void round_menu(SDL_Surface **ecran,int score,int best,int time,int clr) //draws the round menu(pause,score,best socre,time elapsed)
{
  SDL_Surface *tmp = NULL;
  TTF_Font *police = NULL;
  SDL_Color black = {255,255,255},blue = {140,140,140},bck = {31,34,40},colr;
  SDL_Rect pos;
  char opti[4][50] = {"Pause","","",""};
  sprintf(opti[1], "Score : %d", score);
  sprintf(opti[2], "Best : %d", best);
  sprintf(opti[3], "Time : %02d min", time / 60);

  TTF_Init();

  if(clr) colr = black;
  else colr = blue;


  police = TTF_OpenFont("Fonts/DejaVuSerif.ttf",20);
  TTF_SetFontStyle(police,TTF_STYLE_ITALIC);
  pos.x = 360;
  pos.y = 50;
  tmp = TTF_RenderText_Shaded(police,opti[0],colr,bck);
  SDL_BlitSurface(tmp,NULL,*ecran,&pos);
  for(size_t i=1;i<4;i++)
  {
      pos.y += 50;
      tmp = TTF_RenderText_Shaded(police,opti[i],black,bck);
      SDL_BlitSurface(tmp,NULL,*ecran,&pos);
  }
  TTF_CloseFont(police);
  TTF_Quit();
}

void pause(SDL_Surface **ecran,int score,int best,choices T,field grille,int Gtime,int *cnt) //draws and handles the pause menu(save,continue,replay,shows score,shows best score,shows scoreboard)
{
  SDL_Surface *tmp = NULL,*swp = NULL,*back = NULL;
  TTF_Font *police = NULL;
  SDL_Color black = {255,255,255},blue = {140,140,140},bck = {31,34,40};
  SDL_Rect pos;
  game gm;
  SDL_Event event;
  opt tt;
  char scr[2][25],options[3][9] = {"Save","Continue","Replay"};
  int c = 1,w;

  swp = SDL_CreateRGBSurface(SDL_HWSURFACE,510,510,32,0,0,0,0);
  pos.x = 0;
  pos.y = 0;
  SDL_BlitSurface(*ecran,NULL,swp,&pos);

  TTF_Init();

  init(ecran,340,510);

  back = IMG_Load("sprites/back.png");
  SDL_BlitSurface(back,NULL,*ecran,&pos);

  sprintf(scr[1], "Score : %d", score);
  sprintf(scr[0], "Best : %d", best);
  police = TTF_OpenFont("Fonts/DejaVuSerif.ttf",30);
  TTF_SetFontStyle(police,TTF_STYLE_ITALIC);
  w = (*ecran)->w / 2 - 150;
  pos.x = w;
  pos.y = 50;
  for(size_t i=0;i<2;i++)
  {
    tmp = TTF_RenderText_Blended(police,scr[i],black);
    SDL_BlitSurface(tmp,NULL,*ecran,&pos);
    pos.y += 50;
  }

  pos.x = 0;
  pos.y = 0;
  load_score(ecran);
  SDL_BlitSurface(*ecran,NULL,back,&pos);
  police = TTF_OpenFont("Fonts/DejaVuSerif.ttf",20);
  pos.x = w;
  pos.y = 150;
  for(size_t i=0;i<3;i++)
  {
    tmp = TTF_RenderText_Blended(police,options[i],blue);
    tt.t[i+3] = tmp;
    tmp = TTF_RenderText_Blended(police,options[i],black);
    tt.t[i] = tmp;
    SDL_BlitSurface(tmp,NULL,*ecran,&pos);
    if(i==1) pos.x += 50;
    pos.x += 100;
  }
  SDL_Flip(*ecran);
  while (c)
  {
    SDL_WaitEvent(&event);

    switch (event.type)
    {

      case SDL_QUIT:
        c = 0;
        *cnt = 0;
        break;

      case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == SDL_BUTTON_LEFT)
        {

          if(check(event.button.y,150,180))
          {

            if (check(event.button.x,w,w + tt.t[0]->w))
            {
              save(&swp,grille,T,score,Gtime);
              pos.x = 0;
              pos.y = 0;
              back = IMG_Load("sprites/back.png");
              SDL_BlitSurface(back,NULL,*ecran,&pos);
              police = TTF_OpenFont("Fonts/DejaVuSerif.ttf",30);
              tmp = TTF_RenderText_Blended(police,"Saved Succesfully!!",black);
              pos.x = (*ecran)->w / 2 - tmp->w / 2;
              pos.y = (*ecran)->h / 2 - tmp->h / 2;
              SDL_BlitSurface(tmp,NULL,*ecran,&pos);
              SDL_Flip(*ecran);
              SDL_Delay(2000);
              pos.x = 0;
              pos.y = 0;
              init(ecran,510,510);
              SDL_BlitSurface(swp,NULL,*ecran,&pos);
              c = 0;
              break;
            }
            else
            {
              if(check(event.button.x,w + 100,w + 100 + tt.t[1]->w))
              {
                pos.x = 0;
                pos.y = 0;
                init(ecran,510,510);
                SDL_BlitSurface(swp,NULL,*ecran,&pos);
                c = 0;
                break;
              }
              else
              {
                if(check(event.button.x,w + 250,w + 250 + tt.t[2]->w))
                {
                  init(ecran,510,510);
                  play(ecran,&(gm.gr),&(gm.T),&(gm.scr),&(gm.Gtime),best,1,0);
                  *cnt = 0;
                  c = 0;
                  break;
                }
              }
            }
          }

        }
        break;

      case SDL_MOUSEMOTION:
        pos.x = 0;
        pos.y = 0;
        SDL_BlitSurface(back,NULL,*ecran,&pos);
        pos.x += w;
        pos.y += 150;
        for(size_t i=0;i<3;i++)
        {
          tmp = tt.t[i];
          pos.x = w + i * 100;
          int r;

          if(i==2) r = 50;
          else r = 0;

          pos.x = w + i * 100 + r;

          if(check(event.motion.x,w + i * 100 + r,w + i * 100 + r + tt.t[i]->w) && check(event.motion.y,150,150 + tt.t[i]->h)) tmp = tt.t[i+3];

          SDL_BlitSurface(tmp,NULL,*ecran,&pos);
        }

        break;
      }
      SDL_Flip(*ecran);
    }
}

void game_over(SDL_Surface **ecran,int sit,int score,int best,int *cnt) //draws and handles the game-over menu(adds the player to the scoreboard,replay,quit,shows score,shows best score)
{
  if(!sit)
  {
    SDL_Delay(1000);
    SDL_Surface *tmp = NULL,*swp = NULL,*back = NULL,*swp1 = NULL,*swp2 = NULL;
    TTF_Font *police = NULL;
    SDL_Color black = {255,255,255},blue = {140,140,140},bck = {31,34,40};
    SDL_Rect pos,p;
    game gm;
    SDL_Event event;
    opt tt;
    char scr[25],options[2][9] = {"Quit","Replay"},name[20];
    int c = 1,w[2],cr = 1,a;

    swp = SDL_CreateRGBSurface(SDL_HWSURFACE,510,510,32,0,0,0,0);
    swp1 = SDL_CreateRGBSurface(SDL_HWSURFACE,340,510,32,0,0,0,0);
    swp2 = SDL_CreateRGBSurface(SDL_HWSURFACE,340,510,32,0,0,0,0);
    pos.x = 0;
    pos.y = 0;
    SDL_BlitSurface(*ecran,NULL,swp,&pos);

    TTF_Init();

    init(ecran,340,510);
    back = IMG_Load("sprites/back.png");
    SDL_BlitSurface(back,NULL,*ecran,&pos);

    sprintf(scr, "Score : %d", score);
    police = TTF_OpenFont("Fonts/DejaVuSerif.ttf",30);
    TTF_SetFontStyle(police,TTF_STYLE_ITALIC);
    w[0] = (*ecran)->w / 2 - 150;
    w[1] = (*ecran)->w / 2 + 70;
    pos.x = w[0];
    pos.y = 50;
    tmp = TTF_RenderText_Blended(police,scr,black);
    SDL_BlitSurface(tmp,NULL,*ecran,&pos);

    tmp = TTF_RenderText_Blended(police,"Game Over!!!!",black);
    pos.x = (*ecran)->w / 2 - tmp->w / 2;
    a = pos.x;
    pos.y = 250;
    SDL_BlitSurface(tmp,NULL,*ecran,&pos);

    p.x = 0;
    p.y = 0;

    pos.x = 0;
    pos.y = 0;
    SDL_BlitSurface(*ecran,NULL,back,&pos);
    police = TTF_OpenFont("Fonts/DejaVuSerif.ttf",20);
    pos.y = 150;
    for(size_t i=0;i<2;i++)
    {
      tmp = TTF_RenderText_Blended(police,options[i],blue);
      tt.t[i+2] = tmp;
      tmp = TTF_RenderText_Blended(police,options[i],black);
      tt.t[i] = tmp;
      pos.x = w[i];
      SDL_BlitSurface(tmp,NULL,*ecran,&pos);
    }
    SDL_BlitSurface(*ecran,NULL,swp1,&p);

    police = TTF_OpenFont("Fonts/DejaVuSerif.ttf",30);
    tmp = TTF_RenderText_Blended(police,"type in your name:",black);
    pos.x = a - 30;
    pos.y = 300;
    SDL_BlitSurface(tmp,NULL,*ecran,&pos);

    SDL_BlitSurface(*ecran,NULL,swp2,&p);
    back = swp2;
    SDL_Flip(*ecran);

    while (c)
    {
      SDL_WaitEvent(&event);

      switch (event.type)
      {

        case SDL_QUIT:
          c = 0;
          *cnt = 0;
          break;

        case SDL_MOUSEBUTTONDOWN:
          if(event.button.button == SDL_BUTTON_LEFT)
          {

            if(check(event.button.y,150,180))
            {
              if(check(event.button.x,w[0],w[0] + tt.t[0]->w)) {c = 0;*cnt = 0;}
              else
              {
                if(check(event.button.x,w[1],w[1] + tt.t[1]->w))
                {
                  pos.x = 0;
                  pos.y = 0;
                  init(ecran,510,510);
                  play(ecran,&(gm.gr),&(gm.T),&(gm.scr),&(gm.Gtime),best,1,0);
                  c = 0;
                  *cnt = 0;
                  break;
                }
              }
            }
          }
          break;

        case SDL_MOUSEMOTION:
          pos.x = 0;
          pos.y = 0;
          SDL_BlitSurface(back,NULL,*ecran,&pos);
          pos.y += 150;
          for(size_t i=0;i<2;i++)
          {
            tmp = tt.t[i];
            pos.x = w[i];

            if(check(event.motion.x,w[i],w[i] + tt.t[i]->w) && check(event.motion.y,150,150 + tt.t[i]->h)) tmp = tt.t[i+2];

            SDL_BlitSurface(tmp,NULL,*ecran,&pos);
          }

          break;
        case SDL_KEYDOWN:
          if(event.key.keysym.sym == SDLK_RETURN) {cr = 0;add_score(score,name);}
          if(cr)
          {
            int len;
            len = strlen(name);
            police = TTF_OpenFont("Fonts/DejaVuSerif.ttf",30);
            if(event.key.keysym.sym == SDLK_SPACE) strcat(name," ");
            else strcat(name,SDL_GetKeyName(event.key.keysym.sym));
            tmp = TTF_RenderText_Blended(police,name,black);
            SDL_BlitSurface(swp2,NULL,*ecran,&p);
            SDL_Flip(*ecran);
            pos.x = (*ecran)->w / 2 - tmp->w / 2;
            pos.y = 400;
            SDL_BlitSurface(tmp,NULL,*ecran,&pos);
          }
          else
          {
            SDL_BlitSurface(swp1,NULL,*ecran,&p);
            police = TTF_OpenFont("Fonts/DejaVuSerif.ttf",30);
            tmp = TTF_RenderText_Blended(police,"scoreboard updated!!",black);
            pos.x = a - 50;
            pos.y = 300;
            SDL_BlitSurface(tmp,NULL,*ecran,&pos);
            SDL_BlitSurface(tmp,NULL,swp1,&pos);
            back = swp1;
          }
          break;
      }
      SDL_Flip(*ecran);
    }

      TTF_CloseFont(police);/*
      SDL_FreeSurface(back);
      SDL_FreeSurface(tmp);
      SDL_FreeSurface(swp);
      SDL_FreeSurface(swp1);
      SDL_FreeSurface(swp2);*/

  }
}
