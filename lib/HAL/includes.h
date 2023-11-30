#ifdef WINDOWS_TEST
#include "mock_avr_io.h"
#else
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pc_comm.h"
