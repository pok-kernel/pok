/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * be made according to the POK licence. You CANNOT use this file or a part
 * of a file for your own project.
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2025 POK team
 */

#ifndef __POK_CONS_H__
#define __POK_CONS_H__

#if defined(POK_NEEDS_DEBUG) || defined(POK_NEEDS_CONSOLE) ||                  \
    defined(POK_NEEDS_INSTRUMENTATION) || defined(POK_NEEDS_COVERAGE_INFOS) || \
    defined(POK_NEEDS_USER_DEBUG)

#include <arch.h>
#include <errno.h>
#include <types.h>

#define CONS_ESCAPE 255
#define CONS_CLEAR 1
#define CONS_COLOR 2
#define CONS_SETX 3
#define CONS_SETY 4

#define CONS_BLACK 0x0
#define CONS_BLUE 0x1
#define CONS_GREEN 0x2
#define CONS_CYAN 0x3
#define CONS_RED 0x4
#define CONS_MAGENTA 0x5
#define CONS_YELLOW 0x6
#define CONS_WHITE 0x7

#define CONS_BLINK (1 << 7)
#define CONS_LIGHT (1 << 3)

#define CONS_FRONT(Color) (Color)
#define CONS_BACK(Color) (Color << 4)

struct s_cons {
  char row;
  char col;
  char cur_attr;
};

void pok_cons_print_char(const char c);
void pok_cons_attr(uint8_t attr);
void pok_cons_disable_cursor(void);
pok_bool_t pok_cons_write(const char *s, size_t length);
void pok_cons_clear(void);
#endif

#ifdef POK_NEEDS_CONSOLE

void pok_cons_get_char(char *c);

#endif

int pok_cons_init(void);

#endif
