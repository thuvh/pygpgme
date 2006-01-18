import sys
import unittest

import gpgme.tests

def main(argv):
    runner = unittest.TextTestRunner(verbosity=2)
    if not runner.run(gpgme.tests.test_suite()).wasSuccessful():
        return 1
    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
