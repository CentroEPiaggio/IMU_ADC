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
* \file         command_processing.c
*

* \brief        Command processing functions.
* \date         February 01, 2018
* \author       _Centro "E.Piaggio"_
* \copyright    (C) 2012-2016 qbrobotics. All rights reserved.
* \copyright    (C) 2017-2018 Centro "E.Piaggio". All rights reserved.
*/

//=================================================================     includes
#include "command_processing.h"
#include "interruptions.h"
#include "utils.h"
#include "globals.h"
#include "commands.h"
#include "IMU_functions.h"
#include <STDIO.H>

//================================================================     variables

reg8 * EEPROM_ADDR = (reg8 *) CYDEV_EE_BASE;

//==============================================================================
//                                                            RX DATA PROCESSING
//==============================================================================
//  This function checks for the availability of a data packet and process it:
//      - Verify checksum;
//      - Process commands;
//==============================================================================

void commProcess(){

    uint8 CYDATA rx_cmd;

    rx_cmd = g_rx.buffer[0];

//==========================================================     verify checksum

    if (!(LCRChecksum(g_rx.buffer, g_rx.length - 1) == g_rx.buffer[g_rx.length - 1])){
        // Wrong checksum
        g_rx.ready = 0;
        return;
    }


    switch(rx_cmd) {
       
//=========================================================     CMD_SET_BAUDRATE
            
        case CMD_SET_BAUDRATE:
            cmd_set_baudrate();
            break;  
            
//=============================================================     CMD_GET_INFO

        case CMD_GET_INFO:
            infoGet( *((uint16 *) &g_rx.buffer[1]));
            break;

//============================================================     CMD_GET_PARAM
            
        case CMD_GET_PARAM_LIST:
            get_param_list( *((uint16 *) &g_rx.buffer[1]));
            break;

//=================================================================     CMD_PING
            
        case CMD_PING:
            cmd_ping();
            break;

//=========================================================     CMD_STORE_PARAMS
            
        case CMD_STORE_PARAMS:
            cmd_store_params();
            break;

//=================================================     CMD_STORE_DEFAULT_PARAMS

        case CMD_STORE_DEFAULT_PARAMS:
            if(memStore(DEFAULT_EEPROM_DISPLACEMENT))
                sendAcknowledgment(ACK_OK);
            else
                sendAcknowledgment(ACK_ERROR);
            break;

//=======================================================     CMD_RESTORE_PARAMS

        case CMD_RESTORE_PARAMS:
            if(memRestore())
                sendAcknowledgment(ACK_OK);
            else
                sendAcknowledgment(ACK_ERROR);
            break;

//=============================================================     CMD_INIT_MEM

        case CMD_INIT_MEM:
            if(memInit())
                sendAcknowledgment(ACK_OK);
            else
                sendAcknowledgment(ACK_ERROR);
            break;

//===========================================================     CMD_BOOTLOADER

        case CMD_BOOTLOADER:
            //Not sure if ACK_OK is correct, should check
            sendAcknowledgment(ACK_OK);
            CyDelay(1000);
            FTDI_ENABLE_REG_Write(0x00);
            CyDelay(1000);
            Bootloadable_Load();
            break;

//=====================================================     CMD_GET_IMU_READINGS

        case CMD_GET_IMU_READINGS:
            cmd_get_imu_readings();
            break;                

//=====================================================     CMD_GET_SENSORS

        case CMD_GET_SENSORS:
            cmd_get_sensors();
            break;                
            
//=========================================================== ALL OTHER COMMANDS
        default:
            break;
            
    }
}


//==============================================================================
//                                                                     INFO SEND
//==============================================================================

void infoSend(){
    unsigned char packet_string[1100];
    infoPrepare(packet_string);
    UART_RS485_PutString(packet_string);
}


//==============================================================================
//                                                              COMMAND GET INFO
//==============================================================================

void infoGet(uint16 info_type) {
    static unsigned char packet_string[1100];

    //==================================     choose info type and prepare string

    switch (info_type) {
        case INFO_ALL:
            infoPrepare(packet_string);
            UART_RS485_PutString(packet_string);  
            break;
        case INFO_READING:                        
            infoReading(packet_string);
            UART_RS485_PutString(packet_string);
            break;
        default:
            break;
    }
}

//==============================================================================
//                                                            GET PARAMETER LIST
//==============================================================================

void get_param_list(uint16 index)
{
    //Package to be sent variables
    uint8 packet_data[  PARAM_BYTE_SLOT   +     // Number of connected IMUs
                        6*PARAM_BYTE_SLOT +     // IMUs ID
                        9*PARAM_BYTE_SLOT +     // Mag cal parameters
                        PARAM_BYTE_SLOT   +     // 1 - Device ID
                        PARAM_BYTE_SLOT*N_IMU_MAX + // IMU configurations
                        PARAM_BYTE_SLOT   +     // SPI read delay
                        PARAM_MENU_SLOT   + PARAM_BYTE_SLOT + 1 ] = "";
    uint16 num_imus_id_params = 6;
    uint16 num_mag_cal_params = 0;
    uint16 first_imu_parameter = 2;
    uint16 packet_length = PARAM_BYTE_SLOT +
                           num_imus_id_params*PARAM_BYTE_SLOT +
                           num_mag_cal_params*PARAM_BYTE_SLOT +
                           PARAM_BYTE_SLOT   + 
                           (uint16)(PARAM_BYTE_SLOT*N_IMU_Connected) +
                           PARAM_BYTE_SLOT + PARAM_MENU_SLOT + PARAM_BYTE_SLOT + 1;

    //Auxiliary variables
    uint16 CYDATA i, j, k, h;
    uint16 start_byte = 0;

    //Parameters menu string definitions
    char n_imu_str[26]          = "Number of connected IMUs:";
    char ids_str[11]            = "";
    char mag_param_str[20]      = "Mag cal parameters:";
    char id_str[16]             = "";
    char imu_table_str[42]      = "";
    char spi_read_delay_str[26] = "";
    
    //Strings lenghts
    uint8 CYDATA id_str_len = strlen(id_str);
    uint8 CYDATA n_imu_str_len = strlen(n_imu_str);
    uint8 CYDATA ids_str_len = strlen(ids_str);
    uint8 CYDATA mag_param_str_len = strlen(mag_param_str);
    uint8 CYDATA imu_table_str_len = strlen(imu_table_str);
    uint8 CYDATA spi_read_delay_str_len = strlen(spi_read_delay_str);
    
    char spi_delay_menu[118]    = "";
    uint8 CYDATA spi_delay_menu_len;
    
    sprintf(spi_delay_menu, "0 -> None\n1 -> Low (%u us delay for each 8-bit register read)\n2 -> High (%u us delay for each 8-bit register read)\n", (int)SPI_DELAY_LOW, (int)SPI_DELAY_HIGH);
    spi_delay_menu_len = strlen(spi_delay_menu);

    // Compute number of read parameters depending on N_IMU_Connected and
    // update packet_length
    num_mag_cal_params = (uint16)(N_IMU_Connected / 2);
    if ( (N_IMU_Connected - num_mag_cal_params*2) > 0 ) num_mag_cal_params++;
    
    packet_length = PARAM_BYTE_SLOT +
                    num_imus_id_params*PARAM_BYTE_SLOT +
                    num_mag_cal_params*PARAM_BYTE_SLOT +
                    PARAM_BYTE_SLOT   + 
                    (uint16)(PARAM_BYTE_SLOT*N_IMU_Connected) +
                    PARAM_MENU_SLOT + PARAM_BYTE_SLOT + 1;

    first_imu_parameter = 1 + num_imus_id_params + num_mag_cal_params + 2;
    packet_data[0] = CMD_GET_PARAM_LIST;
    packet_data[1] = 1 + num_imus_id_params + num_mag_cal_params + 1 + (uint8)N_IMU_Connected + 1;        // NUM_PARAMS

    switch(index) {
        case 0:         //List of all parameters with relative types
            /*-------------N IMU--------------*/
            start_byte = 0;
            packet_data[2] = TYPE_UINT8;
            packet_data[3] = 1;
            packet_data[4] = (uint8)N_IMU_Connected;
            for(i = n_imu_str_len; i != 0; i--)
                packet_data[5 + n_imu_str_len - i] = n_imu_str[n_imu_str_len - i];
                
            /*-------------IMUS ID--------------*/
            start_byte = start_byte + PARAM_BYTE_SLOT;
            i = 0;
            for (k = 0; k < num_imus_id_params; k++){
                sprintf(ids_str, "Port %u ID:", k);
                h = 4;
                ids_str_len = strlen(ids_str);
                packet_data[2+start_byte + PARAM_BYTE_SLOT*k] = TYPE_UINT8;
                packet_data[3+start_byte + PARAM_BYTE_SLOT*k] = 3;
                
                for (j = 3*k; j <= 3*k+2; j++) {  // for each possible imu on port k
                    if (IMU_connected[i] == j) {
                        packet_data[h+start_byte + PARAM_BYTE_SLOT*k] = (uint8)IMU_connected[i];               
                        i++;
                    } 
                    else {
                        packet_data[h+start_byte + PARAM_BYTE_SLOT*k] = 255;
                    }
                    h++;
                }

                //if (IMU_connected[i] >= 3*k && IMU_connected[i] <= 3*k + 2)
                for(j = ids_str_len; j != 0; j--)
                    packet_data[7+start_byte + PARAM_BYTE_SLOT*k + ids_str_len - j] = ids_str[ids_str_len - j];
            }
            
            /*-------------GET MAG PARAM--------------*/
            start_byte = start_byte + PARAM_BYTE_SLOT*num_imus_id_params;
            for (k = 0; k < num_mag_cal_params; k++){
                packet_data[2+start_byte + PARAM_BYTE_SLOT*k] = TYPE_UINT8;
                
                packet_data[3+start_byte + PARAM_BYTE_SLOT*k] = 3;
                packet_data[4+start_byte + PARAM_BYTE_SLOT*k] = (uint8) MagCal[IMU_connected[2*k]][0];
                packet_data[5+start_byte + PARAM_BYTE_SLOT*k] = (uint8) MagCal[IMU_connected[2*k]][1];
                packet_data[6+start_byte + PARAM_BYTE_SLOT*k] = (uint8) MagCal[IMU_connected[2*k]][2];
                
                // check if there is a second value
                if ( N_IMU_Connected < 2*(k+1) ) {
                    // there is only one value
                    for(j = mag_param_str_len; j != 0; j--)
                        packet_data[7+start_byte + PARAM_BYTE_SLOT*k + mag_param_str_len - j] = mag_param_str[mag_param_str_len - j];
                }
                else {
                    // fill the second value
                    packet_data[3+start_byte + PARAM_BYTE_SLOT*k] = 6;
                    packet_data[7+start_byte + PARAM_BYTE_SLOT*k] = (uint8) MagCal[IMU_connected[2*k+1]][0];
                    packet_data[8+start_byte + PARAM_BYTE_SLOT*k] = (uint8) MagCal[IMU_connected[2*k+1]][1];
                    packet_data[9+start_byte + PARAM_BYTE_SLOT*k] = (uint8) MagCal[IMU_connected[2*k+1]][2];
                
                    for(j = mag_param_str_len; j != 0; j--)
                        packet_data[10+start_byte + PARAM_BYTE_SLOT*k + mag_param_str_len - j] = mag_param_str[mag_param_str_len - j];
                }
            }
            
            /*-----------------ID-----------------*/
            
            start_byte = start_byte + PARAM_BYTE_SLOT*num_mag_cal_params;
            sprintf(id_str, "%u - Device ID:", first_imu_parameter-1);
            id_str_len = strlen(id_str);
            packet_data[2+start_byte] = TYPE_UINT8;
            packet_data[3+start_byte] = 1;
            packet_data[4+start_byte] = c_mem.id;
            for(i = id_str_len; i != 0; i--)
                packet_data[5+start_byte + id_str_len - i] = id_str[id_str_len - i];
                
            /*-------------GET IMUS MODE-------------*/
            
            start_byte = start_byte + PARAM_BYTE_SLOT;
            for (i = 0; i < (uint8)N_IMU_Connected; i++){
                sprintf(imu_table_str, "%u - IMU %d configuration:", first_imu_parameter + i, (int) IMU_connected[i]);
                imu_table_str_len = strlen(imu_table_str);
            
                packet_data[(uint16)(2 + start_byte + PARAM_BYTE_SLOT*i)] = TYPE_UINT8;
                packet_data[(uint16)(3 + start_byte + PARAM_BYTE_SLOT*i)] = 5;
                
                packet_data[(uint16)(4 + start_byte + PARAM_BYTE_SLOT*i)] = (uint8)(c_mem.IMU_conf[IMU_connected[i]][0]);
                packet_data[(uint16)(5 + start_byte + PARAM_BYTE_SLOT*i)] = (uint8)(c_mem.IMU_conf[IMU_connected[i]][1]);
                packet_data[(uint16)(6 + start_byte + PARAM_BYTE_SLOT*i)] = (uint8)(c_mem.IMU_conf[IMU_connected[i]][2]);
                packet_data[(uint16)(7 + start_byte + PARAM_BYTE_SLOT*i)] = (uint8)(c_mem.IMU_conf[IMU_connected[i]][3]);
                packet_data[(uint16)(8 + start_byte + PARAM_BYTE_SLOT*i)] = (uint8)(c_mem.IMU_conf[IMU_connected[i]][4]);

                for(j = imu_table_str_len; j != 0; j--)
                    packet_data[(uint16)(9 + start_byte + PARAM_BYTE_SLOT*i + imu_table_str_len - j)] = imu_table_str[imu_table_str_len - j];
            }  
          

            /*-----------------SPI DELAY-----------------*/
            
            start_byte = start_byte + (uint16)(PARAM_BYTE_SLOT*N_IMU_Connected) ;
            sprintf(spi_read_delay_str, "%u - SPI read delay:", first_imu_parameter+N_IMU_Connected);
            packet_data[2+start_byte] = TYPE_FLAG;
            packet_data[3+start_byte] = 1;
            packet_data[4+start_byte] = c_mem.SPI_read_delay;
            switch(c_mem.SPI_read_delay) {
                case 0: 
                    strcat(spi_read_delay_str, " None"); 
                    spi_read_delay_str_len = 26;
                    break;
                case 1: 
                    strcat(spi_read_delay_str, " Low"); 
                    spi_read_delay_str_len = 25;
                    break;
                case 2: 
                    strcat(spi_read_delay_str, " High");
                    spi_read_delay_str_len = 26;
                    break;
                default:
                    break;
            }            
            for(i = spi_read_delay_str_len; i != 0; i--)
                packet_data[5+start_byte + spi_read_delay_str_len - i] = spi_read_delay_str[spi_read_delay_str_len - i];
            //The following byte indicates the number of menus at the end of the packet to send
            packet_data[5+start_byte + spi_read_delay_str_len] = 1;
                  
            
            /*------------PARAMETERS MENU-----------*/
            start_byte = start_byte + PARAM_BYTE_SLOT;
            for(i = spi_delay_menu_len; i!= 0; i--)
                packet_data[(uint16)(2 + start_byte) + spi_delay_menu_len - i] = spi_delay_menu[spi_delay_menu_len - i];

            packet_data[packet_length - 1] = LCRChecksum(packet_data,packet_length - 1);
            commWrite(packet_data, packet_length);
            UART_RS485_ClearTxBuffer();
        break;

//=========================================================     other_params
        default: 
            
            if (index < first_imu_parameter-1)
                break;
            
            if (index == first_imu_parameter+N_IMU_Connected) {
                g_mem.SPI_read_delay = g_rx.buffer[3];  //SPI read delay - uint8
                break;
            }
            
            if (index == first_imu_parameter-1) {
                g_mem.id = g_rx.buffer[3];          //ID - uint8
            }
            else {
            
                //Set Imu table (index > = first_imu_parameter)
                g_mem.IMU_conf[IMU_connected[index-first_imu_parameter]][0] = g_rx.buffer[3];
                g_mem.IMU_conf[IMU_connected[index-first_imu_parameter]][1] = g_rx.buffer[4];
                g_mem.IMU_conf[IMU_connected[index-first_imu_parameter]][2] = g_rx.buffer[5];
                g_mem.IMU_conf[IMU_connected[index-first_imu_parameter]][3] = g_rx.buffer[6];
                g_mem.IMU_conf[IMU_connected[index-first_imu_parameter]][4] = g_rx.buffer[7];
                
                // Recompute IMU packets dimension
                imus_data_size = 1; //header    
                for (i = 0; i < N_IMU_Connected; i++)
                {
                    single_imu_size[IMU_connected[i]] = 1 + 6*g_mem.IMU_conf[IMU_connected[i]][0] + 6*g_mem.IMU_conf[IMU_connected[i]][1] + 6*g_mem.IMU_conf[IMU_connected[i]][2] + 16*g_mem.IMU_conf[IMU_connected[i]][3] + 2*g_mem.IMU_conf[IMU_connected[i]][4]+ 1;
                    imus_data_size = imus_data_size + single_imu_size[IMU_connected[i]];
                }
                imus_data_size = imus_data_size + 1;    //checksum
            
            }
            
        break;
    }
}


//==============================================================================
//                                                           PREPARE DEVICE INFO
//==============================================================================

void infoPrepare(unsigned char *info_string)
{
    int i;
    unsigned char str[100];
    if(c_mem.id != 250){                //To avoid dummy board ping
        strcpy(info_string, "");
        strcat(info_string, "\r\n");
        strcat(info_string, "Firmware version: ");
#ifdef SOMA
        strcat(info_string, SoMa);
#else        
        strcat(info_string, VERSION);
#endif        
        strcat(info_string, "\r\n\r\n");

        strcat(info_string, "DEVICE INFO\r\n");
        sprintf(str, "ID: %d\r\n", (int) c_mem.id);
        strcat(info_string, str);

#ifdef SOMA
        strcat(info_string, "Low pass filter frequency for accelerometers: 184 Hz\r\n");
#else
        strcat(info_string, "Low pass filter frequency for accelerometers: 10 Hz\r\n");
#endif

        sprintf(str, "IMU Connected: %d\r\n", (int) N_IMU_Connected);
        strcat(info_string, str);
        
        strcat(info_string, "\r\n");
        
        strcat(info_string, "IMUs CONFIGURATION\r\n");
        for (i=0; i<N_IMU_Connected; i++){
            sprintf(str, "Imu %d \r\n\tID: %d\r\n", i, (int) IMU_connected[i]);
            strcat(info_string, str);
            
            sprintf(str, "\tAccelerometers: ");
            if ((c_mem.IMU_conf[IMU_connected[i]][0]))
                strcat(str, "YES\r\n");
            else
                strcat(str, "NO\r\n"); 
            strcat(str, "\tGyroscopes: ");
            if ((c_mem.IMU_conf[IMU_connected[i]][1]))
                strcat(str, "YES\r\n");
            else
                strcat(str, "NO\r\n"); 
            strcat(str, "\tMagnetometers: ");
            if ((c_mem.IMU_conf[IMU_connected[i]][2]))
                strcat(str, "YES\r\n");
            else
                strcat(str, "NO\r\n");
            strcat(str, "\tQuaternion: ");                
            if ((c_mem.IMU_conf[IMU_connected[i]][3]))
                strcat(str, "YES\r\n");
            else
                strcat(str, "NO\r\n");
            strcat(str, "\tTemperature: ");
            if ((c_mem.IMU_conf[IMU_connected[i]][4]))
                strcat(str, "YES\r\n");
            else
                strcat(str, "NO\r\n");
                 
            strcat(info_string, str);
        }       
        
        strcat(info_string, "\r\n");
        
        sprintf(str, "Time between two reading cycles: %.3f ms", (float)(((uint32)timer_value0 - (uint32)timer_value)/1000.0));
        strcat(info_string, str);
        strcat(info_string, "\r\n");
        strcat(info_string, "\r\n");
        
        sprintf(str, "debug: %ld", (uint32)timer_value0 - (uint32)timer_value); //5000001
        strcat(info_string, str);
        strcat(info_string, "\r\n");
        strcat(info_string, "\r\n");
    }
}


void infoReading(unsigned char* info_string)
{
    int i;
    unsigned char str[100];
    
    strcpy(info_string, "");
    strcat(info_string, "\r\n");           
    strcat(info_string, "SENSORS INFO\r\n");
    for (i=0; i<N_IMU_Connected; i++){
        sprintf(str, "Imu %d \r\n\tID: %d\r\n", i, (int) IMU_connected[i]);
        strcat(info_string, str);
        
        if ((c_mem.IMU_conf[IMU_connected[i]][0])){
            sprintf(str, "\tAcc: %d\t%d\t%d\r\n", (int16) g_imu[i].accel_value[0], (int16) g_imu[i].accel_value[1],(int16) g_imu[i].accel_value[2]);
            strcat(info_string, str);
        }

        if ((c_mem.IMU_conf[IMU_connected[i]][1])){
            sprintf(str, "\tGyro: %d\t%d\t%d\r\n", (int16) g_imu[i].gyro_value[0], (int16) g_imu[i].gyro_value[1],(int16) g_imu[i].gyro_value[2]);
            strcat(info_string, str);
        }

        if ((c_mem.IMU_conf[IMU_connected[i]][2])){
            sprintf(str, "\tMag: %d\t%d\t%d\r\n", (int16) g_imu[i].mag_value[0], (int16) g_imu[i].mag_value[1],(int16) g_imu[i].mag_value[2]);
            strcat(info_string, str);
        }
        
        if ((c_mem.IMU_conf[IMU_connected[i]][3])){
            sprintf(str, "\tQuat: %.3f\t%.3f\t%.3f\t%.3f\r\n", (float) g_imu[i].quat_value[0], (float) g_imu[i].quat_value[1],(float) g_imu[i].quat_value[2], (float) g_imu[i].quat_value[3]);
            strcat(info_string, str);
        }
        
        if ((c_mem.IMU_conf[IMU_connected[i]][4])){
            sprintf(str, "\tTemperature: %d\r\n", (int16) g_imu[i].temp_value);
            strcat(info_string, str);
        }
    }
}
//==============================================================================
//                                                      WRITE FUNCTION FOR RS485
//==============================================================================

void commWrite(uint8 *packet_data, const uint16 packet_lenght)
{
    uint16 CYDATA index;

    // frame - start
    UART_RS485_PutChar(':');
    UART_RS485_PutChar(':');
    
    // frame - ID
    UART_RS485_PutChar(g_mem.id);
    
    // frame - length
    UART_RS485_PutChar((uint8)packet_lenght);
    
    // frame - packet data
    for(index = 0; index < packet_lenght; ++index)
        UART_RS485_PutChar(packet_data[index]);
    
    index = 0;

    while(!(UART_RS485_ReadTxStatus() & UART_RS485_TX_STS_COMPLETE) && index++ <= 1000){}

    RS485_CTS_Write(1);
    RS485_CTS_Write(0);
}


//==============================================================================
//                                                             CHECKSUM FUNCTION
//==============================================================================

// Performs a XOR byte by byte on the entire vector

uint8 LCRChecksum(uint8 *data_array, uint8 data_length) {
    
    uint8 CYDATA i;
    uint8 CYDATA checksum = 0x00;
    
    for(i = 0; i < data_length; ++i)
       checksum ^= data_array[i];
  
    return checksum;
}

//==============================================================================
//                                                       ACKNOWLEDGMENT FUNCTION
//==============================================================================

void sendAcknowledgment(const uint8 value) {
    int packet_lenght = 2;
    uint8 packet_data[2];

    packet_data[0] = value;
    packet_data[1] = value;

    commWrite(packet_data, packet_lenght);
}

//==============================================================================
//                                                                  STORE MEMORY
//==============================================================================

/**
* This function stores current memory settings on the eeprom with the specified
* displacement
**/

uint8 memStore(int displacement)
{
    int i;  // iterator
    uint8 writeStatus;
    int pages;
    uint8 ret_val = 1;

    // Disable Interrupt
    ISR_RS485_RX_Disable();


    // Retrieve temperature for better writing performance
    CySetTemp();

    memcpy( &c_mem, &g_mem, sizeof(g_mem) );

    pages = sizeof(g_mem) / 16 + (sizeof(g_mem) % 16 > 0);

    for(i = 0; i < pages; ++i) {
        writeStatus = EEPROM_Write(&g_mem.flag + 16 * i, i + displacement);
        if(writeStatus != CYRET_SUCCESS) {
            ret_val = 0;
            break;
        }
    }

    memcpy( &g_mem, &c_mem, sizeof(g_mem) );

    // Re-Enable Interrupt
    ISR_RS485_RX_Enable();

    return ret_val;
}


//==============================================================================
//                                                                 RECALL MEMORY
//==============================================================================

/**
* This function loads user settings from the eeprom.
**/

void memRecall()
{
    uint16 i;

    for (i = 0; i < sizeof(g_mem); i++) {
        ((reg8 *) &g_mem.flag)[i] = EEPROM_ADDR[i];
    }

    //check for initialization
    if (g_mem.flag == FALSE) {
        memRestore();
    } else {
        memcpy( &c_mem, &g_mem, sizeof(g_mem) );
    }
}


//==============================================================================
//                                                                RESTORE MEMORY
//==============================================================================

/**
* This function loads default settings from the eeprom.
**/

uint8 memRestore() {
    uint16 i;

    for (i = 0; i < sizeof(g_mem); i++) {
        ((reg8 *) &g_mem.flag)[i] = EEPROM_ADDR[i + (DEFAULT_EEPROM_DISPLACEMENT * 16)];
    }

    //check for initialization
    if (g_mem.flag == FALSE) {
        return memInit();
    } else {
        return memStore(0);
    }
}

//==============================================================================
//                                                                   MEMORY INIT
//==============================================================================

/**
* This function initialize memory when eeprom is compromised.
**/

uint8 memInit()
{
    uint8 i;
    //uint8 j;
    
    //initialize memory settings
    g_mem.id            = 1;

    
    // set the initialized flag to show EEPROM has been populated
    g_mem.flag = TRUE;
    
    // Default value
    for (i = 0; i< N_IMU_MAX; i++){
        g_mem.IMU_conf[i][0] = 1;
        g_mem.IMU_conf[i][1] = 1;
        g_mem.IMU_conf[i][2] = 0;
        g_mem.IMU_conf[i][3] = 0;
        g_mem.IMU_conf[i][4] = 0;
        //for (j=0; j< NUM_OF_DATA; j++) {
        //    g_mem.IMU_conf[i][j] = 0;
        //}
    }
    
    //write that configuration to EEPROM
    return ( memStore(0) && memStore(DEFAULT_EEPROM_DISPLACEMENT) );
}

//==============================================================================
//                                                    ROUTINE INTERRUPT FUNCTION
//==============================================================================
/**
* Bunch of functions used on request from UART communication
**/

void cmd_set_baudrate(){
    
    // Set BaudRate
    c_mem.baud_rate = g_rx.buffer[1];
    
    switch(g_rx.buffer[1]){
        case 13:
            CLOCK_UART_SetDividerValue(13);
            break;
        default:
            CLOCK_UART_SetDividerValue(3);
    }
}

void cmd_ping(){

    uint8 packet_data[2];

    // Header
    packet_data[0] = CMD_PING;
    
    // Load Payload
    packet_data[1] = CMD_PING;

    // Send Package to uart
    commWrite(packet_data, 2);
}

void cmd_store_params(){
   
    if(memStore(0))
        sendAcknowledgment(ACK_OK);
    else
        sendAcknowledgment(ACK_ERROR);
}

void cmd_get_imu_readings(){
    //Retrieve accelerometers, gyroscopes and magnetometers readings
    
    uint8 k_imu;
    uint8 c = 1;
    uint8 k = 0;
    uint8 gl_c = 1;
    
    // Packet: header + imu id(uint8) + imu flags(uint8) + crc  
    uint8 packet_data[350];
    uint8 single_packet[22];
    
    //Header package 
    packet_data[0] = CMD_GET_IMU_READINGS;
 
    for (k_imu = 0; k_imu < N_IMU_Connected; k_imu++) 
    {	
        
        single_packet[0] = (uint8) 0x3A; //':';
        if (c_mem.IMU_conf[IMU_connected[k_imu]][0]){
            *((int16 *) &single_packet[c]) = (int16) g_imu[k_imu].accel_value[0];
            *((int16 *) &single_packet[c+2]) = (int16) g_imu[k_imu].accel_value[1];
            *((int16 *) &single_packet[c+4]) = (int16) g_imu[k_imu].accel_value[2];
            c = c + 6;
        }
        if (c_mem.IMU_conf[IMU_connected[k_imu]][1]){
            *((int16 *) &single_packet[c])   = (int16) g_imu[k_imu].gyro_value[0];
            *((int16 *) &single_packet[c+2]) = (int16) g_imu[k_imu].gyro_value[1];
            *((int16 *) &single_packet[c+4]) = (int16) g_imu[k_imu].gyro_value[2];
            c = c + 6;
        }
        if (c_mem.IMU_conf[IMU_connected[k_imu]][2]){
            *((int16 *) &single_packet[c])   = (int16) g_imu[k_imu].mag_value[0];
            *((int16 *) &single_packet[c+2]) = (int16) g_imu[k_imu].mag_value[1];
            *((int16 *) &single_packet[c+4]) = (int16) g_imu[k_imu].mag_value[2];
            c = c + 6;
        }
        if (c_mem.IMU_conf[IMU_connected[k_imu]][3]){
            *((float *) &single_packet[c])   = (float) g_imu[k_imu].quat_value[0];
            *((float *) &single_packet[c+4]) = (float) g_imu[k_imu].quat_value[1];
            *((float *) &single_packet[c+8]) = (float) g_imu[k_imu].quat_value[2];
            *((float *) &single_packet[c+12]) = (float) g_imu[k_imu].quat_value[3];
            c = c + 16;
        }
        if (c_mem.IMU_conf[IMU_connected[k_imu]][4]){
            *((int16 *) &single_packet[c])   = (int16) g_imu[k_imu].temp_value;
            c = c + 2;
        }
        single_packet[single_imu_size[IMU_connected[k_imu]] - 1] = (uint8) 0x3A; //':';
        c = 1;
               
        for(k=0; k < single_imu_size[IMU_connected[k_imu]]; k++) {
            packet_data[gl_c + k] = (uint8) single_packet[k]; 
        }
        gl_c = gl_c + single_imu_size[IMU_connected[k_imu]];  
        
        memset(&single_packet, 0, sizeof(single_packet));     
        
    }

    // Calculate Checksum and send message to UART 
    packet_data[imus_data_size-1] = LCRChecksum (packet_data, imus_data_size-1);
    commWrite(packet_data, imus_data_size);
}

void cmd_get_sensors(){
    
    uint8 packet_data[6];

    // Header        
    packet_data[0] = CMD_GET_SENSORS;
    
    *((int16 *) &packet_data[1]) = (int16) sensor_value_1;
    *((int16 *) &packet_data[3]) = (int16) sensor_value_2;

    packet_data[5] = LCRChecksum (packet_data, 5);

    commWrite(packet_data, 6);

}


/* [] END OF FILE */