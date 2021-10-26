#ifndef __Function_Code_Definition__H
#define __Function_Code_Definition__H


// ****************************************************************************************************
// * File Version 2 :       octobre-22-2021
// ****************************************************************************************************


// ****************************************************************************************************
// User define
// ****************************************************************************************************
#define    MASTER_ADDR             0    // Base ADDRESS
#define    BROADCAST_ADDR          255    // Broadcast Address
#define    ERROR_MASK_BIT          127    // Error Mask in the Function Code (bit 7)
#define    FC_ERROR_BIT            7    // error bit position for function code
#define    DEFAULT_TX_LEN          2    // default length for TX payload
#define    DEVICE_TYPE_A           1    // Standard Socket
#define    DEVICE_TYPE_B           2    // Socket with Relay
#define    DEVICE_TYPE_C           3    // Socket Metered
#define    DEVICE_TYPE_D           4    // Socket Metered with Relay
#define    DEVICE_TYPE_IMM_1L      5    // IMM single line with 3 branch current
#define    DEVICE_TYPE_IMM_3L      6    // IMM three lines with 6 branch current
#define    DEVICE_TYPE_IMM_3L_N    7    // IMM three lines with 6 branch current + neutral


// ****************************************************************************************************
// Function code define
// ****************************************************************************************************
#define    FC_REQUEST_ADDR     1    // Enumeration
#define    FC_RESET            2    // Reset command (enumeration from device restart  if ctrl line = 0)
#define    FC_SET_CTRL         3    // SET CTRL line (must be send at end of enumeration by the Base)
#define    FC_HELLO            4    // Hello function
#define    FC_FW_VERSION       5    // Return Firmware Version
#define    FC_SERIAL_NUMBER    6    // Return serial number stored in Flash Memory during Production
#define    FC_STATUS           21    // Status and configuration indicators
#define    FC_RMS_VOLTS        22    // Line RMS voltage reading in 0.01 V
#define    FC_RMS_AMPS         23    // Line or Neutral RMS current reading in mA
#define    FC_PEAK_AMPS        24    // Line or Neutral  peak current reading in mA
#define    FC_WATTS            25    // Line active power reading in mW
#define    FC_VAR              26    // Line reactive power reading in mVAR
#define    FC_VA               27    // Line apparent power reading in mVA
#define    FC_WH               28    // Line active energy reading in mWh
#define    FC_FREQ             29    // Line frequency in 0.01 Hz from IMM
#define    FC_PHASE            30    // Line 2 or 3 phase angle relative to line 1 in 0.1 degrees
#define    FC_READ_DATA        31    // Read all value from IMM or Type-C or Type-D (Lenght depend from Type)
#define    FC_READ_DESC        41    // PDU Descriptor from EERAM (XTO Lenght, Serial N? Nb branch, Neutral, Nb sockets)
#define    FC_READ_EERAM       42    // Read EERAM page (256 bytes )
#define    FC_WRITE_EERAM      43    // Write EERAM page (256 Bytes ?)
#define    FC_READ_RELAY       61    // Relay Status (0 =open;  1= close)
#define    FC_WRITE_RELAY      62    // Set Relais
#define    FC_READ_TEMP        71    // Temperature in 0.1 degrees celcius from STM32 internal sensor (optionnal)
#define    FC_READ_ROT         91    // Read Relay Operating Time
#define    FC_MEAS_ROT         92    // Measure Relay Operating Time
#define    FC_SROT_TO_FLASH    93    // Store Relay Operating Time to Flash Memory
#define    FC_CALIBRATE        94    // Start Calibration for Metering chipset


// ****************************************************************************************************
// Length define
// ****************************************************************************************************
#define    LEN_REQUEST_ADDR          2
#define    LEN_RESET                 2
#define    LEN_SET_CTRL              2
#define    LEN_HELLO                 2
#define    LEN_FW_VERSION            2
#define    LEN_SERIAL_NUMBER         4
#define    LEN_STATUS                2
#define    LEN_RMS_VOLTS             2
#define    LEN_RMS_AMPS              4
#define    LEN_PEAK_AMPS             4
#define    LEN_WATTS                 4
#define    LEN_VAR                   4
#define    LEN_VA                    4
#define    LEN_WH                    4
#define    LEN_FREQ                  2
#define    LEN_PHASE                 2
#define    LEN_READ_DESC             25
#define    LEN_READ_EERAM            256
#define    LEN_WRITE_EERAM           2
#define    LEN_READ_RELAY            2
#define    LEN_WRITE_RELAY           2
#define    LEN_READ_TEMP             2
#define    LEN_READ_ROT              2
#define    LEN_MEAS_ROT              2
#define    LEN_SROT_TO_FLASH         2
#define    LEN_CALIBRATE             2
#define    LEN_READ_DATA_IMM_3L      142     // DEDICATED LENGTH FOR IMM_3L
#define    LEN_READ_DATA_IMM_1L      54     // DEDICATED LENGTH FOR IMM_1L
#define    LEN_READ_DATA_SOCKET_CD   28     // DEDICATED LENGTH FOR SOCKET_CD


#endif
