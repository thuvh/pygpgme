import shlex
import subprocess

from setuptools import setup, Extension

def pkgconfig(*args):
    argv = ["pkg-config"] + list(args)
    output = subprocess.check_output(argv).decode('ASCII')
    return shlex.split(output)

gpgme_cflags = pkgconfig("--cflags", "gpgme >= 1.13.0")
gpgme_libs = pkgconfig("--libs", "gpgme >= 1.13.0")

setup(ext_modules=[
    Extension(
        'gpgme._gpgme',
        ['lib/gpgme.c',
         'lib/pygpgme-error.c',
         'lib/pygpgme-data.c',
         'lib/pygpgme-context.c',
         'lib/pygpgme-engine-info.c',
         'lib/pygpgme-key.c',
         'lib/pygpgme-signature.c',
         'lib/pygpgme-import.c',
         'lib/pygpgme-keyiter.c',
         'lib/pygpgme-constants.c',
         'lib/pygpgme-genkey.c',
         ],
        extra_compile_args=gpgme_cflags,
        extra_link_args=gpgme_libs)
])
