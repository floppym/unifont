
SHELL = /bin/sh

BINDIR = ../../bin

FONTFORGE = fontforge

COPYRIGHT = "Copyright (C) 2013 Roman Czyborra, Paul Hardy, et al.  \
Licensed under the GNU General Public License; either version 2, or \
(at your option) a later version, with the GNU Font Embedding Exception."

VERSION = 6.3.20131217

#
# How to build unifont.ttf from GNU Unifont's unifont.hex
# -------------------------------------------------------
# Written by Luis Alejandro Gonzalez Miranda - http://www.lgm.cl/
#

#
# 2008 - Paul Hardy created this Makefile from Luis' original
# howto-build.sh and other bash scripts.  Those original scripts
# don't appear in this archive, but they can be retrieved from
# http://www.lgm.cl/.
#

# First of all, you need a Perl interpreter and FontForge.
#
# I don't remember all the steps, but I think it was as described by
# this script.

# This division is done only so the Simplify and RemoveOverlap
# operations don't use up too much memory, and because
# a .sfd generated from the whole unifont.hex would be too big to
# process all at once.

all: hex2sfd merge sfd2ttf sbit

hex2sfd: unifont.hex $(BINDIR)/hex2sfd
	echo "Separating unifont.hex into 16 parts"
	for i in 0 1 2 3 4 5 6 7 8 9 A B C D E F ; \
	   do \
	   grep ^$$i unifont.hex > $$i.hex ; \
	   $(BINDIR)/hex2sfd < $$i.hex > $$i.sfd ; \
	done
	for i in 0 1 2 3 4 5 6 7 8 9 A B C D E F ; \
	   do \
	   $(FONTFORGE) -lang=ff -c \
	      'Open($$1); \
	       SelectAll(); \
	       RemoveOverlap(); \
	       Simplify(64,1); \
	       Save($$1);' \
	      $$i.sfd ; \
	   \rm -f $$i.hex ; \
	done

# We join all the generated .sfd files into a single one,
# and generate the final TrueType font.

merge:
	echo "Merging all .sfd files"
	$(FONTFORGE) -script all.pe

sfd2ttf:
	echo "Converting .sfd font into .ttf font"
	$(FONTFORGE) -lang=ff -c \
	   'Open($$1); \
            SetFontNames("UnifontMedium", "GNU", "Unifont", "Medium", $(COPYRIGHT), "$(VERSION)"); \
	    Generate($$2)' unifont.sfd unifont.ttf
	\rm -f unifont.hex

#
# This fontforge script reads a BDF font file and generates an SBIT font file.
# Author: written by Qianqian Fang, given to Paul Hardy in 2008.
# The SBIT font is far smaller than the default outline TrueType font
# and takes far less time to build than the outline font.  However, it
# isn't scalable.  An SBIT font could be created and merged with the
# larger TTF font using fontforge, but I (Paul Hardy) haven't noticed
# any degradation in the screen rendering of just the outline TTF font
# that this Makefile produces as its final product.  This is with
# daily use of this Makefile's default TrueType font.
#
# This builds an SBIT font from the unifont_sample BDF font.  The
# BDF font already contains font name, etc., so they don't need to
# be set using SetFontNames; those parameters are left null so the
# existing font's values will be preserved.  However, Fontforge
# does not read the FONT_VERSION property so Paul Hardy added the
# the SetFontNames call.
#
sbit:
	$(FONTFORGE) -lang=ff -c \
	   'New(); \
            SetFontNames("","","","","","$(VERSION)"); \
	    Import($$1); \
	    Generate($$2, "ttf"); \
	    Close()' \
	   unifont_sample.bdf unifont_sample.ttf

clean:
	\rm -f [0-9A-F].hex
	\rm -f [0-9A-F].sfd
	\rm -f *.bdf

distclean: clean
	\rm -f unifont.bdf
	\rm -f unifont.hex
	\rm -f unifont.sfd
	\rm -f unifont.ttf
	\rm -f unifont-sbit.ttf

.PHONY: all hex2sfd merge sfd2ttf sbit clean distclean
