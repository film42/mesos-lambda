MapReduce Example
=================

This is a really dumb example, but it gets the point across. It's using `lambda/ruby_calc` from the `examples` directory of this project.
Here's an example execution of the map reduce example:


```
# 1. Make sure the mesos master and slave are started with a docker containerizer.
# 2. Change the mesos master ip in src/server.cpp, make and run the scheduler_server program.
mesos_lambda $ make
mesos_lambda $ ./scheduler_server
Server started on port 8080
```

```
# Then, in a different shell, run the map reduce example.
mesos_lambda/mesos_lambda_map_reduce $ ruby map_reduce.rb 2> /dev/null
operator: +
reducer: *
fixed_rhs: 5
[38, 51, 35, 24, 94, 80, 96, 25, 14, 84]
SIMULATE
120435585476976000
=============== BEGIN MAP ================
COMMAND 1 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjM4LCJyaHMiOjUsIm9wZXJhdG9yIjoiKyJ9" }'
COMMAND 2 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjUxLCJyaHMiOjUsIm9wZXJhdG9yIjoiKyJ9" }'
COMMAND 10 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjg0LCJyaHMiOjUsIm9wZXJhdG9yIjoiKyJ9" }'
COMMAND 3 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjM1LCJyaHMiOjUsIm9wZXJhdG9yIjoiKyJ9" }'
COMMAND 4 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjI0LCJyaHMiOjUsIm9wZXJhdG9yIjoiKyJ9" }'
COMMAND 5 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjk0LCJyaHMiOjUsIm9wZXJhdG9yIjoiKyJ9" }'
COMMAND 6 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjgwLCJyaHMiOjUsIm9wZXJhdG9yIjoiKyJ9" }'
COMMAND 7 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjk2LCJyaHMiOjUsIm9wZXJhdG9yIjoiKyJ9" }'
COMMAND 8 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjI1LCJyaHMiOjUsIm9wZXJhdG9yIjoiKyJ9" }'
COMMAND 9 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjE0LCJyaHMiOjUsIm9wZXJhdG9yIjoiKyJ9" }'
RESPONSE 9 - {"calculation":"14 + 5","result":19}
RESPONSE 4 - {"calculation":"24 + 5","result":29}
RESPONSE 2 - {"calculation":"51 + 5","result":56}
RESPONSE 8 - {"calculation":"25 + 5","result":30}
RESPONSE 3 - {"calculation":"35 + 5","result":40}
RESPONSE 5 - {"calculation":"94 + 5","result":99}
RESPONSE 6 - {"calculation":"80 + 5","result":85}
RESPONSE 1 - {"calculation":"38 + 5","result":43}
RESPONSE 10 - {"calculation":"84 + 5","result":89}
RESPONSE 7 - {"calculation":"96 + 5","result":101}
=============== MAP COMPLETE ================
=============== BEGIN REDUCE ================
=============== REDUCE ROUND 1 ================
[43, 56, 40, 29, 99, 85, 101, 30, 19, 89]
COMMAND 4 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjEwMSwicmhzIjozMCwib3BlcmF0b3IiOiIqIn0=" }'
COMMAND 3 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjk5LCJyaHMiOjg1LCJvcGVyYXRvciI6IioifQ==" }'
COMMAND 1 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjQzLCJyaHMiOjU2LCJvcGVyYXRvciI6IioifQ==" }'
COMMAND 2 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjQwLCJyaHMiOjI5LCJvcGVyYXRvciI6IioifQ==" }'
COMMAND 5 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjE5LCJyaHMiOjg5LCJvcGVyYXRvciI6IioifQ==" }'
RESPONSE 4 - {"calculation":"101 invoke.sh lamb.rb 30","result":3030}
RESPONSE 3 - {"calculation":"99 invoke.sh lamb.rb 85","result":8415}
RESPONSE 2 - {"calculation":"40 invoke.sh lamb.rb 29","result":1160}
RESPONSE 5 - {"calculation":"19 invoke.sh lamb.rb 89","result":1691}
RESPONSE 1 - {"calculation":"43 invoke.sh lamb.rb 56","result":2408}
=============== REDUCE ROUND 2 ================
[2408, 1160, 8415, 3030, 1691]
COMMAND 2 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjg0MTUsInJocyI6MzAzMCwib3BlcmF0b3IiOiIqIn0=" }'
COMMAND 1 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjI0MDgsInJocyI6MTE2MCwib3BlcmF0b3IiOiIqIn0=" }'
RESPONSE 2 - {"calculation":"8415 invoke.sh lamb.rb 3030","result":25497450}
RESPONSE 1 - {"calculation":"2408 invoke.sh lamb.rb 1160","result":2793280}
=============== REDUCE ROUND 3 ================
[2793280, 25497450, 1691]
COMMAND 1 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjI3OTMyODAsInJocyI6MjU0OTc0NTAsIm9wZXJhdG9yIjoiKiJ9" }'
RESPONSE 1 - {"calculation":"2793280 invoke.sh lamb.rb 25497450","result":71221517136000}
=============== REDUCE ROUND 4 ================
[71221517136000, 1691]
COMMAND 1 - curl -X POST localhost:8080/lambda -d '{ "name" : "map_reduce", "resources" : "cpus:0.1;mem:50", "docker_image" : "lambda/ruby_calc", "command" : "./invoke.sh eyJsaHMiOjcxMjIxNTE3MTM2MDAwLCJyaHMiOjE2OTEsIm9wZXJhdG9yIjoiKiJ9" }'
RESPONSE 1 - {"calculation":"71221517136000 invoke.sh lamb.rb 1691","result":120435585476976000}
=============== REDUCE COMPLETE ================
Final result:
120435585476976000
SUCCESS!
```
