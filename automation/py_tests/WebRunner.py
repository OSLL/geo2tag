#!/usr/bin/python

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

from bottle import route, run
from core.HTMLWriter import HTMLWriter
from core.TestCore import TestCore

errMessage = 'Wrong format: try again! Use http://test-tool-address/test-platform/instance="tested-instance"'

@route('/test-platform')
@route('/test-platform/')
@route('/test-platform/<instance>')
def index(instance='instance=tracks.osll.spb.ru:81'):
    if (instance != None):
        netArgs = instance.split('=');
        if (netArgs[0] != 'instance'):
            return errMessage
        else:
            instance = 'http://' + netArgs[1]
            print instance

    context = {'server':instance, 
               'test_dir':'tests'}
    writer = HTMLWriter()
    testCore = TestCore()
    testCore.runTests(context, writer)

    return writer.getPage()


run(host='localhost', port=9595)
