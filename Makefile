# SPDX-License-Identifier: GPL-3.0-only
#   ╔════════════════════════════════════════════════════╗
#   ║                     __             ._      ._      ║
#   ║ _______ ____ _____./  |_ ___       |_| ____| |__   ║
#   ║ \___  / __  \\___ \_  __/ _ \  ____| |/  __| |  \  ║
#   ║  / __/\  ___/| |_> |  || <_> |/___/| |\__ \|  Y  \ ║
#   ║ /____ \\___ \|  __/|__| \___/      |_/___  |__|  / ║
#   ║      \/    \/|_|                         \/    \/  ║
#   ╚════════════════════════════════════════════════════╝

CFLAGS += -DWITH_DBG
CFLAGS += -Wall
CFLAGS += -Werror
CFLAGS += -Iinclude

.PHONY: clean all

all: main

clean:
	@rm -vf *.o */*.o
	@rm -vf main

main: src/main.o src/kernel.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
