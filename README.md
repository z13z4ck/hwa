
HWA
===

HWA is a set of C-preprocessor definitions designed to help developers write
hardware-related code that:

 * is concise and easy to read,
 * produces efficient binary code,
 * simplifies the process of porting the code to a different target.

To achieve these goals, HWA provides:

 * a set of objects that represent the hardware,

 * generic instructions that accept various types and various numbers of
   mandatory and optionnal arguments, that put the focus on the expected result
   rather than on the values to be written in registers,

 * an error-checking mechanism that tries to produce messages that help the
   developer to solve the problems,

 * a transactional processing mechanism that allows further optimization of the
   machine code.

There is actually no penalty in using HWA, either in terms of size, execution
speed or memory used. Because it is not a library, using HWA does not affect
negatively the efficiency of the binary code produced as HWA helps the
compiler's optimizers produce a binary that is at least as efficient as if the
developer had written smart accesses to hardware registers himself.

Any C compiler compatible with the
[C11](https://en.wikipedia.org/wiki/C11_%28C_standard_revision%29) standard
should be able to compile the source code.

As the C-preprocessor can be used to parse assembler source, a few features of
HWA can be used for assembler programming. The implementation of a @ref
atmelavr_swuarta "software UART for Atmel AVR microcontrollers" (see
`atmel/avr/swuarta/`) and a @ref atmelavr_diabolo "bootloader" (see
`atmel/avr/examples/diabolo`) are examples of such a usage.

Here is an example of how to use a watchdog interrupt to make a LED blink with
an Atmel AVR ATtiny44A-PU:


    //  Load HWA definitions for the target device
    //
    #include <hwa/attiny44a_pu.h>


    //  We can use pin numbers as the device name tells HWA the package
    //
    #define PIN_LED      HW_PIN(7)


    //  Service watchdog IRQ
    //
    HW_ISR( watchdog0 )
    {
      //  Blink the LED
      //
      hw( toggle, PIN_LED );
    }


    int main ( )
    {
      //  Create a HWA context preloaded with RESET values to
      //  collect the hardware configuration
      //
      hwa_begin_from_reset();

      //  Configure the LED pin
      //
      hwa( configure, PIN_LED, direction, output );

      //  Configure the watchdog to trigger an IRQ every 250ms
      //
      hwa( configure, watchdog0,
           timeout,   250ms,
           action,    irq );

      //  Configure the core to enter idle mode when asked to sleep
      //
      hwa( configure,  core0,
           sleep,      enabled,
           sleep_mode, idle );

      //  Write this configuration into the hardware
      //
      hwa_commit();

      //  Enable interrupts
      //
      hw_enable_interrupts();

      //  Sleep between interrupts
      //
      for(;;)
        hw_sleep();

      return 0 ;
    }


Several other example projects are provided in the
`vendor/architecture/examples/` directories (e.g. `atmel/avr/examples/`).

Each `examples/` directory contains a `README.md` file that explains how to
compile the examples.


Documentation
=============

A ready-made documentation is available
[here](http://duparq.free.fr/hwa/index.html). Start with the @ref using page.

Building the documentation from sources requires
[Doxygen](http://www.stack.nl/~dimitri/doxygen/) and Gnu Make. Run `make` in the
HWA base directory and open the `doxygen/html/index.html` page.


Status
======

__WARNING!__ The development of HWA is very chaotic and this project is subject
to heavy changes of the code base.


Supported devices
=================

 * HWA supports almost fully Atmel AVR ATtinyX4, ATtinyX5, and ATmegaX8.

 * ST's STM32F103 is the device that initiated the writing of HWA. The code is
   being rewritten.

 * Support of ATmega32U4 and Espressif's ESP8266 has been started but needs to
   be rewritten.


Organization of the code
========================

The base source code of HWA is in the `hwa/` directory.

Device-related sources are stored in `vendor/architecture/` directories
(`atmel/avr/`, `st/stm32/`, `espressif/`, ...) where are stored `classes/`,
`devices/`, and `examples/` directories.


Hosting
=======

HWA is hosted on [Github](http://github.com/duparq/hwa).


Feedback
========

Feedbacks will be greatly appreciated. For any bug report, question or
suggestion, please use my gmail address (duparq) and put HWA in the object.


License
=======

HWA is free software. See the @ref license "license" page for license information.

<br>
