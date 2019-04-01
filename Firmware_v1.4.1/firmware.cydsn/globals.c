// ----------------------------------------------------------------------------
// BSD 3-Clause License

// Copyright (c) 2016, qbrobotics
// Copyright (c) 2017-2018, Centro "E.Piaggio"
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.

// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// POSSIBILITY OF SUCH DAMAGE.
// ----------------------------------------------------------------------------

/**
* \file         globals.c
*
* \brief        Global variables.
* \date         February 01, 2018
* \author       _Centro "E.Piaggio"_
* \copyright    (C) 2012-2016 qbrobotics. All rights reserved.
* \copyright    (C) 2017-2018 Centro "E.Piaggio". All rights reserved.
*/

//=================================================================     includes
#include <globals.h>

//=============================================      global variables definition

struct st_data  g_rx;                       // income data
struct st_mem   g_mem, c_mem;               // memory

// Timer value for debug field

uint32 timer_value;
uint32 timer_value0;

// Bit flag
CYBIT interrupt_flag;

// IMU variables
uint8 N_IMU_Connected;
uint8 IMU_connected[N_IMU_MAX];
int imus_data_size;
int single_imu_size[N_IMU_MAX];

struct st_imu g_imu[N_IMU_MAX];
struct st_imu g_imuNew[N_IMU_MAX];

uint8 Accel[N_IMU_MAX][6];
uint8 Gyro[N_IMU_MAX][6];
uint8 Mag[N_IMU_MAX][6];
uint8 MagCal[N_IMU_MAX][3];
uint8 Temp[N_IMU_MAX][2];
float Quat[4] = {1,0,0,0};

uint16 sensor_value_1, sensor_value_2;
int16 ADC_buf[4];                            /*! ADC measurements buffer */


/* END OF FILE */