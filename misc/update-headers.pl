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
# Created by julien on Sun Aug 10 12:45:15 2008 
#


#!/usr/bin/perl -w

use strict;

my $user;
my $date;
my $force;
my $year;

#####################################################
# COMMON FUNCTIONS
#####################################################
sub has_header 
{
	my $filename = shift;
	my $c;
	my $i = 3;
	open (MYFILE, $filename);
	
	while ($i > 0)
	{
		$c = <MYFILE>;
		if ((defined ($c)) && ($c =~ m/.*POK header.*/))
		{
			close (MYFILE);
			return 1;
		}
		$i--;
	}
	close (MYFILE);
	return 0;
}

sub get_file_content
{
	my $filename = shift;
	my @c;
	my $t;

	return if ! -f $filename;

	open (MYFILE, $filename);

	while ($t = <MYFILE>)
	{
		push (@c,$t);
	}

	close (MYFILE);

	return @c;
}


sub treat_file
{
	my $filename = shift;
	treat_c_file ($filename) if ($filename =~ m/\.c$/);
	treat_c_file ($filename) if ($filename =~ m/\.h$/);
	treat_latex_file ($filename) if ($filename =~ m/\.tex$/);
	treat_txt_file ($filename) if ($filename =~ m/\.txt$/);
	treat_perl_file ($filename) if ($filename =~ m/\.pl$/);
	treat_perl_file ($filename) if ($filename =~ m/\.pm$/);
	treat_aadl_file ($filename) if ($filename =~ m/\.aadl$/);
}

sub treat_dir 
{
	my $dirname = shift;
	my $e;
	my $cname;
	my @elts;

	opendir (MYDIR, $dirname) or die ("cannot open rootdir");

	@elts = readdir (MYDIR);

	closedir (MYDIR);

	foreach $e (@elts)
	{
		next if ($e eq ".");
		next if ($e eq "..");
      next if ($e =~ m/^\./);

		
		$cname = "$dirname/$e";

		treat_file ($cname) if -f $cname;
		treat_dir ($cname) if -d $cname;
	}

}




#####################################################
# C FUNCTIONS
#####################################################
sub add_c_header
{
	my $filename = shift;
	my @contents;
	my $v;
	return if ! -f $filename;

	@contents = get_file_content ($filename);
	open (MYFILE, ">$filename");
	seek (MYFILE,0,0);
	print MYFILE "/*\n";
	print MYFILE " *                               POK header\n";
	print MYFILE " * \n";
	print MYFILE " * The following file is a part of the POK project. Any modification should\n";
	print MYFILE " * made according to the POK licence. You CANNOT use this file or a part of\n";
	print MYFILE " * this file is this part of a file for your own project\n";
	print MYFILE " *\n";
	print MYFILE " * For more information on the POK licence, please see our LICENCE FILE\n";
	print MYFILE " *\n";
	print MYFILE " * Please follow the coding guidelines described in doc/CODING_GUIDELINES\n";
	print MYFILE " *\n";
	print MYFILE " *                                      Copyright (c) 2007-$year POK team \n";
	print MYFILE " *\n";
	print MYFILE " * Created by $user on $date \n";
	print MYFILE " */\n\n";

	foreach $v (@contents)
	{
		print MYFILE $v;
	}
	close (MYFILE);
}


sub remove_c_header
{
	my $filename = shift;
	my @contents;
	my $v;
	my $i;
	return if ! -f $filename;

	@contents = get_file_content ($filename);

	$i = 16;

	unlink ($filename) or die ("cannot remove $filename");
	open (MYFILE, ">$filename");
	seek (MYFILE,0,0);
	
	foreach $v (@contents)
	{
		if ($i == 0)
		{
			print MYFILE $v;
		}
		else
		{
			$i--;
		}
	}
	close (MYFILE);
}

sub treat_c_file
{
	my $filename = shift;


	if ((has_header ($filename) == 1) && ($force == 1))
	{
		remove_c_header ($filename);
	}

	if (has_header ($filename) == 0)
	{
		add_c_header ($filename);
	}
}

#####################################################
# LaTeX FUNCTIONS
#####################################################

sub add_latex_header
{
	my $filename = shift;
	my @contents;
	my $v;
	return if ! -f $filename;

	@contents = get_file_content ($filename);
	open (MYFILE, ">$filename");
	seek (MYFILE,0,0);
	print MYFILE "%\n";
	print MYFILE "%                              POK header\n";
	print MYFILE "% \n";
	print MYFILE "% The following file is a part of the POK project. Any modification should\n";
	print MYFILE "% be made according to the POK licence. You CANNOT use this file or a part\n";
	print MYFILE "% of a file for your own project.\n";
	print MYFILE "%\n";
	print MYFILE "% For more information on the POK licence, please see our LICENCE FILE\n";
	print MYFILE "%\n";
	print MYFILE "% Please follow the coding guidelines described in doc/CODING_GUIDELINES\n";
	print MYFILE "%\n";
	print MYFILE "%                                      Copyright (c) 2007-$year POK team \n";
	print MYFILE "%\n";
	print MYFILE "% Created by $user on $date \n";
	print MYFILE "%\n\n\n";

	foreach $v (@contents)
	{
		print MYFILE $v;
	}

	close (MYFILE);
}


sub treat_latex_file
{
	my $filename = shift;
	if (has_header ($filename) == 0)
	{
		add_latex_header ($filename);
	}
}


#####################################################
# TEXT FUNCTIONS
#####################################################
sub treat_txt_file
{
	my $filename = shift;
	# Nothing to add at this moment
}


#####################################################
# PERL FUNCTIONS
#####################################################
sub add_perl_header
{
	my $filename = shift;
	my @contents;
	my $v;
	return if ! -f $filename;

	@contents = get_file_content ($filename);
	open (MYFILE, ">$filename");
	seek (MYFILE,0,0);
	print MYFILE $contents[0];
	print MYFILE "#                               POK header\n";
	print MYFILE "# \n";
	print MYFILE "# The following file is a part of the POK project. Any modification should\n";
	print MYFILE "# be made according to the POK licence. You CANNOT use this file or a part \n";
	print MYFILE "# of a file for your own project.\n";
	print MYFILE "#\n";
	print MYFILE "# For more information on the POK licence, please see our LICENCE FILE\n";
	print MYFILE "#\n";
	print MYFILE "# Please follow the coding guidelines described in doc/CODING_GUIDELINES\n";
	print MYFILE "#\n";
	print MYFILE "#                                      Copyright (c) 2007-$year POK team \n";
	print MYFILE "#\n";
	print MYFILE "# Created by $user on $date \n";
	print MYFILE "#\n\n\n";

	foreach $v (@contents)
	{
		print MYFILE $v;
	}
	close (MYFILE);
}

sub treat_perl_file
{
	my $filename = shift;
	if (has_header ($filename) == 0)
	{
		add_perl_header ($filename);
	}
}



#####################################################
# AADL FUNCTIONS
#####################################################
sub add_aadl_header
{
	my $filename = shift;
	my @contents;
	my $v;
	return if ! -f $filename;

	@contents = get_file_content ($filename);

	open (MYFILE, ">$filename");
	seek (MYFILE,0,0);
	print MYFILE "--\n";
	print MYFILE "--                              POK header\n";
	print MYFILE "-- \n";
	print MYFILE "--  The following file is a part of the POK project. Any modification should\n";
	print MYFILE "--  be made according to the POK licence. You CANNOT use this file or a part \n";
	print MYFILE "--  of a file for your own project.\n";
	print MYFILE "-- \n";
	print MYFILE "--  For more information on the POK licence, please see our LICENCE FILE\n";
	print MYFILE "--\n";
	print MYFILE "--  Please follow the coding guidelines described in doc/CODING_GUIDELINES\n";
	print MYFILE "--\n";
	print MYFILE "--                                      Copyright (c) 2007-$year POK team \n";
	print MYFILE "--\n";
	print MYFILE "--  Created by $user on $date \n";
	print MYFILE "--\n";

	foreach $v (@contents)
	{
		print MYFILE $v;
	}
	close (MYFILE);
}

sub remove_aadl_header
{
	my $filename = shift;
	my @contents;
	my $v;
	my $i;
	return if ! -f $filename;

	@contents = get_file_content ($filename);

	$i = 16;

	unlink ($filename) or die ("cannot remove $filename");
	open (MYFILE, ">$filename");
	seek (MYFILE,0,0);
	
	foreach $v (@contents)
	{
		if ($i == 0)
		{
			print MYFILE $v;
		}
		else
		{
			$i--;
		}
	}
	close (MYFILE);
}



sub treat_aadl_file
{
	my $filename = shift;

	if ((has_header ($filename) == 1) && ($force == 1))
	{
		remove_aadl_header ($filename);
	}

	if (has_header ($filename) == 0)
	{
		add_aadl_header ($filename);
	}

}


#####################################################
# MAIN ENTRYPOINT
#####################################################

$user = $ENV{'USER'};
$date = gmtime();
my @date_array = gmtime();
$year = $date_array[5] + 1900;
$force = 0;
$force = 1 if (defined ($ARGV[0]) && ($ARGV[0] eq '--force'));
treat_dir ($ENV{'PWD'});
