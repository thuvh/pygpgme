import unittest

import gpgme
from gpgme.tests.util import GpgHomeTestCase

class ContextTestCase(GpgHomeTestCase):

    def test_constructor(self):
        ctx = gpgme.Context()

    def test_protocol(self):
        ctx = gpgme.Context()
        # XXX: this should use symbolic constant names
        self.assertEqual(ctx.protocol, 0) # OpenPGP
        ctx.protocol = 1 # CMS
        self.assertEqual(ctx.protocol, 1)
        ctx.protocol = 0 # CMS
        self.assertEqual(ctx.protocol, 0)

        # check error on setting to invalid protocol value
        def set_protocol(ctx, value):
            ctx.protocol = value
        self.assertRaises(gpgme.error, set_protocol, ctx, 999)

    def test_armor(self):
        ctx = gpgme.Context()
        self.assertEqual(ctx.armor, False)
        ctx.armor = True
        self.assertEqual(ctx.armor, True)
        ctx.armor = False
        self.assertEqual(ctx.armor, False)

    def test_textmode(self):
        ctx = gpgme.Context()
        self.assertEqual(ctx.textmode, False)
        ctx.textmode = True
        self.assertEqual(ctx.textmode, True)
        ctx.textmode = False
        self.assertEqual(ctx.textmode, False)

    def test_include_certs(self):
        ctx = gpgme.Context()
        self.assertEqual(ctx.include_certs, 1)
        ctx.include_certs = 2
        self.assertEqual(ctx.include_certs, 2)

    def test_keylist_mode(self):
        ctx = gpgme.Context()
        self.assertEqual(ctx.keylist_mode, 1)
        ctx.keylist_mode = 2
        self.assertEqual(ctx.keylist_mode, 2)
        ctx.keylist_mode = 1 | 2
        self.assertEqual(ctx.keylist_mode, 1 | 2)

        # check error on setting to invalid protocol value
        def set_keylist_mode(ctx, value):
            ctx.keylist_mode = value
        self.assertRaises(gpgme.error, set_keylist_mode, ctx, 128)

    def test_passphrase_cb(self):
        ctx = gpgme.Context()
        def passphrase_cb(uid_hint, passphrase_info, prev_was_bad, fd):
            pass
        self.assertEqual(ctx.passphrase_cb, None)
        ctx.passphrase_cb = passphrase_cb
        self.assertEqual(ctx.passphrase_cb, passphrase_cb)
        ctx.passphrase_cb = None
        self.assertEqual(ctx.passphrase_cb, None)
        ctx.passphrase_cb = passphrase_cb
        del ctx.passphrase_cb
        self.assertEqual(ctx.passphrase_cb, None)

    def test_progress_cb(self):
        ctx = gpgme.Context()
        def progress_cb(what, type, current, total):
            pass
        self.assertEqual(ctx.progress_cb, None)
        ctx.progress_cb = progress_cb
        self.assertEqual(ctx.progress_cb, progress_cb)
        ctx.progress_cb = None
        self.assertEqual(ctx.progress_cb, None)
        ctx.progress_cb = progress_cb
        del ctx.progress_cb
        self.assertEqual(ctx.progress_cb, None)


def test_suite():
    loader = unittest.TestLoader()
    return loader.loadTestsFromName(__name__)
