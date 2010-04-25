#! /usr/bin/perl
#  Mark Gius
#  Program 1: vig (in perl)
#  CSC456
#  Vigenere Enciphering / Deciphering
#
#  This version is not intended to be graded.  I was just mildly offended
#  that you didn't include perl as real language
#
#  Somewhat less readable than the C++ version, but strict/warnings 
#  makes it considerably more readable than your average perl script.
use strict;
use warnings;
use Getopt::Std;

# copied from spec
sub _usage() {
   print "usage: $0 [ -v] [ -d] key [infile [ outfile ] ]\n";
   exit 1;
}

# option parsing
my %opts;
getopts('vdp:', \%opts);

my $decode = exists($opts{d}) ? -1 : 1; # enables decryption
my $verbose = exists($opts{v}); # currently does nothing

# you have failed to provide a key
if (@ARGV < 1) {
   _usage();
}

my $key = shift;
# toUpper and verify alpha only
$key =~ tr/[a-z]/[A-Z]/;
$key =~ m/([^A-Z]+)/ 
   and print "key must be alphabetic only. String $1 not valid\n" and exit 1;

my @keyArray;
for my $magic (split(//, $key)) {
	push @keyArray, $decode * (ord($magic) - ord('A'));
}


# pull filenames for in and out, default use STDIN/STDOUT
my $inHandle = *STDIN;
if (@ARGV > 0) {
   my $inFilename = shift;
   open($inHandle, '<', "$inFilename") or die $!;
}

my $outHandle = *STDOUT;
if (@ARGV > 0) {
   my $filename = shift;
   open($outHandle, '>', "$filename") or die $!;
}

# converts a single character uses Key array like a circular queue
sub convert {
	my $shift = shift @keyArray;
   my $char = ord(shift);
   $char += $shift;
   if ($char > ord('Z')) { $char -= 26; }
	if ($char < ord('A')) { $char += 26; }
	push @keyArray, $shift;
   return chr($char);
}

# process input, one line at a time
while (<$inHandle>) {
   tr/[a-z]/[A-Z]/; # convert line to uppercase
   foreach my $char (split(//, $_)) {
      if ($char =~ m/[A-Z]/) {
         print convert($char);
      }
      else {
         print $outHandle, $char;
      }
   }
}

# cleanup :D
$inHandle eq *STDIN or close($inHandle)
$outHandle eq *STDOUT or close($outHandle);
