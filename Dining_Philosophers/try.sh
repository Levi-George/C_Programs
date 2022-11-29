#!/bin/bash
for i in 30 40 50 60 70 80
do 
  touch "results$i.txt"
  ./main $i | tee "results$i.txt"
done