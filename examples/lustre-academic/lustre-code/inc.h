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
 * Created by julien on Wed Feb 25 22:07:02 2009 
 */

/********
* ec2c version 0.5
* c header file generated for node : inc 
* to be used with : inc.c 
********/
/*-------- Predefined types ---------*/
#ifndef _inc_EC2C_PREDEF_TYPES
#define _inc_EC2C_PREDEF_TYPES
typedef int _boolean;
typedef int _integer;
typedef char* _string;
typedef double _real;
typedef double _double;
typedef float _float;
#define _false 0
#define _true 1
#endif
/*--------- Pragmas ----------------*/
//MODULE: inc 1 1
//IN: _integer n
//OUT: _integer r
#ifndef _inc_EC2C_SRC_FILE
/*--------Context type -------------*/
struct inc_ctx;
/*--------Context allocation --------*/
extern struct inc_ctx* inc_new_ctx(void* client_data);
/*--------Context copy -------------*/
extern void inc_copy_ctx(struct inc_ctx* dest, struct inc_ctx* src);
/*-------- Reset procedure -----------*/
extern void inc_reset(struct inc_ctx* ctx);
/*-------- Step procedure -----------*/
extern void inc_step(struct inc_ctx* ctx);
/*-------- Input procedures -------------*/
extern void inc_I_n(struct inc_ctx*, _integer);
#endif
