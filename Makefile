#
# unifoundry.com utilities for the GNU Unifont
#
# Typing "make && make install" will make and
# install the binary programs and man pages.
# To build only the font from scratch, use
# "cd font ; make"
#
SHELL = /bin/sh
INSTALL = install

PACKAGE = "unifont"
UNICODE_VERSION = 7.0
PKG_REV = 05
VERSION = $(UNICODE_VERSION).$(PKG_REV)

#
# The settings below will install software, man pages, and documentation
# in /usr/local.  To install in a different location, modify USRDIR to
# your liking.
#
USRDIR = usr
# USRDIR = usr/local
PREFIX = $(DESTDIR)/$(USRDIR)
PKGDEST = $(PREFIX)/share/unifont

VPATH = lib font/plane00 font/ttfsrc

HEXFILES = hangul-syllables.hex nonprinting.hex pua.hex spaces.hex \
	   unassigned.hex unifont-base.hex wqy.hex

COMBINING = font/plane00/bmp-combining.txt

TEXTFILES = ChangeLog INSTALL NEWS README

#
# Whether to build the font or not (default is not).
# Set to non-null value to build font.
#
BUILDFONT=

#
# Whether to install man pages uncompressed (COMPRESS = 0) or
# compressed (COMPRESS != 0).
#
COMPRESS = 1

all: bindir libdir docdir buildfont
	echo "Make is done."

#
# Build a distribution tarball.
#
dist: distclean
	(cd .. ; tar cf $(PACKAGE)-$(VERSION).tar $(PACKAGE)-$(VERSION) && \
		gzip -f -9 $(PACKAGE)-$(VERSION).tar)

bindir:
	set -e ; $(MAKE) -C src

#
# Conditionally build the font, depending on the value of BUILDFONT.
# To build the font unconditionally, use the "fontdir" target below.
#
buildfont:
	if [ x$(BUILDFONT) != x ] ; \
        then \
           set -e ; $(MAKE) -C font ; \
        fi

#
# Not invoked automatically; the font files are taken from
# font/precompiled by default.
#
fontdir:
	set -e ; $(MAKE) -C font

libdir: lib/wchardata.c

docdir:
	set -e ; $(MAKE) -C doc

mandir:
	set -e ; $(MAKE) -C man

precompiled:
	set -e ; $(MAKE) precompiled -C font

#
# Create lib/wchardata.c.  If you want to also build the object file
# wchardata.o, uncomment the last line
#
lib/wchardata.c: $(HEXFILES) $(COMBINING)
	$(INSTALL) -m0755 -d lib
	(cd font/plane00 && sort $(HEXFILES) > ../../unifonttemp.hex)
	bin/unigenwidth unifonttemp.hex $(COMBINING) > lib/wchardata.c
	\rm -f unifonttemp.hex
#	(cd lib && $(CC) $(CFLAGS) -c wchardata.c && chmod 644 wchardata.o )

install: bindir libdir docdir
	$(MAKE) -C src install PREFIX=$(PREFIX)
	$(MAKE) -C man install PREFIX=$(PREFIX) COMPRESS=$(COMPRESS)
	$(MAKE) -C font install PREFIX=$(PREFIX) DESTDIR=$(DESTDIR)
	$(INSTALL) -m0755 -d $(PKGDEST)
	$(INSTALL) -m0644 -p $(TEXTFILES) doc/unifont.txt doc/unifont.info $(PKGDEST)
	for i in $(TEXTFILES) unifont.txt unifont.info ; do \
	   gzip -f -9 $(PKGDEST)/$$i ; \
	done
	$(INSTALL) -m0644 -p lib/wchardata.c $(PKGDEST)
	$(INSTALL) -m0644 -p font/plane00/bmp-combining.txt $(PKGDEST)
	# If "make" wasn't run before, font/compiled won't exist.
	if [ ! -d font/compiled ] ; then \
	   $(INSTALL) -m0644 -p font/precompiled/unifont-$(VERSION).hex   $(PKGDEST)/unifont.hex ; \
	   $(INSTALL) -m0644 -p font/precompiled/unifont-$(VERSION).bmp $(PKGDEST)/unifont.bmp ; \
	else \
	   $(INSTALL) -m0644 -p font/compiled/unifont-$(VERSION).hex   $(PKGDEST)/unifont.hex ; \
	   $(INSTALL) -m0644 -p font/compiled/unifont-$(VERSION).bmp $(PKGDEST)/unifont.bmp ; \
	fi

clean:
	$(MAKE) -C src  clean
	$(MAKE) -C doc  clean
	$(MAKE) -C man  clean
	$(MAKE) -C font clean
	\rm -rf *~

#
# The .DS files are created under Mac OS X
#
distclean:
	$(MAKE) -C src  distclean
	$(MAKE) -C doc  distclean
	$(MAKE) -C man  distclean
	$(MAKE) -C font distclean
	\rm -rf bin lib
	\rm -f unifonttemp.hex
	\rm -rf *~
	\rm -rf .DS* ._.DS*

.PHONY: all bindir docdir mandir fontdir precompiled install clean distclean
