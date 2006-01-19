import os
import shutil
import tempfile
import unittest

import gpgme

__all__ = ['GpgHomeTestCase']

keydir = os.path.join(os.path.dirname(__file__), 'keys')

class GpgHomeTestCase(unittest.TestCase):

    gpg_conf_contents = ''
    import_keys = []

    def keyfile(self, key):
        return open(os.path.join(keydir, key), 'rb')

    def setUp(self):
        self._gpghome = tempfile.mkdtemp(prefix='tmp.gpghome')
        os.environ['GNUPGHOME'] = self._gpghome
        fp = open(os.path.join(self._gpghome, 'gpg.conf'), 'wb')
        fp.write(self.gpg_conf_contents)
        fp.close()

        # import requested keys into the keyring
        ctx = gpgme.Context()
        for key in self.import_keys:
            ctx.import_(self.keyfile(key))

    def tearDown(self):
        del os.environ['GNUPGHOME']
        shutil.rmtree(self._gpghome, ignore_errors=True)
