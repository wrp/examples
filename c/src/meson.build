project(
	'c-examples',
	'c',
	meson_version : '>= 1.3.0',
	version : '0.1',
	default_options : ['warning_level=3', 'c_std=c23'],
)


cc = meson.get_compiler('c')
ldbl_size = cc.sizeof('long double')
summary('long double size', ldbl_size)
add_project_arguments(cc.get_supported_arguments(['-Wno-gnu-designator']),
	language: 'c')

rb = static_library('ring-buffer', sources: ['libring-buffer.c'])
stack = static_library('stack', sources: ['libstack.c'])
bindgen = static_library('bindgen', sources: ['libwrp-bindgen.c'])
xutil = static_library('xutil', sources: ['libxutil.c'])
dot = include_directories('.')

libxutil = declare_dependency(link_with: xutil, include_directories: dot)
librb = declare_dependency(link_with: rb, include_directories: dot)
libstack = declare_dependency(link_with: stack, include_directories: dot)

sqlite3 = dependency('sqlite3', required: false)


foreach name : [
	'arithperm', 'array-declaration', 'array', 'assert', 'atexit', 'bst',
	'bstee', 'casting', 'char-cast', 'check-dups', 'cleanup', 'clearerr',
	'complex', 'compound-initializer', 'ctime', 'digraphs', 'double',
	'dynamic-int', 'filter', 'fizzbuzz', 'float', 'fmemopen', 'fopen',
	'fork-multiply', 'fork-pipe', 'fork-pool', 'fork', 'forkexec',
	'forkpty', 'function-ptr', 'function-ptr2', 'function-ptr3', 'generic',
	'getc', 'getcwd', 'heapify', 'hello', 'http-client', 'initializer',
	'interleave-input', 'isprint', 'jump-table', 'kaprekar', 'limits',
	'link', 'ls-sort', 'ls', 'min-max-heap', 'mkstemp', 'morse', 'morse2',
	'mv', 'nextafter', 'nftw', 'nftw2', 'open', 'opendir', 'palindrome',
	'pipehup', 'poll', 'popen', 'pow', 'prctl', 'primes', 'printf',
	'pyramid', 'qsort', 'rand', 'read-text-into-char-ptr-ptr',
	'read-txt-into-array', 'realloc', 'realpath', 'remove-common-word',
	'replicate-tree', 'scanf-term', 'seek', 'select', 'setitimer',
	'shebang', 'show-mask', 'sieve', 'simple-dc', 'snprintf',
	'sort-by-length', 'sort-generic', 'sortdir', 'standard', 'stat',
	'stop-watch', 'strcspn', 'strftime', 'strlen', 'strncat', 'strncmp',
	'strncpy', 'strptime', 'strtod', 'strtok', 'strtol', 'tcp-client',
	'tcp-server', 'termios', 'ternary', 'time', 'truncate', 'tsearch',
	'udp-client', 'udp-server', 'unblock-signals', 'unix-client',
	'unix-server', 'va_args',
]
	executable(name, name + '.c')
endforeach

executable('calc', 'calc.c', dependencies: [librb, libstack])

foreach name : [
	'access', 'cat', 'chmod', 'diff16', 'dynamic-array', 'dynamic-read',
	'dynamic', 'fgetwc', 'fread', 'fseek', 'fsync', 'getdelim', 'grow',
	'head', 'list', 'load-matrix', 'mmap', 'poly', 'procname',
	'random-word', 'read-indexed', 'read', 'scanf', 'sed1q', 'sigaction',
	'tac',
]
	executable(name, name + '.c', dependencies: [libxutil])
endforeach

if sqlite3.found()
	executable('sqlite', 'sqlite.c', dependencies: [sqlite3])
endif


wd = meson.project_build_root()
test('calc-test',
	find_program('../test/calc-test'),
	workdir: meson.project_source_root(),
	env: ['LONG_DOUBLE_SIZE=@0@'.format(ldbl_size)],
)
foreach name : [
	'arithperm-test', 'bst-test', 'check-dups-test', 'dynamic-array-test',
	'dynamic-read-test', 'dynamic-test', 'fread-test', 'grow-test',
	'list-test', 'mmap-test', 'poly-test', 'pyramid-test',
	'read-indexed-test', 'read-txt-into-array-test', 'scanf-term-test',
	'scanf-test', 'tac-test', 'udp-client-test',
]
	test(name, find_program('../test/' + name), workdir: wd,
		env: ['srcdir=@0@'.format(meson.project_source_root())],
	)
endforeach

if sqlite3.found()
	test('sqlite-test', find_program('../test/sqlite-test'), workdir: wd)
endif

test('coverage-test', find_program('../test/coverage-test'),
	workdir: wd, is_parallel: false, priority: -1)
