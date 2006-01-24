import unittest
import StringIO
from textwrap import dedent

import gpgme
from gpgme.tests.util import GpgHomeTestCase

class ExportTestCase(GpgHomeTestCase):

    import_keys = ['signonly.pub', 'signonly.sec']

    def test_export_by_fingerprint(self):
        ctx = gpgme.Context()
        ctx.armor = True
        keydata = StringIO.StringIO()
        ctx.export('15E7CE9BF1771A4ABC550B31F540A569CB935A42', keydata)

        self.assertTrue(keydata.getvalue().startswith(
            '-----BEGIN PGP PUBLIC KEY BLOCK-----\n'))
        
    def test_export_by_email(self):
        ctx = gpgme.Context()
        ctx.armor = True
        keydata = StringIO.StringIO()
        ctx.export('signonly@example.org', keydata)

        self.assertTrue(keydata.getvalue().startswith(
            '-----BEGIN PGP PUBLIC KEY BLOCK-----\n'))

    def test_export_by_name(self):
        ctx = gpgme.Context()
        ctx.armor = True
        keydata = StringIO.StringIO()
        ctx.export('Sign Only', keydata)

        self.assertTrue(keydata.getvalue().startswith(
            '-----BEGIN PGP PUBLIC KEY BLOCK-----\n'))


def test_suite():
    loader = unittest.TestLoader()
    return loader.loadTestsFromName(__name__)
