#!/usr/bin/env python

from distutils.core import setup, Extension

gpgme = Extension(
    'gpgme._gpgme',
    ['src/gpgme.c'],
    libraries=['gpgme'])

setup(name='pygpgme',
      version='0.0',
      headers=['src/pygpgme.h'],
      ext_modules=[gpgme])
