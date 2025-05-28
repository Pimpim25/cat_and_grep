#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
echo "" > log.txt

for var in -v -c -l -n -h -o
do
  for var2 in -v -c -l -n -h -o
  do
      for var3 in -v -c -l -n -h -o
      do
        if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
        then
          TEST1="for grep.c Makefile $var $var2 $var3"
          echo "$TEST1"
          ./grep $TEST1 > my_grep.txt
          grep $TEST1 > grep.txt
          DIFF_RES="$(diff -s my_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files my_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST1" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt

          TEST2="for grep.c $var $var2 $var3"
          echo "$TEST2"
          ./grep $TEST2 > my_grep.txt
          grep $TEST2 > grep.txt
          DIFF_RES="$(diff -s my_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files my_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST2" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm my_grep.txt grep.txt

          TEST3="-e for -e ^int grep.c Makefile $var $var2 $var3"
          echo "$TEST3"
          ./grep $TEST3 > my_grep.txt
          grep $TEST3 > grep.txt
          DIFF_RES="$(diff -s my_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files my_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST3" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm my_grep.txt grep.txt

          TEST4="-e for -e ^int grep.c $var $var2 $var3"
          echo "$TEST4"
          ./grep $TEST4 > my_grep.txt
          grep $TEST4 > grep.txt
          DIFF_RES="$(diff -s my_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files my_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST4" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt

          TEST5="-e regex -e ^print grep.c $var $var2 $var3 -f pattern.txt"
          echo "$TEST5"
          ./grep $TEST5 > s21_grep.txt
          grep $TEST5 > grep.txt
          DIFF_RES="$(diff -s my_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files my_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST5" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm my_grep.txt grep.txt

          TEST6="-e while -e void grep.c Makefile $var $var2 $var3 -f pattern.txt"
          echo "$TEST6"
          ./grep $TEST6 > my_grep.txt
          grep $TEST6 > grep.txt
          DIFF_RES="$(diff -s my_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files my_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST6" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm my_grep.txt grep.txt

        fi
      done
  done
done

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"
