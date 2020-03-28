/**
 * \file rand_all.c 
 * \brief An example of artificial intelligence plugin doing everything randomly.
 * \version 0.1
 * \author Nicolas Borie ( nicolas dot borie at u-pem dot fr )
 * \date 10 march 2014
 *
 * This A.I. moves, turns gun and radar randomly. Using some static
 * variables, the starship will change randomly its moving angle, gun
 * angle and radar angle. As all decision are taken randomly, the
 * plugin move but doesn't listen to its radar.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../includes/acquisition.h"

static int turn_move=0;
static int turn_gun=0;
static int turn_radar=0;

static double move_modif=0;
static double gun_modif=0;
static double radar_modif=0;

static char me[12]="alea_all.so";

/**
 * void decision_frame(Decision_frame* d)
 * \brief the brilliant decision my AI is taking each frame.
 *
 * \param d : a Decision_frame structure passed by pointer
 * \return void
 *
 * All decision are taken randomly.
 */
void decision_frame(Decision_frame* d);

/**
 * void initialize_my_AI(void)
 * \brief there is nothing to intialize in this
 * \return void
 *
 * plugin but the symbol but be present for loading.
 */
void initialize_my_AI(void);

/**
 * void clean_my_AI(void)
 * \brief Clean nothing because there is nothing to clean.
 * \return void
 *
 * There is nothing to clean for this plugin but the symbol but be present for loading.
 */
void clean_my_AI(void);


void decision_frame(Decision_frame* d){
  d->move = 1;
  if (get_gun_status(me) == 0)
    d->shot = 1;
  else
    d->shot = 0;
  if (turn_move == 37){
    turn_move=0;
    move_modif = 0.05*(double)((rand() % 3) - 1);
  }
  else
    turn_move++;
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

  d->angle_starship = move_modif;
  d->angle_gun = gun_modif;
  d->angle_radar = radar_modif;
}

void initialize_my_AI(void){
  /* nada */
  return ;
}

void clean_my_AI(void){
  /* nada */
  return ;
}

