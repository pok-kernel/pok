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

#ifdef POK_NEEDS_ASSERT

#include <core/dependencies.h>

#include <assert.h>
#include <core/shutdown.h>
#include <libc/stdio.h>

void __assert_failed(const char *assertion, const char *file, unsigned int line,
                     const char *function) {
  printf("%s:%u: %s%sAssertion `%s' failed.\n", file, line,
         function ? function : "", function ? ": " : "", assertion);
  pok_shutdown();
}

#endif
