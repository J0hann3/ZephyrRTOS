#ifndef __SX126x_H__
#define __SX126x_H__


#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#define SX1261                          1
#define SX1262                          2

/*!
 * Radio complete Wake-up Time with margin for temperature compensation
 */
#define RADIO_WAKEUP_TIME               3  // [ms]

/*!
 * \brief Compensation delay for SetAutoTx/Rx functions in 15.625 microseconds
 */
#define AUTO_RX_TX_OFFSET               2

/*!
 * \brief LFSR initial value to compute IBM type CRC
 */
#define CRC_IBM_SEED                    0xFFFF

/*!
 * \brief LFSR initial value to compute CCIT type CRC
 */
#define CRC_CCITT_SEED                  0x1D0F

/*!
 * \brief Polynomial used to compute IBM CRC
 */
#define CRC_POLYNOMIAL_IBM              0x8005

/*!
 * \brief Polynomial used to compute CCIT CRC
 */
#define CRC_POLYNOMIAL_CCITT            0x1021

/*!
 * \brief The address of the register holding the first byte defining the CRC seed
 *
 */
#define REG_LR_CRCSEEDBASEADDR          0x06BC

/*!
 * \brief The address of the register holding the first byte defining the CRC polynomial
 */
#define REG_LR_CRCPOLYBASEADDR          0x06BE

/*!
 * \brief The address of the register holding the first byte defining the whitening seed
 */
#define REG_LR_WHITSEEDBASEADDR_MSB     0x06B8
#define REG_LR_WHITSEEDBASEADDR_LSB     0x06B9

/*!
 * \brief The address of the register holding the packet configuration
 */
#define REG_LR_PACKETPARAMS             0x0704

/*!
 * \brief The address of the register holding the payload size
 */
#define REG_LR_PAYLOADLENGTH            0x0702

/*!
 * \brief The address of the register holding the re-calculated number of symbols
 */
#define REG_LR_SYNCH_TIMEOUT            0x0706

/*!
 * \brief The addresses of the registers holding SyncWords values
 */
#define REG_LR_SYNCWORDBASEADDRESS      0x06C0

/*!
 * \brief The addresses of the register holding LoRa Modem SyncWord value
 */
#define REG_LR_SYNCWORD                 0x0740

/*!
 * Syncword for Private LoRa networks
 */
#define LORA_MAC_PRIVATE_SYNCWORD       0x1424

/*!
 * Syncword for Public LoRa networks
 */
#define LORA_MAC_PUBLIC_SYNCWORD        0x3444

/*!
 * The address of the register giving a 32-bit random number
 */
#define RANDOM_NUMBER_GENERATORBASEADDR 0x0819

/*!
 * The address of the register used to disable the LNA
 */
#define REG_ANA_LNA                     0x08E2

/*!
 * The address of the register used to disable the mixer
 */
#define REG_ANA_MIXER                   0x08E5

/*!
 * The address of the register holding RX Gain value (0x94: power saving, 0x96: rx boosted)
 */
#define REG_RX_GAIN                     0x08AC

/*!
 * Change the value on the device internal trimming capacitor
 */
#define REG_XTA_TRIM                    0x0911

/*!
 * Set the current max value in the over current protection
 */
#define REG_OCP                         0x08E7

/*!
 * \brief Maximum number of register that can be added to the retention list
 */
#define MAX_NB_REG_IN_RETENTION         4

/*!
 * \brief Base address of the register retention list
 */
#define REG_RETENTION_LIST_BASE_ADDRESS 0x029F

/*!
 * \brief WORKAROUND - Optimizing the Inverted IQ Operation, see DS_SX1261-2_V1.2 datasheet chapter 15.4
 */
#define REG_IQ_POLARITY                 0x0736

/*!
 * \brief WORKAROUND - Modulation Quality with 500 kHz LoRa Bandwidth, see DS_SX1261-2_V1.2 datasheet chapter 15.1
 */
#define REG_TX_MODULATION               0x0889

/*!
 * \brief WORKAROUND - Better resistance to antenna mismatch, see DS_SX1261-2_V1.2 datasheet chapter 15.2
 */
#define REG_TX_CLAMP_CFG                0x08D8

/*!
 * \brief RTC control
 */
#define REG_RTC_CTRL                    0x0902

/*!
 * \brief Event clear
 */
#define REG_EVT_CLR                     0x0944

  /*!
   * \brief Represents a sleep mode configuration
   */
  typedef union
  {
    struct
    {
      uint8_t WakeUpRTC : 1;  //!< Get out of sleep mode if wakeup signal received from RTC
      uint8_t Reset     : 1;
      uint8_t WarmStart : 1;
      uint8_t Reserved  : 5;
    } Fields;
    uint8_t Value;
  } SleepParams_t;

  typedef enum
  {
    MODE_SLEEP = 0x00,  //! The radio is in sleep mode
    MODE_STDBY_RC,      //! The radio is in standby mode with RC oscillator
    MODE_STDBY_XOSC,    //! The radio is in standby mode with XOSC oscillator
    MODE_FS,            //! The radio is in frequency synthesis mode
    MODE_TX,            //! The radio is in transmit mode
    MODE_RX,            //! The radio is in receive mode
    MODE_RX_DC,         //! The radio is in receive duty cycle mode
    MODE_CAD            //! The radio is in channel activity detection mode
  } RadioOperatingModes_t;

  typedef enum RadioCommands_e
  {
    RADIO_GET_STATUS                = 0xC0,
    RADIO_WRITE_REGISTER            = 0x0D,
    RADIO_READ_REGISTER             = 0x1D,
    RADIO_WRITE_BUFFER              = 0x0E,
    RADIO_READ_BUFFER               = 0x1E,
    RADIO_SET_SLEEP                 = 0x84,
    RADIO_SET_STANDBY               = 0x80,
    RADIO_SET_FS                    = 0xC1,
    RADIO_SET_TX                    = 0x83,
    RADIO_SET_RX                    = 0x82,
    RADIO_SET_RXDUTYCYCLE           = 0x94,
    RADIO_SET_CAD                   = 0xC5,
    RADIO_SET_TXCONTINUOUSWAVE      = 0xD1,
    RADIO_SET_TXCONTINUOUSPREAMBLE  = 0xD2,
    RADIO_SET_PACKETTYPE            = 0x8A,
    RADIO_GET_PACKETTYPE            = 0x11,
    RADIO_SET_RFFREQUENCY           = 0x86,
    RADIO_SET_TXPARAMS              = 0x8E,
    RADIO_SET_PACONFIG              = 0x95,
    RADIO_SET_CADPARAMS             = 0x88,
    RADIO_SET_BUFFERBASEADDRESS     = 0x8F,
    RADIO_SET_MODULATIONPARAMS      = 0x8B,
    RADIO_SET_PACKETPARAMS          = 0x8C,
    RADIO_GET_RXBUFFERSTATUS        = 0x13,
    RADIO_GET_PACKETSTATUS          = 0x14,
    RADIO_GET_RSSIINST              = 0x15,
    RADIO_GET_STATS                 = 0x10,
    RADIO_RESET_STATS               = 0x00,
    RADIO_CFG_DIOIRQ                = 0x08,
    RADIO_GET_IRQSTATUS             = 0x12,
    RADIO_CLR_IRQSTATUS             = 0x02,
    RADIO_CALIBRATE                 = 0x89,
    RADIO_CALIBRATEIMAGE            = 0x98,
    RADIO_SET_REGULATORMODE         = 0x96,
    RADIO_GET_ERROR                 = 0x17,
    RADIO_CLR_ERROR                 = 0x07,
    RADIO_SET_TCXOMODE              = 0x97,
    RADIO_SET_TXFALLBACKMODE        = 0x93,
    RADIO_SET_RFSWITCHMODE          = 0x9D,
    RADIO_SET_STOPRXTIMERONPREAMBLE = 0x9F,
    RADIO_SET_LORASYMBTIMEOUT       = 0xA0,
  } RadioCommands_t;

  /*!
   * Radio hardware and global parameters
   */
  // typedef struct SX126x_s
  // {
  //   Spi_t              Spi;
  //   PacketParams_t     PacketParams;
  //   PacketStatus_t     PacketStatus;
  //   ModulationParams_t ModulationParams;
  // } SX126x_t;

  /*!
   * ============================================================================
   * Public functions prototypes
   * ============================================================================
   */

  void SX126xSetSleep(SleepParams_t sleepConfig);

  #endif