from setuptools import setup, Extension

setup(ext_modules=[
    Extension(
        'gpgme._gpgme',
        ['src/gpgme.c',
         'src/pygpgme-error.c',
         'src/pygpgme-data.c',
         'src/pygpgme-context.c',
         'src/pygpgme-key.c',
         'src/pygpgme-signature.c',
         'src/pygpgme-import.c',
         'src/pygpgme-keyiter.c',
         'src/pygpgme-constants.c',
         'src/pygpgme-genkey.c',
         ],
        libraries=['gpgme'])
])
