# six_speed_display

This project makes use of two 3-axis accelerometers to determine the angle of a gear shift.
The shifter position is calculated based on the difference between the reference accelerometer ("ref," statically mounted inside the vehicle) and the shifter accelerometer ("shift," mounted to shifter). A reference is necessary to subtract the vehicle acceleration, and an offset is automatically calculated at boot to determine starting angles for both devices. 

Once a shift position has been determined, the MCU renders a character representation and sends it the display via SPI. 

The prototype hardware consists of a STM32G4 Nucleo board running at a clock speed of 168MHz. The accelerometers are ST-LIS3DH mounted to an Adafruit breakout board with supporting circuitry. The display is 320x240px based on the ST7789.
