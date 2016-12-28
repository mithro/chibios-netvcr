# NeTV2 MCU Firmware

This repository contains the MCU firmware for the ARM control processor on the
[NeTV2 board](https://alphamaxmedia.com/w/index.php?title=NeTV2).

The main job of the MCU is to reset the FPGA and program the NOR flash.

The MCU is a MK20DX128VFM5 (
[ARM Cortex-M4 Kinetis K20
Microcontroller](http://www.nxp.com/products/microcontrollers-and-processors/arm-processors/kinetis-cortex-m-mcus/k-series-performance-m4/k2x-usb:K20_USB_MCU)).

The MCU is currently connected to;

 * 2-wire debug to the 0.1" header.
 * USB interface to the USB pins on the mPCIe header.
 * FPGA reset and status pins.
 * SPI NOR flash (via level shifter).
 * Serial UART connected to 0.1" header & FPGA pins.

See [NeTV2 schematic](http://bunniefoo.com/netv2/netv2-evt2-SCH.pdf) for the
full details.

## OS

The MCU runs [ChibiOS](http://chibios.org/).

## Compiling

### Ubuntu

Install the `gcc-arm-none-eabi` package. Set TRGT to `arm-none-eabi-` and then
type `make` in src.

```shell
apt-get install gcc-arm-none-eabi
export TRGT=arm-none-eabi-
cd src
make
```


## License

Custom code is under [FIXME Xobs - Put info here](???)
Chibios RT & NIL is under [GPL 3.0](https://www.gnu.org/licenses/gpl.html)
Chibios HAL is under [Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0)
