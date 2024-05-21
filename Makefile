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

TARGET_LIBS += zepto/zepto.a
TARGET_LIBS += zepto/zepto.so
TARGET_BINS += zepto-dlopen/zepto-dlopen
TARGET_BINS += demo/demo

TARGETS = ${TARGET_BINS} ${TARGET_LIBS}

all: $(TARGETS)

clean:
	@rm -vf ${ZEPTO_OBJS}
	@rm -vf ${DEMO_OBJS}
	@rm -vf ${TARGETS}

"$(PREFIX)/bin":
	mkdir -vp "$@"

"$(PREFIX)/lib":
	mkdir -vp "$@"


install: "$(PREFIX)/bin" "$(PREFIX)/lib" ${TARGET_BINS} ${TARGET_LIBS}
	cp -vt "$(PREFIX)/bin" ${TARGET_BINS}
	cp -vt "$(PREFIX)/lib" ${TARGET_LIBS}

ZEPTO_SRCS += $(wildcard zepto/*.c)
ZEPTO_OBJS := ${ZEPTO_SRCS:c=o}

zepto/zepto.so: ${ZEPTO_OBJS}
	$(CC) -shared -o $@ $^ $(CFLAGS) $(LDFLAGS)

zepto/zepto.a: ${ZEPTO_OBJS}
	$(AR) vrcs $@ $^

DEMO_SRCS += $(wildcard demo/*.c)
DEMO_OBJS := ${DEMO_SRCS:c=o}

demo/demo: ${DEMO_OBJS} zepto/zepto.a
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

ZEPTO_DLOPEN_SRCS += $(wildcard zepto-dlopen/*.c)
ZEPTO_DLOPEN_OBJS := ${ZEPTO_DLOPEN_SRCS:c=o}

zepto-dlopen/zepto-dlopen: ${ZEPTO_DLOPEN_OBJS} zepto/zepto.a
	$(CC) -o $@ $^ -Wl,--export-dynamic -ldl $(CFLAGS) $(LDFLAGS)
