#!/usr/bin/env python

from distutils.core import setup, Extension

gpgme = Extension(
    '_gpgme',
    ['src/gpgme.c',
     'src/pygpgme-error.c',
     'src/pygpgme-data.c',
     'src/pygpgme-context.c',
     'src/pygpgme-key.c'],
    libraries=['gpgme'])

setup(name='pygpgme',
      version='0.0',
      headers=['src/pygpgme.h'],
      py_modules=['gpgme.__init__'],
      ext_modules=[gpgme])
