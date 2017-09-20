#!/bin/sh

for g1_val in $(seq 9.0 1.0 10.0)
do 
   for g2_val in $(seq 15.0 1.0 15.0)
   do 
      python ofdm_TXpower_measurement_N210SBX.py -f 0.5e9 -c $g1_val -g $g2_val -a 60.0      

      # sleep to allow the previous spectrum analyzer session 
      # to end before starting a new session
      sleep 5s
   done 
done
