BJAM=./tools/jam/src/bin.macosxx86/bjam
BJAM_CONFIG=
PREFIX=/usr/local
EPREFIX=/usr/local
LIBDIR=/usr/local/lib
INCLUDEDIR=/usr/local/include
LIBS=

all: .dummy
	@echo "$(BJAM) $(BJAM_CONFIG) --user-config=user-config.jam $(LIBS)"
	@$(BJAM) $(BJAM_CONFIG) --user-config=user-config.jam $(LIBS) || \
	echo "Not all Boost libraries built properly."

clean: .dummy
	rm -rf bin.v2

distclean: clean
	rm -rf Makefile config.log

check: .dummy
	@cd status && ../$(BJAM) $(BJAM_CONFIG) --user-config=../user-config.jam || echo "Some Boost regression tests failed. This is normal for many compilers."

install: .dummy
	@echo "$(BJAM) $(BJAM_CONFIG) --user-config=user-config.jam --prefix=$(PREFIX) --exec-prefix=$(EPREFIX) --libdir=$(LIBDIR) --includedir=$(INCLUDEDIR) $(LIBS) install"
	@$(BJAM) $(BJAM_CONFIG) --user-config=user-config.jam --prefix=$(PREFIX) --exec-prefix=$(EPREFIX) --libdir=$(LIBDIR) --includedir=$(INCLUDEDIR) $(LIBS) install || echo "Not all Boost libraries built properly."

.dummy:

