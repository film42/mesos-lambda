Mesos Lambda
============

This is me attempting to create a proof-of-concept Amazon Lambda using docker + mesos. You can
follow along with the build log.

Download the source of mesos 0.27 and place files in `vendor/mesos-0.27.0`, apply the patch found
in `patches/mesos-0.27.0` to allow docker to respond with a capture from stdout. Build mesos using
their build instructions.

For some reason I cannot remember right now, I needed to add `vendor/protobuf-2.5.0`, which can be
found in the mesos-0.27.0 release. This should probably be cleaned up.

### Show me an example already!

Sure thing, check out the [MapReduce example and example output over here](mesos_lambda_map_reduce).

