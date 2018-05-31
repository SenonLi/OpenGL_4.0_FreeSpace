# Create hello world in the terminal
# perl -e 'print "Hello World\n"'

# On Windows
# perl -e "print \"Hello World\n\""

use strict;
use warnings;
use diagnostics;

# We define the above (pragmas) to force us to write good code
# and to provide information on how to correct errors

# say prints a line followed by a newline
use feature 'say';

# Use a Perl version of switch called given when
use feature "switch";

# Use Perl version 5.16
use v5.24;

# I'm a comment
# Executes in the terminal when you type perltut1.pl
print "Hello World\n";

# Windows programmers should end there code with <STDIN>; so the
# command prompt stays open

# ---------- SCALARS ----------
# There are 3 data types in Perl Scalars, Arrays and Hashes

# Use the my function to declare a variable
# The Sigil $ says we are defining a scalar or single value
# The variable must start with a letter or _ and then numbers
# there after
# A variable receives the default value of undef

my $name = 'Sen';

# You can assign multiple values like this and scalars can
# contain strings or numbers

my ($age, $street) = (40, '4999 Saint-Catherine O suite 308');

# If you use " for strings you can include things like
# newlines with \n and variables
# Backslash quotes to use them in strings

my $my_info = "$name lives on \"$street\"\n";

# You can avoid escaping quotes with q{} for single quotes
# and qq{} for double quotes
$my_info = qq{$name lives on "$street"\n};

print $my_info;

# You can define a long string over multiple lines like this

my $selfie = <<"End_Of_Selfie";
\nThis is Sen's Selfie
\ton multiple lines, 
\tlearning Perl.
End_Of_Selfie

# say ends the line with a newline
say $selfie;
