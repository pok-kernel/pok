/*
 *                               POK header
 * 
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team 
 *
 * Created by julien on Tue Sep 22 13:45:12 2009 
 */

#include <gtypes.h>

void user_inceptor (int volts, int* force, int* position)
{
   (void) volts;
   (void) force;
   (void) position;
}

void user_af (int* backdrive, int* error)
{
   (void) backdrive;
   (void) error;
}

void user_aislc (int positionask, int* voltask, int measuredforce, int measuredposition, int* stickposition, int* error)
{
   (void) positionask;
   (void) voltask;
   (void) measuredforce;
   (void) measuredposition;
   (void) stickposition;
   (void) error;
}

void user_pfc (int stickposition, int* error, int* surfacepositionask, int surfacepositionfeedback)
{
   (void) stickposition;
   (void) error;
   (void) surfacepositionask;
   (void) surfacepositionfeedback;
}

void user_reu (int pfcerror, int aislcerror, int aferror, int externalerror, int* error)
{
   (void) pfcerror;
   (void) aislcerror;
   (void) aferror;
   (void) externalerror;
   (void) error;
}

void user_warning_display (int error)
{
   (void) error;
}
