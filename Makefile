# Makefile
#
# Copyright (C) 2003, 2004, 2005, 2006 Gwenhael LE MOINE
#
# This file is part of NGStar2
#
# NGStar2 is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# NGStar2 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with NGStar2; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

include Makefile.config

SUBDIRS = libngstar2 src

SUBDIRS2 = datas/levelsets
ifdef USE_FLTK2
	SUBDIRS2 += datas/themes
else
ifdef BUILD_EDITOR
	SUBDIRS2 += datas/themes
endif
endif

#~~~~~~~~~~~~~~~~~~#
#~~ Meta-targets ~~#
#~~~~~~~~~~~~~~~~~~#
all:
ifdef STATIC
	for dir in $(SUBDIRS); do \
		echo "=== making $$dir ===";\
		$(MAKE) -C $$dir static; \
	done
else
	for dir in $(SUBDIRS); do \
		echo "=== making $$dir ===";\
		$(MAKE) -C $$dir all; \
	done
endif

install:
ifdef STATIC
	for dir in $(SUBDIRS); do \
		echo "=== installing $$dir ===";\
		$(MAKE) -C $$dir install_static; \
	done
else
	for dir in $(SUBDIRS); do \
		echo "=== installing $$dir ===";\
		$(MAKE) -C $$dir install; \
	done
endif
	for dir in $(SUBDIRS2); do \
		echo "=== installing $$dir ===";\
		$(MAKE) -C $$dir install; \
	done

uninstall:
ifdef STATIC
	for dir in $(SUBDIRS); do \
		echo "=== uninstalling $$dir ===";\
		$(MAKE) -C $$dir uninstall_static; \
	done
else
	for dir in $(SUBDIRS); do \
		echo "=== uninstalling $$dir ===";\
		$(MAKE) -C $$dir uninstall; \
	done
endif
	for dir in $(SUBDIRS2); do \
		echo "=== uninstalling $$dir ===";\
		$(MAKE) -C $$dir uninstall; \
	done

clean:
	-rm -f $(WD)/*~ $(WD)/.kdbg*
	for dir in $(SUBDIRS); do \
		echo "=== cleaning $$dir ===";\
		$(MAKE) -C $$dir clean; \
	done
	for dir in $(SUBDIRS2); do \
		echo "=== cleaning $$dir ===";\
		$(MAKE) -C $$dir clean; \
	done

real-clean:
	$(MAKE) clean
	for dir in $(SUBDIRS); do \
		echo "=== real-cleaning $$dir ===";\
		$(MAKE) -C $$dir real-clean; \
	done
	for dir in $(SUBDIRS2); do \
		echo "=== real-cleaning $$dir ===";\
		$(MAKE) -C $$dir real-clean; \
	done

archive:
	$(MAKE) archive_bin_dynamic archive_src

archive_bin_dynamic:
	echo "=== building archive with dynamic binary ===";
	./configure --prefix $(prefix)
	$(MAKE) real-clean
	$(MAKE)
	$(MAKE) clean
	cd ..; \
	mv $(WD) $(join $(WD), -$(MAJOR).$(MINOR).$(REVISION) ) ; \
	tar jcf $(join $(join $(WD), -$(MAJOR).$(MINOR).$(REVISION)-bin), .tar.bz2) $(join $(shell basename $(WD)), -$(MAJOR).$(MINOR).$(REVISION) ); \
	tar zcf $(join $(join $(WD), -$(MAJOR).$(MINOR).$(REVISION)-bin), .tar.gz) $(join $(shell basename $(WD)), -$(MAJOR).$(MINOR).$(REVISION) ); \
	mv $(join $(WD), -$(MAJOR).$(MINOR).$(REVISION) ) $(WD) ; \
	cd $(WD)

archive_bin_static:
	echo "=== building archive with static binary ===";
	./configure --with-fltk2 --with-ncurses --enable-editor --enable-static --prefix $(prefix)
	$(MAKE) real-clean
	$(MAKE)
	$(MAKE) clean
	cd ..; \
	mv $(WD) $(join $(WD), -$(MAJOR).$(MINOR).$(REVISION) ) ; \
	tar jcf $(join $(join $(WD), -$(MAJOR).$(MINOR).$(REVISION)-bin_static), .tar.bz2) $(join $(shell basename $(WD)), -$(MAJOR).$(MINOR).$(REVISION) ); \
	tar zcf $(join $(join $(WD), -$(MAJOR).$(MINOR).$(REVISION)-bin_static), .tar.gz) $(join $(shell basename $(WD)), -$(MAJOR).$(MINOR).$(REVISION) ); \
	mv $(join $(WD), -$(MAJOR).$(MINOR).$(REVISION) ) $(WD) ; \
	cd $(WD)

archive_src:
	echo "=== building source-only archive ===";
	$(MAKE) real-clean
	cd ..; \
	mv $(WD) $(join $(WD), -$(MAJOR).$(MINOR).$(REVISION) ) ; \
	tar jcf $(join $(join $(WD), -$(MAJOR).$(MINOR).$(REVISION)-src), .tar.bz2) $(join $(shell basename $(WD)), -$(MAJOR).$(MINOR).$(REVISION) ); \
	tar zcf $(join $(join $(WD), -$(MAJOR).$(MINOR).$(REVISION)-src), .tar.gz) $(join $(shell basename $(WD)), -$(MAJOR).$(MINOR).$(REVISION) ); \
	mv $(join $(WD), -$(MAJOR).$(MINOR).$(REVISION) ) $(WD) ; \
	cd $(WD)

