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

import os
from typing import Optional
import unittest

import gpgme
from tests.util import GpgHomeTestCase

class ContextTestCase(GpgHomeTestCase):

    def test_constructor(self) -> None:
        ctx = gpgme.Context()

    def test_protocol(self) -> None:
        ctx = gpgme.Context()
        self.assertEqual(ctx.protocol, gpgme.Protocol.OpenPGP)
        ctx.protocol = gpgme.Protocol.CMS
        self.assertEqual(ctx.protocol, gpgme.Protocol.CMS)
        ctx.protocol = gpgme.Protocol.OpenPGP
        self.assertEqual(ctx.protocol, gpgme.Protocol.OpenPGP)

        # check error on setting to invalid protocol value
        def set_protocol(ctx: gpgme.Context, value: gpgme.Protocol) -> None:
            ctx.protocol = value
        self.assertRaises(gpgme.GpgmeError, set_protocol, ctx, 999)

        def del_protocol(ctx: gpgme.Context) -> None:
            del ctx.protocol
        self.assertRaises(AttributeError, del_protocol, ctx)

    def test_armor(self) -> None:
        ctx = gpgme.Context()
        self.assertEqual(ctx.armor, False)
        ctx.armor = True
        self.assertEqual(ctx.armor, True)
        ctx.armor = False
        self.assertEqual(ctx.armor, False)

        def del_armor(ctx: gpgme.Context) -> None:
            del ctx.armor
        self.assertRaises(AttributeError, del_armor, ctx)

    def test_textmode(self) -> None:
        ctx = gpgme.Context()
        self.assertEqual(ctx.textmode, False)
        ctx.textmode = True
        self.assertEqual(ctx.textmode, True)
        ctx.textmode = False
        self.assertEqual(ctx.textmode, False)

        def del_textmode(ctx: gpgme.Context) -> None:
            del ctx.textmode
        self.assertRaises(AttributeError, del_textmode, ctx)

    def test_offline(self) -> None:
        ctx = gpgme.Context()
        self.assertEqual(ctx.offline, False)
        ctx.offline = True
        self.assertEqual(ctx.offline, True)
        ctx.offline = False
        self.assertEqual(ctx.offline, False)
        with self.assertRaises(AttributeError):
            del ctx.offline

    def test_include_certs(self) -> None:
        ctx = gpgme.Context()
        self.assertEqual(ctx.include_certs, -256)
        ctx.include_certs = 2
        self.assertEqual(ctx.include_certs, 2)

        def del_include_certs(ctx: gpgme.Context) -> None:
            del ctx.include_certs
        self.assertRaises(AttributeError, del_include_certs, ctx)

    def test_keylist_mode(self) -> None:
        ctx = gpgme.Context()
        self.assertEqual(ctx.keylist_mode, gpgme.KeylistMode.LOCAL)
        ctx.keylist_mode = gpgme.KeylistMode.EXTERN
        self.assertEqual(ctx.keylist_mode, gpgme.KeylistMode.EXTERN)
        ctx.keylist_mode = gpgme.KeylistMode.LOCAL | gpgme.KeylistMode.EXTERN
        self.assertEqual(ctx.keylist_mode,
                         gpgme.KeylistMode.LOCAL | gpgme.KeylistMode.EXTERN)

        def del_keylist_mode(ctx: gpgme.Context) -> None:
            del ctx.keylist_mode
        self.assertRaises(AttributeError, del_keylist_mode, ctx)

    def test_passphrase_cb(self) -> None:
        ctx = gpgme.Context()
        def passphrase_cb(uid_hint: Optional[str], passphrase_info: Optional[str], prev_was_bad: bool, fd: int) -> None:
            pass
        self.assertEqual(ctx.passphrase_cb, None)
        ctx.passphrase_cb = passphrase_cb
        self.assertEqual(ctx.passphrase_cb, passphrase_cb)
        ctx.passphrase_cb = None
        self.assertEqual(ctx.passphrase_cb, None)
        ctx.passphrase_cb = passphrase_cb
        del ctx.passphrase_cb
        self.assertEqual(ctx.passphrase_cb, None)

    def test_progress_cb(self) -> None:
        ctx = gpgme.Context()
        def progress_cb(what: Optional[str], type: int, current: int, total: int) -> None:
            pass
        self.assertEqual(ctx.progress_cb, None)
        ctx.progress_cb = progress_cb
        self.assertEqual(ctx.progress_cb, progress_cb)
        ctx.progress_cb = None
        self.assertEqual(ctx.progress_cb, None)
        ctx.progress_cb = progress_cb
        del ctx.progress_cb
        self.assertEqual(ctx.progress_cb, None)

    def test_sender(self) -> None:
        ctx = gpgme.Context()
        self.assertEqual(ctx.sender, None)
        ctx.sender = 'test@example.org'
        self.assertEqual(ctx.sender, 'test@example.org')
        ctx.sender = 'Test <test@example.org>'
        self.assertEqual(ctx.sender, 'test@example.org')
        ctx.sender = None
        self.assertEqual(ctx.sender, None)
        with self.assertRaises(AttributeError):
            del ctx.sender

    def test_get_engine_info(self) -> None:
        ctx = gpgme.Context()
        for info in ctx.get_engine_info():
            self.assertIsInstance(info, gpgme.EngineInfo)
            self.assertIsInstance(info.protocol, gpgme.Protocol)
            self.assertIsInstance(info.file_name, (str, type(None)))
            self.assertIsInstance(info.version, (str, type(None)))
            self.assertIsInstance(info.req_version, (str, type(None)))
            self.assertIsInstance(info.home_dir, (str, type(None)))

    def test_set_engine_info(self) -> None:
        # Add a key using the default $GNUPGHOME based keyring.
        ctx = gpgme.Context()
        with self.keyfile('key1.pub') as fp:
            ctx.import_(fp)

        # If we set $GNUPGHOME to a dummy value, we can't read in the
        # keywe just loaded.
        os.environ['GNUPGHOME'] = '/no/such/dir'
        ctx = gpgme.Context()
        self.assertRaises(gpgme.GpgmeError, ctx.get_key,
                          'E79A842DA34A1CA383F64A1546BB55F0885C65A4')

        # But if we configure the context using set_engine_info(), it
        # will find the key.
        ctx = gpgme.Context()
        ctx.set_engine_info(gpgme.Protocol.OpenPGP, None, self._gpghome)
        key = ctx.get_key('E79A842DA34A1CA383F64A1546BB55F0885C65A4')
        self.assertTrue(key)

        info = [info for info in ctx.get_engine_info()
                if info.protocol == gpgme.Protocol.OpenPGP].pop()
        self.assertEqual(info.home_dir, self._gpghome)
