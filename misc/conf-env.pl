#! /usr/bin/perl -w
#                               POK header
# 
# The following file is a part of the POK project. Any modification should
# be made according to the POK licence. You CANNOT use this file or a part 
# of a file for your own project.
#
# For more information on the POK licence, please see our LICENCE FILE
#
# Please follow the coding guidelines described in doc/CODING_GUIDELINES
#
#                                      Copyright (c) 2007-2009 POK team 
#
# Created by julien on Sun Aug 10 12:45:15 2008 
#

use strict;
use Getopt::Long;

my $OCARINA_VERSION = "2.0w";

my $conf_file = "misc/mk/config.mk";
my $use_floppy          = 0;
my $use_xcov            = 0;
my $use_spoq            = 0;
my $use_instrumentation = 0;
my $help 					= 0;

my $sys_kind;

my %makevars =
(
 "CONFIG_CFLAGS"	=>  "",
 "CONFIG_LDFLAGS"	=>  "",
 "CD"		=>  "cd",
 );

my @cflags =
(
 "-W",
 "-Wall",
 "-g",
 "-m32"
 );

my @ldflags =
(
 "--warn-common",
 );

my %tools_common =
(
 "RM"	       =>  ["rm"],
 "CAT"         =>  ["cat"],
 "CP"          =>  ["cp"],
 "TOUCH"       =>  ["touch"],
 "COPY"        =>  ["cp"],
 "GZIP"        =>  ["gzip"],
 "SED"         =>  ["sed"],
 "ECHO"        =>  ["echo"],
 "GREP"        =>  ["grep"],
 "TOUCH"       =>  ["touch"],
 "MKDIR"       =>  ["mkdir"],
 "MV"          =>  ["mv"],
 "PERL"        =>  ["perl"],
 "SLEEP"       =>  ["sleep"],
 "KILL"        =>  ["kill"],
 "AWK"         =>  ["gawk", "awk"],
 );

my %tools_arch;
$tools_arch{"x86"} =
{
 "AR" 	      =>  ["i386-unknown-linux-gnu-ar.exe", "i386-elf-ar" , "ar"],
 "CC"          =>  ["i386-unknown-linux-gnu-gcc.exe", "i386-elf-gcc", "gcc"],
 "CXX"	      =>  ["i386-unknown-linux-gnu-g++.exe", "i386-elf-g++", "g++"],
 "LD"	         =>  ["i386-unknown-linux-gnu-ld.exe", "i386-elf-ld" , "ld"],
 "OBJDUMP"     =>  ["i386-unknown-linux-gnu-objdump.exe", "i386-elf-objdump" , "objdump"],
 "OBJCOPY"     =>  ["i386-unknown-linux-gnu-objcopy.exe", "i386-elf-objcopy" , "objcopy"],
 "RANLIB"      =>  ["i386-unknown-linux-gnu-ranlib.exe", "i386-elf-ranlib"  , "ranlib"],
 "QEMU"        =>  ["qemu", "i386-softmmu", "qemu-system-i386"],
 };

$tools_arch{"ppc"} =
{
 "AR" 	      =>  ["powerpc-elf-ar","powerpc-linux-gnu-ar"],
 "CC"          =>  ["powerpc-elf-gcc","powerpc-linux-gnu-gcc"],
 "LD"	         =>  ["powerpc-elf-ld","powerpc-linux-gnu-ld"],
 "OBJDUMP"     =>  ["powerpc-elf-objdump","powerpc-linux-gnu-objdump"],
 "OBJCOPY"     =>  ["powerpc-elf-objcopy","powerpc-linux-gnu-objcopy"],
 "RANLIB"      =>  ["powerpc-elf-ranlib","powerpc-linux-gnu-ranlib"],
 "QEMU"        =>  ["qemu-system-ppc", "ppc-softmmu", "qemu-system-ppc"],
 };

$tools_arch{"sparc"} =
{
 "AR" 	      =>  ["sparc-elf-ar","sparc-linux-gnu-ar"],
 "CC"          =>  ["sparc-elf-gcc","sparc-linux-gnu-gcc"],
 "LD"	         =>  ["sparc-elf-ld","sparc-linux-gnu-ld"],
 "OBJDUMP"     =>  ["sparc-elf-objdump","sparc-linux-gnu-objdump"],
 "OBJCOPY"     =>  ["sparc-elf-objcopy","sparc-linux-gnu-objcopy"],
 "RANLIB"      =>  ["sparc-elf-ranlib","sparc-linux-gnu-ranlib"],
 "QEMU"        =>  ["qemu-system-sparc", "sparc-softmmu", "qemu-system-sparc"],
 "TSIM"        =>  ["tsim-leon3"],
 };


my %tools_flags =
(
 "RM" =>  ["-r", "-f"],
 );

my %colors =
(
 "RED"	=> "\033[0;31m",     "LRED"	=> "\033[1;31m",
 "GREEN"	=> "\033[0;32m",     "LGREEN"	=> "\033[1;32m",
 "BLUE"	=> "\033[0;34m",     "LGREEN"	=> "\033[1;34m",
 "STD"	=> "\033[;m",
 );

# {{ Functions definitions
	sub check_qemu_xcov
	{
		my $qemubinary = $makevars{"QEMU_x86"};
		my $good = 0;
		my $line = "";

      open ( FIC , "$qemubinary -trace 2>&1 |") or return 1;
      $line = <FIC>;
      return 1 if (! (defined ($line)));
      chomp ($line);

		$good = 1 if ($line =~ /requires an argument/);

      if ($good == 1)
      {
         print $colors{"GREEN"}."XCOV is set and QEMU supports -trace option\n".$colors{"STD"};
      }
      else
      {
         print $colors{"RED"}."WARNING: XCOV is set and QEMU does not support -trace option\n".$colors{"STD"};
      }

      return 0;
	}

   sub check_ocarina
   {
      my $line = "";
	  my $version = "";
      open ( FIC , "ocarina -V 2>/dev/null|") or return 1;
      $line = <FIC>;
      return 1 if (! (defined ($line)));
      chomp ($line);

      ($version) =($line =~ /^[a-zA-Z]+\s([\.0-9a-zA-Z]+)\s.*$/);
     
      if ( "$version" ne "$OCARINA_VERSION")
      {
         print $colors{"RED"}."WARNING: POK required Ocarina version $OCARINA_VERSION, incorrect version \n".$colors{"STD"};
      }
      else
      {
         print $colors{"GREEN"}."Ocarina version is good\n".$colors{"STD"};
      }

      return 0;
   }

   sub concat_flags
   {
      my ($flag, @values)   = @_;

      $makevars{$flag} .= join(" ", @values);
   }

   sub create_env ()
   {
      open (CONFILE, "+>".$conf_file) || die("Can't write file: ".$conf_file."\n");

      printf CONFILE ".SILENT:\n\n";
      foreach my $key ( sort keys(%makevars) )
      {
         printf CONFILE $key."=".$makevars{$key}."\n";
      }

      close (CONFILE);
   }

   sub check_arch_tools
   {
      my $ret = 0;
      my $bin_path;
      my $arch;
      my $key;
      my %tools;

      $arch = shift;

      die ("Unknown architecture: $arch\n") unless exists $tools_arch{$arch};

      foreach my $tool ( keys(%{$tools_arch{$arch}}) )
      {
         my $tmp_ret = 1;

         print "checking for ".$tool." for $arch architecture ...";

         foreach my $bin ( @{$tools_arch{$arch}{$tool}} )
         {
            $key = $tool . "_" . $arch;
            $bin_path = `which $bin 2>/dev/null` or next;
				chomp $bin_path;
            my $retval = $? >> 8;

            if ( $retval == 0 )
            {
               $tmp_ret = 0;
               $makevars{$key} = $bin_path;

               if ( exists($tools_flags{$tool}) )
               {
                  $makevars{$key} .= " ".join(" ", @{$tools_flags{$tool}})
               }

               print $colors{"GREEN"}." yes\n".$colors{"STD"};
               last;
            }
         }

         if ( $tmp_ret != 0 )
         {
            $ret = 1;
            print $colors{"RED"}." no\n".$colors{"STD"};
         }

	 if ($tool eq "CC")
	 {
	     if ($tmp_ret == 0)
	     {
		 my $languages;
		 my @tokens;
		 $languages = `$bin_path -v 2>&1 | sed -r \'/Configured with.*/!d; s/.*--enable-languages=\([^ ]*\).*/\\1/g; s/,/ /g\'`;
		 @tokens = split(/ /,$languages);
		 print "checking for ".$tool." Ada support for $arch architecture ...";
		 @tokens = grep("ada", @tokens);

		 if ($#tokens != 0)
		 {
		     print $colors{"GREEN"}." yes\n".$colors{"STD"};
		 }
		 else
		 {
		     print $colors{"RED"}." no\n".$colors{"STD"};
		 }
	     }
	     else
	     {
		 print "checking for ".$tool." Ada support for $arch architecture ...";
		 print $colors{"RED"}." no\n".$colors{"STD"};
	     }
	 }

      }

      if ($ret == 0)
      {
         print STDERR $colors{"BLUE"}."Architecture $arch was configured\n".$colors{"STD"};
      }
      else
      {
         print STDERR $colors{"RED"}."Architecture $arch was NOT configured\n".$colors{"STD"};
      }

      return $ret;
   }

   sub check_sys ()
   {

      print "checking system ...";

      open ( FIC , "uname |");

      $sys_kind = <FIC>;

      close (FIC);

      if ($sys_kind =~ /FreeBSD/ )
      {
         $makevars{'ELF_MODE'} = "elf_i386_fbsd";
      }
      else
      {
         $makevars{'ELF_MODE'} = "elf_i386";
      }
      print $colors{"GREEN"}." ok\n".$colors{"STD"};
   }


   sub check_common_tools()
   {
      my $ret = 0;
      my $bin_path;
      my %tools;

      foreach my $tool ( keys(%tools_common) )
      {
         my $tmp_ret = 1;

         print "checking for ".$tool."...";
         foreach my $bin ( @{$tools_common{$tool}} )
         {
            $bin_path = `which $bin 2>/dev/null` or next;
				chomp $bin_path;
            my $retval = $? >> 8;

            if ( $retval == 0 )
            {
               $tmp_ret = 0;
               $makevars{$tool} = $bin_path;

               if ( exists($tools_flags{$tool}) )
               {
                  $makevars{$tool} .= " ".join(" ", @{$tools_flags{$tool}})
               }

               print $colors{"GREEN"}." yes\n".$colors{"STD"};
               last;
            }
         }

         if ( $tmp_ret != 0 )
         {
            $ret = 1;
            print $colors{"RED"}." no\n".$colors{"STD"};
         }

      }
      return $ret;
   }

   sub disable_colors()
   {
      foreach my $color ( keys(%colors) )
      {
         $colors{$color} = "";
      }
   }

	sub check_libxml
	{
		my $ret = system ("/usr/bin/perl -e 'use XML::LibXML;' >/dev/null 2>&1");  
		if ($ret != 0)
		{
			printf "XML::LibXML (Perl XML/LibXML library) not installed, please install it\n";
			exit 1;
		}
			
	}


# }} Functions definitions

# {{ Main
   GetOptions ('with-floppy' => \$use_floppy, 'with-spoq' => \$use_spoq, 'with-xcov' => \$use_xcov, 'with-instrumentation' => \$use_instrumentation , 'help' => \$help);
	if ($help)
	{
		print "conf-env.pl configures pok for its execution\n";
      print "\n";
		print "Supported options:\n";
		print "  --help                    Print this help\n";
		print "  --with-xcov               Try to use xcov from project coverage\n";
		print "  --with-instrumentation    Automatically instrument the code of POK\n";
		print "  --with-floppy             Add rules to install POK automatically in floppy disk images\n";
      print "\n";
      print "Example:\n";
      print "  ./misc/conf-env.pl --with-xcov --with-instrumentation\n";
      print "\n";
      print "Additional information\n";
      print "  You can automatically use this program by invoking make configure\n";
      print "  in the root directory of POK\n";
		exit 0;
	}

   if ($use_floppy == 1)
   {
      $tools_arch{"x86"}{"MCOPY"} = [ "mcopy"]; 
      $tools_arch{"x86"}{"MMD"} = [ "mmd"]; 
   }

   if ($use_xcov == 1)
   {
      $tools_common{"XCOV"} = [ "xcov"]; 
   }


   if ($use_spoq == 1)
   {
      $tools_arch{"x86"}{"QEMU"} = [ "qemu-spoqed"]; 
   }


	check_libxml ();
   check_common_tools ();

   my $nb_arch = 0;

   for my $v ("x86", "ppc", "sparc")
   {
      
      if (check_arch_tools ($v) == 0)
      {
         $nb_arch++; 
      }
   }

   if ($nb_arch == 0)
   {
      print $colors{"RED"}."No architecture was configured, will exiting.\n".$colors{"STD"};
   }

   check_sys ();

   concat_flags ("CONFIG_CFLAGS", @cflags);
   concat_flags ("CONFIG_LDFLAGS", @ldflags);
   
   if ($sys_kind =~ /CYGWIN/ )
   {
      my $tmp_path = `cygpath.exe -d $ENV{PWD}/misc/grub-boot-only.img`;
      chomp ($tmp_path);
      $tmp_path =~ s/\\/\\\\/g;
      
      my $tmp_path2 = `cygpath.exe -d $ENV{PWD}/toolchain/qemu`;
      chomp ($tmp_path2);
      $tmp_path2 =~ s/\\/\\\\/g;

      $makevars{'CONFIG_QEMU_x86'} = "-fda \'$tmp_path\' -L \'$tmp_path2\' ";
   }
   else
   {
      $makevars{'CONFIG_QEMU_x86'} = " -fda grub-boot-only.img ";
   }
   
 
   if ($use_instrumentation == 1)
   {
   	print $colors{"GREEN"}."Set instrumentation flag\n".$colors{"STD"};
      $makevars{"INSTRUMENTATION"} = "1";
   }
	else
	{
      $makevars{"INSTRUMENTATION"} = "0";
	}


   if ($use_spoq == 1)
   {
      $makevars{"SPOQ"} = "1"; 
   }
  
   create_env ();

   check_ocarina ();

	check_qemu_xcov () if ($use_xcov == 1);

   print $colors{"GREEN"}."DONE !\n".$colors{"STD"};

# }} Main

