/**
 * \file rand_move_info.c 
 * \brief An example of artificial intelligence plugin moving randomly and displaying information.
 * \version 0.1
 * \author Nicolas Borie ( nicolas dot borie at u-pem dot fr )
 * \date 10 march 2014
 *
 * This A.I. moves, and turns radar randomly. Using some static
 * variables, the starship will change randomly its moving angle and
 * radar angle. Each frame, this A.I. write on the standard output
 * information it collected.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../includes/acquisition.h"

static int turn_move=0;
static int turn_radar=0;

static double move_modif=0;
static double radar_modif=0;

static char me[18]="rand_move_info.so";

/**
 * void decision_frame(Decision_frame* d)
 * \brief Function which produce a random move and scan elements with the radar.
 *
 * \param d: a pointer to Decision_frame structure
 * \return void
 *
 * One a random move of the starship and the radar is done, the plugin
 * write all information the standard output. To left you time to read
 * it, the plugin make a pause waiting for you to type the `Enter`
 * key. THIS IS NOT A VALID PLUGIN... Its purpose is just to show you
 * how to collect all possible information. This is also a test plugin
 * to check that the internal trigonometric functions work fine.
 */
void decision_frame(Decision_frame* d){
  View_starship* S=NULL;
  int nb_starship;
  View_missile* M=NULL; 
  int nb_missile;
  int i;

  d->move = 1;
  if (turn_move == 37){
    turn_move=0;
    move_modif = 0.05*(double)((rand() % 3) - 1);
  }
  else
    turn_move++;
  if (turn_radar == 15){
    turn_radar = 0;
    radar_modif = 0.15*(double)((rand() % 3) - 1);
  }
  else
    turn_radar++;

  d->angle_starship = move_modif;
  d->angle_radar = radar_modif;

  printf("\n ---- FRAME INFORMATIONS ---- \n");
  printf("My position in abscicca : %lf\n", get_x(me));
  printf("My position in ordinate : %lf\n", get_y(me));
  printf("My life point : %d\n", get_life(me));
  printf("My gear (1=front, 0=neutral, -1=reverse) : %d\n", get_life(me));
  printf("My team number : %d\n", get_nb_team(me));
  printf("My gun status (0 if ready for fire) : %d\n", get_nb_team(me));
  printf("My moving angle : %lf\n", get_move_angle(me));
  printf("My gun angle : %lf\n", get_gun_angle(me));
  printf("My radar angle : %lf\n", get_radar_angle(me));

  get_scan_from_radar(me, &S, &nb_starship, &M, &nb_missile);

  printf("\n--> What my radar see :\n");
  for (i=0 ; i<nb_starship ; i++){
    printf("I see a starship : \n");
    printf("x = %lf\n", S[i].x);
    printf("y = %lf\n", S[i].y);
    printf("moving angle = %lf\n", S[i].angle_move);
    printf("gear = %d\n", S[i].speed);
    printf("team number = %d\n", S[i].nb_team);
  }
  for (i=0 ; i<nb_missile ; i++){
    printf("I see a missile : \n");
    printf("x = %lf\n", M[i].x);
    printf("y = %lf\n", M[i].y);
    printf("moving angle = %lf\n", M[i].angle_move);
  }
  assert(scanf("%*c") == 0);
}

/**
 * void initialize_my_AI(void)
 * \brief Initialization of this test plugin
 * 
 * \return void
 *
 * There is nothing to initialize in this plugin.
 */
void initialize_my_AI(void){
  return ;
}

/**
 * void clean_my_AI(void)
 * \brief Clean this test plugin
 * 
 * \return void
 *
 * There is nothing needing to be cleaned in this plugin.
 */
void clean_my_AI(void){
  return ;
}
