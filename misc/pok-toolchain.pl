#!/usr/bin/perl
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
# Created by julien on Tue Oct 28 01:55:28 2008 
#


#!/usr/bin/perl

use strict;
use Term::ANSIColor;
use Getopt::Long;
use File::Temp;

my $nocheck    = 0;
my $nogenerate = 0;
my $check      = 1;
my $norun      = 0;
my $arinc653   = 0;
my $generate   = 1;
my $debug      = 0;
my $models;
my @models;
my $rootnode;
my $ocarina_path;

my %real_theorems = ("one_security_level_by_partition"      => "One_Security_Level_By_Memory",
                     "major_time_frame"                     => "scheduling_major_frame",
                     "error_coverage"                       => "check_error_coverage",
                     "memory_component_in_model"            => "Contains_Memories",
                     "vprocessors_provide_vbuses"           => "Virtual_Bus_On_Virtual_Processors",
                     "buses_provide_vbuses"                 => "Virtual_Bus_On_Buses",
                     "bell_lapadula"                        => "bell_lapadula",
                     "biba"                                 => "biba",
                     "memory_requirements_partition"        => "check_memory_requirements_partitions",
                     "memory_requirements_thread"           => "check_memory_requirements_threads",
                     "omission_transient"                   => "check_omission_transient",
                     "partitions_execution"                 => "partitions_execution",
                     "omission_permanent"                   => "check_omission_permanent",
                     "hm_compliance_module"                 => "check_processor_hm_validity",
                     "hm_compliance_partition"              => "check_virtual_processor_hm_validity",
                     "hm_compliance_task"                   => "check_thread_hm_validity",
                     "mils2"                                => "MILS_1",
                     "mils2"                                => "MILS_2",
                     "cipher_algorithms"                    => "Check_Virtual_Bus_Cipher_Algorithms",
                     "virtual_buses_implementation"         => "check_virtual_buses_implementation",
                     "check_criticality"                    => "check_partitions_criticality"
                     );


my %real_theorems_title = ("one_security_level_by_partition"   => "Check that partition component share the same memory level",
                           "major_time_frame"                  => "Check Major Time Frame compliance",
                           "error_coverage"                    => "Check error coverage",
                           "memory_component_in_model"         => "Check that AADL model contain memory components",
                           "vprocessors_provide_vbuses"        => "Check that virtual processors contain virtual buses",
                           "buses_provide_vbuses"              => "Check that buses provides virtual buses",
                           "partitions_execution"              => "Check that each partition is executed at least one time by the module",
                           "bell_lapadula"                     => "Check Bell-Lapadula security policy",
                           "biba"                              => "Check Biba security policy",
                           "memory_requirements_partition"     => "Check Partitions Memory requirements",
                           "memory_requirements_thread"        => "Check Threads Memory requirements",
                           "omission_transient"                => "Check for transient errors between partitions",
                           "omission_permanent"                => "Check for permanent errors between partitions",
                           "mils1"                             => "Check that connected partitions share the same security levels (MILS)",
                           "mils2"                             => "Check that connections support appropriate security levels (MILS)",
                           "hm_compliance_module"              => "Check compliance of Health Monitoring service for modules",
                           "hm_compliance_partition"           => "Check compliance of Health Monitoring service for partitions",
                           "hm_compliance_task"                => "Check compliance of Health Monitoring service for partitions processes",
                           "cipher_algorithms"                 => "Check that each virtual bus with a different security level has a different cipher key",
                           "virtual_buses_implementation"      => "Check that each virtual bus provides protection mechanisms",
                           "check_criticality"                 => "Check that partitions declare their criticality level"
                          );

my %real_theorems_reasons = ("one_security_level_by_partition"   =>
                                 "All components of a partition must share the same security level. In your case, components probably share different security levels or two partitions at different security levels are bound to the same memory component",
                             "major_time_frame"                  => "The major time frame on a processor must be equal to the sum of all partition slots. In your case, one or several processor may have an erroneous major time frame regarding its slots.",
                             "error_coverage"                    => "Some errors are not recovered during the execution of your system. A thread, a processor or a virtual processor may miss to catch an error while the system is executing",
                             "memory_component_in_model"         => "Your model does not contain any memory component. It means that you didn't specify the memory requirements of your partitions and thus, have not set partition memory settings.",
                             "vprocessors_provide_vbuses"        => "Virtual processors (partitions runtime) must provide information about their security levels. Your virtual processors should provide this information through the Provided_Virtual_Bus_Class property. In addition, virtual buses must declare their security levels with the POK::Security_Level property.",
                             "buses_provide_vbuses"              => "Buses must provides or contain virtual buses to indicate the security/safety layers they provide. You should add virtual buses subcomponents to bus or use the Provides_Virtual_Bus_Class property on them",
                             "bell_lapadula"                     => "Your model does not enforce the Bell Lapadula security policy. A partition is reading a data at a higher security level or is writing a data at a lower security level",
                             "biba"                              => "Your model does not enforce the Biba security policy. A partition is reading a data at a lower security level or is writing a data at a higher security level",
                             "memory_requirements_partition"     => "Specified memory requirements are not correct. The size of partition memory is wrong regarding the requirements of its tasks (Source_Code_Size property and so on)",
                             "memory_requirements_thread"        => "Specified memory requirements of some threads are not correct. The size declared in threads must be greater or equal to the sum of the required memory size of its subprograms. You must check your model and make sure that subprogram requirements are correct or thread memory requirements are correct regarding its called subprograms",
                             "omission_transient"                => "A partition at a lower safety level can impact a partition at a higher security level due to its reconfiguration strategies (restart a partition may miss some data). This detects transient errors",
                             "omission_permanent"                => "A partition at a lower safety level can impact a partition at a higher security level due to its reconfiguration strategies (restart a partition may miss some data). This detects permanent errors.",
                             "mils1"                             => "Your model does not enforce MILS isolation rules. Connected partitions must share the same security levels. In your model, some partitions are connected but share different security levels. It may result in a security leak in your system.",
                             "mils2"                             => "Some connections of your system does not support the security levels of their connected partitions. You have to check your model and be sure that connected partitions have the same security levels but also that connections support these security levels",
                             "hm_compliance_module"              => "You must define ARINC653::HM_Errors and ARINC653::HM_Actions on AADL processor components to specify the health monitoring policy. If you did it, some values are not allowed on processors",
                             "hm_compliance_task"                => "You must define ARINC653::HM_Errors and ARINC653::HM_Actions on AADL threads components to specify the health monitoring policy. If you did it, some values are not allowed on threads",
                             "hm_compliance_partition"           => "You must define ARINC653::HM_Errors and ARINC653::HM_Actions on AADL virtual processor components to specify the health monitoring policy of your partitions. If you did it, some values are not allowed on virtual processors",
                           "virtual_buses_implementation"        => "You must define protection mechanisms for each virtual bus using the Implemented_As property",
                           "partitions_execution"                => "Check that each virtual processor of each processor is listed in the partitions slots allocation.",
                             "check_criticality"                 => "You didn't associate a Criticality in your partitions. You must define the property ARINC653::Criticality or POK::Criticality_Level with your virtual processor component"
                            );


my $make;

#Prints the usage of the toolchain program
sub usage
{
   print "Use toolchain.pl --models=model1,model2\n";
	print "--no-generate       Generate and build code from models\n";
   print "--no-check          Look for security or safety issues\n";
   print "--no-run            Do not run compiled system\n";
   print "--root=system_name  Indicate the root node of your AADL model\n";
   print "--arinc653          Generate ARINC653 code\n";
}

sub check_files
{
   return 0 if ($#models == -1);
   
   foreach my $m (@models)
   {
      return 0 if ( ! -f $m);
   }
   return 1;
}

sub check_tools
{
   $make = "make";
   my $sysname = `uname`;

   chomp ($sysname);


   if ($sysname eq "FreeBSD")
   {
      $make = "gmake";
   }
}

sub get_models_str
{
   my $str = "";
   
   foreach my $m (@models)
   {
      $str .= "$m ";
   }
   return $str . "\$POK_PATH/misc/aadl-library.aadl ";
}

sub get_infos
{
   my $str = "";
   for my $m (@models)
   {
      open FIC, $m;
      while ( my $line = <FIC> )
      {
         if ($line =~ /.*system implementation\s+(.*)\s*/)
         {
            $str = $1;
         }
      }
      close FIC;
   }
   
   $str =~ tr/A-Z/a-z/;
   $str =~ tr/\./_/;
   
   #FIXME : get the security model of the system
   return ($str, "biba");
}

sub get_nodes_names
{
   my @names;
   my $name;
   my $analyze = 0;

   for my $m (@models)
   {
      open FIC, $m;
      while ( my $line = <FIC> )
      {
         if ($line =~ /.*system implementation\s+(.*)\s*/)
         {
            $analyze = 1;
         }

         if ($analyze == 1)
         {
            if ($line =~ /\s+([a-zA-Z0-9\_]+)\s*:\s*processor.+/)
            {
               $name = $1;
 
               $name =~ tr/A-Z/a-z/;
               $name =~ tr/\./_/;
               push @names,$name;
            }
         }
      }
      close FIC;
   }
     
   #FIXME : get the security model of the system
   return @names;
}

sub check_real_theorem
{
   my $theorem = shift;
   my $dir;
   my $m;
   my $line;
   my $insystem = 0;
   my $command_str;
   my $checked = 1;
   my $nblines = 0;

   if ($debug)
   {
      $dir = File::Temp::tempdir (CLEANUP => 0);
   }
   else
   {
      $dir = File::Temp::tempdir (CLEANUP => 1);
   }

   if ($debug)
   {
      print "Temporary directory $dir\n";
   }

   foreach $m (@models)
   {
      open SRC , $m;
      open DST , ">$dir/$m";

      while ($line = <SRC>)
      {
         if ($line =~ /system implementation/)
         {
            if (defined ($rootnode))
            {
               $line =~ /system\s+implementation\s+([a-zA-Z0-9\.\_]+)/;
               
               my $systemname = $1;
               if ($systemname eq $rootnode)
               {
                  $insystem = 1;
               }
            }
            else
            {
               $insystem = 1;
            }
         }

         if ( ($line =~ /\s*end\s+[a-zA-Z0-9\_]+/) && ($insystem == 1))
         {
            print DST "annex real_specification {** \n";
            print DST "   theorem added_theorem \n";
            print DST "      foreach s in system_set do \n";
            print DST "       requires ( " . $real_theorems{$theorem} ." ); \n";
            print DST "      check (1 = 1);\n";
            print DST "   end added_theorem;\n";
            print DST "**};\n";
            $insystem = 0;
         }
         chomp ($line);
         print DST  "$line\n";
      }

      close DST;
      close SRC;
   }

   $command_str = "ocarina -aadlv2 -f -g real_theorem -real_lib \$POK_PATH/misc/real.lib";

   foreach $m (@models)
   {
      $command_str .= ' '.$dir.'/'.$m;
   }

   $command_str .= " \$POK_PATH/misc/aadl-library.aadl";

   if (defined ($rootnode))
   {
      $command_str .= " -r $rootnode";
   }

   $command_str .= " 2>&1";

   if ($debug)
   {
      print "Command: $command_str";
   }

   open CMD, "$command_str |" or return 0;
   while ($line = <CMD>)
   {
      if($debug)
      {
         print $line;
      }
      if ($line =~ /FALSE/)
      {
         $checked = 0;
      }

      if ($line =~ /.*fatal error.*/)
      {
         $checked = 0;
      }
      $nblines++;
   }
   close CMD;

   return 0 if ($nblines == 0);
   return $checked;
}

sub check_model_correctness
{
   my $ret;
   my $command_opts = "-aadlv2 -f -i ";
   my $command_str;
   my $v;
   my ($root_name, $security_model) = get_infos ();

   $command_opts .= "-r $rootnode " if (defined ($rootnode));

   $command_str = "ocarina " . $command_opts . get_models_str . "2>/dev/null";

   $ret = system ($command_str);

   return 0 if ($ret != 0);

   return 1;
}




sub verify
{
   my $v;
   my $ret = 1;
   my $blanks = 0;
   my $title;
   my $reason;

   foreach $v (keys %real_theorems)
   {
      $title   = $real_theorems_title{$v};
      $reason  = $real_theorems_reasons{$v};
      use Term::ANSIColor qw(:constants);
      print BOLD, WHITE, "$title";
      
      $blanks = 70 - length ($title);
      while ($blanks > 0)
      {
         print ".";
         $blanks--;
      }

      if (check_real_theorem ($v) == 0)
      {
         print color("red"), " NOT validated\n", color("reset");
         my $len = length ($reason);
         my $i = 0;
         while ($i < $len)
         {
            print "   ";
            if ( $len - $i > 65)
            {
               print (substr ($reason, $i, 65));
               $i = $i + 65;
            }
            else
            {
               print (substr ($reason, $i, $len - $i));
               $i = $len;
            }
            print "\n";
         }
      }
      else
      {
         print color("green"), " validated \n", color("reset");
      }
   }
   return 1;
}

sub generate_and_build
{
   my $ret;
   my $command_opts = "-aadlv2 -f -g pok_c ";
   my $command_str;
   my $arinc_xml_filename;
   my $v;
   my ($root_name, $security_model) = get_infos ();
   my @nodes = get_nodes_names ();

   $command_opts .= "-r $rootnode " if (defined ($rootnode));

   $command_opts = " -k no-assert -k arinc653 " . $command_opts if ($arinc653 != 0);

   $command_str = "ocarina " . $command_opts . get_models_str;

   print "Will execute $command_str\n";
   $ret = system ($command_str);

   return 0 if ($ret != 0);

   if ($arinc653 != 0)
   {
      foreach $v (@nodes) 
      {
         $arinc_xml_filename = "generated-code/$v"."_arinc653-conf.xml";
         print ("Check ARINC653 deployment file compliance for node $v ($arinc_xml_filename)\n");

         $command_str = $ENV{'POK_PATH'} . "/misc/check-arinc653-conffile.pl $arinc_xml_filename 2>/dev/null";
         $ret = system ($command_str);

         if ($ret != 0)
         {
            print color("red"), "\n\n    ARINC653 deployment file is NOT conform to XSD schema\n\n\n", color("reset");
         }
         else
         {
            print color("green"), "\n\n      ARINC653 deployment file is conform to XSD schema\n\n\n", color("reset");
         }
      }
   }

   foreach $v (@nodes)
   {
      print ("Compile node $v (generated-code/$v)\n");
      chdir ("generated-code/".$v) or die ("cannot find generated code directory");

      if ($norun)
      {
         $ret=system ("$make  clean all >/dev/null");
      }
      else
      {
         $ret=system ("$make clean all run >/dev/null");
      }
      return 0 if ($ret != 0);
      chdir ("../..");
   }
   return 1;
}


#################################################################
# MAIN PROGRAM HERE
#################################################################

GetOptions ('models=s' => \$models,
            'root=s' => \$rootnode,
				'no-generate' => \$nogenerate,
				'no-check' => \$nocheck,
				'no-run' => \$norun,
				'debug' => \$debug,
				'arinc653' => \$arinc653,
            ) or die "Cannot read options!\n";

check_tools();


if ($nocheck)
{
   $check = 0;
}

if ($nogenerate)
{
   $generate = 0;
}

if ($generate == 0 and $check == 0)
{
   print "You MUST choose an ACTION to perform\n";
   usage ();
   exit (1);
}

@models = split (/,/, $models);
$ocarina_path = `ocarina-config --prefix` or die ("You must have ocarina on your system");
chomp ($ocarina_path);

if (! check_files() )
{
   usage();
   exit (1);
}

if ( ! check_model_correctness() )
{
   print color("red"), "AADL models are not syntaxically correct, please fix them !\n", color("reset");
   exit (1);
}

if ($check)
{
   if ( ! verify() )
   {
      print ("Verification failed\n");
   }
}

if ($generate)
{
   if (!generate_and_build ())
   {
      print ("Code generation failed\n");
      exit (1);
   }
}
