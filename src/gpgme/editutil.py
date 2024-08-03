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

"""Utilities related to editing keys.

Currently only contains a utility function for editing the owner trust
value of a key in a keyring.
"""

__metaclass__ = type

__all__ = ['edit_sign', 'edit_trust']

import functools
import io
import os
from typing import Callable, Concatenate, Generator, ParamSpec

import gpgme


P = ParamSpec("P")
KeyEditorGen = Generator[str | None, tuple[gpgme.Status, str | None], None]


def key_editor(function: Callable[Concatenate[gpgme.Context, gpgme.Key, P], KeyEditorGen]) -> Callable[Concatenate[gpgme.Context, gpgme.Key, P], None]:
    """A decorator that lets key editor callbacks be written as generators."""
    @functools.wraps(function)
    def wrapper(ctx: gpgme.Context, key: gpgme.Key, *args: P.args, **kwargs: P.kwargs) -> None:
        # Start the generator and run it once.
        gen = function(ctx, key, *args, **kwargs)
        try:
            gen.__next__()
        except StopIteration:
            return

        def edit_callback(status: gpgme.Status, args: str | None, fd: int) -> None:
            if status in (gpgme.Status.EOF,
                          gpgme.Status.GOT_IT,
                          gpgme.Status.NEED_PASSPHRASE,
                          gpgme.Status.GOOD_PASSPHRASE,
                          gpgme.Status.BAD_PASSPHRASE,
                          gpgme.Status.USERID_HINT,
                          gpgme.Status.SIGEXPIRED,
                          gpgme.Status.KEYEXPIRED,
                          gpgme.Status.PROGRESS,
                          gpgme.Status.KEY_CREATED,
                          gpgme.Status.ALREADY_SIGNED,
                          gpgme.Status.KEY_CONSIDERED):
                return
            try:
                data = gen.send((status, args))
            except StopIteration:
                raise gpgme.GpgmeError(gpgme.ErrSource.UNKNOWN, gpgme.ErrCode.GENERAL)

            if data is not None:
                os.write(fd, data.encode('ASCII'))

        output = io.BytesIO()
        try:
            ctx.edit(key, edit_callback, output)
        finally:
            gen.close()

    return wrapper


@key_editor
def edit_trust(ctx: gpgme.Context, key: gpgme.Key, trust: gpgme.Validity) -> KeyEditorGen:
    """Edit the trust level of the given key."""
    if trust not in (gpgme.Validity.UNDEFINED,
                     gpgme.Validity.NEVER,
                     gpgme.Validity.MARGINAL,
                     gpgme.Validity.FULL,
                     gpgme.Validity.ULTIMATE):
        raise ValueError('Bad trust value %d' % trust)

    status, args = yield None

    assert args == 'keyedit.prompt'
    status, args = yield 'trust\n'

    assert args == 'edit_ownertrust.value'
    status, args = yield '%d\n' % trust

    if args == 'edit_ownertrust.set_ultimate.okay':
        status, args = yield 'Y\n'

    assert args == 'keyedit.prompt'
    status, args = yield 'quit\n'

    assert args == 'keyedit.save.okay'
    status, args = yield 'Y\n'


@key_editor
def edit_sign(ctx: gpgme.Context, key: gpgme.Key, index: int = 0,
              local: bool = False, norevoke: bool = False,
              expire: bool = True, check: int = 0) -> KeyEditorGen:
    """Sign the given key.

    index:    the index of the user ID to sign, starting at 1.  Sign all
               user IDs if set to 0.
    local:    make a local signature
    norevoke: make a non-revokable signature
    command:  the type of signature.  One of sign, lsign, tsign or nrsign.
    expire:   whether the signature should expire with the key.
    check:    Amount of checking performed.  One of:
                 0 - no answer
                 1 - no checking
                 2 - casual checking
                 3 - careful checking
    """
    if index < 0 or index > len(key.uids):
        raise ValueError('user ID index out of range')
    command = 'sign'
    if local:
        command = 'l%s' % command
    if norevoke:
        command = 'nr%s' % command
    if check not in [0, 1, 2, 3]:
        raise ValueError('check must be one of 0, 1, 2, 3')

    status, args = yield None

    assert args == 'keyedit.prompt'
    status, args = yield 'uid %d\n' % index

    assert args == 'keyedit.prompt'
    status, args = yield '%s\n' % command

    while args != 'keyedit.prompt':
        if args == 'keyedit.sign_all.okay':
            status, args = yield 'Y\n'
        elif args == 'sign_uid.expire':
            status, args = yield '%s\n' % ('Y' if expire else 'N')
        elif args == 'sign_uid.class':
            status, args = yield '%d\n' % check
        elif args == 'sign_uid.okay':
            status, args = yield 'Y\n'
        else:
            raise AssertionError("Unexpected state %r" % ((status, args),))

    status, args = yield 'quit\n'

    assert args == 'keyedit.save.okay'
    status, args = yield 'Y\n'
