from setuptools import setup, Extension

setup(ext_modules=[
    Extension(
        'gpgme._gpgme',
        ['lib/gpgme.c',
         'lib/pygpgme-error.c',
         'lib/pygpgme-data.c',
         'lib/pygpgme-context.c',
         'lib/pygpgme-key.c',
         'lib/pygpgme-signature.c',
         'lib/pygpgme-import.c',
         'lib/pygpgme-keyiter.c',
         'lib/pygpgme-constants.c',
         'lib/pygpgme-genkey.c',
         ],
        libraries=['gpgme'])
])
