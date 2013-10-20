#
# unifoundry.com utilities for the GNU Unifont
#
# Typing "make && make install" will make and
# install the binary programs and man pages.
# To build only the font from scratch, use
# "cd font ; make"
#
SHELL = /bin/sh

DATE = 20131020
MAJORVERSION = 6.3
VERSION = $(MAJORVERSION).$(DATE)
#
# The settings below will install software, man pages, and documentation
# in /usr/local.  To install in a different location, modify USRDIR to
# your liking.
#
# USRDIR = usr
USRDIR = usr/local
PREFIX = $(DESTDIR)/$(USRDIR)
PKGDEST = $(PREFIX)/share/unifont

#
# Whether to install man pages uncompressed (COMPRESS = 0) or
# compressed (COMPRESS != 0).
#
COMPRESS = 1

DIRS = bin man font

all: bindir fontdir libdir
	echo "Make is done."

bindir:
	set -e ; $(MAKE) -C src

mandir:
	set -e ; $(MAKE) -C man

fontdir:
	set -e ; $(MAKE) -C font

precompiled:
	set -e ; $(MAKE) precompiled -C font

#
# Create lib/wchardata.c.  If you want to also build the object file
# wchardata.o, uncomment the last line
libdir:
	install -m0755 -d lib
	(cd font/hexsrc ; \
		sort hangul-syllables.hex pua.hex spaces.hex \
			unassigned.hex unifont-base.hex wqy.hex \
			> ../../unifonttemp.hex)
	bin/unigenwidth unifonttemp.hex font/ttfsrc/combining.txt \
		> lib/wchardata.c
	\rm -f unifonttemp.hex
#	(cd lib ; $(CC) -c wchardata.c ; chmod 644 wchardata.o )

install: bindir libdir
	$(MAKE) -C src install PREFIX=$(PREFIX)
	$(MAKE) -C man install PREFIX=$(PREFIX) COMPRESS=$(COMPRESS)
	$(MAKE) -C font install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR)
	install -m0755 -d $(PKGDEST)
	install -m0644 -p README TUTORIAL LICENSE $(PKGDEST)
	gzip -f -9 $(PKGDEST)/README
	gzip -f -9 $(PKGDEST)/TUTORIAL
	gzip -f -9 $(PKGDEST)/LICENSE
	install -m0644 -p lib/wchardata.c $(PKGDEST)
	install -m0644 -p font/ttfsrc/combining.txt $(PKGDEST)
	# If "make" wasn't run before, font/compiled won't exist.
	if [ ! -d font/compiled ] ; then \
	   install -m0644 -p font/precompiled/unifont-$(VERSION).hex   $(PKGDEST)/unifont.hex ; \
	   install -m0644 -p font/precompiled/unifont-$(VERSION).bmp $(PKGDEST) ; \
	else \
	   install -m0644 -p font/compiled/unifont-$(VERSION).hex   $(PKGDEST)/unifont.hex ; \
	   install -m0644 -p font/compiled/unifont-$(VERSION).bmp $(PKGDEST) ; \
	fi

clean:
	$(MAKE) -C src clean
	$(MAKE) -C man clean
	$(MAKE) -C font clean

#
# The .DS files are created under Mac OS X
# The build-stamp file is created under Debian
#
distclean:
	$(MAKE) -C src distclean
	$(MAKE) -C man distclean
	$(MAKE) -C font distclean
	\rm -rf bin lib
	\rm -f unifonttemp.hex
	\rm -rf .DS* ._.DS*
	\rm -f build-stamp

.PHONY: all bindir mandir fontdir precompiled install clean distclean
