'''
Created on Jun 12, 2012

@author: bac1ca
'''

from TestUtil import abstract

class TestTemplate(object):
    '''
    class TestTemplate
    '''
    
    def setUp(self, context): pass
    
    @abstract    
    def execute(self, context): pass

    def setDown(self, context): pass

    @abstract    
    def isEnabled(self): pass
    
    @abstract    
    def getName(self): pass

    @abstract    
    def getDescription(self): pass

