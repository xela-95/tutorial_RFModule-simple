Tutorial on YARP RFModule
=========================
This tutorial will show you YARP support classes for handling command line parameters and writing modules that perform periodic activities and respond to network commands. 

In particular we will use:
- [yarp::os::ResourceFinder](http://www.yarp.it/classyarp_1_1os_1_1ResourceFinder.html)
- [yarp::os::RFModule](http://www.yarp.it/classyarp_1_1os_1_1RFModule.html)


# How to build the tutorial
 - Open a terminal and switch to the folder which contains C++ code (e.g., `tutorial_rfmodule-simple/`)
 - create a build directory
 - compile and build

 ```bash
 $ cd tutorial_rfmodule-simple
 $ mkdir build
 $ cd build
 $ cmake ../
 $ make
 ```

# How to run the tutorial
- make sure yarp server is running (`yarp where`), if not open a terminal and run it :
```bash
$ yarpserver --write
```
- open another terminal and switch to the build directory and run the `tutorial_rfmodule-simple`:

```bash
$ ./tutorial_rfmodule-simple
```

The module will start executing the function `updateModule()` with periodicity of 1.0 seconds:

```bash
$ ./tutorial_RFModule-simple 
[INFO]Configure module... 
[INFO]Start module... 
[INFO][ 1 ]  updateModule...  
[INFO][ 2 ]  updateModule...  
[INFO][ 3 ]  updateModule...  
[INFO][ 4 ]  updateModule...  
```

You can now terminate the module by hitting `ctrl+C` at the terminal:

```bash
^C[INFO][try 1 of 3] Trying to shut down.
[INFO]RFModule closing.
[INFO]RFModule finished.
[INFO]Main returning... 
```

Even with this simple code we achieved interesting functionalities. We have an executable which performs periodic activities, which is a frequent requirement in robotics. 
In addition, our module can be terminated `smoothly` by sending a `ctrl+C` signal. This is important if we want to perform shutdown operations like parking the robot, 
turning off the motors etc.

We will now enhance this module with the following functionlities:

- Change periodicity with command line parameter `--period`
- Add an interface to respond to commands from a port

## Parsing command line parameters

An instance of ResourceFinder, `rf`, is initialized with command line parameters `argc`, `argv`. It is easy to lookup parameters from the command line using 
the  following code inside the `RFModule::configure()` function:

```c++
  bool configure(yarp::os::ResourceFinder &rf)
    {
        count=0;
        period=1.0; //default value

        //user resource finder to parse parameter --period
        if (rf.check("period"))
             period=rf.find("period").asDouble();

        return true;
    }
    
```

Recompile and execute. See the different behavior of:

```bash
$ ./tutorial_rfmodule-simple --period 0.1
$ ./tutorial_rfmodule-simple --period 2.0
```

## Adding an interface to respond to commands from a port

The module already declares an `RPCServer` port. We now add code to open the port, and configure the module to dispatch messages received from the port 
to a respond function.

Add the following code within `configure':


```c++
    handlerPort.open("/myModule");
    attach(handlerPort);
```

Add the following function:

```c++
    bool respond(const Bottle& command, Bottle& reply)
    {
        yInfo()<<"Responding to command";

        //parse input
        if (command.check("period"))
        {
            period=command.find("period").asDouble();
            reply.addString("ack");
            return true;

        }

        // dispatch received data to the RFModule::respond() function
        // this function handles the quit message
        return RFModule::respond(command, reply);
    }
```

Now all messages received from the port `MyModule` will be dispatched to the method `respond`. We can parse the message and modify the behavior of the module accordingly. Notice that by default the `RFModule` will automatically shutdown when a quit message is received.

You can now talk to the respond method using the `yarp rpc` tool, for example:

```bash
$ yarp rpc /myModule
>>period 2
Response: ack
>>quit
Response: [bye]
>>
```

The last command will terminete our module. 

## Managing termination: cleanup of resources
Proper termination of a module is a critical issue. The module may be blocked waiting for data from a port or a mutex. Before asking the module to terminate we need to unblock the module. This is done by overriding the function `RFModule::interrupt`:

```c++
/*
 * Interrupt function. Use this function to execute
 * operations that need to be done before module shutdown.
*/
bool interruptModule()
{
    yInfo()<<"Interrupting your module";
    return true;
}
```

Other activities need to be done when the module shuts down, before exiting the main function. This is done by overriding the function `RFModule::close`:

```c++
/*
* Close function, to perform operation after shutdown
*/
bool close()
{
     yInfo()<<"Calling close function";
     handlerPort.close();
     return true;
}
```   

Now the behavior of the module when shutting down should be something like this:
```bash
[INFO][ 55 ]  updateModule...  
[INFO][ 56 ]  updateModule...  
[INFO][ 57 ]  updateModule...  
[INFO]Responding to command 
[INFO]Interrupting your module 
[INFO]RFModule closing.
[INFO]Calling close function 
[INFO]RFModule finished.
[INFO]Main returning... 
```

Notice in particular the order of execution of the functions.


## Tutorial Solution

Note that the solution is provided from within the branch [**`solution`**](https://github.com/vvv-school/tutorial_rfmodule-simple/tree/solution).

# [How to complete the assignment](https://github.com/vvv-school/vvv-school.github.io/blob/master/instructions/how-to-complete-assignments.md)


