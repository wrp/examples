
.PHONY: foo
foo:
	m4 -d -D fatal_if_undef=$$'ifdef(`$$1\',DEFINED,\
		`errprint(fatal: ``$$1\'\' undefined\n)m4exit(1)\')' \
		-D value='defined in makefile' input

#j.	m4 -D fatal_if_undef=$$'ifkdef(`$$1\',,errprint(fatal: `$$1\' undefined\n)'
#		m4exit(1)))' -D value='defined in makefile' input
