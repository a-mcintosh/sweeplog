
EXTRA_DIST	= example-cfg.json nomacro.pl

bin_PROGRAMS	= scan

dist_man_MANS	= scan.1

scan_SOURCES	= scan.c
if USE_ASM
endif
scan_LDFLAGS	= $(PTHREAD_FLAGS)
scan_LDADD	= @LIBCURL@ @PTHREAD_LIBS@ @WS2_LIBS@
scan_CFLAGS	= -fno-strict-aliasing
scan_CPPFLAGS	= @LIBCURL_CPPFLAGS@ $(PTHREAD_FLAGS)

