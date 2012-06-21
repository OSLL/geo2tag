#!/usr/bin/python

'''
 @author: bac1ca89 
'''

from core.TestCore import TestCore
from core.ConsoleWriter import ConsoleWriter


def runLocal():
    context = {'server':'http://tracks.osll.spb.ru:81', 
               'test_dir':'tests'}
    writer = ConsoleWriter()
    testCore = TestCore()
    testCore.runTests(context, writer)
    print writer.getPage()
     
    
if __name__ == '__main__':
    runLocal()
