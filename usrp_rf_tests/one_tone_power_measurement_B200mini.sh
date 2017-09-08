#!/bin/sh

for g_val in $(seq 20 1.0 89.0)
do 
   for a_val in $(seq 0.05 0.05 1.0)
   do 
      python one_tone_power_measurement_B200mini.py -f 0.5e9 -g $g_val -a $a_val

      # sleep to allow the previous spectrum analyzer session 
      # to end before starting a new session
      sleep 5s
   done 
done
