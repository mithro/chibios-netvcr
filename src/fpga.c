#include "hal.h"

int fpgaProgrammed(void)
{

  return palReadPad(IOPORT3, 2);
}

void fpgaConnect(void)
{
  /* Ensure SPI is disconnected */
  palSetPadMode(IOPORT3, 4, PAL_MODE_INPUT);
  palSetPadMode(IOPORT3, 5, PAL_MODE_INPUT);
  palSetPadMode(IOPORT3, 6, PAL_MODE_INPUT);
  palSetPadMode(IOPORT3, 7, PAL_MODE_INPUT);

  /* Assert FPGA_DRIVE to move the SPI to the FPGA */
  palSetPad(IOPORT2, 0);
  chThdSleepMilliseconds(1);

  /* Set FPGA_INIT as an input, to allow it to float */
  palSetPadMode(IOPORT2, 1, PAL_MODE_INPUT);

  /* Put the FPGA into reset */
  palSetPadMode(IOPORT3, 1, PAL_MODE_OUTPUT_PUSHPULL);
  palClearPad(IOPORT3, 1); // MCU_F_PROG
  chThdSleepMicroseconds(50);
  palSetPad(IOPORT3, 1); // MCU_F_PROG

  while (!fpgaProgrammed())
    ;
  palClearPad(IOPORT1, 4); // LED on
}

void fpgaDisconnect(void)
{
  /* Deassert FPGA_DRIVE to move the SPI to the MCU */
  palClearPad(IOPORT2, 0);
  chThdSleepMilliseconds(1);
}