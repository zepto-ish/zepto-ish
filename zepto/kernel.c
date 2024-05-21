/* SPDX-License-Identifier: GPL-3.0-only */
/*  ╔════════════════════════════════════════════════════╗
 *  ║                     __             ._      ._      ║
 *  ║ _______ ____ _____./  |_ ___       |_| ____| |__   ║
 *  ║ \___  / __  \\___ \_  __/ _ \  ____| |/  __| |  \  ║
 *  ║  / __/\  ___/| |_> |  || <_> |/___/| |\__ \|  Y  \ ║
 *  ║ /____ \\___ \|  __/|__| \___/      |_/___  |__|  / ║
 *  ║      \/    \/|_|                         \/    \/  ║
 *  ╚════════════════════════════════════════════════════╝
 */

/**
 * Implementation of the zepto-ish API surface.
 */

#include <kernel.h>

// System headers used internally by the kernel
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/******************
 *   INTERRUPTS   *
 ******************/

static T_DINH* handlers[Z_INH_MAX];

static void z_inh_signal_handler(int signal) {
	UINT intno = 0;
	// WARNING: compiler will not error on missing mappings
	switch (signal) {
		case SIGUSR1: intno = INTNO_USR1; break;
		case SIGUSR2: intno = INTNO_USR2; break;
		case SIGTERM: intno = INTNO_TERM; break;
		case SIGALRM: intno = INTNO_ALRM; break;
		case  SIGINT: intno =  INTNO_INT; break;
	}
	if (handlers[intno]) {
		handlers[intno]->inthdr();
	}
}

ER def_inh (INHNO inhno, T_DINH *pk_dinh) {
	TRACE("Defining interrupt handler #%d", inhno);

	// No interrupt handler attributes
	if (pk_dinh->inhatr != 0) {
		return E_RSATR;
	}

	// Only signals implemented for now
	if (inhno >= Z_INH_MAX) {
		return E_PAR;
	}

	if (inhno < Z_INH_SIGNAL_MAX) {
		UINT num = 0;

		switch ((INTNO)inhno) {
			case INTNO_USR1: num = SIGUSR1; break;
			case INTNO_USR2: num = SIGUSR2; break;
			case INTNO_TERM: num = SIGTERM; break;
			case INTNO_ALRM: num = SIGALRM; break;
			case  INTNO_INT: num =  SIGINT; break;
		}

		TRACE("Installing signal handler...");
		signal(num, z_inh_signal_handler);
	}

	handlers[inhno] = pk_dinh;

	return E_OK;
}

/***************************
 *   Vendor System Calls   *
 ***************************/

void vconsole_stdoutf(char* fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stdout, fmt, argp);
	va_end(argp);
}

void vconsole_stderrf(char* fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
}

void vprocess_exit(UB code) {
	exit(code);
}

/*********************************
 *   zepto-specific interfaces   *
 *********************************/

static BOOL zepto_init_done = false;


extern void _kernel_initialize_tasks();
extern void _kernel_schedule_tasks();

/**
 * To be called as early as possible to setup internals to zepto-ish.
 *
 * NOTE: this may be removed in the future.
 */
void zepto_init() {
	TRACE("Initializing zepto-ish...");

	_kernel_initialize_tasks();

	TRACE("Done initializing zepto-ish!\n")
	zepto_init_done = TRUE;
}

/**
 * To be called to launch the main loop.
 *
 * This function will not return.
 *
 * NOTE: this may be removed in the future.
 */
void zepto_run() {
	if (zepto_init_done != TRUE) {
		PANIC("zepto_run() called before zepto_init()!!", E_SYS);
	}
	TRACE("\n");
	TRACE("==================");
	TRACE("Tasks main loop...");
	TRACE("==================");

	_kernel_schedule_tasks();

	PANIC("Exited zepto_run() loop!!", E_SYS);
}

extern ER static_setup();

int main(int argc, char** argv) {
	// Do some initial housekeeping
	zepto_init();

	// TODO: figure out a more re-usable scheme
	static_setup();

	// Launch the main loop
	zepto_run();

	// This should not happen
	PANIC("End of main() reached!", E_SYS);

	return 0;
}
