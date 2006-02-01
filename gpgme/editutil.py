"""Utilities related to editing keys.

Currently only contains a utility function for editing the owner trust
value of a key in a keyring.
"""

__metaclass__ = type

__all__ = ['edit_trust']

import os
import StringIO
import gpgme


class _EditData:
    """Simple base class to wrap 'edit key' interactions"""

    STATE_START = 0
    STATE_ERROR = -1

    def __init__(self):
        self.state = self.STATE_START
        self.transitions = {}
        # a default state transition to try and quit the edit on error
        self.addTransition(self.STATE_ERROR,
                           gpgme.STATUS_GET_LINE, 'keyedit.prompt',
                           self.STATE_ERROR, 'quit\n')

    def addTransition(self, state, status, args, newstate, data):
        self.transitions[state, status, args] = newstate, data

    def do_edit(self, ctx, key):
        output = StringIO.StringIO()
        ctx.edit(key, self.callback, output)

    def callback(self, status, args, fd):
        if status in (gpgme.STATUS_EOF,
                      gpgme.STATUS_GOT_IT,
                      gpgme.STATUS_NEED_PASSPHRASE,
                      gpgme.STATUS_GOOD_PASSPHRASE,
                      gpgme.STATUS_BAD_PASSPHRASE,
                      gpgme.STATUS_USERID_HINT,
                      gpgme.STATUS_SIGEXPIRED,
                      gpgme.STATUS_KEYEXPIRED,
                      gpgme.STATUS_PROGRESS,
                      gpgme.STATUS_KEY_CREATED,
                      gpgme.STATUS_ALREADY_SIGNED):
            return

        if (self.state, status, args) in self.transitions:
            self.state, data = self.transitions[self.state, status, args]
            if data is not None:
                os.write(fd, data)
        else:
            self.state = STATE_ERROR
            raise gpgme.error(gpgme.ERR_SOURCE_UNKNOWN, gpgme.ERR_GENERAL)


class _EditTrust(_EditData):
    # states
    STATE_COMMAND = 1
    STATE_VALUE   = 2
    STATE_CONFIRM = 3
    STATE_QUIT    = 4

    def __init__(self, trust):
        _EditData.__init__(self)

        self.addTransition(self.STATE_START,
                           gpgme.STATUS_GET_LINE, 'keyedit.prompt',
                           self.STATE_COMMAND, 'trust\n')

        self.addTransition(self.STATE_COMMAND,
                           gpgme.STATUS_GET_LINE, 'edit_ownertrust.value',
                           self.STATE_VALUE, '%d\n' % trust)

        self.addTransition(self.STATE_VALUE,
                           gpgme.STATUS_GET_LINE, 'keyedit.prompt',
                           self.STATE_QUIT, 'quit\n')

        self.addTransition(self.STATE_VALUE,
                           gpgme.STATUS_GET_BOOL, 'edit_ownertrust.set_ultimate.okay',
                           self.STATE_CONFIRM, 'Y\n')

        self.addTransition(self.STATE_CONFIRM,
                           gpgme.STATUS_GET_LINE, 'keyedit.prompt',
                           self.STATE_QUIT, 'quit\n')

        self.addTransition(self.STATE_QUIT,
                           gpgme.STATUS_GET_BOOL, 'keyedit.save.okay',
                           self.STATE_CONFIRM, 'Y\n')


def edit_trust(ctx, key, trust):
    if trust not in (gpgme.VALIDITY_UNDEFINED,
                     gpgme.VALIDITY_NEVER,
                     gpgme.VALIDITY_MARGINAL,
                     gpgme.VALIDITY_FULL,
                     gpgme.VALIDITY_ULTIMATE):
        raise ValueError('Bad trust value %d' % trust)
    statemachine = _EditTrust(trust)
    statemachine.do_edit(ctx, key)
