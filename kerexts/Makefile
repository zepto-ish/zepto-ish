# SPDX-License-Identifier: GPL-3.0-only
#   ╔════════════════════════════════════════════════════╗
#   ║                     __             ._      ._      ║
#   ║ _______ ____ _____./  |_ ___       |_| ____| |__   ║
#   ║ \___  / __  \\___ \_  __/ _ \  ____| |/  __| |  \  ║
#   ║  / __/\  ___/| |_> |  || <_> |/___/| |\__ \|  Y  \ ║
#   ║ /____ \\___ \|  __/|__| \___/      |_/___  |__|  / ║
#   ║      \/    \/|_|                         \/    \/  ║
#   ╚════════════════════════════════════════════════════╝

TARGET_KEREXTS += kerexts/simplest.so

# Add all TARGET_KEREXTS to the TARGETS.
TARGETS += ${TARGET_KEREXTS}

"$(PREFIX)/kerexts":
	mkdir -vp "$@"

# Install kerexts
install:: "$(PREFIX)/kerexts" ${TARGET_KEREXTS}
	cp -vt "$(PREFIX)/kerexts" ${TARGET_KEREXTS}

clean::
	@rm -vf ${TARGET_KEREXTS:so=o}

kerexts/simplest.so: kerexts/simplest.o
	$(CC) -shared -o $@ $^ $(CFLAGS) $(LDFLAGS)

