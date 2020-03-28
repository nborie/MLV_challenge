#ifndef _GAME_
#define _GAME_

#include "acquisition.h"

#define NB_FRAME_PER_SEC 32
#define LIFE_STARSHIP 4
#define PI 3.14159265359
#define SIZE_ABS 10
#define SIZE_ORD 10
#define SIZE_STARSHIP 0.25
#define ANGLE_VISION 0.4
#define MISSILE_SPEED SIZE_ABS/(2.0*NB_FRAME_PER_SEC)
#define STARSHIP_SPEED SIZE_ABS/(5.5*NB_FRAME_PER_SEC)
#define VANISH_LIMIT (0.1*SIZE_ABS)
#define RELOAD_GUN 32

typedef struct missile{
  double abs;            /*!< The position along absicca. */
  double ord;            /*!< The position along ordinate. */
  double angle;          /*!< An angle giving the trajectory. */
  struct missile *next;  /*!< A reference to the next missile (NULL if none). */
}Missile;                /*!< Alias for struct missile. */

typedef Missile* M_list;

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


typedef Starship* S_list; /*!< Alias for Starship*. */

typedef struct{
  Starship* ships;  /*!< An array of Starships. */
  int nb_member;    /*!< The number of Starships in the team. */
  int team_number;  /*!< The number of the Team. */
}Team;

#define NB_FRAME_EXPLOSION_PER_IMG 2

typedef struct explosion{
  double abs;              /*!< The absicca of the position at which the explosion is displayed. */
  double ord;              /*!< The ordinate of the position at which the explosion is displayed. */
  int type;                /*!< The type of the explosion (0=small, 1=big). */
  int current_img;         /*!< The index of the current image. */
  int current_frame;       /*!< Number of frames in which the current image is displayed. */
  int nb_img;              /*!< Number of image in the explosion. */
  struct explosion* next;  /*!< Link to the next explosion to display. */
}Explosion;                /*!< Alias for struct explosion. */

typedef Explosion* E_list; /*!< Alias for Explosion*. */

typedef struct{
  double abs_max;    /*!< The maximum absicca of the Space. */
  double ord_max;    /*!< The maximum ordinate of the Space. */
  M_list missiles;   /*!< The list of all Missiles. */
  S_list starships;  /*!< The list of all Starships. */
  S_list deads;      /*!< The list of dead Starships. */
  E_list explosions; /*!< The list of all Explosions. */
  Team* team;        /*!< The array of team present in the Space. */
  int nb_team;       /*!< The number of team in the Space. */
  int frame_number;  /*!< The number of the current frame. */
  int busy;          /*!< An integer between 0 and 100 measuring the charge of the program. */
}Space;


typedef struct plugin{
  char* name;
  Starship* starship;
  AI decision_frame;
  void (*init)(void);
  void (*clean)(void);
  int ok;
  int selected;
}Plugin;


Starship* create_starship(S_list L, int nb_team);
void get_random_position_without_colision(S_list L, double* x, double* y);
void add_starship(S_list* L, int nb_team);
Missile* create_missile(double abs, double ord, double angle);
void add_missile(M_list* L, double abs, double ord, double angle);
int number_missiles(M_list* L);
void move_missiles(M_list* L);
void free_missiles(M_list* L);
void collision_starship_wall(Starship* s);
void collision_starships_wall(S_list L);
int end_frame_resolution(Space* S);
void free_space(Space* S);
int nb_team_in_game(Space* W);
int nb_sharship_in_game(Space* W);
void initialize_plugins(S_list S);
void clean_plugins(S_list S);
void save_frame_information(S_list S, M_list M, int frame);

#endif
