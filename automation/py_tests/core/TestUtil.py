'''
Created on Jun 14, 2012

@author: bac1ca
'''

import os
import re
from sets import Set

def abstract(func):
    def closure(*dt, **mp):
        raise NotImplementedError("Method %s is pure virtual" % func.__name__)
    return closure

def forName(class_string):
    """Returns class object specified by a string.

    Args:
        class_string: The string representing a class.

    Raises:
        ValueError if module part of the class is not specified.
    """
    module_name, _, class_name = class_string.rpartition('.')
    if module_name == '':
        raise ValueError('Class name must contain module part.')
    someType = getattr(
        __import__(module_name, globals(), locals(), [class_name], -1),
        class_name)
    return someType()

def getTests(testDir):
    tests = os.listdir(testDir)
    testsSet = Set()
    match = re.findall('Test\w*\.py' , tests.__str__())
    for testName in match:
        testsSet.add(testName[:-3]);
    return testsSet

def testName(name):
    return 'tests' + '.' + name + '.' + name
    