$prog = "perl vig.pl";
$testcase = shift;

open(TESTCASE, '<', "$testcase") or die $!;

@testArray = <TESTCASE>;
$input = shift @testArray;

while(@testArray) {
   $args = shift @testArray;
   $expectedOutput = shift @testArray;
   $output = `echo -n "$input" | $prog $args`;
   if ($expectedOutput ne $output) {
      print "\"$expectedOutput\" does not match \"$output\"" and exit 1;
   }
   $input = $output;
}

close(TESTCASE);
