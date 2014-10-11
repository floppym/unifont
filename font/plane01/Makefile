
#
# Create the Plane 1 unassigned.hex file, with filler glyphs.
# Create filler glyphs in areas where Unicode scripts are not assigned;
# these are placed in directory "all/" so they are not included with
# the main font build, which reads "plane01/*.hex".
#
all: plane01.hex plane01-nonprinting.hex plane01-unassigned.hex
	if [ ! -d all ]; then \
		mkdir all; \
	fi
	../../bin/unihexfill < unassigned-ranges.txt > plane01-unassigned.hex
	../../bin/unihexfill < noscript-ranges.txt   > all/noscript.hex
	sort -u all/noscript.hex *.hex > all/plane01-all.hex

clean:

distclean:
	\rm -rf all

.PHONY: all clean distclean
