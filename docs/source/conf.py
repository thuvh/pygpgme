# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

import os

import gpgme


project = 'PyGPGME'
copyright = '2024, PyGPGME contributors'
author = 'PyGPGME contributors'

release = gpgme.__version__

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.doctest',
    'sphinx.ext.napoleon',
    'sphinx.ext.todo',
]

templates_path = ['_templates']
exclude_patterns = []

autodoc_default_options = {
    'member-order': 'groupwise',
}

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'

html_context = {
    'display_github': True,
    'github_user': 'jhenstridge',
    'github_repo': 'pygpgme',
    'github_version': os.environ.get('READTHEDOCS_GIT_IDENTIFIER', os.environ.get('GITHUB_REF', 'main')),
    'conf_py_path': '/docs/source/',
}
