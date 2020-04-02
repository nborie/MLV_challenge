#include "../includes/interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void initialize_graphical_paramters(ParamInterface *gp)
{
    gp->abs_max = NB_PIXEL_SPACE_WIDTH;
    gp->ord_max = NB_PIXEL_SPACE_HEIGHT;
    gp->menu_width = NB_PIXEL_MENU;
}

void create_graphical_window(ParamInterface *gp)
{
    MLV_create_window("Starship code contest", NULL, gp->abs_max + gp->menu_width, gp->ord_max);
    initialize_graphical_paramters_extra(gp);
}

void initialize_graphical_paramters_extra(ParamInterface *gp)
{
    int i;
    char path[256];

    gp->background = MLV_load_image("img/background.png");
    if (gp->background == NULL)
    {
        fprintf(stderr, "Image loading error\n");
        exit(2);
    }
    gp->img_missile = MLV_load_image("img/bullet.png");
    if (gp->img_missile == NULL)
    {
        fprintf(stderr, "Image loading error\n");
        exit(2);
    }
    gp->img_starship = MLV_load_image("img/starship_red.png");
    if (gp->img_starship == NULL)
    {
        fprintf(stderr, "Image loading error\n");
        exit(2);
    }

    gp->border = (int)(((double)NB_PIXEL_SPACE_WIDTH / (double)SIZE_ABS) * SIZE_STARSHIP);
    gp->x_scale = (double)(NB_PIXEL_SPACE_WIDTH - (2 * gp->border)) / (double)SIZE_ABS;
    gp->y_scale = (double)(NB_PIXEL_SPACE_HEIGHT - (2 * gp->border)) / (double)SIZE_ORD;
    gp->width_life = NB_PIXEL_MENU / (3 * LIFE_STARSHIP);
    /* Loading the explosions images */
    gp->explosions = (MLV_Image ***)malloc(2 * sizeof(MLV_Image **));
    if (gp->explosions == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    /* Small explosions */
    gp->explosions[0] = (MLV_Image **)malloc(15 * sizeof(MLV_Image *));
    if (gp->explosions[0] == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    for (i = 1; i <= 15; i++)
    {
        sprintf(path, "img/explosions/explosion_petite_%d.png", i);
        gp->explosions[0][i - 1] = MLV_load_image(path);
        if (gp->explosions[0][i - 1] == NULL)
        {
            fprintf(stderr, "Image loading error\n");
            exit(2);
        }
    }
    /* Big explosions */
    gp->explosions[1] = (MLV_Image **)malloc(23 * sizeof(MLV_Image *));
    if (gp->explosions[1] == NULL)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    for (i = 1; i <= 23; i++)
    {
        sprintf(path, "img/explosions/explosion_%d.png", i);
        gp->explosions[1][i - 1] = MLV_load_image(path);
        if (gp->explosions[1][i - 1] == NULL)
        {
            fprintf(stderr, "Image loading error\n");
            exit(2);
        }
    }
}

void erase_graphical_window(ParamInterface *gp)
{
    MLV_draw_filled_rectangle(0, 0, gp->abs_max + gp->menu_width, gp->ord_max, MLV_rgba(0, 0, 0, 255));
}

void free_graphical_window(ParamInterface *gp)
{
    free_graphical_paramters(gp);
    MLV_free_window();
}

void display_fps(ParamInterface *gp, int frame_number, int busy)
{
    char s[128];

    MLV_draw_line(gp->abs_max + 1, gp->ord_max - 25, gp->abs_max + gp->menu_width,
                  gp->ord_max - 25, MLV_rgba(255, 255, 255, 255));
    sprintf(s, "(charge : %03d/100)  %d s - %02d frames", busy, frame_number / NB_FRAME_PER_SEC, frame_number % NB_FRAME_PER_SEC);
    MLV_draw_text(gp->abs_max + (gp->menu_width / 2) - 110, gp->ord_max - 20, s, MLV_rgba(255, 255, 255, 255));
}

void display_menu(ParamInterface *gp, S_list L, S_list D, int nb_team)
{
    int i;
    Starship *s;
    int abs = gp->abs_max + 20;
    int current_ord = 20;
    int finded;
    char msg[128];

    MLV_draw_filled_rectangle(gp->abs_max + 1, 0, gp->menu_width - 1, gp->ord_max, MLV_rgba(0, 0, 0, 255));
    MLV_draw_line(gp->abs_max + 1, 0, gp->abs_max + 1, gp->ord_max, MLV_rgba(255, 255, 255, 255));

    /* Alive starships... */
    for (i = 0; i < 13; i++)
    {
        s = L;
        finded = 0;
        while (s != NULL)
        {
            if (s->nb_team == i)
            {
                if (finded == 0)
                {
                    finded = 1;
                    sprintf(msg, "Team %d:", i + 1);
                    MLV_draw_text(abs, current_ord, msg, MLV_rgba(255, 255, 255, 255));
                    current_ord += 20;
                }
                sprintf(msg, "Starship %s", s->plugin->name);
                MLV_draw_text(abs, current_ord, msg, MLV_rgba(255, 255, 255, 255));
                current_ord += 20;
                MLV_draw_filled_rectangle(abs, current_ord, (s->life * gp->width_life), 5, MLV_rgba(0, 120, 0, 255));
                if (LIFE_STARSHIP - s->life > 0)
                    MLV_draw_filled_rectangle((s->life * gp->width_life) + abs, current_ord,
                                              ((LIFE_STARSHIP - s->life) * gp->width_life), 5, MLV_rgba(150, 0, 0, 255));
                current_ord += 30;
            }
            s = s->next;
        }
        if (finded == 1)
            current_ord += 10;
    }

    current_ord = 20;
    /* Dead starship */
    abs = gp->abs_max + (gp->menu_width / 2) + 20;
    s = D;
    finded = 0;
    while (s != NULL)
    {
        if (finded == 0)
        {
            finded = 1;
            sprintf(msg, "Dead starships :");
            MLV_draw_text(abs, current_ord, msg, MLV_rgba(255, 255, 255, 255));
            current_ord += 20;
        }
        sprintf(msg, "Starship %s", s->plugin->name);
        MLV_draw_text(abs, current_ord, msg, MLV_rgba(255, 255, 255, 255));
        current_ord += 20;
        s = s->next;
    }
}

void scale_point(ParamInterface *gp, int *x, int *y, const double *abs, const double *ord)
{
    *x = gp->x_scale * (*abs) + gp->border;
    *y = gp->y_scale * (*ord) + gp->border;
}

void draw_starship(ParamInterface *gp, Starship *s)
{
    int x, y, x_c, y_c;
    double x_gun, y_gun;

    int abs_radar[3];
    int ord_radar[3];
    double x_radar[3];
    double y_radar[3];

    /* Draw the body of the Starship. */
    scale_point(gp, &x, &y, &(s->abs), &(s->ord));
    MLV_draw_image(gp->img_starship, x - 20, y - 20);
    /* MLV_draw_filled_circle(x, y, gp->border, MLV_rgba(255,0,0,255)); */

    /* Draw the gun of the Starship. */
    x_gun = s->abs + cos(s->angle_gun) * (0.3);
    y_gun = s->ord + sin(s->angle_gun) * (0.3);
    scale_point(gp, &x_c, &y_c, &(x_gun), &(y_gun));

    MLV_draw_line(x, y, x_c, y_c, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(x + 1, y, x_c + 1, y_c, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(x, y + 1, x_c, y_c + 1, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(x - 1, y, x_c - 1, y_c, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(x, y - 1, x_c, y_c - 1, MLV_rgba(255, 255, 255, 255));

    MLV_draw_line(x + 1, y + 1, x_c + 1, y_c + 1, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(x - 1, y + 1, x_c - 1, y_c + 1, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(x - 1, y - 1, x_c - 1, y_c - 1, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(x + 1, y - 1, x_c + 1, y_c - 1, MLV_rgba(255, 255, 255, 255));

    /* Draw the vision of the Starship. */
    abs_radar[0] = x;
    ord_radar[0] = y;
    x_radar[1] = s->abs + cos(s->angle_radar - ANGLE_VISION) * ((0.5) * SIZE_ABS);
    y_radar[1] = s->ord + sin(s->angle_radar - ANGLE_VISION) * ((0.5) * SIZE_ORD);
    x_radar[2] = s->abs + cos(s->angle_radar + ANGLE_VISION) * ((0.5) * SIZE_ABS);
    y_radar[2] = s->ord + sin(s->angle_radar + ANGLE_VISION) * ((0.5) * SIZE_ORD);
    scale_point(gp, abs_radar + 1, ord_radar + 1, x_radar + 1, y_radar + 1);
    scale_point(gp, abs_radar + 2, ord_radar + 2, x_radar + 2, y_radar + 2);
    MLV_draw_filled_polygon(abs_radar, ord_radar, 3, MLV_rgba(0, 255, 0, 20));
}

void draw_starships(ParamInterface *gp, S_list L)
{
    while (L != NULL)
    {
        draw_starship(gp, L);
        L = L->next;
    }
}

void draw_missile(ParamInterface *gp, Missile *m)
{
    int x, y;

    scale_point(gp, &x, &y, &(m->abs), &(m->ord));
    MLV_draw_image(gp->img_missile, x - 6, y - 6);
}

void draw_missiles(ParamInterface *gp, M_list L)
{
    while (L != NULL)
    {
        draw_missile(gp, L);
        L = L->next;
    }
}

void draw_explosion(ParamInterface *gp, Explosion *e)
{
    int x, y;

    scale_point(gp, &x, &y, &(e->abs), &(e->ord));
    if (e->type == 0)
        MLV_draw_image(gp->explosions[0][e->current_img - 1], x - 16, y - 16);
    else
        MLV_draw_image(gp->explosions[1][e->current_img - 1], x - 32, y - 32);
}

void draw_explosions(ParamInterface *gp, E_list L)
{
    while (L != NULL)
    {
        draw_explosion(gp, L);
        L = L->next;
    }
}

void display_win(int nb_team)
{
    char msg[64];

    sprintf(msg, "Team %d win !!!", nb_team + 1);
    MLV_draw_filled_rectangle(100, 100, 400, 400, MLV_rgba(0, 0, 0, 255));
    MLV_draw_text(200, 200, msg, MLV_rgba(255, 255, 255, 255));
    MLV_actualise_window();
}

void draw_space(ParamInterface *gp, Space *S)
{
    erase_graphical_window(gp);

    MLV_draw_image(gp->background, 0, 0);

    draw_starships(gp, S->starships);
    draw_missiles(gp, S->missiles);
    draw_explosions(gp, S->explosions);

    display_menu(gp, S->starships, S->deads, S->nb_team);
    display_fps(gp, S->frame_number, S->busy);

    MLV_actualise_window();
}

void free_graphical_paramters(ParamInterface *gp)
{
    int i;

    MLV_free_image(gp->background);
    MLV_free_image(gp->img_missile);
    MLV_free_image(gp->img_starship);
    for (i = 1; i <= 15; i++)
        MLV_free_image(gp->explosions[0][i - 1]);
    free(gp->explosions[0]);
    for (i = 1; i <= 23; i++)
        MLV_free_image(gp->explosions[1][i - 1]);
    free(gp->explosions[1]);
    free(gp->explosions);
}

void draw_introduction(ParamInterface *gp)
{
    MLV_Image *intro = MLV_load_image("img/intro.png");

    MLV_draw_image(intro, 0, 0);
    MLV_actualise_window();
    MLV_wait_keyboard_or_mouse(NULL, NULL, NULL, NULL, NULL);
}

int draw_selection_space(ParamInterface *gp, List plugins, Space *W)
{
    int i;
    int x, y;
    int ord_current = 85;
    int abs_current = 90;
    int nb_team;
    int nb_starship = nb_sharship_in_game(W);
    Starship *new_starship = NULL;
    Starship *current = NULL;
    int selected_plugin = -1;
    char team_print[3];
    int team_number_hit = 0;
    int new_team;

    erase_graphical_window(gp);

    MLV_draw_text(500, 20, "Clic on bot and team number before launching battle", MLV_rgba(255, 255, 255, 255));

    MLV_draw_text(300, ord_current, "Starship bot available : ", MLV_rgba(255, 255, 255, 255));

    MLV_draw_line(70, 70, 70, 750, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(270, 110, 270, 750, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(470, 110, 470, 750, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(670, 70, 670, 750, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(70, 70, 670, 70, MLV_rgba(255, 255, 255, 255));

    for (i = 0; i < 18; i++)
        MLV_draw_line(70, 70 + i * 40, 670, 70 + i * 40, MLV_rgba(255, 255, 255, 255));

    ListItem plugin_index = HEAD(plugins);
    while (NON_NULL(plugin_index))
    {
        Plugin* current_plugin = CAST(Plugin*, ITEM_CONTENT(plugin_index));

        if ((i != 0) && (i % 16 == 0))
        {
            abs_current += 200;
            ord_current = 85;
        }

        ord_current += 40;
        if (current_plugin -> ok)
        {
            if (!(current_plugin -> selected))
                MLV_draw_text(abs_current + 10, ord_current, current_plugin -> name, MLV_rgba(255, 255, 255, 255));
            else
                MLV_draw_text(abs_current + 10, ord_current, "Bot in game...", MLV_rgba(0, 200, 0, 255));
        }
        else
        {
            MLV_draw_text(abs_current + 10, ord_current, "Loading failed...", MLV_rgba(255, 0, 0, 255));
        }
        
        plugin_index = NEXT(plugin_index);

    }

    abs_current += 200;
    ord_current = 85;

    MLV_draw_line(770, 70, 1230, 70, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(770, 70, 770, 666, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(1185, 126, 1185, 666, MLV_rgba(255, 255, 255, 255));
    MLV_draw_line(1230, 70, 1230, 666, MLV_rgba(255, 255, 255, 255));

    MLV_draw_text(950, ord_current, "Bot in Game : ", MLV_rgba(255, 255, 255, 255));
    MLV_draw_text(1185, ord_current, "Team", MLV_rgba(255, 255, 255, 255));

    for (i = 0; i < 13; i++)
    {
        MLV_draw_line(770, 70 + 56 + 45 * i, 1230, 70 + 56 + 45 * i, MLV_rgba(255, 255, 255, 255));
    }

    /* Starship in Game */
    i = 0;
    current = W->starships;
    while (current != NULL)
    {
        MLV_draw_text(790, 140 + 45 * i, (current->plugin)->name, MLV_rgba(255, 255, 255, 255));
        sprintf(team_print, "%d", (current->nb_team) + 1);
        MLV_draw_text(1202, 140 + 45 * i, team_print, MLV_rgba(255, 255, 255, 255));
        i++;
        current = current->next;
    }

    MLV_draw_rectangle(830, 690, 360, 60, MLV_rgba(255, 255, 255, 255));
    MLV_draw_filled_rectangle(833, 693, 354, 54, MLV_rgba(0, 150, 0, 255));
    MLV_draw_text(960, 710, "START GAME", MLV_rgba(0, 0, 0, 255));

    MLV_actualise_window();

    MLV_wait_mouse(&x, &y);

    /* clic on the start button */
    nb_team = nb_team_in_game(W);
    if ((((x > 830) && (x < 1190)) && ((y > 690) && (y < 750))))
    {
        if (nb_team > 1)
            return 1;
        else
            return 0;
    }

    /* clic on bot list */
    current = NULL;
    if (((x > 70) && (x < 670)) && ((y > 110) && (y < 750)))
    {
        for (i = 0; i < 18; i++)
        {
            if ((y > 70 + 40 * (i + 1)) && (y < 70 + 40 * (i + 2)))
                selected_plugin = i;
        }
        if (x > 270)
            selected_plugin += 16;
        if (x > 470)
            selected_plugin += 16;
        if (selected_plugin < LIST_SIZE(plugins))
        {
            Plugin* the_selected_plugin = LIST_GET(Plugin*, plugins, selected_plugin);
            if (nb_starship < 12 && !(the_selected_plugin->selected))
            {
                the_selected_plugin -> selected = 1;
                new_starship = create_starship(W->starships, 0);
                new_starship->plugin = the_selected_plugin;
                the_selected_plugin -> starship = new_starship;
                current = W->starships;
                if (current == NULL)
                {
                    W->starships = new_starship;
                }
                else
                {
                    while (current->next != NULL)
                        current = current->next;
                    current->next = new_starship;
                }
            }
        }
    }

    /* clic on team number */
    if (((x > 1185) && (x < 1230)) && ((y > 126) && (y < 666)))
    {
        for (i = 0; i < 18; i++)
        {
            if ((y > 126 + 45 * i) && (y < 126 + 45 * (i + 1)))
                team_number_hit = i;
        }
        current = W->starships;
        for (i = 0; i < team_number_hit; i++)
        {
            if (current == 0)
                return 0;
            else
            {
                current = current->next;
            }
        }
        /* change the team of current now... */
        new_team = current->nb_team + 1;
        /* no interest in puting a too much large number... */
        if ((new_team > nb_team) || (nb_sharship_in_game(W) <= new_team))
        {
            new_team = 0;
        }
        current->nb_team = new_team;
    }

    return 0;
}
