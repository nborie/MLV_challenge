#ifndef _INTERFACE_
#define _INTERFACE_

#include <MLV/MLV_all.h>
#include "game.h"

#define NB_PIXEL_SPACE_WIDTH 800
#define NB_PIXEL_SPACE_HEIGHT 800
#define NB_PIXEL_MENU 500

typedef struct{
  int abs_max;             /*!< The number of pixels of the Space (width). */
  int ord_max;             /*!< The number of pixels of the Space (height). */
  int menu_width;          /*!< The width of th menu on the rigth (in pixels). */
  MLV_Image* background;   /*!< A background image for the Space. */
  MLV_Image* img_missile;  /*!< An image for Missile in the Space. */
  MLV_Image* img_starship; /*!< A default image for Starship. */
  MLV_Image*** explosions; /*!< A pointeur to a 2d-array the images of explosions. */
  double x_scale;          /*!< Dilatation coefficient in absicca. */
  double y_scale;          /*!< Dilatation coefficient in ordinate. */
  int border;              /*!< Border of the space in number of pixels. */
  int width_life;          /*!< The width in pixels of a life point of Starship. */
}ParamInterface;


void initialize_graphical_paramters(ParamInterface* gp);
void create_graphical_window(ParamInterface* gp);
void initialize_graphical_paramters_extra(ParamInterface* gp);
void erase_graphical_window(ParamInterface* gp);
void free_graphical_window(ParamInterface* gp);
void draw_space(ParamInterface* gp, Space* S);
void scale_point(ParamInterface* gp, int *x, int *y, const double *abs, const double *ord);
void display_win(int nb_team);
void free_graphical_paramters(ParamInterface* gp);
void draw_introduction(ParamInterface* gp);
int draw_selection_space(ParamInterface* gp, Plugin* plugins, int nb_plugin, Space* W);

#endif
