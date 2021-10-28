#ifndef __FUNCTION_BLOCK_H
#define __FUNCTION_BLOCK_H

// ****************************************************************************************************
// *File Generated          mars-17-2021
// ****************************************************************************************************


// ****************************************************************************************************
// User define
// ****************************************************************************************************
#define    MASTER_ADDR           0x00    // Base ADDRESS
#define    BROADCAST_ADDR        0xFF    // Broadcast Address
#define    ERROR_MASK_BIT        0x7F    // Error Mask in the Function Code (bit 7)
#define    FC_ERROR_BIT			 0x07	//  function code error bit position
#define    DEFAULT_TX_LEN        0x02    // Default TX length (fc + addr + payload16 + crc16)
#define    DEVICE_TYPE_A         0x01    // Standard socket
#define    DEVICE_TYPE_B         0x02    // Socket with relay
#define    DEVICE_TYPE_C         0x03    // Socket metered
#define    DEVICE_TYPE_D         0x04    // Socket metered with relay
#define    DEVICE_TYPE_IMM_1L    0x05    // IMM single lines with 3 branch current
#define    DEVICE_TYPE_IMM_3L    0x06    // IMM three line with 6 branch current

// ****************************************************************************************************
// Function code define
// ****************************************************************************************************
#define    FC_REQUEST_ADDR       0x01    // Enumeration: (start if CTRL = 0) (Device is the MASTER in this case !)
#define    FC_RESET              0x02    // Reset command (enumeration restart after if ctrl line = 0)
#define    FC_SET_CTRL           0x03    // SET CTRL line (must be send at end of enumeration by the Base
#define    FC_HELLO              0x04    // Hello function
#define    FC_FW_VERSION         0x05    // Return Firmware Version
#define    FC_STATUS             0x06    // Status and configuration indicators

#define    FC_RMS_VOLTS_L1       0x07    // Line 1 RMS voltage reading in 0.01 V
#define    FC_RMS_AMPS_L1        0x08    // Line 1 RMS current reading in mA
#define    FC_PEAK_AMPS_L1       0x09    // Line 1 peak current reading in mA
#define    FC_WATTS_L1           0x0A    // Line 1 active power reading in mW
#define    FC_VAR_L1             0x0B    // Line 1 reactive power reading in mVAR
#define    FC_VA_L1              0x0C    // Line 1 apparent power reading in mVA
#define    FC_WH_L1              0x0D    // Line 1 active energy reading in mWh

#define    FC_RMS_VOLTS_L2       0x0E    // Line 2 RMS voltage reading in 0.01 V
#define    FC_RMS_AMPS_L2        0x0F    // Line 2 RMS current reading in mA
#define    FC_PEAK_AMPS_L2       0x10    // Line 2 peak current reading in mA
#define    FC_WATTS_L2           0x11    // Line 2 active power reading in mW
#define    FC_VAR_L2             0x12    // Line 2 reactive power reading in mVAR
#define    FC_VA_L2              0x13    // Line 2 apparent power reading in mVA
#define    FC_WH_L2              0x14    // Line 2 active energy reading in mWh

#define    FC_RMS_VOLTS_L3       0x15    // Line 3 RMS voltage reading in 0.01 V
#define    FC_RMS_AMPS_L3        0x16    // Line 3 RMS current reading in mA
#define    FC_PEAK_AMPS_L3       0x17    // Line 3 peak current reading in mA
#define    FC_WATTS_L3           0x18    // Line 3 active power reading in mW
#define    FC_VAR_L3             0x19    // Line 3 reactive power reading in mVAR
#define    FC_VA_L3              0x1A    // Line 3 apparent power reading in mVA
#define    FC_WH_L3              0x1B    // Line 3 active energy reading in mWh

#define    FC_FREQ               0x1C    // Line 3 reactive power reading in mVAR
#define    FC_PHASE_12           0x1D    // Line 3 apparent power reading in mVA
#define    FC_PHASE_13           0x1E    // Line 3 active energy reading in mWh

#define    FC_RMS_AMPS_B1        0x1F    // Branch 1 RMS current reading in mA
#define    FC_PEAK_AMPS_B1       0x20    // Branch 1 PEAK current reading in mA
#define    FC_RMS_AMPS_B2        0x21    // Branch 2 RMS current reading in mA
#define    FC_PEAK_AMPS_B2       0x22    // Branch 2 PEAK current reading in mA
#define    FC_RMS_AMPS_B3        0x23    // Branch 3 RMS current reading in mA
#define    FC_PEAK_AMPS_B3       0x24    // Branch 3 PEAK current reading in mA
#define    FC_RMS_AMPS_B4        0x25    // Branch 4 RMS current reading in mA
#define    FC_PEAK_AMPS_B4       0x26    // Branch 4 PEAK current reading in mA
#define    FC_RMS_AMPS_B5        0x27    // Branch 5 RMS current reading in mA
#define    FC_PEAK_AMPS_B5       0x28    // Branch 5 PEAK current reading in mA
#define    FC_RMS_AMPS_B6        0x29    // Branch 6 RMS current reading in mA
#define    FC_PEAK_AMPS_B6       0x2A    // Branch 6 PEAK current reading in mA

#define    FC_RMS_AMPS_N         0x2B    // Neutral RMS current reading in mA
#define    FC_PEAK_AMPS_N        0x2C    // Neutral PEAK current reading in mA
#define    FC_READ_IMM           0x2D    // Read all value from the IMM
#define    FC_READ_TEMP          0x2E    // Read all value from the IMM

#define    FC_RELAY_READ         0x2F    // Read relay state
#define    FC_RELAY_WRITE        0x30    // Write relay
#define    FC_SKT_FREQ           0x31    // Period or Frequency for the connected socket
#define    FC_READ_ETO           0x32    // ETO for BASE <-> IMM
#define    FC_READ_XML_ETO       0x33    // XML COMPRESSED ETO (Raritan Format)
#define    FC_READ_EERAM         0x34    // Read EERAM page (256 byte ?)
#define    FC_WRITE_EERAM        0x35    // Write EERAM page (256 Bytes ?)

#define    FC_READ_ROT           0x64    // Read Relay Operating Time Counter
#define    FC_MEAS_ROT           0x65    // Measure Relay Operating Time
#define    FC_SROT_TO_FLASH      0x66    // Store Relay Operating Time to Flash Memory
#define    FC_RROT_FROM_FLASH    0x67    // Restore Relay Operating Time from Flash Memory

#define    FC_CALIBRATION        0XA0    // Calibration Rn8302 or Rn8209

// ****************************************************************************************************
// Length define
// ****************************************************************************************************
#define    LEN_REQUEST_ADDR      0x02
#define    LEN_SET_CTRL          0x02
#define    LEN_RESET             0x02
#define    LEN_HELLO             0x02
#define    LEN_FW_VERSION        0x02
#define    LEN_STATUS            0x02
#define    LEN_READ_RN           0x05

#define    LEN_RMS_VOLTS_L1      0x02
#define    LEN_RMS_AMPS_L1       0x04
#define    LEN_PEAK_AMPS_L1      0x04
#define    LEN_WATTS_L1          0x04
#define    LEN_VAR_L1            0x04
#define    LEN_VA_L1             0x04
#define    LEN_WH_L1             0x04

#define    LEN_RMS_VOLTS_L2      0x02
#define    LEN_RMS_AMPS_L2       0x04
#define    LEN_PEAK_AMPS_L2      0x04
#define    LEN_WATTS_L2          0x04
#define    LEN_VAR_L2            0x04
#define    LEN_VA_L2             0x04
#define    LEN_WH_L2             0x04

#define    LEN_RMS_VOLTS_L3      0x02
#define    LEN_RMS_AMPS_L3       0x04
#define    LEN_PEAK_AMPS_L3      0x04
#define    LEN_WATTS_L3          0x04
#define    LEN_VAR_L3            0x04
#define    LEN_VA_L3             0x04
#define    LEN_WH_L3             0x04

#define    LEN_RMS_AMPS_B1       0x04
#define    LEN_PEAK_AMPS_B1      0x04
#define    LEN_RMS_AMPS_B2       0x04
#define    LEN_PEAK_AMPS_B2      0x04
#define    LEN_RMS_AMPS_B3       0x04
#define    LEN_PEAK_AMPS_B3      0x04
#define    LEN_RMS_AMPS_B4       0x04
#define    LEN_PEAK_AMPS_B4      0x04
#define    LEN_RMS_AMPS_B5       0x04
#define    LEN_PEAK_AMPS_B5      0x04
#define    LEN_RMS_AMPS_B6       0x04
#define    LEN_PEAK_AMPS_B6      0x04

#define    LEN_RMS_AMPS_N        0x04
#define    LEN_PEAK_AMPS_N       0x04
#define    LEN_READ_IMM_3        0x8E
#define    LEN_READ_TEMP         0x02
#define    LEN_RELAY_READ        0x02
#define    LEN_RELAY_WRITE       0x02
#define    LEN_SKT_FREQ          0x02
#define    LEN_READ_ETO          0x10
#define    LEN_READ_XML_ETO      0x100
#define    LEN_READ_EERAM        0x100
#define    LEN_WRITE_EERAM       0x100
#define    LEN_READ_ROT          0x02
#define    LEN_MEAS_ROT          0x02
#define    LEN_SROT_TO_FLASH     0x02
#define    LEN_RROT_FROM_FLASH   0x02

#define    LEN_LINE_1            1
#define    LEN_LINE_3            3
#define    LEN_LINE_6            6
#define    LEN_TOTAL_REGISTER_1  26//(2 + 4 * 6)
#define    LEN_BRANCH_REGISTER_1 8//






#endif
