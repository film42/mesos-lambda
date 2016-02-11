require "json"

json_string = ARGV.join
hash = JSON.parse(json_string)

response = { :yo => :lo, :payload => hash }.to_json
puts response
