#! /usr/bin/env python
# encoding: utf-8
# Copyright Mac Clayton 2017

import os

top = '.'
out = '.build'
tools = '''
	compiler_cxx
	msvc
	library_autoloader
	project
	'''

def options(opt):
    opt.load(tools)


def configure(conf):
    conf.load(tools)

def build(bld):
    bld.recurse('original')
    bld.recurse('refactor')