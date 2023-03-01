# Motor_Speed_Control_Software
## Description
- Motor speed control software capable of feeding the motors in feedback loop to have constant motor speed whatever the external factors (Robot load, ground slope).
- The output of the motor (speed) is measured by the encoder sensor that is implemented with the motor. The unit of measurement for the encoder sensor is the 'tick,' which reflects an angle. ( tick_angle == nb_total_ticks / 2Pi )
- Code is written in C++ and customized for Arduino Mega board, but it can be generalized for any other microcontroller.
- To run the code configure your Arduino toolchain environment to fit the Mega board than copy paste it inside your Arduino IDE.
##Hardawre requirements
- Arduino Mega board.
- Motor equipped with an encoder