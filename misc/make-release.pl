#!/usr/bin/perl -w
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
#                                      Copyright (c) 2007-2008 POK team 
#
# Created by julien on Wed Nov 12 21:24:05 2008 
#

use strict;
use File::Temp qw/ tempfile tempdir /;
use File::Copy;
use File::Path;

my $file;
my @date;
my $pok_version = `date '+%Y%m%d'`; chomp ($pok_version);
my $tmpdir = tempdir () or die ("cannot create temp directory");

my $reldir = $tmpdir."/pok-" . $pok_version;
my $curdir = $ENV{'PWD'};

sub package_file
{
   my $file = shift;
   my @subdirs = split /\//, $file;
   my $filename = pop @subdirs;
   my $cdir = $reldir;
   my $line;
   my $go_write = 1;
   my $inputfile;
   my $outputfile;
   my @stat;
   my $inputmode;
   
   foreach my $subdir (@subdirs)
   {
      $cdir = $cdir  . '/' . $subdir;
      if ( ! -d $cdir)
      {
         mkdir ($cdir) or die ("cannot create $cdir");
      }
   }
   
   $inputfile =  "$curdir/$file";
   $outputfile = "$reldir/$file";
   
   @stat = stat ($inputfile) or die ("cannot stat $inputfile");
   $inputmode = $stat[2];
   
   open (INPUTFILE, "$inputfile");
   open (OUTPUTFILE, ">$outputfile");
   
   while ($line = <INPUTFILE>)
   {
      if ($line =~ /CUT HERE/)
      {
         $go_write = 0;
      }

      if ($go_write == 1)
      {
         print OUTPUTFILE $line;  
      }
      
      if ($line =~ /END CUT/)
      {
         $go_write = 1;
      }
   }
   
   close (OUTPUTFILE);
   close (INPUTFILE);
   chmod ($inputmode, $outputfile) or die ("Cannot chmod $outputfile");
}

#########################################
# MAIN FUNCTION HERE
#########################################

print "Release temporary directory is $reldir, current directory $curdir\n";

open (FILE, "misc/release-files") or die ("cannot open filelist to package");

mkdir ($reldir) or die ("Cannot create $reldir");

mkdir ($reldir."/doc");

copy ("doc/userguide/pok-user.pdf" , "$reldir/doc/pok-user.pdf");
copy ("doc/develguide/pok-devel.pdf" , "$reldir/doc/pok-devel.pdf");
copy ("doc/kernel-refman.pdf" , "$reldir/doc/kernel-refman.pdf");
copy ("doc/libpok-refman.pdf" , "$reldir/doc/libpok-refman.pdf");

while ($file = <FILE>)
{
   chomp ($file);
   package_file ($file);
}

close (FILE);

chdir ($tmpdir);
system ("tar cfz pok-$pok_version.tgz pok-$pok_version");
chdir ($curdir);
copy ("$tmpdir/pok-$pok_version.tgz" , "$curdir/pok-$pok_version.tgz");
rmtree ($tmpdir)
