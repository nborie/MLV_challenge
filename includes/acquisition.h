/**
 * \file acquisition.h 
 * \brief Haeder file for the acquisition functionalities.
 * \version 0.1
 * \author Nicolas Borie ( nicolas dot borie at u-pem dot fr )
 * \date 10 march 2014
 *
 */

#ifndef _ACQUISITION_
#define _ACQUISITION_

/**
 * \struct View_starship
 * \brief This structure gathers information scaned by the radar about other starships.
 *
 * When a radar scans and detect another starship in its vision area,
 * information are thus collected in a such structure. The radar can
 * see the exact position, the angle of move, the gear of speed and
 * detect the team of the starship.
 */
typedef struct{
  double x;           /*!< The position in absicca of the starship : beetwen 0.0 and 10.0 (some side effects). */
  double y;           /*!< The position in ordinate of the starship : beetwen 0.0 and 10.0 (some side effects). */
  double angle_move;  /*!< The angle of move of the starship in radian. */
  int speed;          /*!< The gear speed of the starship : 0 neutral, 1 front gear, -1 reverse. */
  int nb_team;        /*!< An integer corresponding the team of the starship. */
}View_starship;


/**
 * \struct View_missile
 * \brief This structure gathers information scaned by the radar about missiles.
 *
 * When a radar scans and detect a missile in its vision area,
 * information are thus collected in a such structure. The radar can
 * see the exact position and the angle of move of a missile.
 */
typedef struct{
  double x;           /*!< The position in absicca of the missile : beetwen -1.0 and 11.0 (some side effects). */
  double y;           /*!< The position in ordinate of the missile : beetwen -1.0 and 11.0 (some side effects). */
  double angle_move;  /*!< The angle of move of the missile in radian. */
}View_missile;

/**
 * \fn double get_x(char* me)
 * \brief Get the position in absicca of the starship piloted by the plugin me.
 *
 * \param me : a plugin name 
 * \return A double floating number
 *
 * The function returns a double between 0.0 and 10.0 corresponding
 * the position in absicca of the starship piloted by the plugin
 * `me`. Some side effect can happen : sometimes and only during one
 * or two frames, the absicca can crosses the bounds during collision
 * resolution.
 */
double get_x(char* me);

/**
 * \fn double get_y(char* me)
 * \brief Get the position in ordinate of the starship piloted by the plugin me.
 *
 * \param me : a plugin name 
 * \return A double floating number
 *
 * The function returns a double between 0.0 and 10.0 corresponding
 * the position in ordinate of the starship piloted by the plugin
 * `me`. Some side effect can happen : sometimes and only during one
 * or two frames, the ordinate can crosses the bounds during collision
 * resolution.
 */
double get_y(char* me);

/**
 * \fn int get_life(char* me)
 * \brief Get the life of the starship piloted by the plugin me.
 *
 * \param me : a plugin name 
 * \return An integer which is the number of hits that the starship can handle before exploding.
 *
 * The function returns a integer which is the number of hits that the
 * starship can handle before exploding. By default, starships have
 * ten life points at the beginning of the fight.
 */
int get_life(char* me);

/**
 * \fn int get_speed(char* me)
 * \brief Get the gear of the starship piloted by the plugin me.
 *
 * \param me : a plugin name 
 * \return An integer which indicates the gear of the starship.
 *
 * The function returns a integer which is the gear of the
 * starship. If the starship doesn't move, it returns `0` for the
 * neutral gear, `1` for the front gear and `-1` for the reverse gear.
 */
int get_speed(char* me);

/**
 * \fn int get_nb_team(char* me)
 * \brief Get the number of the team of the starship piloted by the plugin me.
 *
 * \param me : a plugin name 
 * \return An integer which indicates the number of the team of the starship.
 *
 * The function returns a integer which is the internal number of the
 * team of the starship. The graphical interface displays team number
 * between `1` and `n` when internally, team number goes from `0` to
 * `n-1`.
 */
int get_nb_team(char* me);

/**
 * \fn int get_gun_status(char* me)
 * \brief Get the number of frames needed to reload the gun of the starship piloted by the plugin me.
 *
 * \param me : a plugin name 
 * \return An integer which indicates the number of frames needed to reload the gun of the starship.
 *
 * The function returns a integer which is the number of frames needed
 * to reload the gun of the starship.
 */
int get_gun_status(char* me);

/**
 * \fn double get_move_angle(char* me)
 * \brief Get the moving angle of the starship piloted by the plugin me.
 *
 * \param me : a plugin name 
 * \return A double which is the moving angle of the starship in radian.
 *
 * The function returns a double which is the moving angle of the
 * starship in radian. When you are facing to your screen as the pixel
 * up left is of coordinates `(0,0)`, an angle 0 radian indicates the
 * left, PI/2 indicate the down direction. The trigonometric circle
 * thus turn clockwise (Be aware!!!!).
 */
double get_move_angle(char* me);

/**
 * \fn double get_gun_angle(char* me)
 * \brief Get the angle of the gun of the starship piloted by the plugin me.
 *
 * \param me : a plugin name 
 * \return A double which is the angle of the gun of the starship in radian.
 *
 * The function returns a double which is the angle of the gun of the
 * starship in radian. When you are facing to your screen as the pixel
 * up left is of coordinates `(0,0)`, an angle 0 radian indicates the
 * left, PI/2 indicate the down direction. The trigonometric circle
 * thus turn clockwise (Be aware!!!!).
 */
double get_gun_angle(char* me);

/**
 * \fn double get_radar_angle(char* me)
 * \brief Get the angle of the radar of the starship piloted by the plugin me.
 *
 * \param me : a plugin name 
 * \return A double which is the angle of the radar of the starship in radian.
 *
 * The function returns a double which is the angle of the radar of the
 * starship in radian. When you are facing to your screen as the pixel
 * up left is of coordinates `(0,0)`, an angle 0 radian indicates the
 * left, PI/2 indicate the down direction. The trigonometric circle
 * thus turn clockwise (Be aware!!!!).
 */
double get_radar_angle(char* me);

/**
 * \fn void get_scan_from_radar(char* me, View_starship** S, int* nb_starship, View_missile** M, int* nb_missile);
 * \brief Set information that the radar can get in its current position.
 *
 * \param me: a plugin name.
 * \param S: a pointer to an address of array of `View_starship`, thus of type `View_starship**`
 * \param nb_starship: a pointer to an integer.
 * \param M: a pointer to an address of array of `View_missile`, thus of type `View_missile**`
 * \param nb_missile : a pointer to an integer.
 * \return A double which is the angle of the radar of the starship in radian.
 *
 * This function collect all elements that your radar can
 * see... Visible starship will be placed in an array at address `*S`
 * and will contains `*nb_starship` elements. It does the same for
 * missiles. The function does the allocation and liberation of memory
 * for you.
 */
void get_scan_from_radar(char* me, View_starship** S, int* nb_starship, View_missile** M, int* nb_missile);

#endif
