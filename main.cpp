#include <iostream>
#include <yarp/os/RFModule.h>
#include <yarp/os/Network.h>
#include <yarp/os/LogStream.h>

using namespace std;
using namespace yarp::os;

class MyModule:public RFModule
{
    RpcServer handlerPort; //a port to handle messages
    int count;

    double period;
public:

    double getPeriod()
    {
        return period; //module periodicity (seconds)
    }

    /*
    * This is our main function. Will be called periodically every getPeriod() seconds.
    */
    bool updateModule()
    {
        count++;
        //printf("[%d] updateModule\n", count);
        yInfo()<<"["<<count<<"]"<< " updateModule... ";

        return true;
    }

    /*
    * Message handler. Manage period and quit commands.
    */
    bool respond(const Bottle& command, Bottle& reply)
    {
        yInfo()<<"Responding to command";

        //parse input
        if (command.check("period"))
        {
            period=command.find("period").asFloat64();
            reply.addString("ack");
            return true;

        }

        return RFModule::respond(command, reply);
    }


    /*
    * Configure function. Receive a previously initialized
    * resource finder object. Use it to configure your module.
    * Open port and attach it to message handler.
    */
    bool configure(yarp::os::ResourceFinder &rf)
    {
        count=0;
        period=1.0; //default value
        handlerPort.open("/myModule");
        attach(handlerPort);

        //user resource finder to parse parameter --period

        if (rf.check("period"))
            period=rf.find("period").asFloat64();

        return true;
    }

    /*
    * Interrupt function.
    */
    bool interruptModule()
    {
        yInfo()<<"Interrupting your module, for port cleanup";
        return true;
    }

    /*
    * Close function, to perform cleanup.
    */
    bool close()
    {
        yInfo()<<"Calling close function";
        handlerPort.close();
        return true;
    }
};

int main(int argc, char * argv[])
{
    Network yarp;

    MyModule module;
    ResourceFinder rf;
    rf.configure(argc, argv);
    // rf.setVerbose(true);

    yInfo()<<"Configure module...";
    module.configure(rf);
    yInfo()<<"Start module...";
    module.runModule();

    yInfo()<<"Main returning...";
    return 0;
}


