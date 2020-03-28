#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "../includes/acquisition.h"

#define ANGLE_VISION 0.4
#define PI 3.14159265359

typedef struct starship{
  double abs;            /*!< The position along absicca. */
  double ord;            /*!< The position along ordinate. */
  double angle_move;     /*!< An angle giving the trajectory. */
  double angle_gun;      /*!< The angle of the gun (trajectory of a shot). */
  double angle_radar;    /*!< The angle of the radar (where the starship is sanning). */
  int move;              /*!< The speed of the starship. */
  int shot;              /*!< A boolean telling if the starship is shoting. */
  int reload;            /*!< The state of the gun (ready to fire or relaoding). */
  int nb_team;           /*!< The number of the team. */
  int life;              /*!< The life of the starship. */
  struct plugin* plugin;
  struct starship *next; /*!< A pointeur to the next Starship. */
}Starship;               /*!< Alias for struct starship. */

static char plug_names[12][256];
static Starship starships[12];
static int nb_s = -1;
static View_missile missiles[240];
static int nb_m = -1;
static int frame_number = -1;

static View_missile missiles_radar[240];
static View_starship starships_radar[12];

void reset_static_variables(void){
  int i, j;
  for (i=0 ; i<12 ; i++){
    for (j=0; j<256 ; j++)
      plug_names[i][j] = '\0';
    starships[i].abs = -2.0;
    starships[i].ord = -2.0;
    starships[i].angle_move = 0.0;
    starships[i].angle_gun = 0.0;
    starships[i].angle_radar = 0.0;
    starships[i].move = -10;
    starships[i].life = -1;
    starships[i].reload = -1;
    starships[i].nb_team = -1;
  }
  for (i=0 ; i<240 ; i++){
    missiles[i].x = -2.0;
    missiles[i].y = -2.0;
    missiles[i].angle_move = 0.0;
  }
}

void reset_static_variables_radar(void){
  int i;
  for (i=0 ; i<12 ; i++){
    starships_radar[i].x = -2.0;
    starships_radar[i].y = -2.0;
    starships_radar[i].angle_move = 0.0;
    starships_radar[i].speed = -10;
    starships_radar[i].nb_team = -1;
  }
  for (i=0 ; i<240 ; i++){
    missiles_radar[i].x = -2.0;
    missiles_radar[i].y = -2.0;
    missiles_radar[i].angle_move = 0.0;
  }
}

void load_frame_information(void){
  int nb_starships = 0;
  int nb_missiles = 0;
  int frame;
  FILE* in = fopen("plugins/.info_frame.txt", "r");
  char name[256];

  assert(fscanf(in, "%d\n", &frame) == 1);
  if (frame == frame_number){
    fclose(in);
    return;
  }
  frame_number = frame;
  reset_static_variables();
  while(fscanf(in, "%s\n", name) == 1){
    if (strcmp(name, "MISSILES") == 0){
      break;
    }
    else{
      strcpy(plug_names[nb_starships], name);
    }
    assert(fscanf(in, "%lf %lf %d %d %d %d %lf %lf %lf\n", &(starships[nb_starships].abs), &(starships[nb_starships].ord), 
	   &(starships[nb_starships].life), &(starships[nb_starships].move), 
	   &(starships[nb_starships].nb_team), &(starships[nb_starships].reload), 
	   &(starships[nb_starships].angle_move), &(starships[nb_starships].angle_gun), 
		  &(starships[nb_starships].angle_radar)) == 9);
    nb_starships++;
  }
  while (fscanf(in, "%lf %lf %lf\n", &(missiles[nb_missiles].x), 
		&(missiles[nb_missiles].y), &(missiles[nb_missiles].angle_move)) == 3){
    nb_missiles++;
  }
  nb_s = nb_starships;
  nb_m = nb_missiles;
  fclose(in);
}

double get_x(char* me){
  int i;

  load_frame_information();
  for (i=0 ; i<12 ; i++){
    if (strcmp(me, plug_names[i]) == 0)
      return starships[i].abs;
  }
  return 0.0;
}

double get_y(char* me){
  int i;

  load_frame_information();
  for (i=0 ; i<12 ; i++){
    if (strcmp(me, plug_names[i]) == 0)
      return starships[i].ord;
  }
  return 0.0;
}

int get_life(char* me){
  int i;

  load_frame_information();
  for (i=0 ; i<12 ; i++){
    if (strcmp(me, plug_names[i]) == 0)
      return starships[i].life;
  }
  return 0;
}

int get_speed(char* me){
  int i;

  load_frame_information();
  for (i=0 ; i<12 ; i++){
    if (strcmp(me, plug_names[i]) == 0)
      return starships[i].move;
  }
  return 0;
}

int get_nb_team(char* me){
  int i;

  load_frame_information();
  for (i=0 ; i<12 ; i++){
    if (strcmp(me, plug_names[i]) == 0)
      return starships[i].nb_team;
  }
  return -1;
}

int get_gun_status(char* me){
  int i;

  load_frame_information();
  for (i=0 ; i<12 ; i++){
    if (strcmp(me, plug_names[i]) == 0)
      return starships[i].reload;
  }
  return 0;
}

double get_move_angle(char* me){
  int i;

  load_frame_information();
  for (i=0 ; i<12 ; i++){
    if (strcmp(me, plug_names[i]) == 0)
      return starships[i].angle_move;
  }
  return 0.0;
}

double get_gun_angle(char* me){
  int i;

  load_frame_information();
  for (i=0 ; i<12 ; i++){
    if (strcmp(me, plug_names[i]) == 0)
      return starships[i].angle_gun;
  }
  return 0.0;
}

double get_radar_angle(char* me){
  int i;

  load_frame_information();
  for (i=0 ; i<12 ; i++){
    if (strcmp(me, plug_names[i]) == 0)
      return starships[i].angle_radar;
  }
  return 0.0;
}

double angle_to_target(double x, double y, double a, double b){
  double angle_target;

  angle_target = atan((b - y)/(a - x));
  if ((angle_target > 0.0 && (y > b)) || (angle_target < 0.0 && (y < b))){
    angle_target += PI;
  }
  return angle_target;
}

int is_angle_close(double a1, double a2, double diff){
  double a;

  a = a1-a2;
  while (a > PI){
    a -= 2*PI;
  }
  while (a < -PI){
    a += 2*PI;
  }
  return ((a < diff) && (a > -diff));
}

int is_point_visible_by_radar(double abs, double ord, double radar_angle, double x, double y){
  return is_angle_close(angle_to_target(abs, ord, x, y), radar_angle, ANGLE_VISION);
}

void get_scan_from_radar(char* me, View_starship** S, int* nb_starship, View_missile** M, int* nb_missile){
  int nb_s_radar=0;
  int nb_m_radar=0;
  int i;
  int my_index=0;

  load_frame_information();
  reset_static_variables_radar();

  /* identify the caller... */
  for (i=0 ; i<12 ; i++){
    if (strcmp(me, plug_names[i]) == 0)
      my_index = i;
  }
  /* scan starships... */
  for (i=0 ; i<nb_s ; i++){
    if (i != my_index){
      if (is_point_visible_by_radar(starships[my_index].abs, starships[my_index].ord, 
				    starships[my_index].angle_radar, starships[i].abs, starships[i].ord)){
	starships_radar[nb_s_radar].x = starships[i].abs;
	starships_radar[nb_s_radar].y = starships[i].ord;
	starships_radar[nb_s_radar].angle_move = starships[i].angle_move;
	starships_radar[nb_s_radar].speed = starships[i].move;
	starships_radar[nb_s_radar].nb_team = starships[i].nb_team;
	nb_s_radar++;
      }
    }
  }
  *S = starships_radar;
  *nb_starship = nb_s_radar;

  /* scan missiles... */
  for (i=0 ; i<nb_m ; i++){
    if (is_point_visible_by_radar(starships[my_index].abs, starships[my_index].ord, 
				  starships[my_index].angle_radar, missiles[i].x, missiles[i].y)){
      missiles_radar[nb_m_radar].x = missiles[i].x;
      missiles_radar[nb_m_radar].y = missiles[i].y;
      missiles_radar[nb_m_radar].angle_move = missiles[i].angle_move;
      nb_m_radar++;
    }
  }
  *M = missiles_radar;
  *nb_missile = nb_m_radar;
  return ;
}
