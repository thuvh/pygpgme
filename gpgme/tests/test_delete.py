import unittest
import StringIO

import gpgme
from gpgme.tests.util import GpgHomeTestCase

class DeleteTestCase(GpgHomeTestCase):

    import_keys = ['key1.pub', 'key1.sec', 'key2.pub']

    def test_delete_public_key(self):
        ctx = gpgme.Context()
        # key2
        key = ctx.get_key('93C2240D6B8AA10AB28F701D2CF46B7FC97E6B0F')
        ctx.delete(key)

        # check that it is deleted
        self.assertRaises(gpgme.GpgmeError, ctx.get_key,
                          '93C2240D6B8AA10AB28F701D2CF46B7FC97E6B0F')

    def test_delete_public_key_with_secret_key(self):
        ctx = gpgme.Context()
        # key1
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')
        self.assertRaises(gpgme.GpgmeError, ctx.delete, key)

    def test_delete_secret_key(self):
        ctx = gpgme.Context()
        # key1
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')
        ctx.delete(key, True)

    def test_delete_non_existant(self):
        ctx = gpgme.Context()
        # key2
        key = ctx.get_key('93C2240D6B8AA10AB28F701D2CF46B7FC97E6B0F')
        ctx.delete(key)

        # delete it again
        try:
            ctx.delete(key)
        except gpgme.GpgmeError, exc:
            self.assertEqual(exc[0], gpgme.ERR_SOURCE_GPGME)
            self.assertEqual(exc[1], gpgme.ERR_NO_PUBKEY)
        else:
            self.fail('gpgme.GpgmeError was not raised')


def test_suite():
    loader = unittest.TestLoader()
    return loader.loadTestsFromName(__name__)
