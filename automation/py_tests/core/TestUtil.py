'''
 * Copyright 2012  OSLL Vasily Romanikhin  bac1ca89@gmail.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 
 @author: Vasily Romanikhin bac1ca89@gmail.com
'''

import os
import re
import ConfigParser
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
    tests = []

    parser = ConfigParser.SafeConfigParser()
    parser.read(testDir + "/tests.conf")

    options = []
    section = "Tests_Order";
    for option in parser.options(section):
        options.append(int(option))

    for option in options:
        value = parser.get(section, str(option))        
        tests.append(value)

    return tests

def testName(name):
    return 'tests' + '.' + name + '.' + name
    
