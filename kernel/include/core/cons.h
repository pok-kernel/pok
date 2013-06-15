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
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */


#if defined (POK_NEEDS_CONSOLE) || defined (POK_NEEDS_DEBUG) || defined (POK_NEEDS_INSTRUMENTATION) || defined (POK_NEEDS_COVERAGE_INFOS)

#include <types.h>

typedef void		(*print_char_fn_t)(const char c);
typedef void		(*print_attr_fn_t)(uint8_t attr);

void		pok_print_char(const char c);
void		pok_print_attr(uint8_t attr);

int		pok_print_init(print_char_fn_t print_char_fn,
			       print_attr_fn_t print_attr_fn);

int		pok_write(const char *s, size_t length);

#endif
