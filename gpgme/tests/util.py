import os
import shutil
import tempfile
import unittest

import gpgme

class GpgHomeTestCase(unittest.TestCase):

    gpg_conf_contents = ''
    import_keys = []

    def setUp(self):
        self._gpghome = tempfile.mkdtemp(prefix='tmp.gpghome')
        os.environ['GNUPGHOME'] = self._gpghome
        fp = open(os.path.join(self._gpghome, 'gpg.conf'), 'wb')
        fp.write(self.gpg_conf_contents)
        fp.close()
        # XXX import keys

    def tearDown(self):
        del os.environ['GNUPGHOME']
        shutil.rmtree(self._gpghome, ignore_errors=True)
