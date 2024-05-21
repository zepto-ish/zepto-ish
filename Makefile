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

all: zepto/zepto.a demo/demo

clean:
	@rm -vf ${ZEPTO_OBJS}
	@rm -vf zepto/zepto.so zepto/zepto.a
	@rm -vf main
	@rm -vf demo/demo

"$(PREFIX)/bin":
	mkdir -vp "$@"

"$(PREFIX)/lib":
	mkdir -vp "$@"

ZEPTO_SRCS += $(wildcard zepto/*.c)

ZEPTO_OBJS := ${ZEPTO_SRCS:c=o}

install: "$(PREFIX)/bin" "$(PREFIX)/lib" demo/demo zepto/zepto.a
	cp -vt "$(PREFIX)/bin" demo/demo
	cp -vt "$(PREFIX)/lib" zepto/zepto.a

# zepto.so: ${ZEPTO_OBJS}
# 	$(CC) -shared -o $@ $^ $(CFLAGS) $(LDFLAGS)

zepto/zepto.a: ${ZEPTO_OBJS}
	$(AR) vrcs $@ $^

DEMO_SRCS += $(wildcard demo/*.c)

DEMO_OBJS := ${DEMO_SRCS:c=o}

demo/demo: ${DEMO_OBJS} zepto/zepto.a
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
