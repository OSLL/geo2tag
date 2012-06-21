#!/usr/bin/python

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