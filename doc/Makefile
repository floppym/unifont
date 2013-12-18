
SHELL = /bin/sh

MAKEINFO = makeinfo
TEXI2PDF = texi2pdf

OUTPUT_FILES = unifont.info unifont.txt unifont.pdf

.SUFFIXES:
.SUFFIXES: .texi .txt .info .pdf

.texi.info:
	$(MAKEINFO) $< -o $@

.texi.txt:
	$(MAKEINFO) --plaintext $< -o $@

.texi.pdf:
	$(TEXI2PDF) $<

#
# Comment out the "all:" line to make the documents.  The default is
# to leave the existing unifont.info and unifont.pdf files in place,
# in case the target system doesn't have makeinfo and texi2pdf installed.
#
all:
doc: $(OUTPUT_FILES)

install:

clean:
	\rm -f unifont.aux unifont.cp unifont.fn unifont.ky unifont.log
	\rm -f unifont.pg unifont.toc unifont.tp unifont.vr
	\rm -f *~

#
# Only uncomment the line to remove $(OUTPUT_FILES) if your system has
# texinfo, texi2pdf, TeX, and other necessary software installed to
# rebuild the documentation from scratch.
#
distclean: clean
#	\rm -f $(OUTPUT_FILES)

.PHONY: all install clean distclean
