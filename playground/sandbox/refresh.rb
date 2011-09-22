require 'rubygems'
require 'sinatra'
require 'stubs'

post '/refresh' do
  wasGood = system( "./commit_deploy.sh" )
end

post '/refresh_platform' do
  wasGood = system( "./commit_platform_deploy.sh" )
end

