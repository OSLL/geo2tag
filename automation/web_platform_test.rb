require 'rubygems'
require 'sinatra'
require 'json'
require 'rexml/document'
include REXML

$fail = '<font color="red">FAIL</font>'
$pass = '<font color="green">PASS</font>'
$errn = '<font color="blue">WEB SERVICE ERROR</font>'

$test_count = 0
$pass_count = 0

def test_case(test_name, request, response, errno)
    success = test_exam(response, errno)
    $test_count+=1
    if success == $pass
        $pass_count+=1
    end
    request_table = simple_table("Request", request)
    response_table = simple_table("Response", response)
    return "<h4>TEST " + test_name + " - #{success}</h4>" +
        "#{request_table}" + "#{response_table} <br><br>"
end

def test_exam(response, errno)
    result = $fail
    begin
        response_json = JSON.parse("#{response}")  
        errno_code = response_json["errno"]   
        if errno_code == errno
            result = $pass
        end
    rescue Exception => e  
        result = $errn
    end
    return result
end

def simple_table(header, text)
    return '
    <table id="code", width="100%" border="1" cellspacing="0" cellpadding="4">
        <tr style="background-color: #eaeaea;"">
        <th>' + "#{header}" + '</th>
        </tr>
        <tr style="background-color: #d5d5d5;">
        <td>
        <code>' + "#{text}" + '</code>
        </td>
        </tr>
    </table>'
end

get '/test-platform' do
    $test_count = 0
    $pass_count = 0

    res_page = "<h1>geo2tag.api</h1><br>"
    det_info = ""

    xmlfile = File.new("tests.xml")
    xmldoc = Document.new(xmlfile)
    xmldoc.elements.each("tests/testcase"){ 
      |e| puts "Test title : " + e.attributes["title"]
      name = e.attributes["title"]
      pred = e.elements["pred"].text #TODO
      test = e.elements["test"].text
      post = e.elements["post"].text
      puts "post : " + post 
      response = %x(#{test})
      det_info += test_case(name, "#{test}", response, post.to_i)
    }

    # get summary test's information
    sum_info = simple_table("<h3>Summary<h3>", "#$test_count Tests, #$pass_count Passed, #{$test_count - $pass_count} Failed")
    res_page += sum_info + "<br><br><br><br>" + det_info
    "#{res_page}"
end
