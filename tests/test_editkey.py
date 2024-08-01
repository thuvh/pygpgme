# pygpgme - a Python wrapper for the gpgme library
# Copyright (C) 2006  James Henstridge
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

from io import BytesIO
import os
import unittest

import gpgme
import gpgme.editutil
from tests.util import GpgHomeTestCase

class EditKeyTestCase(GpgHomeTestCase):

    import_keys = ['key1.pub', 'key1.sec', 'key2.pub',
                   'signonly.pub', 'signonly.sec']

    status: gpgme.Status | None
    args: str | None

    def edit_quit_cb(self, status: gpgme.Status, args: str|None, fd: int) -> None:
        if status in [gpgme.Status.EOF, gpgme.Status.GOT_IT, gpgme.Status.KEY_CONSIDERED]:
            return
        self.status = status
        self.args = args
        os.write(fd, b'quit\n')

    def test_edit_quit(self) -> None:
        ctx = gpgme.Context()
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')
        output = BytesIO()

        self.status = None
        self.args = None
        with self.assertWarns(DeprecationWarning):
            ctx.edit(key, self.edit_quit_cb, output)

        self.assertEqual(self.status, gpgme.Status.GET_LINE)
        self.assertEqual(self.args, 'keyedit.prompt')

    def test_edit_ownertrust(self) -> None:
        ctx = gpgme.Context()
        key = ctx.get_key('93C2240D6B8AA10AB28F701D2CF46B7FC97E6B0F')
        self.assertEqual(key.owner_trust, gpgme.Validity.UNKNOWN)

        # try setting each validity:
        for trust in [gpgme.Validity.NEVER,
                      gpgme.Validity.MARGINAL,
                      gpgme.Validity.FULL,
                      gpgme.Validity.ULTIMATE]:
            with self.assertWarns(DeprecationWarning):
                gpgme.editutil.edit_trust(ctx, key, trust)
            key = ctx.get_key('93C2240D6B8AA10AB28F701D2CF46B7FC97E6B0F')
            self.assertEqual(key.owner_trust, trust)

    def test_edit_sign(self) -> None:
        ctx = gpgme.Context()
        # we set the keylist mode so we can see signatures
        ctx.keylist_mode = gpgme.KeylistMode.SIGS
        ctx.signers = [ctx.get_key('15E7CE9BF1771A4ABC550B31F540A569CB935A42')]
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')

        # check that there are no signatures from 0xCB935A42
        for uid in key.uids:
            sigs = [sig for sig in uid.signatures
                    if sig.keyid == 'F540A569CB935A42']
            self.assertEqual(len(sigs), 0)

        with self.assertWarns(DeprecationWarning):
            gpgme.editutil.edit_sign(ctx, key, check=0)
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')

        # check that there is a signature from 0xCB935A42 on each UID
        for uid in key.uids:
            sigs = [sig for sig in uid.signatures
                    if sig.keyid == 'F540A569CB935A42']
            self.assertEqual(len(sigs), 1)
