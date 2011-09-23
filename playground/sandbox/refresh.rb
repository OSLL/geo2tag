require 'rubygems'
require 'sinatra'
require 'stubs'

def getBranch(push)
  #puts "I got some JSON: #{push.inspect}"
  branch = push["ref"]
  branch = branch[11..-1]
end

post '/refresh' do
  branch = getBranch(JSON.parse(params[:payload]))
  #wasGood = system( "./commit_deploy.sh #{branch}" )
end

post '/refresh_platform' do
  branch = getBranch(JSON.parse(params[:payload]))
  #wasGood = system( "./commit_platform_deploy.sh #{branch}" )
end

post '/test' do
  branch = getBranch(JSON.parse(params[:payload]))
  puts branch
end


