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

#include <kernel.h>

void task_2(VP_INT exinf) {
	static INT s_i = 0;
	INT i = 0;
	i++;
	s_i++;
	vconsole_stdoutf("Other task?\n");
	DBG("This should stay at 1: %08d", i);
	DBG("This should increment: %08d", s_i);
	if (i != 1) {
		PANIC("Stack-allocated variables are not working properly...", E_SYS);
	}
	if (s_i <= 10) {
		act_tsk(1);
	}
}
