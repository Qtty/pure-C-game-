typedef struct choices //a struct containing the three pieces to choose from
{
  int c[3];
}choices;

typedef struct field //a struct containing the matrice that simulates the field,meaning the free and used slots in the field
{
  int f[10][10];
}field;

typedef struct cord // a struct that represents the coordinates of a field slot
{

	int x,y;
}cord;

typedef struct ch
{
  cord tar;
  int scr;
}sc;

typedef struct cellule //a type representing linked chains
{

	struct cellule  *nxt;
																				//typedef struct cellule{...} *LS;
	cord info;

}*LS;

typedef struct pec
{
  int scr,x;
  SDL_Rect pos;
}pec;

LS cree_noued()
{
	LS p = NULL;

	p = malloc(sizeof(LS));
	if(p == NULL)
	{
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
   			tmp.x = tar.x;
  			tmp.y = tar.y;
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

void count_gaps(int piece,field grill,int (*gaps)[10])
{
  field grill2;
  int x,a,y,g,p,u=0,z=0,c=0;

  for(size_t y=0;y<10;y++)
  {
    x = 0;
    g = 0;
    p = 0;
    c = 0;
    u = 0;
    z = -1;
    while((x<10)&&(grill.f[x][y] != 1))
    {
      x++;
    }
    if(x < 10)
    {
      for(a=x+1;a<10;a++)
      {
        if(grill.f[a][y] == 0)
        {
          z = a;
          p++;
        }
        else
        {
          u = a;
          g += p;
          p = 0;
        }
        if((u>z)&&(z != -1)) c = 1;
      }
      g += x;
      if(x != 0) c = 1;
      if(c) g += p;
      (*gaps)[y] = g;
    }
    else
    {
      (*gaps)[y] = 0;
    }
  }

}

pec choose(choices t,field *grille)
{
  field grille1,grill2;
  pec pec1;
  SDL_Surface *ec = NULL;
  SDL_Rect pos,ps;
  LS p,q = NULL;
  int tmp,a = 0,gaps[10],g[10],line = 0,psb1[100];
  sc pc[3];
  cord tar,psb[100];

  ec = SDL_CreateRGBSurface(SDL_HWSURFACE,510,340,32,0,0,0,0);
  for(size_t x=0;x<3;x++)
  {
    pos.x = 0;
    pos.y = 0;
    pc[x].scr = 0;

    grill2 = *grille;
    handle(t.c[x],&grill2,&grille1,pos,&tmp);
    p = cords(grille1);
    while(!pilevide(p))
    {
      tar = desempiler(&p);
      empiler(&q,tar);
    }
    while(!pilevide(q))
    {
      tar = desempiler(&q);
      empiler(&p,tar);
    }
    grill2 = *grille;
    a = 0;
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
            else {tmp = 0;break;}
          }
          while(!pilevide(q))
          {
            tar = desempiler(&q);
            empiler(&p,tar);
          }
          if(tmp)
          {
            tar.x = i;
            tar.y = j;
            psb[a] = tar;
            a++;
          }
        }
      }
    }
    count_gaps(t.c[x],grill2,&gaps);

    for(size_t i=0;i<a;i++)
    {
      tar = psb[i];
      psb1[i] = 0;
      pos.x = tar.x * 34;
      pos.y = tar.y * 34;
      handle(t.c[x],&grill2,&grille1,pos,&tmp);
      if(pc[x].scr == 0) pc[x].scr = tmp * 10;

      count_gaps(t.c[x],grill2,&g);

      for(size_t j=0;j<10;j++)
      {
        if(gaps[j] != 0) psb1[i] += (gaps[j] - g[j])*10;
        else psb1[i] += (10-g[j])*10;
      }
      check_lines(&ec,&grill2,&line,0);
      handle_score(&(psb1[i]),line);
      line = 0;
      grill2 = *grille;
      printf("a:%d,%d\n",a,psb1[i]);

    }
    tmp = 0;
    for(size_t i=1;i<a;i++)
    {
      if(psb1[i] > psb1[tmp]) tmp = i;
    }
    printf("scr:%d\n",psb1[tmp]);
    pc[x].tar.x = psb[tmp].x;
    pc[x].tar.y = psb[tmp].y;
    pc[x].scr += psb1[tmp];
  }
  tmp = 0;
  for(size_t i=0;i<3;i++)
  {
    printf("i:%d,%d,%d,%d\n",i,pc[i].tar.x,pc[i].tar.y,pc[i].scr);
    fflush(stdout);
    if(pc[i].scr > pc[tmp].scr) tmp = i;
  }
  tar.x = pc[tmp].tar.x;
  tar.y = pc[tmp].tar.y;
  ps.x = tar.x * 34;
  ps.y = tar.y * 34;

  printf("ok\n");
  fflush(stdout);
  printf("%d\n",handle(t.c[tmp],grille,&grille1,ps,&a));
  fflush(stdout);
  pec1.scr = a * 10;
  pec1.x = tmp;
  pec1.pos = ps;

  return pec1;
}

void drag_ai(SDL_Surface **ecran,pec pec1,choices *t,SDL_Surface *obj[54],int *score,field *grill,int mvm,int nch)
{
  int scr,x,a = 0;
  float A,B,D;
  SDL_Surface *tmp1 = NULL;
  SDL_Rect p,p1,pos;
  x = pec1.x;
  pos = pec1.pos;
  printf("pos:%d,%d\n",pos.x,pos.y);
  if(mvm) pos.x += 510;

  p.y = 0;
  p.x = 0;
  p1.y = 0;
  p1.x = 0;
  if (!mvm) tmp1 = SDL_CreateRGBSurface(SDL_HWSURFACE,510,510,32,0,0,0,0);
  else tmp1 = SDL_CreateRGBSurface(SDL_HWSURFACE,510*2,510,32,0,0,0,0);
  SDL_BlitSurface(*ecran,NULL,tmp1,&p);
  p.y = 350;
  p.x = x * 115 + 70;
  if (mvm) p.x += 510;
  D = p.x - pos.x;
  B = ((pos.y * p.x) - (p.y * pos.x)) / D;
  A = (p.y - pos.y) / D;
  while(p.x != pos.x)
  {
    if(p.x>pos.x) p.x--;
    else p.x++;
    p.y = A*p.x + B;
    SDL_BlitSurface(tmp1,NULL,*ecran,&p1);
    SDL_BlitSurface(obj[(*t).c[x]],NULL,*ecran,&p);
    SDL_Flip(*ecran);
    SDL_Delay(10);

  }
  SDL_BlitSurface(obj[(*t).c[x]],NULL,*ecran,&pos);
  //getchar();
  check_lines(ecran,grill,&a,mvm);
  *score += pec1.scr;
  printf("a:%d\n",a);
  handle_score(score,a);
  a = (*t).c[x] + 27;
  if(!mvm) (*t).c[x] = rand() % 27;
  else (*t).c[x] = nch;



  pos.y = 350;
  pos.x = x * 115;
  if (mvm) pos.x += 510;
  tmp1 = SDL_CreateRGBSurface(SDL_HWSURFACE,obj[a]->w,obj[a]->h,32,0,0,0,0);
  SDL_FillRect(tmp1,NULL,SDL_MapRGB(tmp1->format,31,34,40));
  SDL_BlitSurface(tmp1,NULL,*ecran,&pos);

  SDL_BlitSurface(obj[(*t).c[x] + 27],NULL,*ecran,&pos);
  SDL_Flip(*ecran);
}
