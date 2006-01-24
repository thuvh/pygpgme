import unittest
import os
import StringIO
from textwrap import dedent

import gpgme
from gpgme.tests.util import GpgHomeTestCase

class PassphraseTestCase(GpgHomeTestCase):

    import_keys = ['passphrase.pub', 'passphrase.sec']

    def test_sign_without_passphrase_cb(self):
        ctx = gpgme.Context()
        key = ctx.get_key('EFB052B4230BBBC51914BCBB54DCBBC8DBFB9EB3')
        ctx.signers = [key]
        plaintext = StringIO.StringIO('Hello World\n')
        signature = StringIO.StringIO()

        try:
            new_sigs = ctx.sign(plaintext, signature, gpgme.SIG_MODE_CLEAR)
        except gpgme.error, e:
            self.assertEqual(e[1], 'Bad passphrase')
        else:
            self.fail('gpgme.error not raised')

    def passphrase_cb(self, uid_hint, passphrase_info, prev_was_bad, fd):
        print "uid_hint", repr(uid_hint)
        print "passphrase_info", repr(passphrase_info)
        print "prev_was_bad", repr(prev_was_bad)
        os.write(fd, 'test\n')

    def test_sign_with_passphrase_cb(self):
        ctx = gpgme.Context()
        key = ctx.get_key('EFB052B4230BBBC51914BCBB54DCBBC8DBFB9EB3')
        ctx.signers = [key]
        ctx.passphrase_cb = self.passphrase_cb
        plaintext = StringIO.StringIO('Hello World\n')
        signature = StringIO.StringIO()

        new_sigs = ctx.sign(plaintext, signature, gpgme.SIG_MODE_CLEAR)

def test_suite():
    loader = unittest.TestLoader()
    return loader.loadTestsFromName(__name__)
