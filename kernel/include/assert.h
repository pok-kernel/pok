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
 *                                      Copyright (c) 2007-2022 POK team
 */

#ifndef __POK_ASSERT_H__
#define __POK_ASSERT_H__

#ifdef POK_NEEDS_ASSERT

void __assert_failed(const char *assertion, const char *file, unsigned int line,
                     const char *function);
#define assert(expr)                                                           \
  ((expr) ? ((void)0)                                                          \
          : __assert_failed(#expr, __FILE__, __LINE__, __FUNCTION__))

#else

#define assert(expr) ((void)0)

#endif /* POK_NEEDS_ASSERT */
#endif /* POK_ASSERT_H */
