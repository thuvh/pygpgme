import unittest
import os
import StringIO

import gpgme
import gpgme.editutil
from gpgme.tests.util import GpgHomeTestCase

class EditKeyTestCase(GpgHomeTestCase):

    import_keys = ['key1.pub', 'key1.sec', 'key2.pub']

    def edit_quit_cb(self, status, args, fd):
        if status in [gpgme.STATUS_EOF, gpgme.STATUS_GOT_IT]:
            return
        self.status = status
        self.args = args
        os.write(fd, 'quit\n')

    def test_edit_quit(self):
        ctx = gpgme.Context()
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')
        output = StringIO.StringIO()

        self.status = None
        self.args = None
        ctx.edit(key, self.edit_quit_cb, output)

        self.assertEqual(self.status, gpgme.STATUS_GET_LINE)
        self.assertEqual(self.args, 'keyedit.prompt')

    def test_edit_ownertrust(self):
        ctx = gpgme.Context()
        key = ctx.get_key('93C2240D6B8AA10AB28F701D2CF46B7FC97E6B0F')
        self.assertEqual(key.owner_trust, gpgme.VALIDITY_UNKNOWN)

        # try setting each validity:
        for trust in [gpgme.VALIDITY_NEVER,
                      gpgme.VALIDITY_MARGINAL,
                      gpgme.VALIDITY_FULL,
                      gpgme.VALIDITY_ULTIMATE]:
            gpgme.editutil.edit_trust(ctx, key, trust)
            key = ctx.get_key('93C2240D6B8AA10AB28F701D2CF46B7FC97E6B0F')
            self.assertEqual(key.owner_trust, trust)


def test_suite():
    loader = unittest.TestLoader()
    return loader.loadTestsFromName(__name__)
