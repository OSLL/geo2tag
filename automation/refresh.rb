require 'rubygems'
require 'sinatra'
require 'stubs'
require 'web_platform_test'

set :port, 9494

def getBranch(push)
  #puts "I got some JSON: #{push.inspect}"
  branch = push["ref"]
  branch = branch[11..-1]
end

post '/refresh_webside' do
  branch = getBranch(JSON.parse(params[:payload]))
  wasGood = system( "./build_webside.sh #{branch}" )
end

post '/refresh_platform' do
  branch = getBranch(JSON.parse(params[:payload]))
  #wasGood = system( "./build_platform.sh #{branch}" )
  wasGood = system( "./add_deb_build_task.sh #{branch}" )
  #exec( "./build_platform.sh #{branch}" )
end

post '/test' do
  branch = getBranch(JSON.parse(params[:payload]))
  puts branch
end
