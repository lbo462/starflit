This is the script that is meant to set the voltage value (=speed) in the register Max5434. 
We write the value in the volatile memory, then we save it to the non-volatile memory. 

For each reboot, the non-volatile memory gives to the volatile memory the voltage value (if we don't write into the volatile memory).

NB : We noted that the 2 motors does not respond the same way to the same speed value (one is slower trhan the other). However, this difference is random between each robot. 