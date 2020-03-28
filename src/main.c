#define _DEFAULT_SOURCE

#include <MLV/MLV_all.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>
#include <dirent.h>

#include "../includes/misc.h"
#include "../includes/game.h"
#include "../includes/interface.h"
#include "../includes/acquisition.h"

void set_space(Space* adrr);
void initialise_space(Space* S);


int filter(const struct dirent* d){
  int i=strlen(d->d_name);
  if ((d->d_name[i-3] == '.') && (d->d_name[i-2] == 's') && (d->d_name[i-1] == 'o') && (d->d_name[i] == '\0'))
    return 1;
  return 0;
}

int main(int argc, char* argv[]){
  ParamInterface gp;
  double time_last_frame;
  double absolute_time;
  double to_wait;
  double program_busy;
  int winner = -1;
  int selection;
  Space S;

  /* auto-loading of plugins */
  int i;
  int nb_plugins;
  struct dirent** namelist;
  Plugin* plugins;
  char plug_path[512];
  int loading;
  void* handle;

  srand(time(NULL));

  printf("STARSHIP CODE CONTEST\n");

  /* initialise_space(S); */

  /* Plugins searching... */
  nb_plugins=scandir("plugins", &namelist, filter, alphasort);
  printf("Auto-loading of plugins...\n");
  printf("%d potential plugins founds : \n", nb_plugins);

  plugins=(Plugin*)malloc(nb_plugins*sizeof(Plugin));

  for (i=0 ; i<nb_plugins ; i++){
    printf("- %s", (namelist[i])->d_name);

    strcpy(plug_path, "plugins/");
    strcat(plug_path, (namelist[i])->d_name);

    loading=1;
    plugins[i].selected = 0;
    plugins[i].starship = NULL;

    plugins[i].name = (char*)malloc((strlen((namelist[i])->d_name)+1)*sizeof(char));
    strcpy(plugins[i].name, (namelist[i])->d_name);

    handle=dlopen(plug_path, RTLD_LAZY);;
    if (dlerror() != NULL){
      fprintf(stderr, "%s", dlerror());
      loading=0;
    }
    else{
      plugins[i].starship = NULL;
      plugins[i].decision_frame=(AI)dlsym(handle, "decision_frame");
      if (dlerror() != NULL){
	fprintf(stderr, "%s", dlerror());
	loading=0;
	dlclose(handle);
	continue;
      }
      plugins[i].init=((void (*)(void))dlsym(handle, "initialize_my_AI"));
      if (dlerror() != NULL){
	fprintf(stderr, "%s", dlerror());
	loading=0;
	dlclose(handle);
	continue;
      }
      plugins[i].clean=((void (*)(void))dlsym(handle, "clean_my_AI"));
      if (dlerror() != NULL){
	fprintf(stderr, "%s", dlerror());
	loading=0;
	dlclose(handle);
	continue;
      }
      /* dlclose(handle); */
    }
    if (loading){
      printf("... loading OK\n");
      plugins[i].ok = 1;
    }
    else{
      printf("... loading error\n");
      plugins[i].ok = 0;
    }
  }

  /* first part interface... */
  initialize_graphical_paramters(&gp);
  create_graphical_window(&gp);
  draw_introduction(&gp);
  selection = 0;
  S.starships = NULL;
  S.missiles = NULL;
  S.explosions = NULL;
  S.deads = NULL;
  while (!selection){
    selection = draw_selection_space(&gp, plugins, nb_plugins, &S);
  }
  /* Second part of interface : the game */
  initialize_plugins(S.starships);
  S.frame_number = 0;
  time_last_frame=get_absolute_time();
  program_busy = 0;
  while(winner == -1){
    save_frame_information(S.starships, S.missiles, S.frame_number);
    absolute_time = get_absolute_time();
    to_wait = absolute_time - time_last_frame;
    if (to_wait < (1.0/NB_FRAME_PER_SEC))
      MLV_wait_milliseconds((int)(((1.0/(double)NB_FRAME_PER_SEC) - to_wait)*1000));
    time_last_frame = get_absolute_time();
    program_busy += ((to_wait)/(1.0/(double)NB_FRAME_PER_SEC))*100;
    if (S.frame_number%NB_FRAME_PER_SEC == 0){
      S.busy = (int)(program_busy/NB_FRAME_PER_SEC);
      program_busy = 0;
    }
    draw_space(&gp, &S);
    /* This key function resolve the move and collisions of all
       elements of the space... */
    winner = end_frame_resolution(&S);
    /* Ok for the next frame. */
    S.frame_number++;
  }

  display_win(winner);
  MLV_wait_seconds(5);

  free_graphical_window(&gp);

  clean_plugins(S.starships);
  clean_plugins(S.deads);

  remove("plugins/.info_frame.txt");

  free_space(&S);
  printf("END OK\n");

  return 0;
}
