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
DATE = 20140214
VERSION = $(MAJORVERSION).$(DATE)

COPYRIGHT = "Copyright (C) 2014 Roman Czyborra, Paul Hardy, Qianqian Fang, \
Andrew Miller, et al.  \
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
CONSOLEDEST = $(DESTDIR)/usr/share/consolefonts
FONTDEST    = $(DESTDIR)/usr/share/fonts
PCFDEST     = $(FONTDEST)/X11/misc
TTFDEST     = $(FONTDEST)/truetype/unifont
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
HEXDIR = plane00
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
# Planes 1 through 14 (0x0E) are ordinary;
# Planes 15 (0x0F) and 16 (0x10) are Private Use Area
#
UPPER_FILES = plane0[1-E]/*.hex

#
# Location of the file containing a list of Unicode combining characters.
#
COMBINING = plane00/bmp-combining.txt

UPPER_COMBINING = plane0[0-E]/*combining*.txt

VPATH = plane00 ttfsrc

#
# Location of  TTF source directory, where TTF font is built.
#
TTFSRC = ttfsrc


all: compiled

compiled: $(UNIFILES) $(COMBINING) $(UPPER_FILES) $(UPPER_COMBINING)
	make compiled-files

compiled-files: pcf psf bmp ttf csurttf upperttf uppercsurttf bigpic coverage
	install -p index.html $(COMPILED_DIR)/index.html

#
# Build the aggregate .hex font files
#
hex: distclean
	if [ ! -d $(COMPILED_DIR) ] ; then \
	   mkdir -p $(COMPILED_DIR) ; \
	fi
	sort $(UNIFILES) > $(COMPILED_DIR)/unifont-$(VERSION).hex
	(cd $(HEXDIR) ; sort *.hex) | \
	  egrep -v "^FFF[EF]" | \
	   $(BINDIR)/unigencircles $(COMBINING) plane00/nonprinting.hex \
	   > $(COMPILED_DIR)/unifont_sample-$(VERSION).hex
	sort plane00csur/*.hex $(UNIFILES) \
	   > $(COMPILED_DIR)/unifont_csur-$(VERSION).hex
	sort $(UPPER_FILES) \
	   > $(COMPILED_DIR)/unifont_upper-$(VERSION).hex
	sort plane0[1-F]/*.hex \
	   > $(COMPILED_DIR)/unifont_upper_csur-$(VERSION).hex

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
	$(BINDIR)/hex2bdf --font "Unifont CSUR" \
	   --version "$(VERSION)" --copyright $(COPYRIGHT) \
	     $(COMPILED_DIR)/unifont_csur-$(VERSION).hex \
	   > $(COMPILED_DIR)/unifont_csur-$(VERSION).bdf
	gzip -f -9 <$(COMPILED_DIR)/unifont_csur-$(VERSION).bdf \
	           >$(COMPILED_DIR)/unifont_csur-$(VERSION).bdf.gz
	$(BINDIR)/hex2bdf --font "Unifont Upper" \
	   --version "$(VERSION)" --copyright $(COPYRIGHT) \
	     $(COMPILED_DIR)/unifont_upper-$(VERSION).hex \
	   > $(COMPILED_DIR)/unifont_upper-$(VERSION).bdf
	gzip -f -9 <$(COMPILED_DIR)/unifont_upper-$(VERSION).bdf \
	           >$(COMPILED_DIR)/unifont_upper-$(VERSION).bdf.gz
	$(BINDIR)/hex2bdf --font "Unifont Upper CSUR" \
	   --version "$(VERSION)" --copyright $(COPYRIGHT) \
	     $(COMPILED_DIR)/unifont_upper_csur-$(VERSION).hex \
	   > $(COMPILED_DIR)/unifont_upper_csur-$(VERSION).bdf
	gzip -f -9 <$(COMPILED_DIR)/unifont_upper_csur-$(VERSION).bdf \
	           >$(COMPILED_DIR)/unifont_upper_csur-$(VERSION).bdf.gz

#
# Build a PCF font file from the final .hex file.
#
pcf: bdf
	bdftopcf <$(COMPILED_DIR)/unifont-$(VERSION).bdf \
	         >$(COMPILED_DIR)/unifont-$(VERSION).pcf
	gzip -f -9 $(COMPILED_DIR)/unifont-$(VERSION).pcf
	bdftopcf <$(COMPILED_DIR)/unifont_sample-$(VERSION).bdf \
	         >$(COMPILED_DIR)/unifont_sample-$(VERSION).pcf
	gzip -f -9 $(COMPILED_DIR)/unifont_sample-$(VERSION).pcf
	bdftopcf <$(COMPILED_DIR)/unifont_csur-$(VERSION).bdf \
	         >$(COMPILED_DIR)/unifont_csur-$(VERSION).pcf
	gzip -f -9 $(COMPILED_DIR)/unifont_csur-$(VERSION).pcf
	bdftopcf <$(COMPILED_DIR)/unifont_upper-$(VERSION).bdf \
	         >$(COMPILED_DIR)/unifont_upper-$(VERSION).pcf
	gzip -f -9 $(COMPILED_DIR)/unifont_upper-$(VERSION).pcf
	bdftopcf <$(COMPILED_DIR)/unifont_upper_csur-$(VERSION).bdf \
	         >$(COMPILED_DIR)/unifont_upper_csur-$(VERSION).pcf
	gzip -f -9 $(COMPILED_DIR)/unifont_upper_csur-$(VERSION).pcf

#
# Make the PSF (console) font for APL (A Programming Language).
#
psf: bdf
	bdf2psf --fb \
	   $(COMPILED_DIR)/unifont-$(VERSION).bdf \
	   psf/apl-equivalents.txt \
	   psf/unifont-apl.txt \
	   512 \
	   $(COMPILED_DIR)/Unifont-APL8x16-$(VERSION).psf
	gzip -f -9 $(COMPILED_DIR)/Unifont-APL8x16-$(VERSION).psf

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
# Use all the .hex files in $(HEXDIR) so we add the contents of
# omit.hex (FFFE & FFFF).  Those two glyphs aren't included in the
# unifont_sample font because they cause problems with MS Windows,
# but we can put them in the picture.
#
bigpic: $(COMPILED_DIR)/unifont_sample-$(VERSION).hex
	sort -u $(HEXDIR)/*.hex | \
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
	install -p \
		$(HEXDIR)/bmp-combining.txt \
		$(TTFSRC)/combining.txt
	cd $(TTFSRC) ; \
	   make FONTFILE="unifont" COMBINING="combining" \
		FONTNAME="Unifont" PSNAME="Unifont"
	mv $(TTFSRC)/unifont.sfd $(COMPILED_DIR)/unifont-$(VERSION).sfd 
	mv $(TTFSRC)/unifont.ttf \
		$(COMPILED_DIR)/unifont-$(VERSION).ttf 
	# Second copy unifont_sample.bdf, to make an SBIT font.
	install -p \
		$(COMPILED_DIR)/unifont_sample-$(VERSION).bdf \
		$(TTFSRC)/unifont_sample.bdf
	cd $(TTFSRC) ; \
	   make sbit FONTFILE="unifont_sample" COMBINING="" \
		FONTNAME="Unifont Sample" PSNAME="UnifontSample"
	mv $(TTFSRC)/unifont_sample.ttf \
		$(COMPILED_DIR)/unifont_sample-$(VERSION).ttf 
	gzip -f -9 $(COMPILED_DIR)/unifont-$(VERSION).sfd 

#
# Now build the ConScript Unicode Registry PUA font.
#
csurttf:
	install -p \
		$(COMPILED_DIR)/unifont_csur-$(VERSION).hex \
		$(TTFSRC)/unifont_csur.hex
	sort -u plane00csur/csur-combining.txt $(HEXDIR)/bmp-combining.txt \
		> $(TTFSRC)/combining_csur.txt
	cd $(TTFSRC) ; \
	   make outline FONTFILE="unifont_csur" COMBINING="combining_csur" \
		FONTNAME="Unifont CSUR" PSNAME="UnifontCSUR"
	mv $(TTFSRC)/unifont_csur.sfd \
		$(COMPILED_DIR)/unifont_csur-$(VERSION).sfd 
	mv $(TTFSRC)/unifont_csur.ttf \
		$(COMPILED_DIR)/unifont_csur-$(VERSION).ttf 
	gzip -f -9 $(COMPILED_DIR)/unifont_csur-$(VERSION).sfd 

#
# Now build the ConScript Unicode Registry PUA font.
#
upperttf:
	install -p \
		$(COMPILED_DIR)/unifont_upper-$(VERSION).hex \
		$(TTFSRC)/unifont_upper.hex
	sort -u $(UPPER_COMBINING) > $(TTFSRC)/combining_upper.txt
	cd $(TTFSRC) ; \
	   make outline FONTFILE="unifont_upper" COMBINING="combining_upper" \
		FONTNAME="Unifont Upper" PSNAME="UnifontUpper"
	mv $(TTFSRC)/unifont_upper.sfd \
		$(COMPILED_DIR)/unifont_upper-$(VERSION).sfd 
	mv $(TTFSRC)/unifont_upper.ttf \
		$(COMPILED_DIR)/unifont_upper-$(VERSION).ttf 
	gzip -f -9 $(COMPILED_DIR)/unifont_upper-$(VERSION).sfd 


#
# ConScript Unicode Registry PUA font beyond Plane 0.
#
uppercsurttf:
	install -p \
		$(COMPILED_DIR)/unifont_upper_csur-$(VERSION).hex \
		$(TTFSRC)/unifont_upper_csur.hex
	sort -u plane0[1-F]csur/*combining*.txt \
		> $(TTFSRC)/combining_upper_csur.txt
	cd $(TTFSRC) ; \
	   make outline FONTFILE="unifont_upper_csur" COMBINING="combining_upper_csur" \
		FONTNAME="Unifont Upper CSUR" PSNAME="UnifontUpperCSUR"
	mv $(TTFSRC)/unifont_upper_csur.sfd \
		$(COMPILED_DIR)/unifont_upper_csur-$(VERSION).sfd 
	mv $(TTFSRC)/unifont_upper_csur.ttf \
		$(COMPILED_DIR)/unifont_upper_csur-$(VERSION).ttf 
	gzip -f -9 $(COMPILED_DIR)/unifont_upper_csur-$(VERSION).sfd 

#
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
	                  $(COMPILED_DIR)/Unifont-APL8x16-$(VERSION).psf.gz \
	                  $(COMPILED_DIR)/unifont-$(VERSION).ttf \
	                  $(COMPILED_DIR)/unifont_csur-$(VERSION).pcf.gz \
	                  $(COMPILED_DIR)/unifont_csur-$(VERSION).ttf \
	                  $(COMPILED_DIR)/unifont_sample-$(VERSION).hex \
	                  $(COMPILED_DIR)/unifont_sample-$(VERSION).bdf.gz \
	                  $(COMPILED_DIR)/unifont_sample-$(VERSION).pcf.gz \
	                  $(COMPILED_DIR)/unifont_sample-$(VERSION).ttf \
	                  $(COMPILED_DIR)/coverage.txt \
	                  $(COMPILED_DIR)/unifont-$(VERSION).bmp \
	                  $(COMPILED_DIR)/unifont_upper-$(VERSION).hex \
	                  $(COMPILED_DIR)/unifont_upper-$(VERSION).bdf.gz \
	                  $(COMPILED_DIR)/unifont_upper-$(VERSION).pcf.gz \
	                  $(COMPILED_DIR)/unifont_upper-$(VERSION).ttf \
	                  $(COMPILED_DIR)/unifont_upper_csur-$(VERSION).hex \
	                  $(COMPILED_DIR)/unifont_upper_csur-$(VERSION).bdf.gz \
	                  $(COMPILED_DIR)/unifont_upper_csur-$(VERSION).pcf.gz \
	                  $(COMPILED_DIR)/unifont_upper_csur-$(VERSION).ttf \
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
	$(INSTALL) -m0755 -d $(CONSOLEDEST)
	$(INSTALL) -m0755 -d $(PCFDEST)
	$(INSTALL) -m0755 -d $(TTFDEST)
	if [ ! -d $(COMPILED_DIR) ] ; then \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/Unifont-APL8x16-$(VERSION).psf.gz $(CONSOLEDEST)/Unifont-APL8x16.psf.gz ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/unifont-$(VERSION).pcf.gz $(PCFDEST)/unifont.pcf.gz ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/unifont_sample-$(VERSION).pcf.gz $(PCFDEST)/unifont_sample.pcf.gz ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/unifont_csur-$(VERSION).pcf.gz     $(PCFDEST)/unifont_csur.pcf.gz ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/unifont-$(VERSION).ttf             $(TTFDEST)/unifont.ttf ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/unifont_sample-$(VERSION).ttf      $(TTFDEST)/unifont_sample.ttf ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/unifont_csur-$(VERSION).ttf        $(TTFDEST)/unifont_csur.ttf ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/unifont_upper-$(VERSION).ttf       $(TTFDEST)/unifont_upper.ttf ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/precompiled/unifont_upper_csur-$(VERSION).ttf  $(TTFDEST)/unifont_upper_csur.ttf ; \
	else \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/Unifont-APL8x16-$(VERSION).psf.gz $(CONSOLEDEST)/Unifont-APL8x16.psf.gz ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/unifont-$(VERSION).pcf.gz $(PCFDEST)/unifont.pcf.gz ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/unifont_sample-$(VERSION).pcf.gz    $(PCFDEST)/unifont_sample.pcf.gz ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/unifont_csur-$(VERSION).pcf.gz      $(PCFDEST)/unifont_csur.pcf.gz ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/unifont-$(VERSION).ttf              $(TTFDEST)/unifont.ttf ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/unifont_sample-$(VERSION).ttf       $(TTFDEST)/unifont_sample.ttf ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/unifont_csur-$(VERSION).ttf         $(TTFDEST)/unifont_csur.ttf ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/unifont_upper-$(VERSION).ttf        $(TTFDEST)/unifont_upper.ttf ; \
	   $(INSTALL) -m0644 -p $(CURDIR)/$(COMPILED_DIR)/unifont_upper_csur-$(VERSION).ttf   $(TTFDEST)/unifont_upper_csur.ttf ; \
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

.PHONY: all hex bdf pcf coverage pagecount bmp bigpic csurttf upper ttf \
	precompiled install clean distclean 
