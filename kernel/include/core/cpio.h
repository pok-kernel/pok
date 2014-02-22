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


#ifndef __POK_KERNEL_CPIO_H__
#define __POK_KERNEL_CPIO_H__

enum cpio_format
{
   CPIO_BIN_FMT,
   CPIO_ODC_FMT,
   CPIO_NEWC_FMT,
   CPIO_CRC_FMT,
   CPIO_TAR_FMT,
   CPIO_USTAR_FMT,
   CPIO_HPBIN_FMT,
   CPIO_HPODC_FMT
};

struct cpio_bin_header
{
  unsigned short	c_magic;
  unsigned short	c_dev;
  unsigned short	c_ino;
  unsigned short	c_mode;
  unsigned short	c_uid;
  unsigned short	c_gid;
  unsigned short	c_nlink;
  unsigned short	c_rdev;
  unsigned short	c_mtime[2];
  unsigned short	c_namesize;
  unsigned short	c_filesize[2];
};

struct cpio_file
{
  int			      cpio_fmt;
  void			   *cpio_addr;
  void			   *curr_header;
  void			   *curr_fileaddr;
  unsigned int		curr_filesz;
  char			   *curr_filename;
  unsigned int		curr_filename_len;
  int             (*next_header)(struct cpio_file *cpio);
};

int   cpio_open (struct cpio_file *cpio, void *file);
char  *cpio_get_filename(struct cpio_file *cpio);
int   cpio_next_file(struct cpio_file *cpio);
void  *cpio_get_fileaddr(struct cpio_file *cpio);

#endif /* __POK_KERNEL_CPIO_H__ */

