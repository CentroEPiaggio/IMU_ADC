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
 *  \file       commands.h
 *
 *  \brief      Definitions for commands, parameters and packages.
 *  \date         February 01, 2018
 *  \author       _Centro "E.Piaggio"_
 *  \copyright    (C) 2012-2016 qbrobotics. All rights reserved.
 *  \copyright    (C) 2017-2018 Centro "E.Piaggio". All rights reserved.
 *
**/

#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED


//==============================================================================
//                                                                      COMMANDS
//==============================================================================


/** \name QB Move Commands
 * \{
**/

enum qbmove_command
{

//=========================================================     general commands

    CMD_PING                    = 0,    ///< Asks for a ping message
    CMD_STORE_PARAMS            = 3,    ///< Stores all parameters in memory and
                                        ///  loads them
    CMD_STORE_DEFAULT_PARAMS    = 4,    ///< Store current parameters as factory parameters
    CMD_RESTORE_PARAMS          = 5,    ///< Restore default factory parameters
    CMD_GET_INFO                = 6,    ///< Asks for a string of information about

    CMD_SET_VALUE               = 7,    ///< Not Used
    CMD_GET_VALUE               = 8,    ///< Not Used

    CMD_BOOTLOADER              = 9,    ///< Sets the bootloader modality to update the
                                        ///  firmware
    CMD_INIT_MEM                = 10,   ///< Initialize the memory with the defalut values

    CMD_GET_PARAM_LIST          = 12,   ///< Command to get the parameters list or to set
                                        ///  a defined value chosen by the use

//=========================================================     Other commands
    CMD_SET_BAUDRATE        = 139,   ///< Command for setting baudrate
                                    ///  of communication
    
    CMD_GET_N_IMU           = 160,  // Get number of IMUs connected to board
    CMD_GET_IMU_READINGS    = 161,  // Retrieve accelerometers, gyroscopes and magnetometers readings
    CMD_GET_IMU_PARAM       = 162,
    
    //CMD_GET_ADC_RAW         = 166
    CMD_GET_ADC_RAW         = 136    //this is the change between version 1.4 (official) and 1.4.1!!!
};

/** \} */

enum acknowledgment_values
{
    ACK_ERROR           = 0,
    ACK_OK              = 1
};

//==============================================    data types enumeration

enum data_types {
    TYPE_FLAG    = 0,
    TYPE_INT8    = 1,
    TYPE_UINT8   = 2,
    TYPE_INT16   = 3,
    TYPE_UINT16  = 4,
    TYPE_INT32   = 5,
    TYPE_UINT32  = 6,
    TYPE_FLOAT   = 7,
    TYPE_DOUBLE  = 8
};

#define PARAM_BYTE_SLOT     50      //Number of bytes reserved to a param information
#define PARAM_MENU_SLOT     150     //Number of bytes reserved to a param menu

/** \} */

//==============================================================================
//                                                                   INFORMATION
//==============================================================================
/** \name QB Move Information Strings */
/** \{ */
#define INFO_ALL        0 ///< All system information.
#define INFO_READING    1 ///< IMUs reading information.

/** \} */

// ----------------------------------------------------------------------------
#endif
/* [] END OF FILE */