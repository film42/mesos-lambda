Build Log
=========

#### 2016-02-10

**Getting Mesos and Docker playing nicely:**

NOTE: Because I'm on OSX, mesos didn't like the docker-machine TCP bridge (wanted unix socket), so I spun up a digital ocean instance and
added the linux slave to my master running on OSX. Mostly because I wanted to see if I could.


This is how I started the master:
```
mesos-master --work_dir=/tmp --ip=0.0.0.0 --hostname=55.10.33.171 --advertise_ip=55.10.33.171
```
NOTE: `advertise_ip` is extremely important! If you don't set this, the remote slave cannot talk to the master. It might bind initially but
will complain about the hostname or ip not matching, saying "it is not the expected master." Remember this and you won't have to feel the
pain I had to feel.


This is how I started the slave:
```
mesos-slave --work_dir=/tmp --master=55.10.33.171:5050 --log_dir=/tmp/ --port=5055 --containerizers="mesos,docker" --hostname=99.553.121.25 --ip=99.553.121.25
```
NOTE: The user you're executing with will match the destination user if you use mesos-execute, so make sure you match those up.



Here's how you can manually kick off a task using a docker container (in this case, using ruby docker image):

```
mesos-execute --master=55.10.33.171:5050 --name="docker test" --command="ruby -e 'puts :ok'" --containerizer=docker --docker_image=ruby
```

Looking at the logs we see:
```
--container="mesos-5d7af030-508c-4f4a-ac9b-2726dfe454a8-S0.8fef48e8-eb83-43fa-8a6e-b2e18bc13819" --docker="docker" --docker_socket="/var/run/docker.sock" --help="false" --initialize_driver_logging="true" --launcher_dir="/usr/libexec/mesos" --logbufsecs="0" --logging_level="INFO" --mapped_directory="/mnt/mesos/sandbox" --quiet="false" --sandbox_directory="/tmp/slaves/5d7af030-508c-4f4a-ac9b-2726dfe454a8-S0/frameworks/0d703ec6-b629-4314-a634-ee3f1db964e9-0000/executors/docker test/runs/8fef48e8-eb83-43fa-8a6e-b2e18bc13819" --stop_timeout="0ns"
--container="mesos-5d7af030-508c-4f4a-ac9b-2726dfe454a8-S0.8fef48e8-eb83-43fa-8a6e-b2e18bc13819" --docker="docker" --docker_socket="/var/run/docker.sock" --help="false" --initialize_driver_logging="true" --launcher_dir="/usr/libexec/mesos" --logbufsecs="0" --logging_level="INFO" --mapped_directory="/mnt/mesos/sandbox" --quiet="false" --sandbox_directory="/tmp/slaves/5d7af030-508c-4f4a-ac9b-2726dfe454a8-S0/frameworks/0d703ec6-b629-4314-a634-ee3f1db964e9-0000/executors/docker test/runs/8fef48e8-eb83-43fa-8a6e-b2e18bc13819" --stop_timeout="0ns"
Registered docker executor on 99.553.121.25
Starting task docker test
ok
Shutting down
```
