# Automatic-tea-maker
Automatic tea maker using arduino

The tea maker contains a servomotor attached to an arm that introduces the tea in the cup, and takes out the tea bag when the time is up. Everything is implemented using TimerOne, the internal interrupts being more efficient. It is implemented like a finite state maching, each state being described by 3 boolean variables. The states are: introducing the tea bag, the waiting state, taking out the tea bag, waiting for the tea to have the optimum temperature (the tea maker also has a temperature sensor). Each stage is displayed on an I2C LCD.
