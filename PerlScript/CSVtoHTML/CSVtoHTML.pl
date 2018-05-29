#!/usr/bin/perl

use strict;
use warnings;
use Time::Local;
use v5.24;

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

sub process_timings
{
	my $pathname = shift;
	my $alltimes = {};

	open (FILE, '<'.$pathname) || harakiri("Could not open file: '".$pathname."'");

	# Process the first line of the file to the the dates of the runs
	my $line = <FILE>;
	chomp $line; # remove \n at the end of $line
	$line =~ s/^"//; # Remove the leading quote
	$line =~ s/"$//; # Remove the trailing quote
	my @dates = split /"?,"?/, $line; # Split the fields at the commas and include the internal quotes
	shift @dates; # Remove the first column header "Process Name"
	shift @dates; # Remove the second column header "Command Name"
	shift @dates; # Remove the third column header "Call Index"

	while (defined($line=<FILE>))
	{
		chomp $line;
		$line =~ s/^"//; # Remove the leading quote
		$line =~ s/"$//; # Remove the trailing quote
		my @times = split /"?,"?/, $line; # Split the fields at the commas and include the internal quotes

		my $process = shift @times;
		my $command = shift @times;
		my $index   = shift @times;

		if (!$command) # Only check for the overall process timings, not individual commands/operations
		{
			$alltimes->{$process}->{'Minimum'} = 10000;
			$alltimes->{$process}->{'Maximum'} = 0.001;
			$alltimes->{$process}->{'Latest'}  = 0;
			$alltimes->{$process}->{'Number'}  = 0;
			$alltimes->{$process}->{'Average'} = 0;

			for (my $i=0; $i<@times; $i++)
			{
				if ($times[$i] && $times[$i] > 0.001)
				{
					$alltimes->{$process}->{$dates[$i]} = $times[$i];
					$alltimes->{$process}->{'Maximum'}  = $times[$i] if ($times[$i] > $alltimes->{$process}->{'Maximum'});
					$alltimes->{$process}->{'Minimum'}  = $times[$i] if ($times[$i] < $alltimes->{$process}->{'Minimum'});
					$alltimes->{$process}->{'Latest'}   = $times[$i];
					$alltimes->{$process}->{'Average'} += $times[$i];
					$alltimes->{$process}->{'Number'}++;
				}
			}

			if ($alltimes->{$process}->{'Number'})
			{
				$alltimes->{$process}->{'Average'} /= $alltimes->{$process}->{'Number'};
			}
		}
	}

	return (\@dates, $alltimes);
}

#-----------------------------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------

my $results = $ARGV[0] || 'AllData.csv'; # if $ARGV[0] is defined, $results = $ARGV[0]; else $results = 'AllData.csv'
my $output = $results;
$output =~ s/\.[a-z]+$/.html/i; # match replace .* to .html, case insensitive; $output = 'AllData.html'
# http://perldoc.perl.org/perlretut.html
#    "don't" =~ / .+? \b{wb} /x;  # matches the whole string

#  Here less than < sign indicates that file has to be opend in read-only mode.
# Here TEMPLATE is the file handle which will be used to read the file.
open (TEMPLATE, '<AllData.template') || harakiri('ERROR: Could not open file: AllData');
open (HTML, '>'.$output) || harakiri('ERROR: Could not open file: '.$output);
my $line;

# Prepare the header for the HTML file
while ($line=<TEMPLATE>)
{
	last if ($line =~ /INSERT DATA/); # read TEMPLATE and write into HTML util line contains "INSERT DATA"
	print HTML $line;
}

if ($line =~ /INSERT DATA/)
{
	my ($dates, $times) = process_timings($results);
	print "Found timings for ".scalar(keys(%$times))." processes.\n";

	foreach my $step (sort keys %$times)
	{
		# Do not report processes for which the difference between the max an min time is 2 seconds or less.
		if (($times->{$step}->{'Maximum'}-$times->{$step}->{'Minimum'}) > 2 || $times->{$step}->{'Number'} < 5)
		{
			my $name = $step;
			$name =~ s/\s*\S?monitor\S?//;

			print HTML '				"'.$name.'": {'."\n";
			print HTML '					label: "'.$name.'",'."\n";
			print HTML '					runs: '.$times->{$step}->{'Number'}.",\n";
			print HTML '					min: '.$times->{$step}->{'Minimum'}.",\n";
			print HTML '					max: '.$times->{$step}->{'Maximum'}.",\n";
			print HTML '					avg: '.$times->{$step}->{'Average'}.",\n";
			print HTML '					last: '.$times->{$step}->{'Latest'}.",\n";
			print HTML '					data: [';

			foreach my $date (sort @$dates)
			{
				if ($times->{$step}->{$date})
				{
					my ($year, $month, $day, $hours, $min) = ($date =~ /([0-9][0-9][0-9][0-9])-([0-9][0-9])-([0-9][0-9]) ([0-9][0-9]):([0-9][0-9])/);
					my $timestamp = timelocal(0, $min, $hours, $day, $month-1, $year)*1000;
					print HTML '['.$timestamp.', '.($times->{$step}->{$date}/$times->{$step}->{'Latest'}*100).'], ';
				}
			}
			print HTML ']'."\n";
			print HTML '				},'."\n";
		}
	}
}

# Write out the rest of the HTML template
while ($line=<TEMPLATE>) { print HTML $line; }

close HTML;
close TEMPLATE;
