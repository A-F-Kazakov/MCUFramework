#ifndef IOPORTS_H
#define IOPORTS_H

#ifdef __AVR_ARCH__

#include <avr/io.h>

#if defined(PORTCFG_MPCMASK) //XMega family 
#include "XMegaPorts.h"
#else
#include "avr/avrports.h"
#endif

#elif !defined(__AVR_ARCH__)

#include <native_ports.h>
#include <iopin.h>

#endif

#endif // IOPORTS_H