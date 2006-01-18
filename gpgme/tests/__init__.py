import unittest

def test_suite():
    import gpgme.tests.test_context
    suite = unittest.TestSuite()
    suite.addTest(gpgme.tests.test_context.test_suite())
    return suite
