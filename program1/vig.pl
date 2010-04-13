#! /usr/bin/perl
use strict;
use warnings;
use Getopt::Std;

# copied from spec
sub _usage() {
   print "usage: $0 [ -v] [ -d] key [infile [ outfile ] ]\n";
   exit 1;
}

my %opts;
getopts('vds:p:', \%opts);

my $decode = exists($opts{d}) ? -1 : 1; # enables decryption
my $verbose = exists($opts{v}); # currently does nothing
my $skip = exists($opts{s}) ? $opts{s} : 0;
my $period = exists($opts{p}) ? $opts{p} : 0;

# validate options
$skip =~ m/^\d+$/ or print "Invalid skip value \"$skip\"\n" and exit 1;
$period =~ m/^\d+$/ or print "Invalid period value \"$period\"\n" and exit 1;

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

# converts a single character
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
         print $char;
      }
   }
}

# cleanup :D
$outHandle eq *STDOUT or close($outHandle);
