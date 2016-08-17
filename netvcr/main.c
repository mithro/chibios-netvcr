/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

#include "printf.h"
#include "orchard.h"
#include "orchard-shell.h"

static uint8_t done_state = 0;
/* Triggered when done goes low, white LED stops flashing */
static void extcb1(EXTDriver *extp, expchannel_t channel) {
  (void)extp;
  (void)channel;

  done_state = 1;
}

static const EXTConfig extcfg = {
  {
   {EXT_CH_MODE_FALLING_EDGE | EXT_CH_MODE_AUTOSTART, extcb1, PORTA, 1}
  }
};

extern void programDumbRleFile(void);

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  orchardShellInit();

  chprintf(stream, "\r\n\r\nNeTVCR bootloader.  Based on build %s\r\n", gitversion);
  chprintf(stream, "Core free memory : %d bytes\r\n", chCoreGetStatusX());

  // IOPORT1 = PORTA, IOPORT2 = PORTB, etc...
  palClearPad(IOPORT1, 4);    // white LED, active low
  palSetPad(IOPORT3, 3);    // MCU_F_MODE, set to 1, specifies JTAG mode. Clear to 0 for SPI.
  palSetPad(IOPORT2, 0);    // FPGA_DRIVE, normally enable FPGA to drive the SPI bus
  //  palSetPad(IOPORT2, 1);    // MCU_F_INIT initially an input, don't stall configuration
  palSetPad(IOPORT3, 1);    // MCU_F_PROG, set PROG high, pulse low to reset the FPGA


  // pulse PROG now that the Mode pin has been setup
  chThdSleepMilliseconds(1);
  palClearPad(IOPORT3, 1);    // MCU_F_PROG, set PROG high, pulse low to reset the FPGA (min width = 250ns)
  chThdSleepMilliseconds(1);
  palSetPad(IOPORT3, 1);
  
  /*
   * Activates the EXT driver 1.
   */
  palSetPadMode(IOPORT3, 2, PAL_MODE_INPUT_PULLUP);  // FPGA done
  extStart(&EXTD1, &extcfg);

  programDumbRleFile();

#if 0
  update_html_file();

  //  usbd_init();
  //  usbd_connect(__TRUE);

  //  os_evt_wait_or(TRANSFER_FINISHED_SUCCESS | TRANSFER_FINISHED_FAIL, NO_TIMEOUT);

  //  os_dly_wait(200);
  
  //  usbd_connect(__FALSE);
#endif
  
  while (1) {
    chThdSleepMilliseconds(500);
    if( done_state )
      palTogglePad(IOPORT1, 4);
  }
}
