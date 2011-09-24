require 'rubygems'
require 'sinatra'
require 'json'

post '/auth' do
  
  def authCondition(auth)
    user0 = ["Paul",    "000000000000000", "test"]
    user1 = ["Kirill",  "000000000000001", "test"]
    user2 = ["Mark",    "000000000000010", "test"]
    user3 = ["Yevgeni", "000000000000011", "test"]
    user4 = ["Vasilii", "000000000000100", "test"]
    auth == user0 || auth == user1 || auth == user2 || auth == user3 || auth == user4 
  end
  
  def getAuthToken(auth)
    args = auth[0] + auth[1] + auth[2] 
    Digest::MD5.hexdigest(args)
  end

  data = JSON.parse(request.body.read)
  login = data["login"]
  device = data["device"]
  password = data["password"]
  auth = [login, device, password] 
  
  if authCondition(auth) 
    authToken = getAuthToken(auth)
    content_type :json
    { :auth_token => authToken, :status => 'Ok', :status_description => 'Authorization was successful' }.to_json
  else
    content_type :json
    { :status => 'Error', :status_description => 'Wrong login or password' }.to_json
  end

end

post '/message' do

  def toLog(toUser, typeMess, message)
    logDir = ENV['WEBGEO_HOME'] + "/airomo_logs"
    system( "mkdir #{logDir}" )
    system( "echo '#{message}' > $WEBGEO_HOME/airomo_logs/#{typeMess}_to_#{toUser}-$(date)" )
  end

  # consts
  typeMail    = "email"
  typeSms     = "sms"

  data = JSON.parse(request.body.read)
  authToken   = data["auth_token"]
  toUser      = data["to_user"]
  messType    = data["type"]
  message     = data["message"]

  # TODO chech auth token
  status = ""
  statusDescription = ""

  if messType == typeMail || messType == typeSms
    toLog(toUser, messType, message)
    status = "Ok"
    statusDescription = "Message was send"
  else
    status = "Error"
    statusDescription = "Type of message is an incorrect"
  end

  content_type :json
  { :status => status, :status_description => statusDescription }.to_json
end
