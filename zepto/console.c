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
 * Implementation of the zepto-ish console API surface.
 */

#include <kernel.h>

// System headers used internally by the kernel
#include <stdarg.h>
#include <stdio.h>

static FP vconsole_before_output = NULL;
static FP vconsole_after_output = NULL;

void vconsole_stdoutf(char* fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	if (vconsole_before_output) { vconsole_before_output(); }
	vfprintf(stdout, fmt, argp);
	if (vconsole_after_output) { vconsole_after_output(); }
	va_end(argp);
}

void vconsole_stderrf(char* fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	if (vconsole_before_output) { vconsole_before_output(); }
	vfprintf(stderr, fmt, argp);
	if (vconsole_after_output) { vconsole_after_output(); }
	va_end(argp);
}

ER vconsole_reghooks(FP before, FP after) {
	if (vconsole_before_output != NULL || vconsole_after_output != NULL) {
		return E_OBJ;
	}

	vconsole_before_output = before;
	vconsole_after_output  = after;

	return E_OK;
}
