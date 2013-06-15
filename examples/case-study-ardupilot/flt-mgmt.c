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
 * Created by julien on Thu Oct 15 13:04:22 2009 
 */

#include <libc/stdio.h>
#include <core/time.h>
#include <libm.h>
#include <gtypes.h>

#define   NB_WAYPOINTS 6

#define   HEADING_MAX 15
#define   HEADING_MIN -15

#define   ALTITUDE_MAX 40
#define   ALTITUDE_MIN -45

#define   REVERSE_YAW 1 

#define DISTANCE_LIMIT 4000
/*
 * The max distance allowed to travel from home.  
 */

#define KP_HEADING 10
#define KI_HEADING .01
#define KD_HEADING 0.001

#define KP_ALTITUDE 4
#define KI_ALTITUDE 0.001
#define KD_ALTITUDE 2

#define LAUNCH_ALTITUDE 0 /* Set to 0 by default */

#define RAD_TO_DEG 57.295779513082320876798154814105
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

uint8_t   current_wp=1;
/* This variables stores the actual waypoint we are trying to reach..  */

float wp_lat[NB_WAYPOINTS+1];
float wp_lon[NB_WAYPOINTS+1];
int   wp_alt[NB_WAYPOINTS+1];

uint32_t wp_distance=0;   /* Stores the distances from the current waypoint */

int      middle_yaw=90;               /* The central position of yaw */
int      middle_thr=90;               /* The central position */


int heading_previous_error; 
float heading_I;             /* Stores the result of the integrator */
int altitude_previous_error;
float altitude_I;            /* Stores the result of the integrator */

int wp_bearing=0;             /* Stores the bearing from the current waypoint */

int   course=0;            /* Course over ground... */

const float  kp[]={KP_HEADING,KP_ALTITUDE};	
const float  ki[]={KI_HEADING,KI_ALTITUDE};	 
const float  kd[]={KD_HEADING,KD_ALTITUDE};

uint64_t millis ()
{
   uint64_t tmp;
   pok_time_gettick (&tmp);
   return tmp;
}


int PID_altitude(int PID_set_Point, int PID_current_Point)
{
  static unsigned int altitude_PID_timer;
  /*
   * Timer to calculate the dt of the PID
   */

  static float altitude_D;
  /* 
   * Stores the result of the derivator
   */

  static int altitude_output;
  /*
   * Stores the result of the PID loop  
   */

  int PID_error=0;

  float dt=(float)(millis()-altitude_PID_timer)/1000; 
  /* calculating dt, you must divide it by 1000, 
   * because this system only undestand seconds.. 
   * and is normally given in millis
   */

  PID_error=PID_set_Point-PID_current_Point;
  /* Computes the error */

  altitude_I+= (float)PID_error*dt; 
  altitude_I=constrain(altitude_I,20,-20); /* Limit the PID integrator...  */
  /* Integratior part */

  altitude_D=(float)((float)PID_error-(float)altitude_previous_error)/((float)dt);
  /* Derivation part */

  altitude_output= (kp[1]*PID_error);  /* Adding proportional */
  altitude_output+=(ki[1]*altitude_I); /* Adding integrator result.. */
  altitude_output+= (kd[1]*altitude_D);/* Adding Derivator result.. */

  /* Plus all the PID results and limit the output...  */
  altitude_output = constrain(altitude_output,ALTITUDE_MIN,ALTITUDE_MAX);
  /* PID_P+PID_I+PID_D */

  altitude_previous_error=PID_error;
  /* Saving the actual error to use it later (in derivating part)... */

  altitude_PID_timer=millis();
  /* Saving the last execution time, important to calculate the dt...  */

  return altitude_output;
  /* Returns the result */
}

void flt_mgmt_init ()
{
   wp_distance = 0;
   middle_thr  = 90;
   middle_yaw  = 90;

   /* 
    * Declaring waypoints 
    * static declaration
    */

   wp_lat[1]=  34.982613;
   wp_lon[1]= -118.443357; 
   wp_alt[1]=50;
   /* meters */

   wp_lat[2]= 34.025136;
   wp_lon[2]=-118.445254; 
   wp_alt[2]=100;
   /* meters */

   wp_lat[3]=34.018287;
   wp_lon[3]=-118.456048; 
   wp_alt[3]=100;
   /* meters */

   wp_lat[4]= 34.009332;
   wp_lon[4]=-118.467672; 
   wp_alt[4]=50;
   /* meters */

   wp_lat[5]=  34.006476;
   wp_lon[5]=-118.465413; 
   wp_alt[5]=50;
   /* meters */

   wp_lat[6]=  34.009927;
   wp_lon[6]= -118.458320; 
   wp_alt[6]= 20;
   /* meters */

}

int PID_heading(int PID_error)
{ 
  static unsigned int heading_PID_timer;
  /* Timer to calculate the dt of the PID */
  static float heading_D; 
  /* Stores the result of the derivator */
  static int heading_output;
  /* Stores the result of the PID loop */
  float dt=(float)(millis()-heading_PID_timer)/1000;
  /* calculating dt, you must divide it by 1000, 
   * because this system only undestands seconds.. 
   * and is normally given in millis 
   */

  heading_I+= (float)PID_error*(float)dt; 
  /* 1000 microseconds / 1000 = 1 millisecond */
  heading_I=constrain(heading_I,HEADING_MIN,HEADING_MAX); 
  /* Limit the PID integrator...  */
  /* Integratior part */

  heading_D=((float)PID_error-(float)heading_previous_error)/(float)dt;
  /* Derivation part */

  heading_output=0;
  /* Clearing the variable.	 */

  heading_output=(kp[0]*PID_error);
  /* Proportional part, is just the 
   * KP constant * error.. and addidng 
   * to the output
   */
  
  heading_output+= (ki[0]*heading_I);
  /* Adding integrator result... */

  heading_output+= (kd[0]*heading_D);
  /* Adding derivator result....  */

  heading_output = constrain(heading_output,HEADING_MIN,HEADING_MAX);
  /* limiting the output....  */

  heading_previous_error=PID_error;
  /* Saving the actual error to use it later (in derivating part)... */

  heading_PID_timer=millis(); /* Saving the last execution time, important to calculate the dt... */

  /*
   * Adds all the PID results and limit the output... 
   */

  if(REVERSE_YAW == 1)
  {
    return (int)(-1*heading_output); 
  }
  else
  {
    return (int)(heading_output);
  }
  /*
   * Checking if the user have selected normal 
   * or reverse mode (servo)... 
   */
}

int compass_error (int PID_set_Point, int PID_current_Point)
{
   float PID_error=0;
   /* Temporary variable */
   
   if(fabs(PID_set_Point-PID_current_Point) > 180) 
   {
      if(PID_set_Point-PID_current_Point < -180)
      {
         PID_error=(PID_set_Point+360)-PID_current_Point;
      }
      else
      {
         PID_error=(PID_set_Point-360)-PID_current_Point;
      }
   }
   else
   {
      PID_error=PID_set_Point-PID_current_Point;
   }

   return PID_error;
}

uint32_t get_gps_dist(float flat1, float flon1, float flat2, float flon2)
{
  float x = 69.1 * (flat2 - flat1); 
  float y = 69.1 * (flon2 - flon1) * cos(flat1/57.3);

  return (float) sqrt((float)(x*x) + (float)(y*y))*1609.344; 
}

int get_gps_course(float flat1, float flon1, float flat2, float flon2)
{
  float calc;
  float bear_calc;

  float x = 69.1 * (flat2 - flat1); 
  float y = 69.1 * (flon2 - flon1) * cos(flat1/57.3);

  calc = atan2 (y,x);

  bear_calc = degrees(calc);

  if(bear_calc<=1){
    bear_calc=360+bear_calc; 
  }
  return bear_calc;
}



void flt_mgmt_simulation (int altitude, float latitude, float longitude, int* speed, int* angle)
{
   printf ("[MGMT] received lat=%f, long=%f, simulate speed=%d, angle=%d\n", latitude, longitude, *speed, *angle);

   wp_bearing = get_gps_course (latitude, longitude, wp_lat[current_wp], wp_lon[current_wp]);

   /*
    * Calculating Bearing, this function 
    * is located in the GPS_Navigation tab.. 
    */

   wp_distance=get_gps_dist(latitude, longitude, wp_lat[current_wp], wp_lon[current_wp]);

   /*
    * Calculating Distance, this function is 
    * located in the GPS_Navigation tab.. 
    */

   *angle = middle_yaw + PID_heading (compass_error (45, course)); 
  
   if(get_gps_dist(latitude, longitude, wp_lat[0], wp_lon[0]) > DISTANCE_LIMIT) 
   {
      current_wp=0; /* Return home...  */
   } 

   /*
    * old version
    * *speed =middle_thr+PID_altitude(wp_alt[current_wp], (alt-launch_altitude));
    */

  *speed = PID_altitude(wp_alt[current_wp], (altitude-LAUNCH_ALTITUDE));
  /* FIXME : may need to be changed later according to the original version */

  if(*speed < 45)
  { 
     /* 
      * Just to limit the result 
      */
    *speed = 45;
  }



    if(wp_distance<30)//Checking if the waypoint distance is less than 30, and check if the lock is open
    {
      current_wp++; //Switch the waypoint
      if(current_wp>NB_WAYPOINTS)//Check if we've passed all the waypoints, if yes will return home.. 
      {
        current_wp=0; 
      }
    }
}

