import unittest
import os
import StringIO
from textwrap import dedent

import gpgme
from gpgme.tests.util import GpgHomeTestCase

class ProgressTestCase(GpgHomeTestCase):

    import_keys = ['key1.pub', 'key1.sec']

    def progress_cb(self, what, type_, current, total):
        self.progress_cb_called = True

    def test_sign_with_progress_cb(self):
        ctx = gpgme.Context()
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')
        ctx.signers = [key]
        ctx.progress_cb = self.progress_cb
        plaintext = StringIO.StringIO('Hello World\n')
        signature = StringIO.StringIO()

        self.progress_cb_called = False
        new_sigs = ctx.sign(plaintext, signature, gpgme.SIG_MODE_CLEAR)

        # ensure that progress_cb has been run
        self.assertEqual(self.progress_cb_called, True)

        self.assertEqual(new_sigs[0].type, gpgme.SIG_MODE_CLEAR)
        self.assertEqual(new_sigs[0].fpr,
                        'E79A842DA34A1CA383F64A1546BB55F0885C65A4')

def test_suite():
    loader = unittest.TestLoader()
    return loader.loadTestsFromName(__name__)
