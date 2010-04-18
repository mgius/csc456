#! /bin/bash
# testing script for program 1

cat testing/input1 | perl vig.pl d | diff - testing/output1
perl vig.pl d testing/input1 | diff - testing/output1
perl vig.pl d testing/input1 | perl vig.pl -d d | diff - testing/input1U

cat testing/input1 | ./vig d | diff - testing/output1
./vig d testing/input1 | diff - testing/output1
./vig d testing/input1 | ./vig -d d | diff - testing/input1U

cat testing/input2 | perl vig.pl ham | diff - testing/output2
perl vig.pl ham testing/input2 | diff - testing/output2
perl vig.pl ham testing/input2 | perl vig.pl -d ham | diff - testing/input2U

cat testing/input2 | ./vig ham | diff - testing/output2
./vig ham testing/input2 | diff - testing/output2
./vig ham testing/input2 | ./vig -d ham | diff - testing/input2U
