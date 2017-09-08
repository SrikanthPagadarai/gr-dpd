#!/bin/sh

for g_val in $(seq 0 0.5 31.5)
do 
   for a_val in $(seq 0.05 0.05 0.75)
   do 
      python two_tone_power_measurement.py -f 0.5e9 -g $g_val -a $a_val

      # sleep to allow the previous spectrum analyzer session 
      # to end before starting a new session
      sleep 5s
   done 
done
