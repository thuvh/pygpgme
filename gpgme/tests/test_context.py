import unittest

import gpgme
from gpgme.tests.util import GpgHomeTestCase

class ContextTestCase(GpgHomeTestCase):

    def test_create_context(self):
        ctx = gpgme.Context()

    def test_armor(self):
        ctx = gpgme.Context()
        self.assertEqual(ctx.armor, False)
        ctx.armor = True
        self.assertEqual(ctx.armor, True)
        ctx.armor = False
        self.assertEqual(ctx.armor, False)

def test_suite():
    loader = unittest.TestLoader()
    return loader.loadTestsFromName(__name__)
