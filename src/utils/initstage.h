#ifndef INITUTILS_H_
#define INITUTILS_H_

/* Init sequence:
 * 0.  Initialize cross layer databases
 * 1.  Fetch parameters, register to signals, write to cross layer databases
 * 2.  Fetch pointers to other modules, read from cross layer databases
 * >3. (Custom init stages)
 * N-1 Last stage before simulation starts

 It is possible to change the number of custom init stages by using the NUM_CUSTOM_INIT_STAGES macro.
 */

/***** CUSTOM SECTION *****/
#define NUM_CUSTOM_INIT_STAGES 2  // redefine if needed

/***** STD SECTION - DO NOT MODIFY *****/
#define INITSTAGE_ENVIRONMENT           0
#define INITSTAGE_BASEMODULE            1

#define INITSTAGE_PARAMS_FETCH          2
#define INITSTAGE_STATE_INIT            2
#define INITSTAGE_SIGNAL_REGISTRATION   2

#define INITSTAGE_MODULES_FETCH         3

// leave std init stages
#define INITSTAGE_EXIT_STD              4  // end of std init stages

#define NUM_INIT_STAGES                 INITSTAGE_EXIT_STD + NUM_CUSTOM_INIT_STAGES + 1

// reenter std init stages for last init stage
#define INITSTAGE_LAST_STAGE            NUM_INIT_STAGES - 1
#define INITSTAGE_BEEP_INIT             INITSTAGE_LAST_STAGE
#define INITSTAGE_GRAPHICS_INIT         INITSTAGE_LAST_STAGE
#define INITSTAGE_STATS_SUBSCRIPTION    INITSTAGE_LAST_STAGE

#endif