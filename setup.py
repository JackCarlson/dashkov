from distutils.core import setup, Extension

dashkov_mod = Extension("dashkov", ["clientwrapper.cpp", "dashkov.cpp"])

setup(name = "dashkov", ext_modules=[dashkov_mod])
