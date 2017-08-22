#!/usr/bin/env python3
#  -*- coding: utf8 -*-

from setuptools import setup

setup(
    name='eufa-tools',
    version='0.1',
    packages=['.'],
    url='https://github.com/Becavalier/Eufa/tools',
    keywords=['wasm', 'webassembly', 'eufa'],
    license='MIT',
    author='YHSPY',
    author_email='yhorg@hotmail.com',
    description='A high efficient utility functions library written in webassembly with profiling tools.',
    install_requires=[
        'setuptools',
    ],
    entry_points={
        'console_scripts': [
            'eufa_ds = decode_sections:__main__'
        ]
    }
)
