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

//
// WARNING: This task ***is not portable***.
//          It relies on a Linux host system to work.
//

#include <kernel.h>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char buf[1025];

static void sigio_handler(int sig) {
	int ret;
	memset(&buf, 0, 1025);
	ret = read(STDIN_FILENO, buf, 1024);
	if (ret < 0) {
		// Not our time to read...
		return;
	}
	DBG("SIGIO handler hit!");

	iwup_tsk(3);
}

/**
 * This task implements a terrible "pseudo-console" excercising SIGIO
 * as a method to wait on input.
 */
void task_3(VP_INT exinf) {
	DBG("Task #3 setting-up things...");
	if (signal(SIGIO, sigio_handler) == SIG_ERR) {
		PANIC("signal SIGIO", errno);
	}
	if (fcntl(STDIN_FILENO, F_SETOWN, getpid()) < 0) {
		PANIC("fcntl F_SETOWN error", 3);
	}
	if (fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK|O_ASYNC) < 0) {
		PANIC("fcntl error for O_NONBLOCK|O_ASYNC", 3);
	}
	DBG("Finished setting-up!");

	for (;;) {
		DBG("Task #3 sleeping...");
		slp_tsk();
		DBG("Resumed #3!");

		// Assume no input == ^D
		if (buf[0] == 0) {
			// And assume it means quitting
			strncpy(buf, "quit\n", 6);
		}
		// Strip newline
		char* newline_pos = strchr(buf, '\n');
		if (newline_pos != NULL) {
			newline_pos[0] = 0;
		}

		DBG(" → '%s' %d", buf, buf[0] == '1');

		// Only check first char...
		if (buf[0] == '1') { act_tsk(1); }
		if (buf[0] == '2') { act_tsk(2); }
		if (buf[0] == 'q') {
			DBG("Request to quit...");
			exit(0);
		}
	}
}
