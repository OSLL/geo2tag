'''
Created on Jun 16, 2012

@author: bac1ca
'''
from TestWriter import TestWriter

class HTMLWriter(TestWriter):
    mPage = ''
    
    mTestCount = 0
    mPassCount = 0
    
    def __init__(self):
        self.mTestCount = 0
        self.mPassCount = 0

    mFail = '<font color="red">FAIL</font>'
    mPass = '<font color="green">PASS</font>'
    mErrn = '<font color="blue">WEB SERVICE ERROR</font>'
    mDsbl = '<font color="silver">DISABLED</font>'
    
    def __genHeader__(self, testName, result):
        return '<h4>TEST: ' + testName + ' - ' + result + '</h4>'
    
    def __genDescription__(self, description):
        return '<b>DESCRIPTION: ' + description + '</b><br>'
    
    def __genTable__(self, header, lines):
        table = '<table id="code", width="100%" border="1" cellspacing="0" cellpadding="4"> \
        <tr style="background-color: #eaeaea;""> \
        <th>' + header + '</th> \
        </tr>'
        for line in lines:
            table = table + '<tr style="background-color: #d5d5d5;"> \
            <td>\
            <code>' + line + '</code> \
            </td> \
            </tr>'
        table = table + '</table>'
        return table


    def addTestItem(self, name, description, enabled, result, log):
        self.mTestCount += 1
        if (enabled):
            if (result):
                self.mPassCount += 1
                self.mPage = self.mPage + self.__genHeader__(name, self.mPass)
            else:
                self.mPassCount += 0
                self.mPage = self.mPage + self.__genHeader__(name, self.mFail)
            self.mPage = self.mPage + self.__genDescription__(description)
            self.mPage = self.mPage + self.__genTable__('Test Log', log)
        else:
            self.mPassCount += 1
            self.mPage = self.mPage + self.__genHeader__(name, self.mDsbl)
        self.mPage = self.mPage + '<br>'


    def getPage(self):
        sumInfo = [str(self.mTestCount) + ' Tests\
        ,' + str(self.mPassCount) + ' Passed,' + str(self.mTestCount - self.mPassCount) + ' Failed']
        sumInfo = self.__genTable__('<h3>Summary<h3>', sumInfo)
        return '<h1>Geo2Tag API test</h1>' + sumInfo + '<br><br><br><br>' + self.mPage
            
      
      

        
