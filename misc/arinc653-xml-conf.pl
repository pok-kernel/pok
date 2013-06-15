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


# This program takes as parameter an ARINC653 compliant XML file
# that describes the overall architecture of the system and generates
# configuration files to deploy POK systems.

#!/usr/bin/perl -w

use strict;
use Data::Dumper;
use XML::LibXML;
use XML::LibXML::XPathContext;

###################################################################
# GLOBAL VARIABLES
###################################################################

my $header_file;
my $source_file;
my $parser;
my $doc;
my $xpc;


###################################################################
# FUNCTIONS
###################################################################

sub usage 
{
   print STDERR "Use check-arinc653-conffile.pl filename\n";
   print STDERR "Once the program finished, it outputs two\n";
   print STDERR "deployment files: deployment.h and deployment.c";
}

sub begin_header
{
   print $header_file "#ifndef __POK_ARINC653_GENERATED_CONFIGURATION_H__\n";
   print $header_file "#define __POK_ARINC653_GENERATED_CONFIGURATION_H__\n";
   print $header_file "#include <core/schedvalues.h>\n";
}


sub begin_source
{
   print $source_file "#include <core/partition.h>\n";
   print $source_file "#include <core/error.h>\n";
   print $source_file "#include <types.h>\n";
   print $source_file '#include "deployment.h"'."\n";
}


sub end_header
{
   print $header_file "#endif";
}

sub headers
{
   for my $file ($header_file, $source_file)
   {
      print $file "/************************************************/\n";
      print $file "/* This is a deployment file generated from XML */\n";
      print $file "/* configuration file. This corresponds to the  */\n";
      print $file "/* kernel configuration. Changes will be lost   */\n";
      print $file "/* when you re-run the configuration tool.      */\n";
      print $file "/************************************************/\n";
      print $file "/* Generated with the POK configuration         */\n";
      print $file "/* generator                                    */\n";
      print $file "/*                                 (c) POK team */\n";
      print $file "/*                         http://pok.gunnm.org */\n";
      print $file "/************************************************/\n";
   }
}

sub header_general_purpose
{
   #General purpose variables
   print $header_file "\n\n";
   print $header_file "#define POK_NEEDS_GETTICK 1\n";
   print $header_file "#define POK_NEEDS_SCHED   1\n";
   print $header_file "\n\n";

   #Generate POK_CONFIG_NB_THREADS
   print $header_file "\n\n";
   print $header_file "/* #define POK_CONFIG_NB_THREADS   1234                   */\n";
   print $header_file "/* In POK_CONFIG_NB_THREADS, you MUST set the number      */\n";
   print $header_file "/* of threads handled by the system. The number comprise  */\n";
   print $header_file "/* ARINC653 processes, init process for each partition    */\n";
   print $header_file "/* and the recovery handler process.                      */\n";
   print $header_file "/* We CANNOT generate this information since the XML file */\n";
   print $header_file "/* does not contain it.                                   */\n";
   print $header_file "\n\n";

   #Generate POK_CONFIG_PARTITIONS_NTHREADS
   print $header_file "\n\n";
   print $header_file "/* #define POK_CONFIG_PARTITIONS_NTHREADS  {1,2,3,4}       */\n";
   print $header_file "/* In POK_CONFIG_PARTITIONS_NTHREADS, you MUST set the     */\n";
   print $header_file "/* numbner of threads/ARINC653 processesfor each partition.*/\n";
   print $header_file "/* defines the number of threads contained in one          */\n";
   print $header_file "/* partition. Here, the first partition contains           */\n";
   print $header_file "/* one process, the second two processes and so on.        */\n";
   print $header_file "/* We CANNOT generate this information since the XML file*/\n";
   print $header_file "/* does not contain it.*/\n";
   print $header_file "\n\n";

}

sub header_partitions_general
{
   my @result = $xpc->findnodes("//Partition");

   if ($#result > 0)
   {
      print $header_file "#define POK_NEEDS_PARTITIONS   1\n";
      print $header_file "#define POK_CONFIG_NB_PARTITIONS   " . ($#result + 1) . "\n";
   }
}

sub get_port_id 
{
   my $lookup;
   my @queueing_ports;
   my @sampling_ports;
   my $id;
   my $tmp;

   @queueing_ports = $xpc->findnodes("//Queueing_Port");
   @sampling_ports = $xpc->findnodes("//Sampling_Port");

   $lookup = shift;
   $id = 0;
   foreach $tmp (@queueing_ports)
   {
      if ($tmp->getAttribute ("Name") eq $lookup) 
      {
         return $id;
      }
      $id++;
   }

   foreach $tmp (@sampling_ports)
   {
      if ($tmp->getAttribute ("Name") eq $lookup) 
      {
         return $id;
      }
      $id++;
   }

   return -1;
}

sub get_max_port_id 
{
   my @queueing_ports = $xpc->findnodes("//Queueing_Port");
   my @sampling_ports = $xpc->findnodes("//Sampling_Port");
   return ($#queueing_ports + $#sampling_ports + 1);
}


sub get_partition_id 
{
   my $tmp;
   my @result;
   my $pid;
   my $searched_name;

   $searched_name = shift;
   
   @result = reverse ($xpc->findnodes("//Partition"));
   $pid = 0;
   for $tmp (@result)
   {
      if ($tmp->getAttribute("PartitionName")  eq $searched_name)
      {
         return $pid;
      }
      $pid++;
   }
   print STDERR "[ERROR] Cannot find partition with name $searched_name\n";
   print STDERR "[ERROR] so that we cannot compute its identifier. Generator\n";
   print STDERR "assumes its id is -1\n";

   return -1;
}

sub header_partitions_scheduler
{
   my @result = $xpc->findnodes("//Partition");
   my @result2;
   my @sizes;
   my $size;
   my $tmp;
   my $pn;
   my $expr;
   my $init = 0;

   print $header_file "\n\n";
   print $header_file "#define POK_NEEDS_SCHED_RR 1\n";
   print $header_file "/* By default, partitions use the Round Robin (RR) scheduler*/\n";
   print $header_file "/* This can be changed by modifying the configuration.      */\n";
   print $header_file "\n\n";


   print $header_file "\n\n";
   print $header_file "#define POK_CONFIG_PARTITIONS_SCHEDULER {";

   for $tmp (@result)
   {
      print $header_file "," if ($init == 1);
      print $header_file "POK_SCHED_RR";
      $init = 1;
   }
   print $header_file "}\n";

   print $header_file "/* The POK_CONFIG_PARTITIONS_SCHEDULER indicates the        */\n";
   print $header_file "/* scheduling algorithm for each partition. By default, we  */\n";
   print $header_file "/* set the Round Robin scheduler for each partition.        */\n";
   print $header_file "\n\n";
}

sub header_partitions_memory
{
   my @result = $xpc->findnodes("//Partition");
   my @result2;
   my @sizes;
   my $size;
   my $tmp;
   my $pn;
   my $expr;
   my $init = 0;

   for $tmp (@result)
   {
      $size = 0;
      $pn = $tmp->getAttribute ("PartitionName");
      chomp ($pn);
      $expr = '//Partition_Memory[@PartitionName="'.$pn.'"]/Memory_Requirements';
      @result2 = $xpc->findnodes ($expr);
      for $tmp (@result2)
      {
         $size = $size + $tmp->getAttribute ("SizeBytes");
      }
      push @sizes, $size;
   }

   print $header_file "#define POK_CONFIG_PARTITIONS_SIZE   {";
   foreach $tmp (@sizes)
   {
      print $header_file "," if ($init == 1);
      print $header_file $tmp;
      $init = 1;
   }
   print $header_file "}";
}

sub header_partitions_ports
{
   my @queueing_ports = $xpc->findnodes("//Queueing_Port");
   my @sampling_ports = $xpc->findnodes("//Sampling_Port");
   my @partitions = $xpc->findnodes ("//Partition");
   my $nb_ports;
   my $init = 0;
   my $portname;
   my $tmp;
   my $tmpxpc;
   my $nbports;
   my @nbports_partitions;

   $nb_ports = $#queueing_ports + $#sampling_ports + 2;
   # We add +2 because of array length in perl

   if ($#queueing_ports >= 0)
   {
      print $header_file "#define POK_NEEDS_QUEUEING_PORTS 1\n";
   }

   if ($#sampling_ports >= 0)
   {
      print $header_file "#define POK_NEEDS_SAMPLING_PORTS 1\n";
   }


   print $header_file "#define POK_CONFIG_NB_PORTS          $nb_ports\n";
   print $header_file "#define POK_CONFIG_NB_GLOBAL_PORTS   $nb_ports\n";
   print $header_file "\n";

   print $header_file "typedef enum {\n";
   $init = 0;
   foreach $tmp (@queueing_ports)
   {
      $portname = $tmp->getAttribute("Name") . "_global";
      print $header_file ",\n" if ($init == 1);
      print $header_file "   " . $portname . "=" . get_port_id ($portname);
      $init = 1;
   }

   foreach $tmp (@sampling_ports)
   {
      $portname = $tmp->getAttribute("Name")."_global";
      print $header_file ",\n" if ($init == 1);
      print $header_file "   " . $portname . "=" . get_port_id ($portname);
      $init = 1;
   }
   print $header_file "\n} pok_port_identifier_t\n\n";


   print $header_file "typedef enum {\n";
   $init = 0;
   foreach $tmp (@queueing_ports)
   {
      $portname = $tmp->getAttribute("Name");
      print $header_file ",\n" if ($init == 1);
      print $header_file "   " . $portname . "=" . get_port_id ($portname);
      $init = 1;
   }

   foreach $tmp (@sampling_ports)
   {
      $portname = $tmp->getAttribute("Name");
      print $header_file ",\n" if ($init == 1);
      print $header_file "   " . $portname . "=" . get_port_id ($portname);
      $init = 1;
   }
   print $header_file ",\n   invalid_local_port = " . (get_max_port_id() + 1 ). "\n} pok_port_local_identifier_t\n\n";

   print $header_file "#define POK_CONFIG_PARTITIONS_PORTS {";
   $init = 0;
   foreach $tmp (@partitions)
   {
      $tmpxpc = XML::LibXML::XPathContext->new ($tmp);
      @queueing_ports = $tmpxpc->findnodes ('//Partition[@PartitionName="'.$tmp->getAttribute("PartitionName").'"]/Queueing_Port');
      @sampling_ports = $tmpxpc->findnodes ('//Partition[@PartitionName="'.$tmp->getAttribute("PartitionName").'"]/Sampling_Port');
      $nbports = $#queueing_ports + $#sampling_ports + 2;
      # We add +2 because of array length in Perl

      print $header_file "," if ($init == 1);
      print $header_file $nbports;
      $init = 1;
   }
   print $header_file "}\n";
}


sub header_partitions_scheduling
{
   my @windows = $xpc->findnodes("//Window_Schedule");
   my @module  = $xpc->findnodes("//Module_Schedule");
   my $modulenode;
   my $windowid;
   my $nwindows;
   my $majorframe;
   my @slots;
   my @slots_allocation;
   my $slotlength;
   my $slotpartition;
   my $tmp;
   my $init = 0;

   if ($#module != 0)
   {
      printf STDERR "[WARNING] There should be only one ModuleSchedule element\n";
   }

   $modulenode = pop @module;
   $majorframe = $modulenode->getAttribute ("MajorFrameSeconds");
   $majorframe = $majorframe * 1000; #Convert from seconds to milliseconds

   print $header_file "\n\n";
   print $header_file "#define POK_CONFIG_SCHEDULING_MAJOR_FRAME $majorframe\n";
   print $header_file "/* Corresponds to the MajorFrameSeconds converted to */\n";
   print $header_file "/* millisecond.                                      */\n";
   print $header_file "\n\n";


   for $tmp (@windows)
   {
      $slotlength = $tmp->getAttribute ("WindowDurationSeconds");

      if (! defined $slotlength)
      {
         print STDERR "Attribute WindowDurationSeconds not found in Window_Schedule XML element\n";
      }

      $slotlength = $slotlength * 1000; #Convert to millisecond
      push @slots, $slotlength;

      $slotpartition = $tmp->parentNode->getAttribute("PartitionName");
      if (! defined $slotpartition)
      {
         $slotpartition = "unknown";
         print STDERR "[ERROR] Attribute PartitioName not found in Partition_Schedule XML element\n";
      }

      push @slots_allocation, get_partition_id ($slotpartition);
   }


   print $header_file "#define POK_CONFIG_SCHEDULING_SLOTS {";
   $init = 0;
   foreach $tmp (@slots)
   {
      print $header_file "," if ($init == 1);
      print $header_file $tmp;
      $init = 1;
   }
   print $header_file "}\n\n";
   print $header_file "/* POK_CONFIG_SCHEDULING_SLOTS corresponds to all time*/\n";
   print $header_file "/* slots for partitions execution.                    */\n";
   print $header_file "\n";


   print $header_file "#define POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION {";
   $init = 0;
   foreach $tmp (@slots_allocation)
   {
      print $header_file "," if ($init == 1);
      print $header_file $tmp;
      $init = 1;
   }
   print $header_file "}\n\n";
   print $header_file "/* POK_CONFIG_SCHEDULING_SLOTS_ALLOCATION corresponds*/\n";
   print $header_file "/* to slots allocation (which partition is executed  */\n";
   print $header_file "/* for each time slot.                               */\n";
   print $header_file "\n";
}


sub port_get_nb_destination
{
   my $portname;
   my @channels;
   my @sources;
   my @dests;
   my $tmp;

   $portname = shift;

   @channels = $xpc->findnodes("//Channel");

   foreach $tmp (@channels)
   {
      @sources = $tmp->findnodes ('Source/Standard_Partition[@PortName="'.$portname.'"]');
      @dests = $tmp->findnodes ('Destination');
      if ($#sources >= 0)
      {
         return $#dests + 1;
      }
   }
   print STDERR "Cannot find connection for port $portname\n";
   return -1;
}


sub partition_nb_ports
{
   my @partitions;
   my $partition;
   my @queueing_ports;
   my @sampling_ports;
   my $partition_name;

   $partition_name = shift;

   @partitions = $xpc->findnodes ('//Partition[@PartitionName="'.$partition_name.'"]');

   if ($#partitions != 0)
   {
      print STDERR "Cannot find partition with name $partition_name\n";
   }

   $partition = pop (@partitions);

   @queueing_ports = $partition->findnodes ("/Queueing_Port");
   @sampling_ports = $partition->findnodes ("/Sampling_Port");

   return $#queueing_ports + $#sampling_ports + 2;

   return 1;
}

sub find_destinations_from_source 
{
   my $portname;
   my @channels;
   my @sources;
   my @dests;
   my $tmp;

   $portname = shift;

   @channels = $xpc->findnodes("//Channel");

   foreach $tmp (@channels)
   {
      @sources = $tmp->findnodes ('Source/Standard_Partition[@PortName="'.$portname.'"]');
      @dests = $tmp->findnodes ('Destination');
      if ($#sources >= 0)
      {
         return @dests;
      }
   }
   return undef;
}


sub source_partitions_ports
{
   my @queueing_ports = $xpc->findnodes("//Queueing_Port");
   my @sampling_ports = $xpc->findnodes("//Sampling_Port");
   my @partitions = $xpc->findnodes ("//Partition");
   my @kinds;
   my @portdestinations;
   my @nbdestinations;
   my @destinations;
   my @identifiers;
   my @global_to_local;
   my @local_to_global;
   my @names;
   my $init;
   my $tmp;
   my $tmp2;
   my $tmp3;
   my $tmpxpc;
   my $nbports;

   $init = 0;

   foreach $tmp (@queueing_ports)
   {
      push @kinds, "POK_PORT_KIND_QUEUEING";
      push @identifiers, $tmp->getAttribute ("Name");
      push @names, $tmp->getAttribute ("Name");
      push @global_to_local, $tmp->getAttribute ("Name");
      push @local_to_global, $tmp->getAttribute ("Name") . "_global";

      if ($tmp->getAttribute ("Direction") eq "DESTINATION")
      {
         push @nbdestinations, 0;
         push @destinations , "NULL";
      }
      else
      {
         push @nbdestinations, port_get_nb_destination ($tmp->getAttribute("Name"));
         push @destinations , $tmp->getAttribute("Name") . "_destinations";
      }
   }

   foreach $tmp (@sampling_ports)
   {
      push @kinds, "POK_PORT_KIND_SAMPLING";
      push @identifiers, $tmp->getAttribute ("Name");
      push @names, $tmp->getAttribute ("Name");
      push @global_to_local, $tmp->getAttribute ("Name");
      push @local_to_global, $tmp->getAttribute ("Name") . "_global";

      if ($tmp->getAttribute ("Direction") eq "DESTINATION")
      {
         push @nbdestinations, 0;
         push @destinations , "NULL";
      }
      else
      {
         push @nbdestinations, port_get_nb_destination ($tmp->getAttribute ("Name"));
         push @destinations , $tmp->getAttribute ("Name") . "_destinations";
      }
   }


   #Generate destinations arrays
   foreach $tmp (@queueing_ports)
   {
      if ($tmp->getAttribute ("Direction") eq "SOURCE")
      {
         @portdestinations = find_destinations_from_source ($tmp->getAttribute ("Name"));
         $init = 0;
         print $source_file "uint8_t " . $tmp->getAttribute ("Name")."_destinations[" . port_get_nb_destination ($tmp->getAttribute ("Name")). "] = {";
         foreach $tmp2 (@portdestinations)
         {
            foreach $tmp3 ($tmp2->findnodes ("Standard_Partition"))
            {
               print $source_file "," if ($init == 1);
               print $source_file $tmp3->getAttribute ("PortName");
               $init = 1;
            }
         }
         print $source_file "}\n";
      }
   }

   foreach $tmp (@sampling_ports)
   {
      if ($tmp->getAttribute ("Direction") eq "SOURCE")
      {
         @portdestinations = find_destinations_from_source ($tmp->getAttribute ("Name"));
         $init = 0;
         print $source_file "uint8_t " . $tmp->getAttribute ("Name")."_destinations[" . port_get_nb_destination ($tmp->getAttribute ("Name")). "] = {";
         foreach $tmp2 (@portdestinations)
         {
            foreach $tmp3 ($tmp2->findnodes ("Standard_Partition"))
            {
               print $source_file "," if ($init == 1);
               print $source_file $tmp3->getAttribute ("PortName");
               $init = 1;
            }
         }
         print $source_file "}\n";
      }
   }

   #The pok_ports_names variable
   $init = 0;
   print $source_file "uint8_t pok_ports_names[POK_CONFIG_NB_PORTS] = {";
   foreach $tmp (@names)
   {
      print $source_file "," if ($init == 1);
      print $source_file '"' . $tmp . '"';
      $init = 1;
   }
   print $source_file "}\n";

   #The pok_ports_identifiers variable
   $init = 0;
   print $source_file "uint8_t pok_ports_identifiers[POK_CONFIG_NB_PORTS] = {";
   foreach $tmp (@identifiers)
   {
      print $source_file "," if ($init == 1);
      print $source_file  $tmp ;
      $init = 1;
   }
   print $source_file "}\n";

   #The pok_ports_nb_destinations variable
   $init = 0;
   print $source_file "uint8_t pok_ports_nb_destinations[POK_CONFIG_NB_PORTS] = {";
   foreach $tmp (@nbdestinations)
   {
      print $source_file "," if ($init == 1);
      print $source_file  $tmp ;
      $init = 1;
   }
   print $source_file "}\n";

   #The pok_ports_kinds variable
   $init = 0;
   print $source_file "uint8_t pok_ports_kinds[POK_CONFIG_NB_PORTS] = {";
   foreach $tmp (@kinds)
   {
      print $source_file "," if ($init == 1);
      print $source_file  $tmp ;
      $init = 1;
   }
   print $source_file "}\n";


   #The pok_ports_destinations variable
   $init = 0;
   print $source_file "uint8_t* pok_ports_destinations[POK_CONFIG_NB_PORTS] = {";
   foreach $tmp (@destinations)
   {
      print $source_file "," if ($init == 1);
      print $source_file  $tmp ;
      $init = 1;
   }
   print $source_file "}\n";


   #The pok_ports_local_to_global
   $init = 0;
   print $source_file "uint8_t pok_local_ports_to_global_ports[POK_CONFIG_NB_PORTS] = {";
   foreach $tmp (@local_to_global)
   {
      print $source_file "," if ($init == 1);
      print $source_file  $tmp ;
      $init = 1;
   }
   print $source_file "}\n";


   #The pok_ports_global_ports_to_local_ports variable
   $init = 0;
   print $source_file "uint8_t pok_global_ports_to_local_ports[POK_CONFIG_NB_PORTS] = {";
   foreach $tmp (@global_to_local)
   {
      print $source_file "," if ($init == 1);
      print $source_file  $tmp ;
      $init = 1;
   }
   print $source_file "}\n";


   #The pok_ports_nb_ports_by_partitions
   print $source_file "uint8_t pok_ports_nb_ports_by_partition[POK_CONFIG_NB_PARTITIONS] = {";
   $init = 0;
   foreach $tmp (@partitions)
   {
      $tmpxpc = XML::LibXML::XPathContext->new ($tmp);
      @queueing_ports = $tmpxpc->findnodes ('//Partition[@PartitionName="'.$tmp->getAttribute("PartitionName").'"]/Queueing_Port');
      @sampling_ports = $tmpxpc->findnodes ('//Partition[@PartitionName="'.$tmp->getAttribute("PartitionName").'"]/Sampling_Port');
      $nbports = $#queueing_ports + $#sampling_ports + 2;
      # We add +2 because of array length in Perl

      print $source_file "," if ($init == 1);
      print $source_file $nbports;
      $init = 1;
   }
   print $source_file "}\n";


   #The partports array
   foreach $tmp (@partitions)
   {
      $tmpxpc = XML::LibXML::XPathContext->new ($tmp);
      @queueing_ports = $tmpxpc->findnodes ('//Partition[@PartitionName="'.$tmp->getAttribute("PartitionName").'"]/Queueing_Port');
      @sampling_ports = $tmpxpc->findnodes ('//Partition[@PartitionName="'.$tmp->getAttribute("PartitionName").'"]/Sampling_Port');
      $nbports = $#queueing_ports + $#sampling_ports + 2;

      print $source_file "uint8_t " . $tmp->getAttribute("PartitionName") . "_partport" . "[" . $nbports . "] = {";
      $init = 0;
      foreach  $tmp2 (@queueing_ports)
      {
         print $source_file "," if ($init == 1);
         print $source_file $tmp2->getAttribute("Name");
         $init = 1;
      }
      foreach  $tmp2 (@sampling_ports)
      {
         print $source_file "," if ($init == 1);
         print $source_file $tmp2->getAttribute("Name");
         $init = 1;
      }
      print $source_file "}\n";
   }


   #The pok_ports_by_partitions
   print $source_file "uint8_t* pok_ports_by_partition[POK_CONFIG_NB_PARTITIONS] = {";
   $init = 0;
   foreach $tmp (@partitions)
   {
      $tmpxpc = XML::LibXML::XPathContext->new ($tmp);
      print $source_file "," if ($init == 1);
      print $source_file $tmp->getAttribute("PartitionName") . "_partport";
      $init = 1;
   }
   print $source_file "}\n";
}




###################################################################
# Main program now
###################################################################


if ($#ARGV != 0)
{
   usage;
   exit 3;
}

my $xmlfile = shift;

if ( ! -f $xmlfile)
{
   print STDERR "XML file does not exists ($xmlfile)\n";
   usage;
   exit 2;
}

if ( -f  "deployment.c")
{
   print "[WARNING] file deployment.c already exists\n";
}

if ( -f  "deployment.h")
{
   print "[WARNING] file deployment.h already exists\n";
}

open ($header_file, ">deployment.h");
open ($source_file, ">deployment.c");

$parser = XML::LibXML->new;
$doc = $parser->parse_file ($xmlfile);
$xpc = XML::LibXML::XPathContext->new ($doc);

begin_header;
begin_source;
headers();
header_general_purpose ();
header_partitions_general ();
header_partitions_memory ();
header_partitions_scheduler ();
header_partitions_scheduling ();
header_partitions_ports ();
source_partitions_ports ();
end_header;

close ($source_file);
close ($header_file);


exit 0;
