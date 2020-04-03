#include "../includes/game.h"
#include "../includes/plugins.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


Starship* create_starship(S_list L, int nb_team){
  Starship* s;

  s = (Starship*)malloc(sizeof(Starship));
  if (s == NULL){
    fprintf(stderr, "Memory allocation error\n");
    exit(1);
  }
  s->nb_team = nb_team;
  s->life = LIFE_STARSHIP;
  s->reload = 0;
  s->shot = 0;
  s->move = 0;
  s->angle_move = (((double)rand())/(double)RAND_MAX)*2*PI;
  s->angle_gun = ((double)rand())*2*PI / RAND_MAX;
  s->angle_radar = ((double)rand())*2*PI / RAND_MAX;
  get_random_position_without_colision(L, &(s->abs), &(s->ord));
  s->plugin = NULL;
  s->next = NULL;
  return s;
}

void get_random_position_without_colision(S_list L, double* x, double* y){
  S_list copy = L;
  *x = (((double)rand())/ (double)RAND_MAX)*SIZE_ABS;
  *y = (((double)rand())/ (double)RAND_MAX)*SIZE_ORD;

  while(L != NULL){
    if (((L->abs - *x)*(L->abs - *x) + (L->ord - *y)*(L->ord - *y)) < 4.5*SIZE_STARSHIP*SIZE_STARSHIP){
      get_random_position_without_colision(copy, x, y);
      return ;
    }
    L = L->next;
  }
}

void add_starship(S_list* L, int nb_team){
  Starship* new;

  new = create_starship(*L, nb_team);
  new->next = *L;
  *L = new;
}

Missile* create_missile(double abs, double ord, double angle){
  Missile* m;

  m = (Missile*)malloc(sizeof(Missile));
  if (m == NULL){
    fprintf(stderr, "Memory allocation error\n");
    exit(1);
  }
  m->abs = abs;
  m->ord = ord;
  m->angle = angle;
  m->next=NULL;
  return m;
}

void add_missile(M_list* L, double abs, double ord, double angle){
  Missile* new;

  new = create_missile(abs, ord, angle);
  new->next = *L;
  *L = new;
}

int number_missiles(M_list* L){
  int i=0;
  Missile* m = *L;

  while (m != NULL){
    i++;
    m = m->next;
  }
  return i;
}

Explosion* create_small_explosion(double abs, double ord, int type){
  Explosion* e;
  
  e = (Explosion*)malloc(sizeof(Explosion));
  if (e == NULL){
    fprintf(stderr, "Memory allocation error\n");
    exit(1);
  }
  
  e->abs = abs;
  e->ord = ord;
  e->type = type;
  e->current_img = 1;
  e->current_frame = 1;
  if (type == 0)
    e->nb_img = 15;
  else
    e->nb_img = 23;
  return e;
}

void add_explosion(E_list* L, Explosion* e){
  e->next = *L;
  *L = e;
}

void resolve_explosion(E_list* L){
  Explosion* e=*L;
  Explosion* previous = NULL;

  while(e != NULL){
    if (e->current_frame < NB_FRAME_EXPLOSION_PER_IMG){
      e->current_frame++;
      previous = e;
    }
    else{
      if (e->current_img < e->nb_img){
	e->current_img++;
	e->current_frame=1;
	previous = e;
      }
      else{
	/* It is time to free this explosion from the list... */
	if (previous != NULL){
	  previous->next = e->next;
	}
	else{
	  *L = e->next;
	}
	free(e);
      }
    }
    e = e->next;
  }
}

void move_missiles(M_list* L){
  Missile* m = *L;
  Missile* previous = NULL;

  while (m != NULL){
    m->abs += cos(m->angle)*(MISSILE_SPEED);
    m->ord += sin(m->angle)*(MISSILE_SPEED);
    m = m->next;
  }
  m = *L;
  while (m != NULL){
    if ((m->abs < 0-VANISH_LIMIT) || 
	(m->abs > SIZE_ABS+VANISH_LIMIT) || 
	(m->ord < 0-VANISH_LIMIT) || 
	(m->ord > SIZE_ORD+VANISH_LIMIT)){
      if (previous == NULL){
	*L = m->next;
	/* The missile has leave the Space game and is free here... */
	free(m);
	m = *L;
      }
      else{
	previous->next = m->next;
	free(m);
	m = previous->next;
      }
    }
    else{
      previous = m;
      m = m->next;
    }    
  }
}

void free_missiles(M_list* L){
  Missile* m;
  
  while (*L != NULL){
    m = (*L)->next;
    free(*L);
    *L = m;
  }
}

void collision_starship_wall(Starship* s){
  if ((s->abs <= 0.0) && (cos(s->angle_move) < 0)){
    s->angle_move = PI-s->angle_move;;
  }
  if ((s->ord <= 0.0) && (sin(s->angle_move) < 0)){
    s->angle_move = -s->angle_move;
  }
  if ((s->abs >= SIZE_ABS) && (cos(s->angle_move) > 0)){
    s->angle_move = PI-s->angle_move;;
  }
  if ((s->ord >= SIZE_ORD) && (sin(s->angle_move) > 0)){
    s->angle_move = -s->angle_move;
  }
}

void collision_starships_wall(S_list L){
  while(L != NULL){
    collision_starship_wall(L);
    L = L->next;
  }
}

void collision_between_starships(S_list L){
  Starship* current = L;
  Starship* other;
  double new_angle;

  while(current != NULL){
    other = current->next;
    while(other != NULL){
      if ((((current->abs - other->abs)*(current->abs - other->abs)) + 
	   ((current->ord - other->ord)*(current->ord - other->ord))) <= (4*SIZE_STARSHIP*SIZE_STARSHIP)){
	new_angle = atan((current->ord - other->ord)/(current->abs - other->abs));
	if ((new_angle > 0.0 && (current->abs < other->abs)) || (new_angle < 0.0 && (current->abs < other->abs))){
	  other->angle_move = new_angle;
	  current->angle_move = PI + new_angle;
	}
	else{
	  other->angle_move = PI + new_angle;
	  current->angle_move = new_angle;	  
	}
      }
      other = other->next;
    }
    current = current->next;
  }
}

void move_starship(Starship* s){
  s->abs += cos(s->angle_move)*(STARSHIP_SPEED)*(s->move);
  s->ord += sin(s->angle_move)*(STARSHIP_SPEED)*(s->move);
}

void move_starships(S_list L){
  while(L != NULL){
    move_starship(L);
    L = L->next;
  }
}

void shot_starship(Starship* s, M_list* L){
  Missile* new;
  double start_x;
  double start_y;

  if (s->reload == 0){
    start_x = s->abs + cos(s->angle_gun)*(1.4*SIZE_STARSHIP);
    start_y = s->ord + sin(s->angle_gun)*(1.4*SIZE_STARSHIP);
    new = create_missile(start_x, start_y, s->angle_gun);
    new->next = *L;
    *L = new;
    s->reload = RELOAD_GUN;
  }
}

void relaod_starships_gun(S_list L, M_list* M){
  while (L != NULL){
    if (L->reload > 0)
      L->reload--;
    L = L->next;
  }
}

void collision_starship_missile(S_list* S, M_list* M, E_list* E, S_list* D){
  Starship* s = *S;
  Starship* prev_s = NULL;
  Starship* next_if_destroyed;
  Missile* m;
  Missile* prev_m;
  int destroyed;

  while(s != NULL){
    destroyed = 0;
    m = *M;
    prev_m = NULL;
    while(m != NULL){
      if ((((s->abs - m->abs)*(s->abs - m->abs)) + 
	   ((s->ord - m->ord)*(s->ord - m->ord))) <= (1.2*SIZE_STARSHIP*SIZE_STARSHIP)){
	/* The missile m has riched the Starship S... */
	if (s->life > 1){
	  s->life--;
	}
	else{
	  /* Destrcution of the Starship with a big explosion... */
	  destroyed = 1;
	  next_if_destroyed = s->next;
	  add_explosion(E, create_small_explosion(s->abs, s->ord, 1));
	  if (prev_s == NULL){
	    *S = s->next;
	  }
	  else{
	    prev_s->next = s->next;
	  }
	  s->next = *D;
	  *D = s;
	}
	add_explosion(E, create_small_explosion(m->abs, m->ord, 0));
	if (prev_m == NULL)
	  *M = m->next;
	else
	  prev_m->next = m->next;
	free(m);
	if (destroyed)
	  break;
      }
      else{
	prev_m = m;
      }
      m = m->next;
    }
    if (!destroyed){
      prev_s = s;
      s = s->next;
    }
    else
      s = next_if_destroyed;
  }
}

int has_winner_team(S_list L){
  int first_finded = -1;
  
  while(L != NULL){
    if (first_finded == -1){
      first_finded = L->nb_team;
    }
    else if (L->nb_team != first_finded)
      return -1;
    L = L->next;
  }
  return first_finded;
}

void decision_maker_starship(S_list L, M_list* M){
  Starship* current=L;
  Decision_frame current_decision;
  Decidor f;
  
  while(current != NULL){
    current_decision.move = 0;
    current_decision.angle_starship = 0.0;
    current_decision.angle_gun = 0.0;
    current_decision.angle_radar = 0.0;
    current_decision.shot = 0;  

    f = (*(Decidor)((current->plugin->ai).take_decision));
    f(&current_decision);
    current->shot = current_decision.shot;
    current->move = current_decision.move;
    current->angle_move += current_decision.angle_starship;
    current->angle_gun += current_decision.angle_gun;
    current->angle_radar += current_decision.angle_radar;
    if (current->shot && (current->reload == 0))
      shot_starship(current, M);
    current = current->next;
  }
}

int end_frame_resolution(Space* S){
  decision_maker_starship(S->starships, &(S->missiles));
  move_starships(S->starships);
  move_missiles(&(S->missiles));
  relaod_starships_gun(S->starships, &(S->missiles));
  collision_starships_wall(S->starships);
  collision_between_starships(S->starships);
  resolve_explosion(&(S->explosions));
  collision_starship_missile(&(S->starships), &(S->missiles), &(S->explosions), &(S->deads));
  return has_winner_team(S->starships);
}

void free_space(Space* S){
  Starship* s;
  Missile* m;
  Explosion* e;
  
  while(S->starships != NULL){
    s = S->starships;
    S->starships = s->next;
    free(s);
  }
  while(S->deads != NULL){
    s = S->deads;
    S->deads = s->next;
    free(s);
  }
  while(S->missiles != NULL){
    m = S->missiles;
    S->missiles = m->next;
    free(m);
  }
  while(S->explosions != NULL){
    e = S->explosions;
    S->explosions = e->next;
    free(e);
  }
}

int nb_team_in_game(Space* W){
  Starship* s = W->starships;
  int ans=0;

  while (s != NULL){
    if (s->nb_team > ans)
      ans = s->nb_team;
    s = s->next;
  }
  return ans+1;
}

int nb_sharship_in_game(Space* W){
  Starship* s = W->starships;
  int ans=0;

  while (s != NULL){
    ans++;
    s = s->next;
  }
  return ans;
}

void initialize_plugins(S_list S){
  Starship* s = S;

  while(s != NULL){
    ((s->plugin->ai).init)();
    s = s->next;
  }
}

void clean_plugins(S_list S){
  Starship* s = S;

  while(s != NULL){
    ((s->plugin->ai).clean)();
    s = s->next;
  }
}

void save_frame_information(S_list S, M_list M, int frame){
  Starship* s = S;
  Missile* m = M;
  FILE* out = fopen("plugins/.info_frame.txt", "w");

  fprintf(out, "%d\n", frame);
  while (s != NULL){
    fprintf(out, "%s\n", s->plugin->name);
    fprintf(out, "%lf %lf %d %d %d %d %lf %lf %lf\n", s->abs, s->ord, 
	    s->life, s->move, s->nb_team, s->reload, 
	    s->angle_move, s->angle_gun, s->angle_radar);
    s = s->next;
  }
  fprintf(out, "MISSILES\n");
  while (m != NULL){
    fprintf(out, "%lf %lf %lf\n", m->abs, m->ord, m->angle);
    m = m->next;
  }
  fclose(out);
}
