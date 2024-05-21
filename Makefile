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

all: zepto/zepto.a

clean:
	@rm -vf ${ZEPTO_OBJS}
	@rm -vf zepto/zepto.so zepto/zepto.a
	@rm -vf main

"$(PREFIX)/bin":
	mkdir -vp "$@"

ZEPTO_SRCS += $(wildcard zepto/*.c)

ZEPTO_OBJS := ${ZEPTO_SRCS:c=o}

install: main "$(PREFIX)/bin"
	cp -vt "$(PREFIX)/bin" main

# zepto.so: ${ZEPTO_OBJS}
# 	$(CC) -shared -o $@ $^ $(CFLAGS) $(LDFLAGS)

zepto/zepto.a: ${ZEPTO_OBJS}
	$(AR) vrcs $@ $^

main: src/main.o zepto.a
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
