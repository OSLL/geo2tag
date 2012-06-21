'''
Created on Jun 16, 2012

@author: bac1ca
'''
from TestUtil import abstract

class TestWriter(object):

    @abstract
    def addTestItem(self, name, description, enabled, result, log): pass
