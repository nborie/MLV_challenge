/**
 * \file rand_shot.c 
 * \brief An example of artificial intelligence plugin shoting randomly but not moving.
 * \version 0.1
 * \author Nicolas Borie ( nicolas dot borie at u-pem dot fr )
 * \date 10 march 2014
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "../includes/acquisition.h"

void decision_frame(Decision_frame* d);
void initialize_my_AI(void);
void clean_my_AI(void);

static int turn_gun=0;
static int turn_radar=0;

static double gun_modif=0;
static double radar_modif=0;

static char me[12]="alea_shot.so";

/**
 * void decision_frame(Decision_frame* d)
 * \brief turning gun and radar randomly and shoting everytime it is possible.
 *
 * \param d: a pointer to a Decision_frame structure
 * \return void
 */
void decision_frame(Decision_frame* d){
  d->move = 0;
  if (get_gun_status(me) == 0)
    d->shot = 1;
  else
    d->shot = 0;
  if (turn_gun == 21){
    turn_gun=0;
    gun_modif = 0.15*(double)((rand() % 3) - 1);
  }
  else
    turn_gun++;
  if (turn_radar == 15){
    turn_radar = 0;
    radar_modif = 0.15*(double)((rand() % 3) - 1);
  }
  else
    turn_radar++;

  d->angle_gun = gun_modif;
  d->angle_radar = radar_modif;
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
