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
 * Created by julien on Tue Nov 24 21:49:41 2009 
 */


/*
 * Functional code taken from the Ardupilot project.
 * Original code released under Apache 2.0 open source license,
 * written by Chris Anderson & Jordi Munoz
 *
 * See:
 *   x http://diydrones.com/profiles/blog/show?id=705844%3ABlogPost%3A44814
 *   x http://code.google.com/p/ardupilot/
 */

/*
 * Defining ranges of the servos (and ESC), must be +-90 degrees. 
 */
#define MAX16_THROTTLE 2100 
/* ESC max position, given in useconds, 
 * in my ungly servos 2100 is fine, you can try 2000
 */

#define MIN16_THROTTLE 1000 
/* ESC position  */

#define MAX16_YAW 2100
/* Servo max position */
#define MIN16_YAW 1000
/* Servo min position */

#define REVERSE_YAW 1 
/* normal = 0 and reverse = 1 */

/* PID max and mins */
#define   HEADING_MAX 15
#define   HEADING_MIN -15

#define   ALTITUDE_MAX 40
#define   ALTITUDE_MIN -45

#define   NB_WAYPOINTS 6
/* Number of waypoints defined */

#define DISTANCE_LIMIT 4000 //The max distance allowed to travel from home.  

/*************************************************************************
 * RTL, if it\u2019s set as true by the user, the autopilot will always 
 * just return to the launch lat/lon (way_lat[0], way_lon[0]) when enabled, 
 * maintaining initial altitude
 **************************************************************************/
#define RTL 1 
/* 0 = waypoint mode, 1 = Return home mode */

#define Kp_heading 10
#define Ki_heading .01
#define Kd_heading 0.001

#define Kp_altitude 4
#define Ki_altitude 0.001
#define Kd_altitude 2

/*
 * PID gains
 * At the begining try to use only proportional.. 
 * The original configuration works fine in my simulator.. 
 */


/*******************************/
#include "WProgram.h"
int            altitude_error (int PID_set_Point, int PID_current_Point);
int            compass_error (int PID_set_Point, int PID_current_Point);
float          constrain_float (float value, float max, float min);
int            get_gps_course (float flat1, float flon1, float flat2, float flon2);
unsigned int   get_gps_dist (float flat1, float flon1, float flat2, float flon2);
void           gps_parse_nmea (void);
void           init_ardupilot (void);
void           Init_servo (void);
void           init_startup_parameters (void);
void           loop ();
int            PID_heading (int PID_error);
int            PID_altitude (int PID_set_Point, int PID_current_Point);
void           pulse_servo_throttle (long angle);
void           pulse_servo_yaw (long angle);
unsigned long  PulseIn (uint8_t pin, unsigned long timeout);
void           reset_PIDs (void);
void           setup ();
void           setup_waypoints (void);
void           send_to_ground (void);
void           test_throttle (void);
void           test_yaw (void);
void           throttle_control (void);
void           yaw_control (void);
/*
 * Functions declarations
 */

float wp_lat[NB_WAYPOINTS+1];
float wp_lon[NB_WAYPOINTS+1];
int wp_alt[NB_WAYPOINTS+1];
/*
 * Waypoints position
 */

byte current_wp=1;            /* This variables stores the actual waypoint we are trying to reach..  */
byte jumplock_wp=0;           /* When switching waypoints this lock will allow only one transition.. */
byte wp_home_lock=0; 
int wp_bearing=0;             /* Stores the bearing from the current waypoint */
unsigned int wp_distance=0;   /* Stores the distances from the current waypoint */

/*******************************/

 int heading_previous_error; 
 float heading_I;             /* Stores the result of the integrator */
 int altitude_previous_error;
 float altitude_I;            /* Stores the result of the integrator */

/*
 * PID loop variables
 */

const float  kp[]={Kp_heading,Kp_altitude};	
const float  ki[]={Ki_heading,Ki_altitude};	 
const float  kd[]={Kd_heading,Kd_altitude};
/* PID K constants, defined at the begining of the code */

/*******************************/
/*       GPS stuff             */
/*******************************/
char buffer[90];
/* Serial buffer to catch GPS data */

char *token;
char *search = ",";
char *brkb, *pEnd;
/*
 * GPS Pointers
 */

byte  fix_position=0;      /* Valid gps position */
float lat=0;               /* Current Latitude */
float lon=0;               /* Current Longitude */
int   alt=0;               /* Altitude,  */
/*
 * lat, long and alt variables are updated by  the gps_parse_nmea
 * function
 */


byte  ground_speed=0;      /* Ground speed? yes Ground Speed. */
int   course=0;            /* Course over ground... */
/*
 * Information from the GPS
 */

byte           gps_new_data_flag=0;         /* A simple flag to know when we've got new gps data. */
unsigned int   launch_altitude =0;          /* launch altitude, altitude in waypoints is relative to starting altitude. */
int            middle_thr=90;               /* The central position */
int            middle_yaw=90;               /* The cnetral position of yaw */
byte           middle_measurement_lock=0;   /* Another lock to void resetting the middle measurement..  */
/*
 * ACME variables
 */

/*******************************/
int test=0;
long test2=0;
int test3=0;
/*
 * Variables used in testing, will go away someday
 */


void setup()
{
  init_ardupilot();
  Init_servo();         /* Initalizing servo, see "Servo_Control" tab. */
  /* Testing servos (max and mins), we are aware of the propeller and won't spin that without warning... =) */
  /* test_throttle(); */
  test_yaw();
  setup_waypoints();    /* See tab "Mission_Setup" */
  init_startup_parameters(); //
 
}

void loop()
{
   /*************************************************************************
    * GPS function, reads and update all the GPS data... This function is located in "GPS_Navigation" tab
    *************************************************************************/
   gps_parse_nmea();

   /*************************************************************************
    *************************************************************************/
   if((gps_new_data_flag&0x01)==0x01)   /* Checking new GPS "GPRMC" data flag in position  */
   {
      digitalWrite(13,HIGH); 
      gps_new_data_flag&=(~0x01);        /* Clearing new data flag... */

      yaw_control();                     /* This function is located in "AutoPilot" tab.  */

      /* If the distance from home is greater than 2000 meters */
      if(get_gps_dist(lat, lon, wp_lat[0], wp_lon[0]) > DISTANCE_LIMIT) 
      {
         current_wp=0; /* Return home...  */
      } 
      /* Print values, just for debugging */
      send_to_ground(); 
   }

  /*************************************************************************/
  /*************************************************************************/
   /* Checking new GPS "GPGGA" data flag */
  if((gps_new_data_flag&0x02)==0x02)
  {
    gps_new_data_flag&=(~0x02); /* Clearing flag */

    throttle_control(); /* This function is located in "AutoPilot" tab. */
  }

  /*************************************************************************/
  /*This is just a prototype, ensure that the autopilot will jump ONLY ONE waypoint, checks RTL mode */
  /*************************************************************************/
  if(RTL==0)//Verify the RTl option (0=Waypoint mode, 1 = Return Home mode)
  {
    if((wp_distance<30)&&(jumplock_wp==0x00))//Checking if the waypoint distance is less than 30, and check if the lock is open
    {
      current_wp++; //Switch the waypoint
      jumplock_wp=0x01; //Lock the waypoint switcher.
      if(current_wp>WAYPOINTS)//Check if we've passed all the waypoints, if yes will return home.. 
      {
        current_wp=0; 
      }
    }

    if(digitalRead(4) == LOW) //Checks the MUX pin to see if we are in manual mode (Low = manual) 
    {
      reset_PIDs(); //Reset all the PIDs
      middle_measurement_lock=0; //status flag, to lock the starting position of the throttle
    }

  }
  else //RTL set
  {
    if(digitalRead(4) == LOW)//Checks the MUX pin to see if we are in manual mode (Low = manual)
    {
      wp_alt[0]=alt; //updates the altitude till we switch to automode, then the autopilot will try to mantain that altitude
      reset_PIDs(); //Reset all the PIDs
      middle_measurement_lock=0; //unlocks the starting positions of throttle and yaw. See the function in "AutoPilot" tab, inside the throttle_control() function
    }
    current_wp=0; //Reseting to home waypoint, just to be sure.. 
  }

  digitalWrite(13,LOW);//Turning off the status LED
}

void send_to_ground(void)
{
    /* Warning if you are using a baud rate 
     * of 4800 this will super slow down the system.. 
     */
    Serial.print("Mot: ");
    Serial.print(test3);
    Serial.print(" Yaw: ");
    Serial.print((int)test);
    /*
    Serial.print(" crs: ");   
    Serial.print(course);
    Serial.print(" WP Dir: ");
    Serial.print((int)wp_bearing);
    Serial.print(" DAlt: "); 
    Serial.print(wp_alt[current_wp]); 
    
    Serial.print(" Dst: ");
    Serial.print(wp_distance);
    Serial.print(" WP: ");
    Serial.println((int)current_wp);*/
    
    Serial.print(" Alt: ");  
    Serial.print((int)alt-launch_altitude);
    Serial.print(" Crs: ");   
    Serial.print(course);
    Serial.print(" Dis: "); 
    Serial.print(wp_distance);
    Serial.print(" Des: ");
    Serial.println((int)wp_bearing);
}

/**************************************************************
 * Special module to limit float values, which is like constrain() 
 * but for variables IEEE 754 (Don't worry about it, it's just a 
 * floating point variables). =)  
 * It's called "ACME" because that's the generic name for everything 
 * in the Road Runner cartoons ;-)
 ***************************************************************/

float constrain_float(float value, float max, float min)
{
  if (value > max)
  {
    value=max;
  }
  if (value < min)
  {
    value=min;
  }
  return value;
}

/***************************************************************************/
//Computes heading the error, and choose the shortest way to reach the desired heading
/***************************************************************************/
int compass_error(int PID_set_Point, int PID_current_Point)
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




//This function is no used.. 
int altitude_error(int PID_set_Point, int PID_current_Point)
{
  int PID_error=PID_set_Point-PID_current_Point;  
  
  return PID_error;
}



/*************************************************************************
 * Throttle Control, reads gps info, executes PID and pulses the motor controller..
 *************************************************************************/
void throttle_control(void)
{

  if((middle_measurement_lock==0)&&(digitalRead(4)==HIGH))//Verify if the lock is open (equal to zero) and if we are in automode.. 
  {
    int read_servo=0; //Declaring a temporary variable
    middle_measurement_lock=1; //Locking this part of the code


    //Now reading the yaw initial position
    while(digitalRead(2)==HIGH){} //Waits until the input pin goes low.
    read_servo=pulseIn(2, HIGH); //Read the pulse length of the receiver
    middle_thr=((read_servo-MIN16_THROTTLE)*180L)/(MAX16_THROTTLE-MIN16_THROTTLE); //Converting the pulse to degrees... 
    Serial.println(read_servo); //Just print values
    Serial.println(middle_thr);

    //Now reading the yaw initial position
    while(digitalRead(3)==HIGH){} //Waits until the input pin goes low.
    read_servo=pulseIn(3, HIGH); //Reads the pulse length of signal from receiver
    middle_yaw=((read_servo-MIN16_YAW)*180L)/(MAX16_YAW-MIN16_YAW); //Converting the pulse to degrees... 
    Serial.println(read_servo); //Just print values
    Serial.println(middle_yaw);
  }

  //Central Position + PID(current altitude, desired altitude - launch altitude).  
  //The result will be the motor speed value.. 
  //I subtract the current altitude (alt) from the "launch_altitude" in order to make it realtive to the start point,
  //Example: If you launch you airp. at 300 meters above sea level, and you want to maintain 50 meters above your head, the airp. will fly at 350 meters above sea level. 
  test3=middle_thr+PID_altitude(wp_alt[current_wp], (alt-launch_altitude));



  if(test3<45){ //Just to limit the result
    test3=45;
  }


  pulse_servo_throttle(test3); //sending position to the motor controller... 
  //the values are given in degrees, where 45 degrees is motor off, 
  //90 degrees is middle thrust, and 135 degrees is full thrust.


}

/*************************************************************************
 * Yaw Control, reads gps info, calculates navigation, executes PID and sends values to the servo.. 
 *************************************************************************/
void yaw_control(void)
{

  wp_bearing=get_gps_course(lat, lon, wp_lat[current_wp], wp_lon[current_wp]);//Calculating Bearing, this function is located in the GPS_Navigation tab.. 
  wp_distance=get_gps_dist(lat, lon, wp_lat[current_wp], wp_lon[current_wp]); //Calculating Distance, this function is located in the GPS_Navigation tab.. 

  //test=middle_yaw+PID_heading(compass_error(wp_bearing, course)); //Central Position + PID(compass_error(desired course, current course)). 
  test=middle_yaw+PID_heading(compass_error(45, course)); 
  
  //The compass error function is located in "ACME" tab.
  pulse_servo_yaw(test);//Sending values to servo, 90 degrees is central position. 

}

/*************************************************************************
 * This functions parses the NMEA strings... 
 * Pretty complex but never fails and works well with all GPS modules and baud speeds.. :-) 
 * Just change the Serial.begin() value in the first tab for higher baud speeds
 *************************************************************************/

void gps_parse_nmea(void)
{
  const char head_rmc[]="GPRMC"; //GPS NMEA header to look for
  const char head_gga[]="GPGGA"; //GPS NMEA header to look for

  static byte unlock=1; //some kind of event flag
  static byte checksum=0; //the checksum generated
  static byte checksum_received=0; //Checksum received
  static byte counter=0; //general counter

  //Temporary variables for some tasks, specially used in the GPS parsing part (Look at the NMEA_Parser tab)
  unsigned long temp=0;
  unsigned long temp2=0;
  unsigned long temp3=0;


  while(Serial.available() > 0)
  {
    if(unlock==0)
    {
      buffer[0]=Serial.read();//puts a byte in the buffer

      if(buffer[0]=='$')//Verify if is the preamble $
      {
        unlock=1; 
      }
    }
    /*************************************************/
    else
    {
      buffer[counter]=Serial.read();


      if(buffer[counter]==0x0A)//Looks for \F
      {

        unlock=0;


        if (strncmp (buffer,head_rmc,5) == 0)//looking for rmc head....
        {

          /*Generating and parsing received checksum, */
          for(int x=0; x<100; x++)
          {
            if(buffer[x]=='*')
            { 
              checksum_received=strtol(&buffer[x+1],NULL,16);//Parsing received checksum...
              break; 
            }
            else
            {
              checksum^=buffer[x]; //XOR the received data... 
            }
          }

          if(checksum_received==checksum)//Checking checksum
          {
            /* Token will point to the data between comma "'", returns the data in the order received */
            /*THE GPRMC order is: UTC, UTC status ,Lat, N/S indicator, Lon, E/W indicator, speed, course, date, mode, checksum*/
            token = strtok_r(buffer, search, &brkb); //Contains the header GPRMC, not used

            token = strtok_r(NULL, search, &brkb); //UTC Time, not used
            //time=  atol (token);
            token = strtok_r(NULL, search, &brkb); //Valid UTC data? maybe not used... 


            //Longitude in degrees, decimal minutes. (ej. 4750.1234 degrees decimal minutes = 47.835390 decimal degrees)
            //Where 47 are degrees and 50 the minutes and .1234 the decimals of the minutes.
            //To convert to decimal degrees, devide the minutes by 60 (including decimals), 
            //Example: "50.1234/60=.835390", then add the degrees, ex: "47+.835390=47.835390" decimal degrees
            token = strtok_r(NULL, search, &brkb); //Contains Latitude in degrees decimal minutes... 

            //taking only degrees, and minutes without decimals, 
            //strtol stop parsing till reach the decimal point "."  result example 4750, eliminates .1234
            temp=strtol (token,&pEnd,10);

            //takes only the decimals of the minutes
            //result example 1234. 
            temp2=strtol (pEnd+1,NULL,10);

            //joining degrees, minutes, and the decimals of minute, now without the point...
            //Before was 4750.1234, now the result example is 47501234...
            temp3=(temp*10000)+(temp2);


            //modulo to leave only the decimal minutes, eliminating only the degrees.. 
            //Before was 47501234, the result example is 501234.
            temp3=temp3%1000000;


            //Dividing to obtain only the de degrees, before was 4750 
            //The result example is 47 (4750/100=47)
            temp/=100;

            //Joining everything and converting to float variable... 
            //First i convert the decimal minutes to degrees decimals stored in "temp3", example: 501234/600000= .835390
            //Then i add the degrees stored in "temp" and add the result from the first step, example 47+.835390=47.835390 
            //The result is stored in "lat" variable... 
            lat=temp+((float)temp3/600000);


            token = strtok_r(NULL, search, &brkb); //lat, north or south?
            //If the char is equal to S (south), multiply the result by -1.. 
            if(*token=='S'){
              lat=lat*-1;
            }

            //This the same procedure use in lat, but now for Lon....
            token = strtok_r(NULL, search, &brkb);
            temp=strtol (token,&pEnd,10); 
            temp2=strtol (pEnd+1,NULL,10); 
            temp3=(temp*10000)+(temp2);
            temp3=temp3%1000000; 
            temp/=100;
            lon=temp+((float)temp3/600000);

            token = strtok_r(NULL, search, &brkb); //lon, east or west?
            if(*token=='W'){
              lon=lon*-1;
            }

            token = strtok_r(NULL, search, &brkb); //Speed overground?
            ground_speed= atoi(token);

            token = strtok_r(NULL, search, &brkb); //Course?
            course= atoi(token);

            gps_new_data_flag|=0x01; //Update the flag to indicate the new data has arrived. 


            jumplock_wp=0x00;//clearing waypoint lock..

          }
          checksum=0;
        }//End of the GPRMC parsing

        if (strncmp (buffer,head_gga,5) == 0)//now looking for GPGGA head....
        {
          /*Generating and parsing received checksum, */
          for(int x=0; x<100; x++)
          {
            if(buffer[x]=='*')
            { 
              checksum_received=strtol(&buffer[x+1],NULL,16);//Parsing received checksum...
              break; 
            }
            else
            {
              checksum^=buffer[x]; //XOR the received data... 
            }
          }

          if(checksum_received==checksum)//Checking checksum
          {

            token = strtok_r(buffer, search, &brkb);//GPGGA header, not used anymore
            token = strtok_r(NULL, search, &brkb);//UTC, not used!!
            token = strtok_r(NULL, search, &brkb);//lat, not used!!
            token = strtok_r(NULL, search, &brkb);//north/south, nope...
            token = strtok_r(NULL, search, &brkb);//lon, not used!!
            token = strtok_r(NULL, search, &brkb);//wets/east, nope
            token = strtok_r(NULL, search, &brkb);//Position fix, used!!
            fix_position =atoi(token); 
            token = strtok_r(NULL, search, &brkb); //sats in use!! Nein...
            token = strtok_r(NULL, search, &brkb);//HDOP, not needed
            token = strtok_r(NULL, search, &brkb);//ALTITUDE, is the only meaning of this string.. in meters of course. 
            alt=atoi(token);
            if(alt<0){
              alt=0;
            }

            if(fix_position >= 0x01) digitalWrite(12,HIGH); //Status LED...
            else digitalWrite(12,LOW);

            gps_new_data_flag|=0x02; //Update the flag to indicate the new data has arrived.
          }
          checksum=0; //Restarting the checksum
        }

        for(int a=0; a<=counter; a++)//restarting the buffer
        {
          buffer[a]=0;
        } 
        counter=0; //Restarting the counter
      }
      else
      {
        counter++; //Incrementing counter
      }
    }
  } 

}
/*************************************************************************
 * //Function to calculate the course between two waypoints
 * //I'm using the real formulas--no lookup table fakes!
 *************************************************************************/
int get_gps_course(float flat1, float flon1, float flat2, float flon2)
{
  float calc;
  float bear_calc;

  float x = 69.1 * (flat2 - flat1); 
  float y = 69.1 * (flon2 - flon1) * cos(flat1/57.3);

  calc=atan2(y,x);

  bear_calc= degrees(calc);

  if(bear_calc<=1){
    bear_calc=360+bear_calc; 
  }
  return bear_calc;
}


/*************************************************************************
 * //Function to calculate the distance between two waypoints
 * //I'm using the real formulas
 *************************************************************************/
unsigned int get_gps_dist(float flat1, float flon1, float flat2, float flon2)
{
  float x = 69.1 * (flat2 - flat1); 
  float y = 69.1 * (flon2 - flon1) * cos(flat1/57.3);

  return (float)sqrt((float)(x*x) + (float)(y*y))*1609.344; 
}

void init_ardupilot(void)
{
  Serial.begin(4800); 
  Serial.println("ArduPilot!!!"); 
  //Declaring pins
  pinMode(2,INPUT);//Servo input; 
  pinMode(3,INPUT);//Servo Input; 
  pinMode(4,INPUT); //MUX pin
  // This next line is a Mode pin, which only works in with three-position toggle switches on your transmitter. 
  // If you want to use it for some special mode, you must change the Attiny code. It is not currently active.
  // When you put the switch in the central position the attiny will set high a pin called "mode", and you can use it to do whatever yowant... 
    pinMode(5,INPUT); // Mode pin (see above)
  pinMode(11,OUTPUT); // Simulator Output pin
  pinMode(12,OUTPUT); // LOCK LED pin in ardupilot board, indicates valid GPS data
  pinMode(13,OUTPUT);// STATS LED pin in ardupilot board, blinks to indicate the board is working well...  
}

void init_startup_parameters(void)
{
  /* yeah a do-while loop, checks over and over again 
   * until we have valid GPS position and lat is 
   * different from zero. 
   * I re-verify the Lat because sometimes fails and 
   * sets home lat as zero. This way never goes wrong.. 
   */
  do{
    gps_parse_nmea();
    /* Reading and parsing GPS data */
    Serial.println ("Waiting for fix position");
  }
  while(((fix_position < 0x01)||(lat==0)));


  //Another verification
  gps_new_data_flag=0;

  do{
    gps_parse_nmea(); 
    /* Reading and parsing GPS data */ 
    Serial.println ("Waiting for new data flags");
  }
  while((gps_new_data_flag&0x01!=0x01)&(gps_new_data_flag&0x02!=0x02)); 

  yaw_control();                 /* I've put this here because i need 
                                    to calculate the distance to the next
                                    waypoint, otherwise it will start at
                                    waypoint 2. */


  launch_altitude=alt;          /* Saving the launch altitude, 
                                   altitude in waypoints is 
                                   relative to starting altitude. */

  wp_lat[0]=lat; /* Saving home latitude */
  wp_lon[0]=lon; /* Saving home longitude */
  wp_alt[0]=100; /* Storing home altitude plus 100, 
                    otherwise it will try to maintain
                    the launch altitude, you really 
                    don't want fly too low..  */
  /* Storing the home position */

  Serial.println((long)(wp_lat[0]*1000000));
  Serial.println((long)(wp_lon[0]*1000000));
  Serial.println((wp_alt[0]));  
  /* Printing the values just to be sure...  */
}

void setup_waypoints(void)
{
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


/****************************************************************************************
 * PID= P+I+D
 ***************************************************************/
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
/* ---------------------------------------------------------------------------*/


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

/*************************************************************************
 * Reset all the PIDs
 *************************************************************************/
void reset_PIDs(void)
{
   heading_previous_error=0;
   heading_I=0; 

   altitude_previous_error=0;
   altitude_I=0;
}

/**************************************************************
 * Configuring the PWM hadware... 
 * If you want to understand this you must read the Data Sheet
 * of atmega168..  
 ***************************************************************/
/* 
 * This part will configure the PWM to control the servo 100% 
 * by hardware, and not waste CPU time.. 
 */
void Init_servo(void)
{   
   digitalWrite(10,LOW);
   /* Defining servo output pins */
   pinMode(10,OUTPUT);
   digitalWrite(9,LOW);
   pinMode(9,OUTPUT);
   /*Timer 1 settings for fast PWM*/

   /*
    * Note: these strange strings that follow, like OCRI1A, are actually 
    * predefined Atmega168 registers. We load the registers and the chip 
    * does the rest.
    */

   /*
    * Remember the registers not declared here remains zero by default... 
    */
   TCCR1A =((1<<WGM11)|(1<<COM1B1)|(1<<COM1A1));
   /* 
    * Please read page 131 of DataSheet, we are changing the registers
    * settings of WGM11,COM1B1,COM1A1 to 1 thats all... 
    */

   TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
   /* 
    * Prescaler set to 8, that give us a resolution of 2us, 
    * read page 134 of data sheet
    */

   OCR1A = 2500; 
   /* 
    * the period of servo 1, remember 2us resolution, 
    * 2500/2 = 1250us the pulse period of the servo...    
    */

   OCR1B = 3000;
   /*
    * the period of servo 2, 3000/2=1500 us, 
    * more or less is the central position... 
    */

   ICR1 = 40000; 
   /*
    * 50hz freq...Datasheet says:
    * (system_freq/prescaler)/target frequency.
    * So (16000000hz/8)/50hz=40000, 
    *
    * Must be 50hz because is the servo standard 
    * (every 20 ms, and 1hz = 1sec) 1000ms/20ms=50hz, 
    * elementary school stuff... 
    */
}


/**************************************************************
 * Function to pulse the throttle servo
 ***************************************************************/
void pulse_servo_throttle(long angle)//Will convert the angle to the equivalent servo position... 
{
   OCR1A=((angle*(MAX16_THROTTLE-MIN16_THROTTLE))/180L+MIN16_THROTTLE)*2L;
   /* angle=constrain(angle,180,0); */
}

/**************************************************************
 * Function to pulse the yaw/rudder servo... 
 ***************************************************************/
void pulse_servo_yaw(long angle)//Will convert the angle to the equivalent servo position... 
{
   OCR1B=((angle*(MAX16_YAW-MIN16_YAW))/180L+MIN16_YAW)*2L;
   /* Scaling */
   /* angle=constrain(angle,180,0); */
}
/**************************************************************
 * Function to test the servos.. 
 ***************************************************************/


/**************************************************************
 * Improved PulseIn by Michal Bacik.. 
 ***************************************************************/
#include "pins_arduino.h"
/* 
 * Same as pulseIn, but tweaked for
 * range 1000 - 2000 usec, and reading 
 * only HIGH phase.
 *
 * Must be compiled in .cpp file, 
 * with -Os compiler switch.
 */

unsigned long PulseIn(uint8_t pin, unsigned long timeout)
{
   const uint8_t           bit      = digitalPinToBitMask(pin), port = digitalPinToPort(pin);
   unsigned long           width    = 1;

   unsigned long           numloops = 0;
   const unsigned long     maxloops = microsecondsToClockCycles(timeout) / 16;

   noInterrupts();

   /* 
    * Wait for the pulse to start
    */

   while((*portInputRegister(port) & bit) != bit)
   {
      if(++numloops == maxloops)
      {
         interrupts();
         return 0;
      }
   }

   /* 
    * Wait for the pulse to stop 
    */
   while((*portInputRegister(port) & bit) == bit)
   {
      width++;
   }

   interrupts();

   return clockCyclesToMicroseconds((width*0xd00L+0x800L)/256L);
}


void test_throttle(void)
{

   pulse_servo_throttle(45);
   digitalWrite(13, HIGH);
   delay(1000);
   pulse_servo_throttle(135);
   digitalWrite(13, LOW);
   delay(1000);
   digitalWrite(13, HIGH);
   pulse_servo_throttle(90);
   delay(1000);

}

void test_yaw(void)
{
   pulse_servo_yaw (90+HEADING_MIN);
   digitalWrite (13, HIGH);
   delay (1500);
   pulse_servo_yaw (90) + HEADING_MAX);
   digitalWrite (13, LOW);
   delay (1500);
   digitalWrite (13, HIGH);
   pulse_servo_yaw (90);
   delay (1500);

}

int main(void)
{
   init();

   setup();

   for (;;)
   {
      loop();
   }

   return 0;
}



/********
 * AADL functinal code
 */

void gps_simulation  (float* latitude, float* longitude)
{
   *longitude = 1.4;
   *latitude = 2.5;
   printf ("[GPS] Simulate latitude %f, longitude %f\n", latitude, longitude);
}

void flt_mgmt_simulation (float latitude, float longitude, int* speed, int* angle)
{
   *speed = 3;
   *angle = 4;
   printf ("[MGMT] received lat=%f, long=%f, simulate speed=%d, angle=%d\n", latitude, longitude, *speed, *angle);
}

void throttle_simulation (int speed)
{
   printf ("[THROTTLE] received speed=%d\n", speed);
}

void yaw_simulation (int angle)
{
   printf ("[YAW] received angle %d\n", angle);
}


