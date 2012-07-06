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
            
      
      

        
