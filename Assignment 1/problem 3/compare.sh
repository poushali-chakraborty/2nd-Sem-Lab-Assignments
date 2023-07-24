#!/bin/env bash
echo "Enter the string to search"
read str

echo "Egrep search"
start=`date +%N`

eval egrep $str p.txt d.txt 

end=`date +%N`
echo time to execute is `expr $end - $start ` ns




echo "Awk search"
start=`date +%N`

eval awk $str p.txt d.txt 

end=`date +%N`
echo time to execute is `expr $end - $start ` ns

echo "lex search => enter the string again"
eval ./a.out p.txt d.txt 
