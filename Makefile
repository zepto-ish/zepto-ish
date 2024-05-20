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
CFLAGS += -Wall -Wextra -Wpedantic
CFLAGS += -Werror
CFLAGS += -Wswitch
CFLAGS += -Wswitch-enum
CFLAGS += -Wenum-compare
CFLAGS += -Wenum-conversion
CFLAGS += -Iinclude

# Annoying :(
CFLAGS += -Wno-unused-parameter

.PHONY: clean all

all: main

clean:
	@rm -vf *.o */*.o
	@rm -vf main

"$(PREFIX)/bin":
	mkdir -vp "$@"

install: main "$(PREFIX)/bin"
	cp -vt "$(PREFIX)/bin" main

main: src/main.o src/kernel.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
