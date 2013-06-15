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


#include <types.h>
#include <core/syscall.h>
#include <libc/stdio.h>

static const char digits[] = "0123456789abcdef";

#define INT_UNSIGNED	   1
#define INT_SIGNED      2
#define FLOAT_SIGNED    3
#define DOUBLE_PRECISION	(6)

#define BASE_HEX        16
#define BASE_OCT        8

#define MY_BUF_SIZE     128

/*
 * some types
 */

struct		s_file
{
   char     buff[MY_BUF_SIZE];
   size_t    pos;
};

union		u_arg
{
   uint32_t    value;
   uint32_t    uint;
   int         sint;
   double       vdouble;
   void        *ptr;
};

typedef int (*t_fmtfun)(union u_arg *arg, struct s_file *file, int flags);

struct		s_format
{
   char         ch;
   t_fmtfun     fun;
   int          flags;
};

/*
 * buffered I/O
 */

static void	my_fflush(struct s_file *file)
{
   pok_syscall2 (POK_SYSCALL_CONSWRITE, (uint32_t)file->buff, file->pos);
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
    my_fflush(file);
}

static void	close_buffered_output(struct s_file *file)
{
  my_fflush(file);
}

/*
 * formatting functions
 */

static int	conv (uint32_t n, int base, int dig[])
{
  int		i = 0;

  while (n)
    {
      dig[i] = n % base;
      ++i;
      n /= base;
    }
  return i - 1;
}

static int	my_printnbr_base (uint32_t       n,
                              const char     base[],
                              int            card,
                              struct s_file  *file)
{
  int		digits[96];
  int		i;
  int		count;

  if (n == 0)
    {
      my_putc('0', file);
      return 1;
    }
  count = i = conv(n, card, digits);
  for (; i >= 0; --i)
  {
    my_putc(base[digits[i]], file);
  }
  return count;
}

static int print_int (union u_arg* value, struct s_file* file, int flags)
{
   int sh = 0;

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
   return my_printnbr_base(value->uint, digits, 10, file) + sh;
}


static int print_float (union u_arg* value, struct s_file* file, int flags)
{
  int floor = value->vdouble;
  uint32_t fractional = 0;
  int res = 0;
  int precision = 0;
  int decimal = 10;

  res +=  my_printnbr_base(floor, digits, 10, file);
  my_putc('.', file);
  (void)flags;

  while (precision < DOUBLE_PRECISION)
  {
    fractional = (value->vdouble - floor) * decimal;
    fractional %= 10;
    res += my_printnbr_base(fractional, digits, 10, file);
    decimal *= 10;
    ++precision;
  }

  return res;
}


static int print_str (union u_arg* value, struct s_file* file, int flags)
{
   int	count = 0;
   char*	s = value->ptr;

   flags = flags;
   for (; *s; ++count, ++s)
      my_putc(*s, file);
   return count;
}

static int print_char (union u_arg* value, struct s_file* file, int flags)
{
   char	c;

   flags = flags;
   c = value->sint;
   my_putc(c, file);
   return 1;
}

static int print_base(union u_arg* value, struct s_file* file, int flags)
{
   return my_printnbr_base(value->uint, digits, flags, file);
}

static const struct s_format formats[] =
{
   { 'd', print_int, INT_SIGNED },
   { 'f', print_float, FLOAT_SIGNED },
   { 'i', print_int, INT_SIGNED },
   { 'u', print_int, INT_UNSIGNED },
   { 's', print_str, 0 },
   { 'c', print_char, 0 },
   { 'o', print_base, BASE_OCT },
   { 'x', print_base, BASE_HEX },
   { 0, NULL, 0 }
};

static int special_char(char fmt, union u_arg* value, struct s_file* file)
{
   int i = 0;

   for (i = 0; formats[i].fun; ++i)
   {
      if (formats[i].ch == fmt)
         break;
   }

   if (formats[i].fun)
   {
     //printf("special_char: %i\n", value->vfloat);
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

int		vprintf(const char* format, va_list args)
{
   struct   s_file*  file;
   union    u_arg    arg;
   int      count;

   count = 0;
   arg.uint = 0;

   for (file = init_buffered_output(); *format; format += (*format == '%' ? 2 : 1))
   {
      if (*format == '%')
      {
         if (!*(format + 1))
         {
            break;
         }
         if (*(format + 1) != '%')
         {
            switch (*(format + 1))
            {
               case 'f':
                  arg.vdouble = va_arg (args, double);
                  break;

               default:
                  arg.value = va_arg (args, uint32_t);
                  break;
            }
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

int		printf(const char *format, ...)
{
  int		res;
  va_list	args;

  va_start(args, format);
  res = vprintf (format, args);
  va_end(args);
  return res;
}

