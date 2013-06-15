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
* c file generated for node : inc 
********/
#include <libc/stdlib.h>
#define _inc_EC2C_SRC_FILE
#include "inc.h"
/*--------
Internal structure for the call
--------*/
typedef struct  {
   void* client_data;
   //INPUTS
   _integer _n;
   //OUTPUTS
   _integer _r;
   //REGISTERS
   _integer M6;
   _boolean M6_nil;
   _boolean M2;
} inc_ctx;
/*--------
Output procedures must be defined,
Input procedures must be used:
--------*/
void inc_I_n(inc_ctx* ctx, _integer V){
   ctx->_n = V;
}
extern void inc_O_r(void*, _integer);
#ifdef CKCHECK
extern void inc_BOT_r(void*);
#endif
/*--------
Internal reset input procedure
--------*/
static void inc_reset_input(inc_ctx* ctx){
   //NOTHING FOR THIS VERSION...
}
/*--------
Reset procedure
--------*/
void inc_reset(inc_ctx* ctx){
   ctx->M6_nil = _true;
   ctx->M2 = _true;
   inc_reset_input(ctx);
}
/*--------
Dynamic allocation of an internal structure
--------*/
inc_ctx* inc_new_ctx(void* cdata){
   inc_ctx* ctx = (inc_ctx*)calloc(1, sizeof(inc_ctx));
   ctx->client_data = cdata;
   inc_reset(ctx);
   return ctx;
}
/*--------
Copy the value of an internal structure
--------*/
void inc_copy_ctx(inc_ctx* dest, inc_ctx* src){
   memcpy((void*)dest, (void*)src, sizeof(inc_ctx));
}
/*--------
Step procedure
--------*/
void inc_step(inc_ctx* ctx){
//LOCAL VARIABLES
   _integer L5;
   _integer L1;
   _integer T6;
//CODE
   L5 = (ctx->M6 + ctx->_n);
   if (ctx->M2) {
      L1 = 0;
   } else {
      L1 = L5;
   }
   inc_O_r(ctx->client_data, L1);
   T6 = L1;
   ctx->M6 = T6;
   ctx->M6_nil = _false;
   ctx->M2 = ctx->M2 && !(_true);
}
