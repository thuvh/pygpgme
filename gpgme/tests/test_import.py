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

    def test_import_stringio(self):
        fp = StringIO.StringIO(self.keyfile('key1.pub').read())
        ctx = gpgme.Context()
        result = ctx.import_(fp)
        self.assertEqual(len(result.imports), 1)
        self.assertEqual(result.imports[0],
                         ('E79A842DA34A1CA383F64A1546BB55F0885C65A4',
                          None, gpgme.IMPORT_NEW))
        

def test_suite():
    loader = unittest.TestLoader()
    return loader.loadTestsFromName(__name__)
