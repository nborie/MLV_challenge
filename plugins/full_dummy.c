/**
 * \file full_dummy.c 
 * \brief An example of artificial intelligence plugin completely dummy.
 * \version 0.1
 * \author Nicolas Borie ( nicolas dot borie at u-pem dot fr )
 * \date 10 march 2014
 *
 * The starship piloted by this plugin just do nothing.
 */

#include <stdio.h>
#include "../includes/acquisition.h"
#include "../includes/plugins.h"

void decision_frame(Decision_frame* d);
void initialize_my_AI(void);
void clean_my_AI(void);

void decision_frame(Decision_frame* d){
  return ;
}

void initialize_my_AI(void){
  return ;
}

void clean_my_AI(void){
  return ;
}
