#! /bin/bash
# testing script for program 1

cat testing/input1 | perl vig.pl d | diff - testing/output1
perl vig.pl d testing/input1 | diff - testing/output1
perl vig.pl d testing/input1 | perl vig.pl -d d | diff - testing/input1U

cat testing/input2 | perl vig.pl ham | diff - testing/output2
perl vig.pl ham testing/input2 | diff - testing/output2
perl vig.pl ham testing/input2 | perl vig.pl -d ham | diff - testing/input2U
