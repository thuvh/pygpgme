import unittest
import StringIO

import gpgme
from gpgme.tests.util import GpgHomeTestCase

class ImportTestCase(GpgHomeTestCase):

    def test_import_file(self):
        fp = self.keyfile('key1.pub')
        ctx = gpgme.Context()
        result = ctx.import_(fp)
        self.assertEqual(result.considered, 1)
        self.assertEqual(result.no_user_id, 0)
        self.assertEqual(result.imported, 1)
        self.assertEqual(result.imported_rsa, 0)
        self.assertEqual(result.unchanged, 0)
        self.assertEqual(result.new_user_ids, 0)
        self.assertEqual(result.new_sub_keys, 0)
        self.assertEqual(result.new_signatures, 0)
        self.assertEqual(result.new_revocations, 0)
        self.assertEqual(result.secret_read, 0)
        self.assertEqual(result.secret_imported, 0)
        self.assertEqual(result.secret_unchanged, 0)
        self.assertEqual(result.skipped_new_keys, 0)
        self.assertEqual(result.not_imported, 0)
        self.assertEqual(len(result.imports), 1)
        self.assertEqual(result.imports[0],
                         ('E79A842DA34A1CA383F64A1546BB55F0885C65A4',
                          None, gpgme.IMPORT_NEW))
        # can we get the public key?
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')

    def test_import_secret_file(self):
        fp = self.keyfile('key1.sec')
        ctx = gpgme.Context()
        result = ctx.import_(fp)
        self.assertEqual(result.considered, 1)
        self.assertEqual(result.no_user_id, 0)
        self.assertEqual(result.imported, 1)
        self.assertEqual(result.imported_rsa, 0)
        self.assertEqual(result.unchanged, 0)
        self.assertEqual(result.new_user_ids, 0)
        self.assertEqual(result.new_sub_keys, 0)
        self.assertEqual(result.new_signatures, 0)
        self.assertEqual(result.new_revocations, 0)
        self.assertEqual(result.secret_read, 1)
        self.assertEqual(result.secret_imported, 1)
        self.assertEqual(result.secret_unchanged, 0)
        self.assertEqual(result.skipped_new_keys, 0)
        self.assertEqual(result.not_imported, 0)
        self.assertEqual(len(result.imports), 2)
        self.assertEqual(result.imports[0],
                         ('E79A842DA34A1CA383F64A1546BB55F0885C65A4',
                          None, gpgme.IMPORT_NEW | gpgme.IMPORT_SECRET))
        self.assertEqual(result.imports[1],
                         ('E79A842DA34A1CA383F64A1546BB55F0885C65A4',
                          None, gpgme.IMPORT_NEW))
        # can we get the public key?
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')
        # can we get the secret key?
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4', True)

    def test_import_stringio(self):
        fp = StringIO.StringIO(self.keyfile('key1.pub').read())
        ctx = gpgme.Context()
        result = ctx.import_(fp)
        self.assertEqual(len(result.imports), 1)
        self.assertEqual(result.imports[0],
                         ('E79A842DA34A1CA383F64A1546BB55F0885C65A4',
                          None, gpgme.IMPORT_NEW))
        # can we get the public key?
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')

    def test_import_concat(self):
        keys = '\n'.join([self.keyfile('key1.pub').read(),
                          self.keyfile('key1.sec').read(),
                          self.keyfile('key2.pub').read()])
        fp = StringIO.StringIO(keys)
        ctx = gpgme.Context()
        result = ctx.import_(fp)
        self.assertEqual(result.considered, 3)
        self.assertEqual(result.no_user_id, 0)
        self.assertEqual(result.imported, 2)
        self.assertEqual(result.imported_rsa, 1)
        self.assertEqual(result.unchanged, 0)
        self.assertEqual(result.new_user_ids, 0)
        self.assertEqual(result.new_sub_keys, 0)
        self.assertEqual(result.new_signatures, 1)
        self.assertEqual(result.new_revocations, 0)
        self.assertEqual(result.secret_read, 1)
        self.assertEqual(result.secret_imported, 1)
        self.assertEqual(result.secret_unchanged, 0)
        self.assertEqual(result.skipped_new_keys, 0)
        self.assertEqual(result.not_imported, 0)
        self.assertEqual(len(result.imports), 4)
        self.assertEqual(result.imports[0],
                         ('E79A842DA34A1CA383F64A1546BB55F0885C65A4',
                          None, gpgme.IMPORT_NEW))
        self.assertEqual(result.imports[1],
                         ('E79A842DA34A1CA383F64A1546BB55F0885C65A4',
                          None, gpgme.IMPORT_NEW | gpgme.IMPORT_SECRET))
        self.assertEqual(result.imports[2],
                         ('E79A842DA34A1CA383F64A1546BB55F0885C65A4',
                          None, gpgme.IMPORT_SIG))
        self.assertEqual(result.imports[3],
                         ('93C2240D6B8AA10AB28F701D2CF46B7FC97E6B0F',
                          None, gpgme.IMPORT_NEW))
        # can we get the public keys?
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')
        key = ctx.get_key('93C2240D6B8AA10AB28F701D2CF46B7FC97E6B0F')
        # can we get the secret key?
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4', True)

    def test_import_empty(self):
        fp = StringIO.StringIO('')
        ctx = gpgme.Context()
        result = ctx.import_(fp)
        self.assertEqual(result.considered, 0)
        self.assertEqual(len(result.imports), 0)

    def test_import_twice(self):
        ctx = gpgme.Context()
        fp = self.keyfile('key1.pub')
        result = ctx.import_(fp)

        fp = self.keyfile('key1.pub')
        result = ctx.import_(fp)
        
        self.assertEqual(result.considered, 1)
        self.assertEqual(result.no_user_id, 0)
        self.assertEqual(result.imported, 0)
        self.assertEqual(result.imported_rsa, 0)
        self.assertEqual(result.unchanged, 1)
        self.assertEqual(result.new_user_ids, 0)
        self.assertEqual(result.new_sub_keys, 0)
        self.assertEqual(result.new_signatures, 0)
        self.assertEqual(result.new_revocations, 0)
        self.assertEqual(result.secret_read, 0)
        self.assertEqual(result.secret_imported, 0)
        self.assertEqual(result.secret_unchanged, 0)
        self.assertEqual(result.skipped_new_keys, 0)
        self.assertEqual(result.not_imported, 0)
        self.assertEqual(len(result.imports), 1)
        self.assertEqual(result.imports[0],
                         ('E79A842DA34A1CA383F64A1546BB55F0885C65A4', None, 0))
        # can we get the public key?
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')

def test_suite():
    loader = unittest.TestLoader()
    return loader.loadTestsFromName(__name__)
