#! /usr/bin/perl
use strict;
use warnings;
use Getopt::Std;

# copied from spec
sub _usage() {
   print "usage: $1 [ -v] [ -d] key [infile [ outfile ] ]\n";
   exit 1;
}

my %opts;
getopts('vs:p:', \%opts);

my $verbose = exists($opts{v}); # currently does nothing
my $skip = exists($opts{s}) ? $opts{s} : 0;
my $period = exists($opts{p}) ? $opts{p} : 0;

# validate options
$skip =~ m/^\d+$/ or print "Invalid skip value \"$skip\"\n" and exit 1;
$period =~ m/^\d+$/ or print "Invalid period value \"$period\"\n" and exit 1;

if (@ARGV < 1) {
   _usage();
}

my $key = shift;

# toUpper and verify alpha only
$key =~ tr/[a-z]/[A-Z]/;
$key =~ m/([^A-Z]+)/ 
   and print "key must be alphabetic only. String $1 not valid\n" and exit 1;
my $keyLength = length $key;

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

# process input, one line at a time
while <$inHandle> {
   tr/[a-z]/[A-Z]/; # convert line to uppercase
}

# chr = num -> char
# ord = char -> num

$outHandle eq *STDOUT or close($outHandle);
