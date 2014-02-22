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
#                                      Copyright (c) 2007-2009 POK team 
#
# Created by julien on Thu Apr 16 16:27:14 2009 
#


#!/usr/bin/perl

use strict;

if ($#ARGV != 0)
{
   print "Use check-arinc653-conffile.pl filename\n";
   exit 3;
}

my $file = shift;
my $xsdfile = $ENV{'POK_PATH'}."/misc/arinc653-schema.xsd";

use XML::LibXML;

my $schema_file = $xsdfile;
my $document    = $file;

my $schema = XML::LibXML::Schema->new(location => $schema_file);

my $parser = XML::LibXML->new;
my $doc    = $parser->parse_file($document);

eval { $schema->validate($doc) };
die $@ if $@;

exit 0;
