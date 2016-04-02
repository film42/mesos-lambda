PROTOBUF_SOURCE="./vendor/protobuf-2.5.0/src/"
MESOS_SOURCE="./vendor/mesos-0.27.0/src"
MESOS_BUILD_SOURCE="./vendor/mesos-0.27.0/build/src"
SERVER_SOURCE="./vendor/scheduler_api"
VENDOR_SOURCE="./vendor"

all:
# Framework
	g++ -g -std=c++11 -stdlib=libc++ -o scheduler_server -I $(VENDOR_SOURCE) -I $(SERVER_SOURCE) -I $(PROTOBUF_SOURCE) -I $(MESOS_SOURCE) -I $(MESOS_BUILD_SOURCE) src/server.cpp -l mesos -l boost_coroutine -l boost_regex -l boost_system
