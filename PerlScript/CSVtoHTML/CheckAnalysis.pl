#!/usr/bin/perl

use strict;
use warnings;

# --------------------------------------------------------------------------------------------------
# As we are samurai, honoring the bushido code, we will die with honor rather than fall into the
# hands of our enemies.
#
sub harakiri
{
  my $error = shift || '';

  if ($error) { print STDERR $error."\n"; }
  print "\n";
  print "Press <Enter> or <Return> to continue...\n";
  my $dummy = <STDIN>;
  exit ($error ? 1 : 0);
}

# --------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------

# Check that we have at least 1 argument (the file name)
if (scalar(@ARGV) < 1)
  {
    my $appname = $0;
    $appname =~ s/^.*[\/\\]//;
    harakiri("$appname analysis.csv [nominal] [tolerance]");
  }

my $nominal   = defined($ARGV[1]) ? (($ARGV[1]*1) || 0.000) : 0.000;
my $threshold = defined($ARGV[2]) ? (($ARGV[2]*1) || 0.000001) : 0.001;
my $errorpoints = 0;

open (FILE, '<'.$ARGV[0]) || harakiri("Could not open file: '".$ARGV[0]."'");
open (LOG, '>'.$ARGV[0].'.log') || harakiri("Could not open file: '".$ARGV[0].".log'");
print LOG 'Checking '.$ARGV[0].' for 3D deviations = '.$nominal.' +/-'.$threshold.".\n";
while(defined(my $line=<FILE>))
  {
    if ($line =~ /^#/) { next; }    # Skip the line if it's a comment
    if ($line !~ /[0-9]/) { next; } # Skip the line if is has no numbers
    
    $line=~s/[\n\r]//g;                 # Remove any CR or LF characters
    my @pointdata = split /, */, $line; # Split the fields of the line at the commas (',').

    # The analysis CSV format has 11 fields for each point.
    # - $pointdata[0]  : Point name
    # - $pointdata[1-3]: X Nominal, Y Nominal, Z Nominal,
    # - $pointdata[4-6]: X Measured, Y Measured, Z Measured,
    # - $pointdata[7-9]: X Deviation, Y Deviation, Z Deviation,
    # - $pointdata[10] : 3D Deviation

    if (scalar(@pointdata) != 11)
      {
	harakiri("Found ".scalar(@pointdata)." fields, but expected 11");
	exit -1;
      }
    
    if (abs($pointdata[10] - $nominal) >= $threshold)
      {
	print LOG $pointdata[0].': '.$pointdata[10]."\n";
	$errorpoints++;
      }
  }
close LOG;
close FILE;

# Delete the input and log files if there were no errors. This will fail
# silently if the file is still locked (open by Excel), but we don't care.
if (!$errorpoints)
  {
    unlink $ARGV[0];
    unlink $ARGV[0].'.log';
  }

# Exit and set the number of failed points in the error code.
exit $errorpoints;
