require "json"
require "base64"


result = begin
  base64_string = ARGV.join
  json_string = Base64.urlsafe_decode64(base64_string)
  hash = JSON.parse(json_string)
  
  lhs = hash["lhs"]
  rhs = hash["rhs"]
  operator = hash["operator"]
  
  calculation = "#{lhs} #{operator} #{rhs}"
  eval(calculation)
rescue
  "error"
end

response = { :calculation => calculation, :result => result }.to_json
puts response
