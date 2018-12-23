typedef struct choices
{
  int c[3];
}choices;

typedef struct field
{
  int f[10][10];
}field;

typedef struct cord
{

	int x,y;
}cord;

typedef struct cellule
{

	struct cellule  *nxt;
																				//typedef struct cellule{...} *LS;
	cord info;

}*LS;

void print_mat(field gr)
{
  for(size_t i=0;i<10;i++)
  {
    for(size_t j=0;j<10;j++)
    {
      printf("%d\t",gr.f[j][i]);
    }
    printf("\n");
  }
}

LS cree_noued()
{
	LS p = NULL;

	p = malloc(sizeof(LS));
	if(p == NULL)
	{
		printf("error\n");
		exit(-1);
	}
	return p;
}

LS initpile()
{
	LS p = NULL;

	return p;
}

int pilevide(LS p)
{

	if(p == NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void empiler(LS *p,cord c)
{

	LS q = NULL;

	q = cree_noued();
	q->info = c;
	q->nxt = *p;
	*p = q;

}

cord desempiler(LS *p)
{
	LS tmp;
  cord c;

  c = (*p)->info;
	tmp = *p;
	*p = (*p)->nxt;
	free(tmp);
  return c;
}

LS cords(field grill)
{
	LS p,q = NULL;
	cord tar,t,tmp,init;

  init.x = 0;
  init.y = 0;

  while(grill.f[init.x][init.y] == -1)
  {
    init.x++;
  }

	p = malloc(sizeof(LS));
	p->info = init;
	p->nxt = NULL;
	while(!pilevide(p))
	{
		tar = desempiler(&p);
		if(grill.f[tar.x][tar.y] == 1)
		{
      grill.f[tar.x][tar.y] = -1;
      tmp.x = tar.x - init.x;
      tmp.y = tar.y - init.y;
			empiler(&q,tmp);
			if(tar.x - 1 >=0)
			{
				t.x = tar.x - 1;
				t.y = tar.y;
				empiler(&p,t);
			}
			if(tar.x + 1 < 10)
			{
				t.x = tar.x + 1;
				t.y = tar.y;
				empiler(&p,t);
			}
			if(tar.y - 1 >=0)
			{
				t.x = tar.x;
				t.y = tar.y - 1;
				empiler(&p,t);
			}
			if(tar.y + 1 < 10)
			{
				t.x = tar.x;
				t.y = tar.y + 1;
				empiler(&p,t);
			}
		}
	}
  return q;
}

void init_field(field *grill,int c)
{
  int i,j;

  for(i=0;i<10;i++)
  {
    for(j=0;j<10;j++)
    {
      (*grill).f[i][j] = c;
    }
  }
}

void init(SDL_Surface **ecran,int x,int y)
{
  SDL_Init(SDL_INIT_VIDEO);

  *ecran = SDL_SetVideoMode(x,y,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption("jeu",NULL);

}

choices draw(SDL_Surface **ecran,SDL_Surface *objs[54],int best)
{
  SDL_Surface *tmp = NULL,*obj[27];
  choices t;
  SDL_Rect pos;
  int i,j;

  for(i=27;i<54;i++)
  {
    obj[i%27] = objs[i];
  }

  SDL_FillRect(*ecran,NULL,SDL_MapRGB((*ecran)->format,43,46,64));

  tmp = SDL_CreateRGBSurface(SDL_HWSURFACE,1,340,32,0,0,0,0);
  SDL_FillRect(tmp,NULL,SDL_MapRGB(tmp->format,0,0,0));

  for(i=0;i<=340;i += 34)
  {
    pos.x = i;
    pos.y = 0;

    SDL_BlitSurface(tmp,NULL,*ecran,&pos);
  }

  tmp = SDL_CreateRGBSurface(SDL_HWSURFACE,340,1,32,0,0,0,0);
  SDL_FillRect(tmp,NULL,SDL_MapRGB(tmp->format,0,0,0));

  for(i=0;i<=340;i += 34)
  {
    pos.x = 0;
    pos.y = i;

    SDL_BlitSurface(tmp,NULL,*ecran,&pos);
  }
  pos.y = 350;
  pos.x = 0;
  for(i=0;i<3;i++)
  {
    j = rand() % 27;
    /*if(i==2)
    {
      j = 6;
    }*/
    t.c[i] = j;
    SDL_BlitSurface(obj[j],NULL,*ecran,&pos);
    pos.x += 115;

  }

  round_menu(ecran,0,best,0,1);

  SDL_Flip(*ecran);

  return t;
}

int check(int x,int b1,int b2)
{
  if((x>=b1)&&(x<=b2))
  {
    return 1;
  }

  else
  {
    return 0;
  }

}

void b3(int i,field *grill,SDL_Rect pos)
{
  switch (i)
  {
    case 1:
      (*grill).f[pos.x][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y + 1] = 1;
      break;
    case 2:
      (*grill).f[pos.x][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y] = 1;
      (*grill).f[pos.x][pos.y + 1] = 1;
      break;
    case 3:
      (*grill).f[pos.x][pos.y] = 1;
      (*grill).f[pos.x][pos.y + 1] = 1;
      (*grill).f[pos.x + 1][pos.y + 1] = 1;
      break;
    case 4:
      (*grill).f[pos.x + 1][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y + 1] = 1;
      (*grill).f[pos.x][pos.y + 1] = 1;
      break;
  }
}

void check_lines(SDL_Surface **ecran,field *grill,int *line)
{
  int i,j,ok;
  SDL_Surface *tmp = NULL,*tmp1 = NULL;
  SDL_Rect pos;
  field grill1;

  init_field(&grill1,-1);

  for(i=0;i<10;i++)
  {
    ok = 1;
    for(j=0;j<10;j++)
    {
      if((*grill).f[i][j] == 0)
      {
        ok = 0;
        break;
      }

    }

    if (ok)
    {
      *line += 1;
      tmp = SDL_CreateRGBSurface(SDL_HWSURFACE,36,35,32,0,0,0,0);
      tmp1 = SDL_CreateRGBSurface(SDL_HWSURFACE,36,35,32,0,0,0,0);
      SDL_FillRect(tmp,NULL,SDL_MapRGB(tmp->format,255,255,255));
      SDL_FillRect(tmp1,NULL,SDL_MapRGB(tmp1->format,43,46,64));
      pos.y = 0;
      pos.x = i*34;
      for(ok=0;ok<10;ok++)
      {
        SDL_BlitSurface(tmp,NULL,*ecran,&pos);
        SDL_Flip(*ecran);
        SDL_Delay(30);
        SDL_BlitSurface(tmp1,NULL,*ecran,&pos);
        SDL_Flip(*ecran);
        pos.y += 34;
        grill1.f[i][ok] = 0;
      }

      tmp = SDL_CreateRGBSurface(SDL_HWSURFACE,1,340,32,0,0,0,0);
      tmp1 = SDL_CreateRGBSurface(SDL_HWSURFACE,340,1,32,0,0,0,0);
      SDL_FillRect(tmp,NULL,SDL_MapRGB(tmp->format,0,0,0));
      pos.y = 0;
      SDL_BlitSurface(tmp,NULL,*ecran,&pos);
      pos.x += 34;
      SDL_BlitSurface(tmp,NULL,*ecran,&pos);
      pos.x = 0;
      pos.y = 0;
      for(ok=0;ok<=10;ok++)
      {
        SDL_BlitSurface(tmp1,NULL,*ecran,&pos);
        pos.y += 34;
      }
    }
  }
  for(i=0;i<10;i++)
  {
    ok = 1;
    for(j=0;j<10;j++)
    {
      if((*grill).f[j][i] == 0)
      {
        ok = 0;
        break;
      }

    }

    if (ok)
    {
      *line += 1;
      tmp = SDL_CreateRGBSurface(SDL_HWSURFACE,36,35,32,0,0,0,0);
      tmp1 = SDL_CreateRGBSurface(SDL_HWSURFACE,36,35,32,0,0,0,0);
      SDL_FillRect(tmp,NULL,SDL_MapRGB(tmp->format,255,255,255));
      SDL_FillRect(tmp1,NULL,SDL_MapRGB(tmp1->format,43,46,64));
      pos.x = 0;
      pos.y = i*34;
      for(ok=0;ok<10;ok++)
      {
        SDL_BlitSurface(tmp,NULL,*ecran,&pos);
        SDL_Flip(*ecran);
        SDL_Delay(30);
        SDL_BlitSurface(tmp1,NULL,*ecran,&pos);
        SDL_Flip(*ecran);
        pos.x += 34;
        grill1.f[ok][i] = 0;
      }

      tmp = SDL_CreateRGBSurface(SDL_HWSURFACE,340,1,32,0,0,0,0);
      tmp1 = SDL_CreateRGBSurface(SDL_HWSURFACE,1,340,32,0,0,0,0);
      SDL_FillRect(tmp,NULL,SDL_MapRGB(tmp->format,0,0,0));
      pos.x = 0;
      SDL_BlitSurface(tmp,NULL,*ecran,&pos);
      pos.y += 34;
      SDL_BlitSurface(tmp,NULL,*ecran,&pos);
      pos.x = 0;
      pos.y = 0;
      for(ok=0;ok<=10;ok++)
      {
        SDL_BlitSurface(tmp1,NULL,*ecran,&pos);
        pos.x += 34;
      }
    }

  }
  for(i=0;i<10;i++)
  {
    for(j=0;j<10;j++)
    {
      if(grill1.f[i][j] == 0) (*grill).f[i][j] = 0;
    }
  }
  SDL_FreeSurface(tmp);
  SDL_FreeSurface(tmp1);

}

void b4(int i,field *grill,SDL_Rect pos)
{
  switch (i)
  {
    case 1:
      (*grill).f[pos.x + 1][pos.y] = 1;
      (*grill).f[pos.x][pos.y + 1] = 1;
      (*grill).f[pos.x + 1][pos.y + 1] = 1;
      (*grill).f[pos.x + 2][pos.y + 1] = 1;
      break;
    case 2:
      (*grill).f[pos.x + 1][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y + 1] = 1;
      (*grill).f[pos.x + 1][pos.y + 2] = 1;
      (*grill).f[pos.x][pos.y + 1] = 1;
      break;
    case 3:
      (*grill).f[pos.x][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y] = 1;
      (*grill).f[pos.x + 2][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y + 1] = 1;
      break;
    case 4:
      (*grill).f[pos.x][pos.y] = 1;
      (*grill).f[pos.x][pos.y + 1] = 1;
      (*grill).f[pos.x][pos.y + 2] = 1;
      (*grill).f[pos.x + 1][pos.y + 1] = 1;
      break;
  }
}

void b52(int i,field *grill,SDL_Rect pos)
{
  switch (i)
  {
    case 1:
      (*grill).f[pos.x][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y] = 1;
      (*grill).f[pos.x + 2][pos.y] = 1;
      (*grill).f[pos.x + 2][pos.y + 1] = 1;
      (*grill).f[pos.x + 2][pos.y + 2] = 1;
      break;
    case 2:
      (*grill).f[pos.x][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y] = 1;
      (*grill).f[pos.x + 2][pos.y] = 1;
      (*grill).f[pos.x][pos.y + 1] = 1;
      (*grill).f[pos.x][pos.y + 2] = 1;
      break;
    case 3:
      (*grill).f[pos.x][pos.y] = 1;
      (*grill).f[pos.x][pos.y + 1] = 1;
      (*grill).f[pos.x][pos.y + 2] = 1;
      (*grill).f[pos.x + 1][pos.y + 2] = 1;
      (*grill).f[pos.x + 2][pos.y + 2] = 1;
      break;
    case 4:
      (*grill).f[pos.x + 2][pos.y] = 1;
      (*grill).f[pos.x + 2][pos.y + 1] = 1;
      (*grill).f[pos.x + 2][pos.y + 2] = 1;
      (*grill).f[pos.x + 1][pos.y + 2] = 1;
      (*grill).f[pos.x][pos.y + 2] = 1;
      break;
  }
}

void b53(int i,field *grill,SDL_Rect pos)
{
  switch (i)
  {
    case 1:
      (*grill).f[pos.x][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y + 1] = 1;
      (*grill).f[pos.x + 1][pos.y + 2] = 1;
      (*grill).f[pos.x][pos.y + 2] = 1;
      break;
    case 2:
      (*grill).f[pos.x][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y] = 1;
      (*grill).f[pos.x + 2][pos.y] = 1;
      (*grill).f[pos.x + 2][pos.y + 1] = 1;
      (*grill).f[pos.x][pos.y + 1] = 1;
      break;
    case 3:
      (*grill).f[pos.x][pos.y] = 1;
      (*grill).f[pos.x + 1][pos.y] = 1;
      (*grill).f[pos.x][pos.y + 1] = 1;
      (*grill).f[pos.x][pos.y + 2] = 1;
      (*grill).f[pos.x + 1][pos.y + 2] = 1;
      break;
    case 4:
      (*grill).f[pos.x][pos.y] = 1;
      (*grill).f[pos.x][pos.y + 1] = 1;
      (*grill).f[pos.x + 1][pos.y + 1] = 1;
      (*grill).f[pos.x + 2][pos.y + 1] = 1;
      (*grill).f[pos.x + 2][pos.y ] = 1;
      break;

  }
}

int handle(int i,field *grill,field *grill1,SDL_Rect pos,int *nb_blc)
{
  int j,j1,ok = 1;

  pos.x = pos.x / 34;
  pos.y = pos.y / 34;

  init_field(grill1,-1);
        switch (i)
        {
          case 0:
            (*grill1).f[pos.x][pos.y] = 1;
            *nb_blc = 1;
            break;

          case 1:
            for(j=0;j<2;j++)
            {
              (*grill1).f[pos.x + j][pos.y] = 1;
            }
            *nb_blc = 2;
            break;

          case 11:
          for(j=0;j<2;j++)
            {
              (*grill1).f[pos.x][pos.y + j] = 1;
            }
            *nb_blc = 2;
            break;

          case 2:
            for(j=0;j<3;j++)
            {
              (*grill1).f[pos.x + j][pos.y] = 1;
            }
            *nb_blc = 3;
            break;

          case 12:
            for(j=0;j<3;j++)
            {
              (*grill1).f[pos.x][pos.y + j] = 1;
            }
            *nb_blc = 3;
            break;

          case 3:
            for(j=0;j<4;j++)
            {
              (*grill1).f[pos.x + j][pos.y] = 1;
            }
            *nb_blc = 4;
            break;

          case 13:
            for(j=0;j<4;j++)
            {
              (*grill1).f[pos.x][pos.y + j] = 1;
            }
            *nb_blc = 4;
            break;

          case 4:
            for(j=0;j<5;j++)
            {
              (*grill1).f[pos.x + j][pos.y] = 1;
            }
            *nb_blc = 5;
            break;

          case 14:
            for(j=0;j<5;j++)
            {
              (*grill1).f[pos.x][pos.y + j] = 1;
            }
            *nb_blc = 5;
            break;

          case 5:
            b3(1,grill1,pos);
            *nb_blc = 3;
            break;

          case 15:
            b3(2,grill1,pos);
            *nb_blc = 3;
            break;

          case 16:
            b3(3,grill1,pos);
            *nb_blc = 3;
            break;

          case 17:
            b3(4,grill1,pos);
            *nb_blc = 3;
            break;

          case 6:
            b4(1,grill1,pos);
            *nb_blc = 4;
            break;

          case 18:
            b4(2,grill1,pos);
            *nb_blc = 4;
            break;

          case 19:
            b4(3,grill1,pos);
            *nb_blc = 4;
            break;

          case 20:
            b4(4,grill1,pos);
            *nb_blc = 4;
            break;

          case 7:
            b52(1,grill1,pos);
            *nb_blc = 5;
            break;

          case 21:
            b52(2,grill1,pos);
            *nb_blc = 5;
            break;

          case 22:
            b52(3,grill1,pos);
            *nb_blc = 5;
            break;

          case 23:
            b52(4,grill1,pos);
            *nb_blc = 5;
            break;

          case 8:
            b53(1,grill1,pos);
            *nb_blc = 5;
            break;

          case 24:
            b53(2,grill1,pos);
            *nb_blc = 5;
            break;

          case 25:
            b53(3,grill1,pos);
            *nb_blc = 5;
            break;

          case 26:
            b53(4,grill1,pos);
            *nb_blc = 5;
            break;

          case 9:
            (*grill1).f[pos.x ][pos.y] = 1;
            (*grill1).f[pos.x + 1][pos.y] = 1;
            (*grill1).f[pos.x ][pos.y + 1] = 1;
            (*grill1).f[pos.x + 1][pos.y + 1] = 1;
            *nb_blc = 4;
            break;

          case 10:
            for(j=0;j<3;j++)
            {
              for(j1=0;j1<3;j1++)
              {
                (*grill1).f[pos.x + j1][pos.y + j] = 1;
              }
            }
            *nb_blc = 9;
            break;

        }

        for(j=0;j<10;j++)
        {
          for(j1=0;j1<10;j1++)
          {
            if((*grill).f[j][j1]==(*grill1).f[j][j1])
            {
              ok = 0;
              break;
            }
          }
        }
        if (ok)
        {
          for(j=0;j<10;j++)
          {
            for(j1=0;j1<10;j1++)
            {
              if((*grill1).f[j][j1] == 1)
              {
                (*grill).f[j][j1] = 1;
              }
            }

          }
        }
        return ok;
}

void handle_score(int *score,int lines)
{
  switch (lines)
  {
    case 1:
      *score += 100;
      break;
    case 2:
      *score += 300;
      break;
    case 3:
      *score += 600;
      break;
    case 4:
      *score += 1000;
      break;
    case 5:
      *score += 1500;
      break;
  }
}

int check_game(choices t,field *grille)
{
  field grille1,grill2;
  SDL_Rect pos;
  LS p,q = NULL;
  int tmp;
  cord tar;

  for(size_t x=0;x<3;x++)
  {
    pos.x = 0;
    pos.y = 0;

    grill2 = *grille;
    handle(t.c[x],&grill2,&grille1,pos,&tmp);
    p = cords(grille1);
    for(size_t i=0;i<10;i++)
    {
      for(size_t j=0;j<10;j++)
      {
        if((*grille).f[i][j] == 0)
        {
          tmp = 1;
          while(!pilevide(p))
          {
            tar = desempiler(&p);
            empiler(&q,tar);
            if((check(i + tar.x,0,9))&&(check(j + tar.y,0,9)))
            {
              if((*grille).f[i + tar.x][j + tar.y] != 0) {tmp = 0;break;}
            }
            else tmp = 0;
          }
          while(!pilevide(q))
          {
            tar = desempiler(&q);
            empiler(&p,tar);
          }
          if(tmp) return 1;
        }
      }
    }
  }
  return 0;



}

void drag(SDL_Surface **ecran,SDL_Surface *piece,field *grill,choices *t,int i,SDL_Surface *obj[54],int *score)
{
  SDL_Surface *ec = NULL,*tmp = NULL;
  SDL_Rect pos,p;
  SDL_Event event;
  field grill1;
  int c = 1,j,pc,nb_blc;

  ec = SDL_CreateRGBSurface(SDL_HWSURFACE,510,510,32,0,0,0,0);

  pos.x = 0;
  pos.y = 0;

  SDL_BlitSurface(*ecran,NULL,ec,&pos);

  while(c)
  {
    SDL_WaitEvent(&event);

    switch (event.type)
    {
      case SDL_MOUSEMOTION:
        p.x = event.motion.x;
        p.y = event.motion.y;

        SDL_BlitSurface(piece,NULL,*ecran,&p);
        SDL_Flip(*ecran);
        SDL_BlitSurface(ec,NULL,*ecran,&pos);

        break;

      case SDL_MOUSEBUTTONUP:
        p.x = event.button.x - (event.button.x % 34);
        p.y = event.button.y - (event.button.y % 34);

        if((p.y + piece->h <= 340)&&(p.x + piece->w <= 340)&&(handle((*t).c[i],grill,&grill1,p,&nb_blc)))
        {

          *score += nb_blc * 10;
          nb_blc = 0;
          pc = (*t).c[i] + 27;
          (*t).c[i] = rand() % 27;
          SDL_BlitSurface(piece,NULL,*ecran,&p);
          p.y = 350;
          p.x = i * 115;

          tmp = SDL_CreateRGBSurface(SDL_HWSURFACE,obj[pc]->w,obj[pc]->h,32,0,0,0,0);
          SDL_FillRect(tmp,NULL,SDL_MapRGB(tmp->format,43,46,64));
          SDL_BlitSurface(tmp,NULL,*ecran,&p);
          SDL_Flip(*ecran);

          check_lines(ecran,grill,&nb_blc);
          handle_score(score,nb_blc);
          SDL_BlitSurface(obj[(*t).c[i] + 27],NULL,*ecran,&p);
          SDL_Flip(*ecran);
        }
        else
        {
          SDL_BlitSurface(ec,NULL,*ecran,&pos);
          SDL_Flip(*ecran);
        }

        c = 0;
        break;
    }

  }
  SDL_FreeSurface(ec);
  SDL_FreeSurface(tmp);

}

void play(SDL_Surface **ecran,field *gr,choices *T,int *scr,int *Gtime,int best,int sit)
{
  SDL_Surface *objs[54],*tmp = NULL;
  SDL_Event event;
  SDL_Rect p;
  TTF_Font *police = NULL;
  SDL_Color black = {0,0,0},blue = {0,0,255},bck = {43,46,64},colr;
  choices t;
  field grill;
  int c = 1,i,j = 11,pt = time(NULL),score = 0;

  if(sit) init_field(&grill,0);
  else grill = *gr;

  if(!sit) pt -= (*Gtime) * 60;

  TTF_Init();
  police = TTF_OpenFont("TP/sdl/DejaVuSerif.ttf",20);
  TTF_SetFontStyle(police,TTF_STYLE_ITALIC);
  tmp = TTF_RenderText_Shaded(police,"Pause",black,bck);

  objs[0] = IMG_Load("sprites/block.jpg");
  objs[1] = IMG_Load("sprites/block2.jpg");
  objs[2] = IMG_Load("sprites/block3_1.jpg");
  objs[3] = IMG_Load("sprites/block4_2.jpg");
  objs[4] = IMG_Load("sprites/block5_1.jpg");
  objs[5] = IMG_Load("sprites/block3_2.png");
  objs[6] = IMG_Load("sprites/block4_3.png");
  objs[7] = IMG_Load("sprites/block5_2.png");
  objs[8] = IMG_Load("sprites/block5_3.png");
  objs[9] = IMG_Load("sprites/block4_1.jpg");
  objs[10] = IMG_Load("sprites/block9.png");
  for(i=1;i<=4;i++)
  {
    objs[j] = rotozoomSurface(objs[i],90.0,0.99,0);
    j++;
  }

  for(i=5;i<=8;i++)
  {
    objs[j] = rotozoomSurface(objs[i],90.0,0.99,0);
    objs[j+1] = rotozoomSurface(objs[i],180.0,0.99,0);
    objs[j+2] = rotozoomSurface(objs[i],270.0,0.99,0);
    j += 3;
  }

  for(i=0;i<27;i++)
  {
    objs[j] = zoomSurface(objs[i],0.5,0.5,0);
    j++;
  }

  if(sit) t = draw(ecran,objs,best);
  else t = *T;

  if(!sit) score = *scr;

  p.x = 400;
  p.y = 50;

  while (c)
  {
    SDL_WaitEvent(&event);
    i = 1;

    switch (event.type)
    {

      case SDL_QUIT:
        c = 0;
        break;

      case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == SDL_BUTTON_LEFT)
        {
          if((check(event.button.y,50,50 + tmp->h))&&(check(event.button.x,360,360 + tmp->w)))
          {
            pause(ecran,score,best,t,grill,(time(NULL) - pt)/60,&c);
            break;
          }

          if(event.button.y > 340)
          {

            if (check(event.button.x,0,110))
            {
              drag(ecran,objs[t.c[0]],&grill,&t,0,objs,&score);
              //break;
            }
            else
            {
              if(check(event.button.x,115,225))
              {
                drag(ecran,objs[t.c[1]],&grill,&t,1,objs,&score);
                //break;
              }
              else
              {
                if(check(event.button.x,230,340))
                {
                  drag(ecran,objs[t.c[2]],&grill,&t,2,objs,&score);
                  //break;
                }
              }
            }
            game_over(ecran,check_game(t,&grill),score,best,&c);
          }

        }
        break;

      case SDL_MOUSEMOTION:
        p.x = event.motion.x;
        p.y = event.motion.y;
        if(check(p.x,360,360 + tmp->w) && check(p.y,50,50 + tmp->h))
        {

          i = 0;

        }
        else
        {
          i = 1;
        }

        break;


    }
    if(score>best) best = score;
    round_menu(ecran,score,best,time(NULL) - pt,i);
    SDL_Flip(*ecran);



  }
  for(i=0;i<54;i++)
  {
    SDL_FreeSurface(objs[i]);
  }

}
