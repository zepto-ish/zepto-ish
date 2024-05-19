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
 * Entrypoint of the simulated environment.
 * Does not use µTRON types.
 */

#include <kernel.h>

#include <ucontext.h>
#include <unistd.h>

/**
 * Sample task function from page 80
 */
#define task_ID 1
void task(VP_INT exinf) {
	/* Body of the task */
	ext_tsk();
}

int main(int argc, char** argv) {
	zepto_init();

	// This would be in the application-specific initialization normally...
	{
		DBG("Starting up...");
		DBG("... creating task");

		ER_ID ret;

		T_CTSK task_info = {0};
		task_info.task = (FP)task;

		ret = cre_tsk(task_ID, &task_info);
		if (ret < 0) {
			PANIC("Could not create task...", ret);
		}
		iact_tsk(task_ID);
	}

	zepto_run();
}
