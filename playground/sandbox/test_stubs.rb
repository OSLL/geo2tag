require 'stubs'
require 'rubygems'
require 'test/unit'
require 'rack/test'

ENV['RACK_ENV'] = 'test'

class StubsTest < Test::Unit::TestCase
  include Rack::Test::Methods

  def app
    Sinatra::Application
  end
  
  # TESTING AUTH
  
  def test_auth_user0
    post '/auth', '{"login":"Paul", "password":"test", "device":"000000000000000"}'
    assert last_response.body.include?('Authorization was successful')
  end
  
  def test_auth_user1
    post '/auth', '{"login":"Kirill", "password":"test", "device":"000000000000001"}'
    assert last_response.body.include?('Authorization was successful')
  end

  def test_auth_user2
    post '/auth', '{"login":"Mark", "password":"test", "device":"000000000000010"}'
    assert last_response.body.include?('Authorization was successful')
  end
  
  def test_auth_user3
    post '/auth', '{"login":"Yevgeni", "password":"test", "device":"000000000000011"}'
    assert last_response.body.include?('Authorization was successful')
  end
  
  def test_auth_user4
    post '/auth', '{"login":"Vasilii", "password":"test", "device":"000000000000100"}'
    assert last_response.body.include?('Authorization was successful')
  end
  
  def test_auth_bad
    post '/auth', '{"login":"Vasilii", "password":"test", "device":"000000000000000"}'
    assert last_response.body.include?('Wrong login or password')
  end

  # TESTING SENDING MESSAGES

  def test_send_true_mess
    post '/message', '{"auth_token":"MMMMMMMMMM", "to_user":"Valilii", "type":"email", "message":"Hello Vasilii! How are you?"}'
    assert last_response.body.include?('Ok')
  end

  def test_send_bad_mess
    post '/message', '{"auth_token":"MMMMMMMMMM", "to_user":"Valilii", "type":"Mail", "message":"Hello Vasilii! How are you?"}'
    assert last_response.body.include?('Error')
  end

end
