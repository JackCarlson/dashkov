from distutils.core import setup, Extension

dashkov_mod = Extension(
	"dashkov", 
	[
		"clientwrapper.cpp", 
		"dashkov.cpp", 
		"dbword.cpp"
	], 
	extra_compile_args=['-std=c++11', '-lsqlite3'], 
	extra_link_args=['-lsqlite3']
)

setup(name = "dashkov", ext_modules=[dashkov_mod])
