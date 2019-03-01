/*==========================================================================
* IMU BOARD project
* Author: Mattia Poggiani
* Centro di Ricerca "E. Piaggio" - University of Pisa
* http://www.centropiaggio.unipi.it
* http://www.naturalmachinemotioninitiative.com/
* For support, please refer to: mattia.poggiani at centropiaggio.unipi.it
*==========================================================================/

IMU
===============
The repository is organized as follows:

1) Firmware

Here you can find the PSoC firmware to be programmed in the IMU board.
It handles the readings of up to 17 connected IMUs and allows the user to choose 
which sensors to read.
Firmware version 1.1 allows to choose between accelerometers, gyroscopes, 
magnetometers and temperature or a combination of them.

Just open the project with PSoC Creator and upload it onto the board


-------------------------------------------------------------------------------------
2) Management

Here are useful libraries to manage the IMU board.
In particular:
- qbAPI contains the API to communicate with the device through FTDI and RS-485 protocol.
- qbadmin contains softwares to interface with the board using the API.
- imuboard_simulink contains Simulink libraries.

Simulink usage:
Install imuboard_simulink libraries in addition to standard qbmove_simulink libraries 
by running 'install.m' file in Matlab (newer than version 2013a).
Use the provided Simulink schemes as an example on how to use the library.