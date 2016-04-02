#include <iostream>
#include <vector>
#include <sstream>

#include <mesos/resources.hpp>
#include <mesos/scheduler.hpp>
#include <mesos/type_utils.hpp>

#include <process/future.hpp>
#include <process/owned.hpp>
#include <process/pid.hpp>

#include <stout/check.hpp>
#include <stout/hashmap.hpp>
#include <stout/none.hpp>
#include <stout/option.hpp>
#include <stout/os.hpp>

//#include "common/parse.hpp"
#include "common/protobuf_utils.hpp"

using namespace mesos;
using namespace mesos::internal;

using process::Future;
using process::Owned;
using process::UPID;

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class MesosLambdaScheduler : public Scheduler
{
public:
  MesosLambdaScheduler(
      const string& _name,
      const string& _command,
      // const Option<hashmap<string, string>>& _environment,
      const string& _resources,
      const Option<string>& _dockerImage,
      const string& _containerizer)
    : name(_name),
      command(_command),
      // environment(_environment),
      resources(_resources),
      dockerImage(_dockerImage),
      containerizer(_containerizer),
      launched(false) {}

  virtual ~MesosLambdaScheduler() {}

  std::string get_response_payload() { return m_response_payload; }

  virtual void registered(
      SchedulerDriver* _driver,
      const FrameworkID& _frameworkId,
      const MasterInfo& _masterInfo) {
    cout << "Framework registered with " << _frameworkId << endl;
  }

  virtual void reregistered(
      SchedulerDriver* _driver,
      const MasterInfo& _masterInfo) {
    cout << "Framework re-registered" << endl;
  }

  virtual void disconnected(
      SchedulerDriver* driver) {}

  virtual void resourceOffers(
      SchedulerDriver* driver,
      const vector<Offer>& offers)
  {
    static const Try<Resources> TASK_RESOURCES = Resources::parse(resources);

    if (TASK_RESOURCES.isError()) {
      cerr << "Failed to parse resources '" << resources
           << "': " << TASK_RESOURCES.error() << endl;
      driver->abort();
      return;
    }

    for (const Offer& offer : offers) {
      if (!launched &&
          Resources(offer.resources()).contains(TASK_RESOURCES.get())) {
        TaskInfo task;
        task.set_name(name);
        task.mutable_task_id()->set_value(name);
        task.mutable_slave_id()->MergeFrom(offer.slave_id());
        task.mutable_resources()->CopyFrom(TASK_RESOURCES.get());

        CommandInfo* commandInfo = task.mutable_command();
        commandInfo->set_value(command);
        // if (environment.isSome()) {
        //   Environment* environment_ = commandInfo->mutable_environment();
        //   foreachpair (const std::string& name,
        //                const std::string& value,
        //                environment.get()) {
        //     Environment_Variable* environmentVariable =
        //       environment_->add_variables();
        //     environmentVariable->set_name(name);
        //     environmentVariable->set_value(value);
        //   }
        // }

        if (dockerImage.isSome()) {
          ContainerInfo containerInfo;

          if (containerizer == "mesos") {
            containerInfo.set_type(ContainerInfo::MESOS);

            ContainerInfo::MesosInfo mesosInfo;

            Image mesosImage;
            mesosImage.set_type(Image::DOCKER);
            mesosImage.mutable_docker()->set_name(dockerImage.get());
            mesosInfo.mutable_image()->CopyFrom(mesosImage);

            containerInfo.mutable_mesos()->CopyFrom(mesosInfo);
          } else if (containerizer == "docker") {
            containerInfo.set_type(ContainerInfo::DOCKER);

            ContainerInfo::DockerInfo dockerInfo;
            dockerInfo.set_image(dockerImage.get());

            containerInfo.mutable_docker()->CopyFrom(dockerInfo);
          } else {
            cerr << "Unsupported containerizer: " << containerizer << endl;;

            driver->abort();

            return;
          }

          task.mutable_container()->CopyFrom(containerInfo);
        }

        vector<TaskInfo> tasks;
        tasks.push_back(task);

        driver->launchTasks(offer.id(), tasks);
        cout << "task " << name << " submitted to slave "
             << offer.slave_id() << endl;

        launched = true;
      } else {
        driver->declineOffer(offer.id());
      }
    }
  }

  virtual void offerRescinded(
      SchedulerDriver* driver,
      const OfferID& offerId) {}

  virtual void statusUpdate(
      SchedulerDriver* driver,
      const TaskStatus& status)
  {
    CHECK_EQ(name, status.task_id().value());
    cout << "Received status update " << status.state()
         << " for task " << status.task_id() << endl;

    cout << "RECEIVED UPDATE:" << endl;
    cout << "Message: " << status.message() << endl;

    if(status.state() == TASK_FINISHED) {

      m_response_payload = std::string(status.data());
      cout << "=== Dumping data ===" << endl;
      cout << status.data() << endl;
      cout << "====================" << endl;
    }

    if (mesos::internal::protobuf::isTerminalState(status.state())) {
      driver->stop();
    }
  }

  virtual void frameworkMessage(
      SchedulerDriver* driver,
      const ExecutorID& executorId,
      const SlaveID& slaveId,
      const string& data) {

    cout << "Framework message!" << endl;
  }

  virtual void slaveLost(
      SchedulerDriver* driver,
      const SlaveID& sid) {}

  virtual void executorLost(
      SchedulerDriver* driver,
      const ExecutorID& executorID,
      const SlaveID& slaveID,
      int status) {}

  virtual void error(
      SchedulerDriver* driver,
      const string& message) {}

private:
  std::string m_response_payload;

  const string name;
  const string command;
  // const Option<hashmap<string, string>> environment;
  const string resources;
  const Option<string> dockerImage;
  const string containerizer;
  bool launched;
};
