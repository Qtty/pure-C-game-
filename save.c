typedef struct p
{
  int score;
  char ct[60],name[20];
}player;

void save(SDL_Surface **ecran,field grille,choices T,int scr,int Time)
{
  FILE *infos;
  game gm;

  gm.gr = grille;
  gm.T = T;
  gm.scr = scr;
  gm.Gtime = Time;

  SDL_SaveBMP(*ecran,"round/round.bmp");
  infos = fopen("round/info","wb");
  fwrite(&gm,sizeof(game),1,infos);
  fclose(infos);
}

int load(SDL_Surface **ecran,game *gm)
{
  FILE *infos;
  SDL_Surface *tmp = NULL;
  SDL_Rect pos;
  game gm1;

  pos.x = 0;
  pos.y = 0;

  infos = fopen("round/info","rb");
  if(infos != NULL)
  {
    fread(&gm1,sizeof(game),1,infos);
    fclose(infos);
    *gm = gm1;
    tmp = IMG_Load("round/round.bmp");
    SDL_BlitSurface(tmp,NULL,*ecran,&pos);
    SDL_Flip(*ecran);
    return 1;
  }
  else
  {
    return 0;
  }

}

void add_score(int score,char usr[20])
{
  FILE *scr;
  player p,l[5];
  char *ct;
  time_t t;
  int i = 0,c = 0,sz,x = 1;

  t = time(NULL);
  ct = ctime(&t);

  p.score = score;
  strcpy(p.ct,ct);
  strcpy(p.name,usr);

  scr = fopen("round/score","rb");
  if(scr == NULL)
  {
    scr = fopen("round/score","wb");
    fwrite(&p,sizeof(p),1,scr);
  }
  else
  {
    while((!feof(scr))&&(i<5))
    {
      fread(&l[i],sizeof(player),1,scr);
      i++;
    }
    fclose(scr);
    scr = fopen("round/score","wb");
    for(size_t j=0;j<i-1;j++)
    {
      if((l[j].score <= p.score)&&(x)) {fwrite(&p,sizeof(p),1,scr);x = 0;}
      if(l[j].score > 0) fwrite(&l[j],sizeof(player),1,scr);
    }
    if(x) fwrite(&p,sizeof(p),1,scr);
  }
  fclose(scr);
}

void load_score(SDL_Surface **ecran)
{
  FILE *scr;
  SDL_Surface *tmp = NULL;
  SDL_Rect pos;
  TTF_Font *police = NULL;
  SDL_Color black = {0,0,0};
  char t[50];
  int i = 1;
  player p;

  TTF_Init();

  police = TTF_OpenFont("TP/sdl/DejaVuSerif.ttf",14);
  TTF_SetFontStyle(police,TTF_STYLE_ITALIC);

  pos.x = (*ecran)->w / 2 - 150;
  pos.y = 250;

  scr = fopen("round/score","rb");
  if(scr != NULL)
  {

    fread(&p,sizeof(player),1,scr);

    while((!feof(scr))&&(i<6))
    {
      i++;
      char pl[260];
      strcpy(pl,p.name);
      sprintf(t,"   %d    ",p.score);
      strcat(pl,t);
      strcat(pl,p.ct);
      pl[strlen(pl) - 1] = ' ';
      tmp = TTF_RenderText_Blended(police,pl,black);
      SDL_BlitSurface(tmp,NULL,*ecran,&pos);
      pos.y += 50;
      fread(&p,sizeof(player),1,scr);
    }
    fclose(scr);
  }

  SDL_FreeSurface(tmp);
  TTF_CloseFont(police);
}

int best()
{
  FILE *scr;
  player p;
  int b;

  scr = fopen("round/score","rb");
  if(scr == NULL) return 0;
  else
  {
    fread(&p,sizeof(player),1,scr);
    b = p.score;
    while(!feof(scr))
    {
      fread(&p,sizeof(player),1,scr);
      if(p.score > b) b = p.score;
    }
    fclose(scr);
  }
  return b;
}
