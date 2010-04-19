#! /bin/bash

for testcase in `ls testcases`
do
   perl testRunner.pl testcases/$testcases
done
