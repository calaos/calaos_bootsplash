MAINTAINERCLEANFILES = Makefile.in
EXTRA_DIST = theme.edc images.edc

EDJE_CC = edje_cc
EDJE_FLAGS = \
-v \
-id $(top_srcdir)/data/images \
-fd $(top_srcdir)/data/fonts

defaultdir = $(pkgdatadir)
default_DATA = default.edj

default.edj: Makefile theme.edc images.edc
	$(EDJE_CC) $(EDJE_FLAGS) \
	$(top_srcdir)/data/theme.edc \
	$(top_builddir)/data/default.edj

clean-local:
	rm -f *.edj
