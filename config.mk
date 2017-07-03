# BFF version
VERSION = 1.0

# Paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

CFLAGS   = -std=c99 -Wall -pedantic -O2
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700
LDFLAGS  = -s
