#define _DEFAULT_SOURCE

#include <MLV/MLV_all.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "../includes/util/misc.h"
#include "../includes/util/error.h"
#include "../includes/game.h"
#include "../includes/interface.h"
#include "../includes/acquisition.h"
#include "../includes/util/str.h"
#include "../includes/util/list.h"

/* void set_space(Space *adrr); */
/* void initialise_space(Space *S); */

int main(int argc, char** argv)
{

    ParamInterface gp;
    double time_last_frame;
    double absolute_time;
    double to_wait;
    double program_busy;
    int winner = -1;
    int selection;
    Space S;
    
    INIT_RANDOM();

    /* auto-load available plugins */
    List plugins = Plugin_load_all("plugins/");
    if (LIST_EMPTY(plugins))
        THROW_UNFORMATTED_AND_KILL("No plugin found.", "Cannot start the challenge.");

    /* first part interface... */
    initialize_graphical_paramters(&gp);
    MLV_init_audio();
    create_graphical_window(&gp);
    
    MLV_Music* main_theme = MLV_load_music("assets/audio/MainTheme.ogg");
    MLV_play_music(main_theme, 1.0, -1);

    /* initialise_space(S); */
    draw_introduction(&gp);
    selection = 0;
    S.starships = NULL;
    S.missiles = NULL;
    S.explosions = NULL;
    S.deads = NULL;
    while (!selection)
        selection = draw_selection_space(&gp, plugins, &S);
    
    /* Second part of interface : the game */
    initialize_plugins(S.starships);
    S.frame_number = 0;
    time_last_frame = get_absolute_time();
    program_busy = 0;
    while (winner == -1)
    {
        save_frame_information(S.starships, S.missiles, S.frame_number);
        absolute_time = get_absolute_time();
        to_wait = absolute_time - time_last_frame;
        if (to_wait < (1.0 / NB_FRAME_PER_SEC))
            MLV_wait_milliseconds((int)(((1.0 / (double)NB_FRAME_PER_SEC) - to_wait) * 1000));
        time_last_frame = get_absolute_time();
        program_busy += ((to_wait) / (1.0 / (double)NB_FRAME_PER_SEC)) * 100;
        if (S.frame_number % NB_FRAME_PER_SEC == 0)
        {
            S.busy = (int)(program_busy / NB_FRAME_PER_SEC);
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

    MLV_free_music(main_theme);
    MLV_free_audio();
    free_graphical_window(&gp);

    clean_plugins(S.starships);
    clean_plugins(S.deads);

    remove("plugins/.info_frame.txt");

    free_space(&S);
    printf("END OK\n");

    return 0;
    
}
