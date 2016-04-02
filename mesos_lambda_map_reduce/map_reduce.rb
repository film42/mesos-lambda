require "thread"
require "json"
require "base64"

size = 10
operator = "+"
reducer = "*"
fixed_rhs = 5

def build_problem(size)
  # Size is the number of requests to make.
  Array.new(size) { rand(1..100) }
end

def simulate(problem, operator, reducer, fixed_rhs)
  problem.map { |p| eval("#{p} #{operator} #{fixed_rhs}") }.reduce(&:"#{reducer}")
end

def command_with_input(input)
<<-EOF
curl -X POST localhost:8080/lambda -d '{ \"name\" : \"map_reduce\", \"resources\" : \"cpus:0.1;mem:50\", \"docker_image\" : \"lambda/ruby_calc\", \"command\" : \"./invoke.sh #{input}\" }'
EOF
end

problem = build_problem(size)

puts "operator: #{operator}"
puts "reducer: #{reducer}"
puts "fixed_rhs: #{fixed_rhs}"
puts problem.inspect

puts "SIMULATE"
simulation = simulate(problem, operator, reducer, fixed_rhs)
puts simulation


commands = problem.map do |p|
  input = { :lhs => p, :rhs => fixed_rhs, :operator => operator }.to_json
  base64_input = ::Base64.urlsafe_encode64(input)
  command = command_with_input(base64_input)
end

##
# MAP
#
puts "=============== BEGIN MAP ================"

number = 0
values = commands.map do |command|
  number += 1

  ::Thread.new(number) do |n|
    puts "COMMAND #{n} - #{command}"
    response = `#{command}`
    puts "RESPONSE #{n} - #{response}"
    json = JSON.parse(response)
    json["result"]
  end
end.map(&:join).map(&:value)

puts "=============== MAP COMPLETE ================"

##
# REDUCE
#

puts "=============== BEGIN REDUCE ================"

round = 0
loop do
  round += 1
  break if values.size == 1

  puts "=============== REDUCE ROUND #{round} ================"
  puts values.inspect

  number = 0
  values = values.each_slice(2).map do |chunk|
    number += 1
    Thread.new(number) do |n|
      if chunk.size == 1
        chunk.first
      else
        v1, v2 = chunk

        # Reducer
        input = { :lhs => v1, :rhs => v2, :operator => reducer }.to_json
        base64_input = ::Base64.urlsafe_encode64(input)
        command = command_with_input(base64_input)

        puts "COMMAND #{n} - #{command}"
        response = `#{command}`
        puts "RESPONSE #{n} - #{response}"
        json = JSON.parse(response)
        json["result"]
      end
    end
  end.map(&:join).map(&:value)
end

puts "=============== REDUCE COMPLETE ================"

puts "Final result:"
final_result = values.first
puts final_result

if final_result == simulation
  puts "SUCCESS!"
end
