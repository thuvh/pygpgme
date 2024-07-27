# pygpgme - a Python wrapper for the gpgme library
# Copyright (C) 2006  James Henstridge
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

import os
import shutil
import subprocess
import tempfile
from typing import BinaryIO
import unittest

import gpgme

__all__ = ['GpgHomeTestCase']

keydir = os.path.join(os.path.dirname(__file__), 'keys')

class GpgHomeTestCase(unittest.TestCase):

    import_keys: list[str] = []

    def keyfile(self, key: str) -> BinaryIO:
        return open(os.path.join(keydir, key), 'rb')

    def setUp(self) -> None:
        self._gpghome = tempfile.mkdtemp(prefix='tmp.gpghome')
        os.environ['GNUPGHOME'] = self._gpghome
        with open(os.path.join(self._gpghome, 'gpg.conf'), 'w') as fp:
            fp.write('pinentry-mode loopback\n')
        with open(os.path.join(self._gpghome, 'gpg-agent.conf'), 'w') as fp:
            fp.write('allow-loopback-pinentry\n')
        subprocess.check_call(['gpg-connect-agent', '/bye'],
                              stdout=subprocess.DEVNULL,
                              stderr=subprocess.DEVNULL)

        # import requested keys into the keyring
        ctx = gpgme.Context()
        for key in self.import_keys:
            with self.keyfile(key) as fp:
                ctx.import_(fp)

    def tearDown(self) -> None:
        # May fail if the agent is not currently running.
        subprocess.call(['gpg-connect-agent', 'KILLAGENT', '/bye'],
                              stdout=subprocess.DEVNULL,
                              stderr=subprocess.DEVNULL)
        del os.environ['GNUPGHOME']
        shutil.rmtree(self._gpghome, ignore_errors=True)
