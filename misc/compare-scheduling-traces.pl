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
#                                      Copyright (c) 2007-2009 POK team 
#
# Created by julien on Fri Oct 16 13:56:22 2009 
#


#!/usr/bin/perl -w

use strict;
use XML::LibXML;

###################################
# Global variables
###################################
my $simufile;
my $simuparser;
my $simudoc;
my $simuxpc;

my $execfile;
my $execparser;
my $execdoc;
my $execxpc;

my $mappingfile;
my $mappingparser;
my $mappingdoc;
my $mappingxpc;

my $nberrors   = 0;
my $nbgood     = 0;
my $iserror;


my %colors =
(
 "RED"	=> "\033[0;31m",     "LRED"	=> "\033[1;31m",
 "GREEN"	=> "\033[0;32m",     "LGREEN"	=> "\033[1;32m",
 "BLUE"	=> "\033[0;34m",     "LGREEN"	=> "\033[1;34m",
 "STD"	=> "\033[;m",
 );



###################################
# Functions 
###################################

sub usage ()
{
   print STDERR "Usage: compare-scheduling-traces.pl mapping events1 events2\n";
   print STDERR "Ex: compare-scheduling-traces.pl mapping-file.xml events-from-cheddar.xml events-from-pok.xml\n";
   print STDERR " * mapping-file.xml         - mapping between Cheddar and POK, created by Ocarina code generator\n";
   print STDERR " * events-from-cheddar.xml  - scheduling events created by Cheddar during scheduling simulation\n";
   print STDERR " * events-from-pok.xml      - scheduling events created by POK during execution\n";
   print STDERR "---\n";
   print STDERR "This tool compares two scheduling traces and analyze\n";
   print STDERR "if they are similar or not.\n";
   print STDERR "Scheduling traces could be retrieved by an\n";
   print STDERR "instrumented version of POK and with Cheddar\n";
   print STDERR "This tool is used to check that scheduling diagrams\n";
   print STDERR "are similar between execution and simulation\n";
}

sub mapping_simu2exec_task
{
   my $simuname   = shift;
   my @result     = $mappingxpc->findnodes ('//thread[@cheddarname="'.$simuname.'"]');
   my $node;

   if ($#result == -1)
   {
      print STDERR $colors{"RED"} . "[CRASH] Cannot find execution task that have the simulation name $simuname\n" . $colors{"STD"};
      exit 3;
   }

   if ($#result > 0)
   {
      print STDERR $colors{"RED"} . "[CRASH] Too many tasks match the simulation name $simuname\n" . $colors{"STD"};
      exit 3;
   }

   $node = pop (@result);
   return $node->getAttribute ("pokname");
}

sub compare_running_tasks ()
{
   my @first_running_task  = reverse ($simuxpc->findnodes ("//running_task"));
   my @second_running_task = reverse ($execxpc->findnodes ("//running_task"));

   my $tmp;
   my $tmptask;
   my $tmptime;
   my $tmptime2;
   my $simutask;
   my $simutime;
   my $mappedtask;
   my $exectime;
   my $exectask;
   my $simuwcet;
   my $execwcet;
   my $text;

   while (1)
   {
      $iserror = 0;

      #Step 1: Get informations from simulation 
      $tmp = pop (@first_running_task);

      return if (! defined ($tmp));
      $text = $tmp->textContent;
      ($simutime, $simutask) = ($text =~ /\s*(\d+)\s*([\w\d\.\_]+)/);
      $tmptime = $tmptime2 = $simutime;

      while (defined ($tmp = pop (@first_running_task)))
      {
         $text = $tmp->textContent;
         ($tmptime, $tmptask) = ($text =~ /\s*(\d+)\s*([\w\d\.\_]+)/);
         if (($tmptask ne $simutask) or ($tmptime != ($tmptime2 + 1)))
         {
            push @first_running_task, $tmp;
            last;
         }
         $tmptime2 = $tmptime;
      }
      $simuwcet = $tmptime - $simutime;
      print "[SIMULATION][TIME $simutime] Task $simutask executed during $simuwcet\n";

      #Step 2: Find corresponding execution task name, see if it exists
      $mappedtask = mapping_simu2exec_task ($simutask);

      #Step 3: Get information from execution
      $tmp = pop (@second_running_task);

      return if (! defined ($tmp));
      $text = $tmp->textContent;
      ($exectime, $exectask) = ($text =~ /\s*(\d+)\s*([\w\d\.\_]+)/);
      $tmptime = $tmptime2 = $exectime;
      $execwcet = 0;
      while (defined ($tmp = pop (@second_running_task)))
      {
         $execwcet = $execwcet + 1;
         $text = $tmp->textContent;
         ($tmptime, $tmptask) = ($text =~ /\s*(\d+)\s*([\w\d\.\_]+)/);
         if ( ( $tmptask ne $exectask) or ($tmptime != ($tmptime2 + 1)))
         {
            push @second_running_task, $tmp;
            last;
         }
         $tmptime2 = $tmptime;
      }
#      $execwcet = $tmptime - $exectime;
      print "[EXECUTION][TIME $exectime] Task $exectask executed during $execwcet\n";

      #Step 4: Compare results
      if ($mappedtask ne $exectask)
      {
         print STDERR $colors{"RED"} . "[ERROR] Simulated and execution task does not match\n" . $colors{"STD"};
         print STDERR $colors{"RED"} . "[ERROR] Task $exectask is executed while task $mappedtask (simulation name: $simutask) should be executed\n" . $colors{"STD"};
         exit 5;
         $iserror = 1;
      }

      if ($execwcet > $simuwcet)
      {
         print STDERR $colors{"RED"} . "[ERROR] Simulated and execution task does not match\n" . $colors{"STD"};
         print STDERR $colors{"RED"} . "[ERROR] Task $exectask has $execwcet execution at execution while it has an execution time of $simuwcet at simulation\n" . $colors{"STD"};
         exit 4;
         $iserror = 1;
      }

      #TODO: Compare periods, should fix some initialization bugs

      print STDERR $colors{"GREEN"} . "[INFO] Simulation of task $simutask and execution $exectask seem to be good\n" . $colors{"STD"};

      if ($iserror == 1)
      {
         $nberrors = $nberrors + 1;
      }
      else
      {
         $nbgood = $nbgood + 1;
      }

   }
}


###################################
# Main Program
###################################

if ($#ARGV != 2)
{
   print STDERR "[ERROR] You must provide three XML files as parameters\n";
   usage ();
   exit 1;
}

$mappingfile   = $ARGV[0];
$simufile      = $ARGV[1];
$execfile      = $ARGV[2];

if ( ! -f $mappingfile)
{
   printf STDERR "File $mappingfile is not readable\n\n";
   usage ();
   exit 1;
}

if ( ! -f $simufile)
{
   printf STDERR "File $simufile is not readable\n\n";
   usage ();
   exit 1;
}


if ( ! -f $execfile)
{
   printf STDERR "File $execfile is not readable\n\n";
   usage ();
   exit 1;
}


$simuparser    = XML::LibXML->new;
$execparser    = XML::LibXML->new;
$mappingparser = XML::LibXML->new;

$simudoc      = $simuparser->parse_file ($simufile);
$execdoc      = $execparser->parse_file ($execfile);
$mappingdoc   = $mappingparser->parse_file ($mappingfile);

$simuxpc      = XML::LibXML::XPathContext->new ($simudoc);
$execxpc      = XML::LibXML::XPathContext->new ($execdoc);
$mappingxpc   = XML::LibXML::XPathContext->new ($mappingdoc);

compare_running_tasks ();
print "Summary: $nberrors error(s) and $nbgood passed tests\n";
exit 0;
