#
# Makefile to build a version of GNU Unifont with the
# unifoundry.com GNU Unifont utilities.  This Makefile
# converts unifont-$(VERSION).hex and related files into a final
# GNU Unifont font file.
#
# This software is released under version 2.0 of the GNU Public License,
# or (at your option) a later version of the GPL.
#
# Paul Hardy, 7 July 2008
#
# commands will be interpreted with the Bourne Shell
#
SHELL = /bin/sh
INSTALL = install
FONTFORGE = fontforge
#
# assembly date of this version
#
MAJORVERSION = 6.3
DATE = 20131215
VERSION = $(MAJORVERSION).$(DATE)

COPYRIGHT = "Copyright (C) 2013 Roman Czyborra, Paul Hardy, et al.  \
Licensed under the GNU General Public License; either version 2, or \
(at your option) a later version, with the GNU Font Embedding Exception."

#
# Path to local unifont-bin utilities.  If your system doesn't
# use GNU's version of "make", use the first BINDIR definition.
# If using GNU's "make", use the second BINDIR definition.
#
# BINDIR = ../bin
BINDIR = $(CURDIR)/../bin
#
# The top-level directory for installing fonts on the system,
# and the installation directories for PCF and TrueType fonts.
#
FONTDEST = $(DESTDIR)/usr/share/fonts
PCFDEST = $(FONTDEST)/X11/misc
TTFDEST = $(FONTDEST)/truetype/unifont
#
# destination directory for compiled fonts
#
COMPILED_DIR = compiled
#
# destination directory for .bmp representation of font
#
BMPDIR = $(COMPILED_DIR)/bmp
#
# Directory with original unifont-$(VERSION).hex files
#
HEXDIR = hexsrc
#
# These are the files for building GNU Unifont with the Qianqian Fang's
# Wen Quan Yi CJK ideographs.  This version provides complete coverage
# of the Unicode Basic Multilingual Plane.
#
# If you want don't want to inlcude blank glyphs in unassigned code points,
# uncomment the UNASSIGNED definition or override from the make command line.
#
# Likewise, uncomment the PUA definiation if you want to use Private
# Use Area glyphs.
#
UNASSIGNED = $(HEXDIR)/unassigned.hex
# UNASSIGNED =

#
# Non-printing glyphs.  There are only about 100 of these, and many end
# users do want to print representations of these glyphs, so they are
# included as an optional assignment.
#
NONPRINTING = $(HEXDIR)/nonprinting.hex
# NONPRINTING =

#
# Private Use Area glyphs.  Uncomment to include four-digit hexadecimal glyphs
# or override from the make command line.
#
# PUA = $(HEXDIR)/pua.hex
PUA = 

# The remaining .hex files will be constant unless a customized font
# is being built.
UNIFONTBASE = $(HEXDIR)/unifont-base.hex
CJK         = $(HEXDIR)/wqy.hex
HANGUL      = $(HEXDIR)/hangul-syllables.hex
SPACES      = $(HEXDIR)/spaces.hex

UNIFILES = $(UNIFONTBASE) $(CJK) $(HANGUL) $(SPACES) $(UNASSIGNED) \
	   $(NONPRINTING) $(PUA)

#
# Location of the file containing a list of Unicode combining characters.
#
COMBINING = ttfsrc/combining.txt

VPATH = hexsrc ttfsrc

#
# Location of  TTF source directory, where TTF font is built.
#
TTFSRC = ttfsrc


all: compiled

compiled: $(UNIFILES) $(COMBINING)
	make compiled-files

compiled-files: pcf bmp ttf bigpic coverage
	install -p index.html $(COMPILED_DIR)/index.html

#
# Build the aggregate .hex font files
#
hex: distclean
	if [ ! -d $(COMPILED_DIR) ] ; then \
	   mkdir -p $(COMPILED_DIR) ; \
	fi
	sort $(UNIFILES) >$(COMPILED_DIR)/unifont-$(VERSION).hex
	(cd $(HEXDIR) ; sort *.hex) | \
	   $(BINDIR)/unigencircles ttfsrc/combining.txt hexsrc/nonprinting.hex \
	   > $(COMPILED_DIR)/unifont_sample-$(VERSION).hex

#
# Build a BDF font file from the final .hex file.
#
bdf: hex
	# First make the default BDF font.  The font name will be "unifont".
	$(BINDIR)/hex2bdf --version "$(VERSION)" --copyright $(COPYRIGHT) \
	   $(COMPILED_DIR)/unifont-$(VERSION).hex \
	   >$(COMPILED_DIR)/unifont-$(VERSION).bdf
	gzip -f -9 <$(COMPILED_DIR)/unifont-$(VERSION).bdf \
	           >$(COMPILED_DIR)/unifont-$(VERSION).bdf.gz
	# Now make a version with combining circles.  The font name
	# will be "unifont_sample" instead of "unifont" to distinguish
	# it from the default font.
	$(BINDIR)/hex2bdf --font "Unifont Sample" \
	   --version "$(VERSION)" --copyright $(COPYRIGHT) \
	     $(COMPILED_DIR)/unifont_sample-$(VERSION).hex \
	   > $(COMPILED_DIR)/unifont_sample-$(VERSION).bdf
	gzip -f -9 <$(COMPILED_DIR)/unifont_sample-$(VERSION).bdf \
	           >$(COMPILED_DIR)/unifont_sample-$(VERSION).bdf.gz

#
# Build a PCF font file from the final .hex file.
#
pcf: bdf
	bdftopcf <$(COMPILED_DIR)/unifont-$(VERSION).bdf \
	         >$(COMPILED_DIR)/unifont-$(VERSION).pcf
	$(FONTFORGE) -lang=ff -c \
	   'Open($$1); \
	    SetFontNames("UnifontMedium", "GNU", "Unifont", "Medium", $(COPYRIGHT), "$(VERSION)"); \
	    Save($$1)' \
	   $(COMPILED_DIR)/unifont-$(VERSION).pcf
	gzip -f -9 $(COMPILED_DIR)/unifont-$(VERSION).pcf
	bdftopcf <$(COMPILED_DIR)/unifont_sample-$(VERSION).bdf \
	         >$(COMPILED_DIR)/unifont_sample-$(VERSION).pcf
	$(FONTFORGE) -lang=ff -c \
	   'Open($$1); \
	    SetFontNames("UnifontSampleMedium", "GNU", "Unifont Sample", "Medium", $(COPYRIGHT), "$(VERSION)"); \
	    Save($$1)' \
	   $(COMPILED_DIR)/unifont_sample-$(VERSION).pcf
	gzip -f -9 $(COMPILED_DIR)/unifont_sample-$(VERSION).pcf

#
# Print coverage of scripts in Basic Multilingual Plane in .txt file.
#
# Note: can't use this older version unless unassigned.hex is merged
# with the rest of the built unifont-$(VERSION).hex final file.
#
# coverage: $(COMPILED_DIR)/unifont-$(VERSION).hex $(BINDIR)/unicoverage
# 	$(BINDIR)/unicoverage < $(COMPILED_DIR)/unifont-$(VERSION).hex \
# 		    > $(COMPILED_DIR)/coverage.txt
#
# Use this version because "unassigned.hex" isn't part of the final
# .hex font by default now:
#
coverage:
	sort $(HEXDIR)/*.hex | \
		$(BINDIR)/unicoverage > $(COMPILED_DIR)/coverage.txt

#
# Print HTML page coverage in Basic Multilingual Plane in .txt file.
#
pagecount: $(COMPILED_DIR)/unifont-$(VERSION).hex $(BINDIR)/unipagecount
	$(BINDIR)/unipagecount -l < $(COMPILED_DIR)/unifont-$(VERSION).hex \
		        > $(COMPILED_DIR)/pagecount.html

#
# Create the .bmp (Windows Bitmap) graphics versions of the glyphs.
#
bmp: hex $(BINDIR)/unihex2bmp
	if [ ! -d $(BMPDIR) ] ; then \
	   mkdir -p $(BMPDIR) ; \
	fi
	for i in 0 1 2 3 4 5 6 7 8 9 A B C D E F; do \
	   for j in 0 1 2 3 4 5 6 7 8 9 A B C D E F; do \
	      $(BINDIR)/unihex2bmp -p$$i$$j \
	         -i$(COMPILED_DIR)/unifont_sample-$(VERSION).hex \
	         -o$(BMPDIR)/uni$$i$$j.bmp ; \
	   done ; \
	done ; \
	echo "Done creating $(BMPDIR)"

#
# Build one bitmap of the entire Unifont as a 4096 x 16 grid.
#
bigpic: $(COMPILED_DIR)/unifont_sample-$(VERSION).hex
	cat $(COMPILED_DIR)/unifont_sample-$(VERSION).hex | \
	   $(BINDIR)/unifontpic -d120 > $(COMPILED_DIR)/unifont-$(VERSION).bmp

#
# Note that $(TTFSRC) must exist, because it contains some source files
# Perform a "make && make clean" because ALL of the files would consume
# over 200 Megabytes if left around.  The .sfd file is about 100 Megabytes,
# and it is created from merging "[0-F].sfd", which take up another 100 MB.
#

ttf:
	# First copy the ordinary version, to make a TrueType font.
	install -p \
		$(COMPILED_DIR)/unifont-$(VERSION).hex \
		$(TTFSRC)/unifont.hex
	# Second copy unifont_sample.bdf, to make an SBIT font.
	install -p \
		$(COMPILED_DIR)/unifont_sample-$(VERSION).bdf \
		$(TTFSRC)/unifont_sample.bdf
	cd $(TTFSRC) ; make && make clean
	mv $(TTFSRC)/unifont.sfd $(COMPILED_DIR)/unifont.sfd 
	mv $(TTFSRC)/unifont.ttf \
		$(COMPILED_DIR)/unifont-$(VERSION).ttf 
	mv $(TTFSRC)/unifont_sample.ttf \
		$(COMPILED_DIR)/unifont_sample-$(VERSION).ttf 
	gzip -f -9 $(COMPILED_DIR)/unifont.sfd 

#
# Copy the newly created files from $(COMPILED_DIR) to the precompiled/
# directory.  This has to be called manually, because the precompiled/
# directory usually remains untouched.
#
precompiled: all
	\rm -rf precompiled
	install -m0755 -d precompiled
	install -m0644 -p $(COMPILED_DIR)/unifont-$(VERSION).hex \
	                  $(COMPILED_DIR)/unifont-$(VERSION).bdf.gz \
	                  $(COMPILED_DIR)/unifont-$(VERSION).pcf.gz \
	                  $(COMPILED_DIR)/unifont-$(VERSION).ttf \
	                  $(COMPILED_DIR)/unifont_sample-$(VERSION).hex \
	                  $(COMPILED_DIR)/unifont_sample-$(VERSION).bdf.gz \
	                  $(COMPILED_DIR)/unifont_sample-$(VERSION).pcf.gz \
	                  $(COMPILED_DIR)/unifont_sample-$(VERSION).ttf \
	                  $(COMPILED_DIR)/coverage.txt \
	                  $(COMPILED_DIR)/unifont-$(VERSION).bmp \
	   precompiled

#
# This is set up for Debian.  Solaris places fonts in
# "/usr/share/fonts/TrueType".  Other unices use other directories.
# The original font format was BDF, but Debian doesn't use that and
# xfs should only need TrueType, so the BDF font isn't installed.
#
# The TrueType font is about 16 Megabytes.  To avoid duplication,
# the fonts are installed as symbolic links back to the original package.
# Alternatively, the fonts can be copied to the destination directory
# with "install -m0644" or moved there (but "mv" is a destructive
# one-time operation).
#
# After installing the new fonts, register them with X Window System using:
#
#      xset fp rehash
#
install:
	if [ x$(CURDIR) = x ] ; \
	then \
	   echo "Fatal Error: CURDIR not defined -- define in Makefile." ; \
	   exit 1 ; \
	fi
	$(INSTALL) -m0755 -d $(PCFDEST)
	$(INSTALL) -m0755 -d $(TTFDEST)
	if [ ! -d $(COMPILED_DIR) ] ; then \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/unifont-$(VERSION).pcf.gz $(PCFDEST)/unifont.pcf.gz ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/unifont_sample-$(VERSION).pcf.gz $(PCFDEST)/unifont_sample.pcf.gz ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/unifont-$(VERSION).ttf    $(TTFDEST)/unifont.ttf ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/unifont_sample-$(VERSION).ttf    $(TTFDEST)/unifont_sample.ttf ; \
	else \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/unifont-$(VERSION).pcf.gz $(PCFDEST)/unifont.pcf.gz ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/unifont_sample-$(VERSION).pcf.gz $(PCFDEST)/unifont_sample.pcf.gz ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/unifont-$(VERSION).ttf    $(TTFDEST)/unifont.ttf ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/unifont_sample-$(VERSION).ttf    $(TTFDEST)/unifont_sample.ttf ; \
	fi

clean:
	\rm -rf $(COMPILED_DIR)/bmp
	\rm -f $(COMPILED_DIR)/unifont-$(VERSION).hex
	\rm -f $(COMPILED_DIR)/unifont-$(VERSION).bdf
	\rm -f $(COMPILED_DIR)/unifont-$(VERSION).bdf.gz
	\rm -f *~

#
# Note that distclean leaves precompiled/ alone.  This is intentional.
# The .DS files are created under Mac OS X.
#
distclean:
	\rm -rf $(COMPILED_DIR)
	$(MAKE) -C ttfsrc distclean
	\rm -f *~
	\rm -rf .DS* ._.DS*

.PHONY: all hex bdf pcf coverage pagecount bmp bigpic ttf precompiled install clean distclean 
