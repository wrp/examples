
bin_PROGRAMS = run
man1_MANS = run.1

CLEANFILES = *.gcda *.gcno *.gcov

EXTRA_DIST = version build-aux/package-version run.txt run.1
.PHONY: version
version:
	git describe --tags --always --dirty > version

run.1: run.txt
	asciidoctor -a ver=${PACKAGE_VERSION} -b manpage $<
