#ifndef _PLUGINS_
#define _PLUGINS_

#include "game.h"
#include "util/list.h"

/**
 * \struct Decision_frame
 * \brief This structure gathers the information needed to pilot a starship each frame.
 *
 * Each frame, an A.I. can change the gear, change the angle move of
 * the starship, turn the gun and the radar and use its gun to shot
 * when this last one is reloaded.
 */
typedef struct _decision_frame {
    int move;               /*!< An integer for the gear : 0 neutral, 1 front gear, -1 reverse. */
    double angle_starship;  /*!< Turning angle : from -0.05 to 0.05 radian each frame. */
    double angle_gun;       /*!< Turning the gun : from -0.15 to 0.15 radian each frame. */
    double angle_radar;     /*!< Turning the radar : from -0.15 to 0.15 radian each frame. */
    int shot;               /*!< An integer for the use of the gun : 1 to shot, 0 otherwise. */
} Decision_frame;

#define DECIDOR_ENTRY_POINT "decision_frame"
#define INITIALIZOR_ENTRY_POINT "initialize_my_AI"
#define TERMINATOR_ENTRY_POINT "clean_my_AI"

typedef CONSUMER(Decision_frame*, Decidor);
typedef Runnable Initializor;
typedef Runnable Terminator;

/**
 * \struct AI
 * \brief The core objet on an A.I. handling the decision making.
 *
 * AI define a pointer function shortcut for the function handling the
 * decision making at each end of frame.
 */
typedef struct _ai {
    Decidor take_decision;
    Initializor init;
    Terminator clean;
} AI;

typedef struct _plugin {
    char* name;
    Starship* starship;
    AI ai;
    Boolean ok;
    Boolean selected;
} Plugin;

void Plugin_free(Plugin* self);
LIST_OF(Plugin) Plugin_load_all(char* directory);

#endif
