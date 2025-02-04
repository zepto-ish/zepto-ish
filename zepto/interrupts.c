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
 * Implementation of the zepto-ish interrupts API surface.
 */

#include <kernel.h>

// System headers used internally by the kernel
#include <signal.h>

extern void _kernel_tasks_reset();

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
		case   SIGIO: intno =   INTNO_IO; break;
	}
	if (handlers[intno]) {
		handlers[intno]->inthdr();
	}
	// Restarts processing tasks, this will likely have unclogged tasks.
	_kernel_tasks_reset();
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
			case   INTNO_IO: num =   SIGIO; break;
		}

		TRACE("Installing signal handler...");
		signal(num, z_inh_signal_handler);
	}

	handlers[inhno] = pk_dinh;

	return E_OK;
}
