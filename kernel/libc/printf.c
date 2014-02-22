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


#if defined (POK_NEEDS_DEBUG) || defined (POK_NEEDS_INSTRUMENTATION) || defined (POK_NEEDS_COVERAGE_INFOS)

#include <types.h>
#include <libc.h>
#include <stdarg.h>
#include <bsp.h>

static const char digits[] = "0123456789abcdef";

#define INT_UNSIGNED    1
#define INT_SIGNED      2

#define BASE_HEX        16
#define BASE_OCT        8

#define MY_BUF_SIZE     8192

/*
 * some types
 */

struct		s_file
{
   char		buff[MY_BUF_SIZE];
   size_t	pos;
};

union		u_arg
{
   unsigned long	value;
   unsigned int	uint;
   int            sint;
   void*          ptr;
};

typedef int (*t_fmtfun)(union u_arg *arg, struct s_file *file, int flags);

struct		s_format
{
   char     ch;
   t_fmtfun	fun;
   int      flags;
};

/*
 * buffered I/O
 */

static void	my_fflush(struct s_file *file)
{
   pok_cons_write (file->buff, file->pos);
   file->pos = 0;
}

static struct s_file*	init_buffered_output(void)
{
   static struct s_file	res;

   res.pos = 0;
   return &res;
}

static void	my_putc(char c, struct s_file *file)
{
   file->buff[file->pos++] = c;

   if (file->pos == MY_BUF_SIZE)
   {
      my_fflush(file);
   }
}

static void	close_buffered_output(struct s_file *file)
{
   my_fflush(file);
}

/*
 * formatting functions
 */

static int	conv(unsigned int n, int base, int dig[])
{
   int i;

   i = 0;

   while (n)
   {
      dig[i] = n % base;
      ++i;
      n /= base;
   }
   return i - 1;
}

static int	my_printnbr_base (unsigned int   n,
                              const char     base[],
                              int            card,
                              struct s_file  *file)
{
   int   digits[96];
   int   i;
   int   count;

   if (n == 0)
   {
      my_putc ('0', file);
      return 1;
   }

   count = i = conv (n, card, digits);

   for (; i >= 0; --i)
   {
      my_putc(base[digits[i]], file);
   }

   return count;
}

static int	print_int (union u_arg* value,
                       struct s_file* file,
                       int flags)
{
   int sh;

   sh = 0;

   if (value->sint == 0)
   {
      my_putc('0', file);
      return 1;
   }

   if (flags == INT_SIGNED)
   {
      if (value->sint < 0)
      {
         my_putc('-', file);
         value->uint = -value->sint;
         sh = 1;
      }
      else
      {
         value->uint = value->sint;
      }
   }

   return my_printnbr_base (value->uint, digits, 10, file) + sh;
}

static int	print_str (union u_arg* value, struct s_file* file, int flags)
{
   int   count;
   char*	s;

   count = 0;
   s     = value->ptr;

   flags = flags;

   for (; *s; ++count, ++s)
   {
      my_putc(*s, file);
   }

   return count;
}

static int	print_char(union u_arg* value, struct s_file* file, int flags)
{
   char	c;

   flags = flags;
   c     = value->sint;

   my_putc (c, file);

   return 1;
}

static int	print_base(union u_arg* value, struct s_file* file, int flags)
{
   return my_printnbr_base (value->uint, digits, flags, file);
}

static const struct s_format formats[] =
{
   { 'd', print_int, INT_SIGNED },
   { 'i', print_int, INT_SIGNED },
   { 'u', print_int, INT_UNSIGNED },
   { 's', print_str, 0 },
   { 'c', print_char, 0 },
   { 'o', print_base, BASE_OCT },
   { 'x', print_base, BASE_HEX },
   { 0, NULL, 0 }
};

static int special_char (char fmt,
                         union u_arg* value,
                         struct s_file* file)
{
   int i;

   for (i = 0; formats[i].fun; ++i)
   {
      if (formats[i].ch == fmt)
      {
         break;
      }
   }

   if (formats[i].fun)
   {
      return formats[i].fun(value, file, formats[i].flags);
   }
   else
   {
      if (fmt != '%')
      {
         my_putc('%', file);
      }

      my_putc(fmt, file);

      return 1 + (fmt != '%');
   }
}

/*
 * finally, printf
 */

int vprintf (const char* format, va_list args)
{
   struct s_file* file;
   union u_arg	   arg;
   int            count;

   count = 0;

   for (file = init_buffered_output();
         *format;
         format += (*format == '%' ? 2 : 1))
   {
      if (*format == '%')
      {
         if (!*(format + 1))
         {
            break;
         }

         if (*(format + 1) != '%')
         {
            arg.value = va_arg(args, unsigned long);
         }

         count += special_char(*(format + 1), &arg, file);
      }
      else
      {
         my_putc(*format, file);
         ++count;
      }
   }

   close_buffered_output(file);
   return count;
}

int printf (const char *format, ...)
{
   int      res;
   va_list  args;

   va_start (args, format);
   res = vprintf (format, args);
   va_end (args);

   return res;
}

#endif
