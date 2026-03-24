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
add_project_arguments(cc.get_supported_arguments(['-Wno-gnu-designator']), language: 'c')

rb = static_library('ring-buffer', sources: ['libring-buffer.c'])
stack = static_library('stack', sources: ['libstack.c'])
bindgen = static_library('bindgen', sources: ['libwrp-bindgen.c'])
xutil = static_library('xutil', sources: ['libxutil.c'])
dot = include_directories('.')

libxutil = declare_dependency(link_with: xutil, include_directories: dot)
librb = declare_dependency(link_with: rb, include_directories: dot)
libstack = declare_dependency(link_with: stack, include_directories: dot)

sqlite3 = dependency('sqlite3', required: false)


names = [
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

foreach name : names
	executable(name, name + '.c')
endforeach


executable('access', 'access.c', dependencies: [libxutil])
executable('calc', 'calc.c', dependencies: [librb, libstack])
executable('cat', 'cat.c', dependencies: [libxutil])
executable('chmod', 'chmod.c', dependencies: [libxutil])
executable('diff16', 'diff16.c', dependencies: [libxutil])
executable('dynamic-array', 'dynamic-array.c', dependencies: [libxutil])
executable('dynamic-read', 'dynamic-read.c', dependencies: [libxutil])
executable('dynamic', 'dynamic.c', dependencies: [libxutil])
executable('fgetwc', 'fgetwc.c', dependencies: [libxutil])
executable('fread', 'fread.c', dependencies: [libxutil])
executable('fseek', 'fseek.c', dependencies: [libxutil])
executable('fsync', 'fsync.c', dependencies: [libxutil])
executable('getdelim', 'getdelim.c', dependencies: [libxutil])
executable('grow', 'grow.c', dependencies: [libxutil])
executable('head', 'head.c', dependencies: [libxutil])
executable('list', 'list.c', dependencies: [libxutil])
executable('load-matrix', 'load-matrix.c', dependencies: [libxutil])
executable('mmap', 'mmap.c', dependencies: [libxutil])
executable('poly', 'poly.c', dependencies: [libxutil])
executable('procname', 'procname.c', dependencies: [libxutil])
executable('random-word', 'random-word.c', dependencies: [libxutil])
executable('read-indexed', 'read-indexed.c', dependencies: [libxutil])
executable('read', 'read.c', dependencies: [libxutil])
executable('scanf', 'scanf.c', dependencies: [libxutil])
executable('sed1q', 'sed1q.c', dependencies: [libxutil])
executable('sigaction', 'sigaction.c', dependencies: [libxutil])
if sqlite3.found()
	executable('sqlite', 'sqlite.c', dependencies: [sqlite3])
endif
executable('tac', 'tac.c', dependencies: [libxutil])


wd = meson.project_build_root()
test('calc-test',
	find_program('../test/calc-test'),
	workdir: meson.project_source_root(),
	env: ['LONG_DOUBLE_SIZE=@0@'.format(ldbl_size)],
)
test('arithperm-test', find_program('../test/arithperm-test'), workdir: wd)
test('bst-test', find_program('../test/bst-test'), workdir: wd)
test('check-dups-test', find_program('../test/check-dups-test'), workdir: wd)
test('coverage-test', find_program('../test/coverage-test'), workdir: wd)
test('dynamic-array-test', find_program('../test/dynamic-array-test'), workdir: wd)
test('dynamic-read-test', find_program('../test/dynamic-read-test'), workdir: wd)
test('dynamic-test', find_program('../test/dynamic-test'), workdir: wd)
test('fread-test', find_program('../test/fread-test'), workdir: wd,
	env: ['srcdir=@0@'.format(meson.project_source_root())])
test('grow-test', find_program('../test/grow-test'), workdir: wd)
test('list-test', find_program('../test/list-test'), workdir: wd)
test('mmap-test', find_program('../test/mmap-test'), workdir: wd)
test('poly-test', find_program('../test/poly-test'), workdir: wd)
test('pyramid-test', find_program('../test/pyramid-test'), workdir: wd)
test('read-indexed-test', find_program('../test/read-indexed-test'), workdir: wd)
test('read-txt-into-array-test', find_program('../test/read-txt-into-array-test'), workdir: wd)
test('scanf-term-test', find_program('../test/scanf-term-test'), workdir: wd)
test('scanf-test', find_program('../test/scanf-test'), workdir: wd)
if sqlite3.found()
	test('sqlite-test', find_program('../test/sqlite-test'), workdir: wd)
endif
test('tac-test', find_program('../test/tac-test'), workdir: wd)
test('udp-client-test', find_program('../test/udp-client-test'), workdir: wd)
