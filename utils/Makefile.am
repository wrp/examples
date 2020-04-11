
bin_PROGRAMS = run

CLEANFILES = *.gcda *.gcno *.gcov

EXTRA_DIST = version build-aux/package-version
.PHONY: version
version:
	git describe --tags --always --dirty > version
