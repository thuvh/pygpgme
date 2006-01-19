#!/usr/bin/env python

from distutils.core import setup, Extension

gpgme = Extension(
    'gpgme._gpgme',
    ['src/gpgme.c',
     'src/pygpgme-error.c',
     'src/pygpgme-data.c',
     'src/pygpgme-context.c',
     'src/pygpgme-key.c',
     'src/pygpgme-signature.c',
     'src/pygpgme-import.c',
     'src/pygpgme-keyiter.c',
     ],
    libraries=['gpgme'])

setup(name='pygpgme',
      version='0.0',
      author='James Henstridge',
      author_email='james@jamesh.id.au',
      ext_modules=[gpgme],
      packages=['gpgme', 'gpgme.tests'],
      package_data={'gpgme.tests': ['keys/*.pub', 'keys/*.sec']})
