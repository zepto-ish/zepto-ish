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
