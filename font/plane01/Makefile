
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
	set -e && \
	   grep "^[0-9A-Fa-f]" unassigned-ranges.txt | \
	   awk 'NF == 1 {print "../../bin/unihexgen " $$1 " " $$1;} \
		NF == 2 {print "../../bin/unihexgen " $$1 " " $$2;}' | \
	   /bin/sh -s > plane01-unassigned.hex
	set -e && \
	   grep "^[0-9A-Fa-f]" noscript-ranges.txt | \
	   awk 'NF == 1 {print "../../bin/unihexgen " $$1 " " $$1;} \
		NF == 2 {print "../../bin/unihexgen " $$1 " " $$2;}' | \
	   /bin/sh -s > all/noscript.hex
	sort -u all/noscript.hex *.hex > all/plane01-all.hex

clean:

distclean:
	\rm -rf all

.PHONY: all clean distclean
