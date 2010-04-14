#! /usr/bin/perl
use strict;
use warnings;
use Getopt::Std;
use POSIX;

# copied from spec
sub _usage() {
   print "usage: $0 [ -v] [ -s num] [ -p num] [infile [ outfile ] ]\n";
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

my $count;
my $curPeriod = 0;
my %table;
# initialize the table
for my $char ("A".."Z") { # can't believe this works
   $table{$char} = 0;
}
while (<$inHandle>) {
   tr/[a-z]/[A-Z]/; # map to uppercase
   foreach my $char (split(//, $_)) {
      if ($char =~ m/[A-Z]/) {
         $skip-- and next if $skip != 0;
         $curPeriod++ and next if $curPeriod != $skip;
         $curPeriod = 0;

         $table{$char}++;
         $count++;
      }
      else {
         next;
      }
   }
}


foreach my $key (sort(keys %table)) {
   my $hist = "";
   for (1..ceil($table{$key} / $count * 100)) {
      $hist .= "*";
   }
   print sprintf("%s: %9d ( %05.2f%%) %s\n", $key, $table{$key}, $table{$key} / $count * 100, $hist);
}
