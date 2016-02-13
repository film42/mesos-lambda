PROTOBUF_SOURCE="./vendor/mesos/3rdparty/libprocess/3rdparty/protobuf-2.5.0/src/protobuf-2.5.0/src/"
MESOS_SOURCE="./vendor/mesos/src"

all:
# Framework
	g++ -g -std=c++11 -stdlib=libc++ -o scheduler -I $(PROTOBUF_SOURCE) -I $(MESOS_SOURCE) src/scheduler.cpp -l mesos 
