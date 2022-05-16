# Pied-Piper-challange
Simba is an add-on to the Rover V2 Arduino-compatible robot. It uses a colour sensor and three microphones with integrated noise filters, to locate and navigate towards a sound source. On detecting a red light, it stops. 

Unique features 
  Gradual deceleration before stopping 1 cm from the target 
  360° detection of sound 
  Filters out sharp obscure noises like footsteps, distant conversation, etc. 
Optimal operational conditions: 
  Rigid surface or level ground 
  Silent indoor environment 
  Fully charged batteries 
  Within 1.1 metres of the sound source  

Adafruit TCS34725 Light & Colour Sensor: 

The Light and Colour Sensor is mounted at the front of the robot and is used to obtain a measurement of the colour temperature (K). The analogue measurement from the sensor determines the speed of the Rover V2’s motors, allowing it to slow to a halt in front of the target. 

Required installed software packages: 
  
  Adafruit TCS34725 
  Adafruit BusIO 

Grove Seeed Studio LM358 Amplified Sound Sensor: 

The 3 Seeed Studio microphones are mounted on the left, right and rear of the Rover V2. The left and right microphones are used determine the angle to the target, while the rear microphone determines whether the target is in front of or behind the Rover V2. 

The signal from the microphones is filtered through three deparated integrated passive high pass filter. The microphone array does not require any additional software packages to be installed. 
