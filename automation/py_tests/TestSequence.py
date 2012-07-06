'''
 * Copyright 2012 OSLL
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
 
 @author: OSLL team
'''

from core.TestCore import TestCore
from datetime import datetime

from tests.TestRegisterUser import TestRegisterUser
from tests.TestAddUser import TestAddUser
from tests.TestLogin import TestLogin
from tests.TestAddChannel import TestAddChannel
from tests.TestSubscribeChannel import TestSubscribeChannel
from tests.TestAvailableChannels import TestAvailableChannels
from tests.TestSubscribedChannels import TestSubscribedChannels
from tests.TestWriteTag import TestWriteTag
from tests.TestLoadTags import TestLoadTags
from tests.TestUnsubscribeChannel import TestUnsubscribeChannel
from tests.TestFilterCircle import TestFilterCircle
from tests.TestFilterCylinder import TestFilterCylinder
from tests.TestFilterPolygon import TestFilterPolygon
from tests.TestFilterRectangle import TestFilterRectangle
from tests.TestFilterBox import TestFilterBox
from tests.TestFilterFence import TestFilterFence
from tests.TestFilterChannel import TestFilterChannel
from tests.TestQuitSession import TestQuitSession
from tests.TestDeleteUser import TestDeleteUser

class TestSequence:

    def __init__(self):
        pass

    def run(self, context, writer):
	testCore = TestCore()

        # RegisterUser test
        testInstance = TestRegisterUser()
        signupLogin = "testLogin_"+ str(datetime.now());
        signupEmail = "testEmail_"+ str(datetime.now());
        testInstance.login = signupLogin
        testInstance.email = signupEmail
	testCore.testProcess(testInstance, context, writer)

        # AddUser test
        testInstance = TestAddUser()
	testCore.testProcess(testInstance, context, writer)

        # Login test
        testInstance = TestLogin()
	testCore.testProcess(testInstance, context, writer)
        authToken = testInstance.authToken;

        # AddChannel test
        testInstance = TestAddChannel()
        testInstance.authToken = authToken
        channelName = "testChannel_"+ str(datetime.now());
        testInstance.name = channelName;
        testCore.testProcess(testInstance, context, writer)

        # Subscribe channel test
        testInstance = TestSubscribeChannel()
        testInstance.authToken = authToken
        testInstance.channel = channelName;
        testCore.testProcess(testInstance, context, writer)

        # WriteTag test
        testInstance = TestWriteTag()
        testInstance.authToken = authToken
        testInstance.channel = channelName;
        testCore.testProcess(testInstance, context, writer)

        # LoadTags test
        testInstance = TestLoadTags()
        testInstance.authToken = authToken
        testCore.testProcess(testInstance, context, writer)

        # AvailableChannels test
        testInstance = TestAvailableChannels()
        testInstance.authToken = authToken
        testCore.testProcess(testInstance, context, writer)

        # SubscribedChannels test
        testInstance = TestSubscribedChannels()
        testInstance.authToken = authToken
        testCore.testProcess(testInstance, context, writer)

        # FilterCircle test
        testInstance = TestFilterCircle()
        testInstance.authToken = authToken
        testCore.testProcess(testInstance, context, writer)

        # FilterCylinder test
        testInstance = TestFilterCylinder()
        testInstance.authToken = authToken
        testCore.testProcess(testInstance, context, writer)

        # FilterPolygon test
        testInstance = TestFilterPolygon()
        testInstance.authToken = authToken
        testCore.testProcess(testInstance, context, writer)

        # FilterRectangle test
        testInstance = TestFilterRectangle()
        testInstance.authToken = authToken
        testCore.testProcess(testInstance, context, writer)

        # FilterBox test
        testInstance = TestFilterBox()
        testInstance.authToken = authToken
        testCore.testProcess(testInstance, context, writer)

        # FilterFence test
        testInstance = TestFilterFence()
        testInstance.authToken = authToken
        testCore.testProcess(testInstance, context, writer)

        # FilterChannel test
        testInstance = TestFilterChannel()
        testInstance.authToken = authToken
        testInstance.channel = channelName;
        testCore.testProcess(testInstance, context, writer)

        # Unsubscribe channel test
        testInstance = TestUnsubscribeChannel()
        testInstance.authToken = authToken
        testInstance.channel = channelName;
        testCore.testProcess(testInstance, context, writer)

        # QuitSession test
        testInstance = TestQuitSession()
        testInstance.authToken = authToken
        testCore.testProcess(testInstance, context, writer)

        # DeleteUser test
        testInstance = TestDeleteUser()
        testCore.testProcess(testInstance, context, writer)
