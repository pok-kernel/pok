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

#include <gtypes.h>
#include <libc/stdio.h>

void user_page_content_manager(types__pagerequestcmd *pcmtofm_request,
                               types__pagecontent pcmtofm_page,
                               types__pagerequestcmd pcmtodm_request,
                               types__pagecontent *pcmtodm_page) {
  printf("[PCM] USER PAGE CONTENT MANAGER\n");
}
