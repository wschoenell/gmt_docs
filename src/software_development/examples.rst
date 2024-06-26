.. _running_examples:

Running Examples
================

GMT Components
---------------

Components are the simple building blocks in the GMT software architecture and they will be used extensively at all levels.
A software component is a unit of composition with contractually specified interfaces and explicit context dependencies only.
Ports are interaction points for components. They can be the data inputs (required interface) or the data outputs (provided interface) of our components.
By using a port, our component is able to de-couple the internals of the component from external entities. The component does not assume anything about external entities.
Any data type can be used in ports: C++ templates, dynamic types, etc.

A GMT Component is able to perform a periodic task and/or is able to interact with other similar components. Components are reactive to their interfaces from the very beginning.
Instead of being called by an external entity they have an internal loop (step) which reacts continuously to inputs and drives the corresponding outputs.
Commands in the sense of “remote calls” from other components do not exist. Only reaction to inputs.

Example: instead of having a moveTo(position) and getPosition() commands we can have:

    - An input port with the demanded_position
    - An output with the current_position
    - The step() reacts to the demanded position and updates the current

The most simple possible component that can be created is like this:

.. code-block:: cpp

    struct SimpleComponent : public Component<Component_Conf>  // most simple possible component
    {
        SimpleComponent(std::string uri): Component(uri) {}
        inline void step()  { log_info("hello, component world"); } // prints a log message every second (default)
    };

This component does not have any input or output ports but has a *step()* function that runs at the default rate of 1 second.
This step only creates a log message at every step but could be used for any other purpose.

Let's see an example with input and output ports. The following diagram show a component called Sum with two inputs ports and one output port.
The output port is the sum of the inputs ports:

.. code-block:: bash

                    Sum
                 +--------+
                 |        |
              <--|x1    x2|<--        x1 = x2+x3
                 |        |
                 |      x3|<--
                 |        |
                 +--------+


The class definition for this component is:

.. code-block:: cpp

    class Sum : public Component<Component_Conf>
    {
        public:
            inline Sum(std::string uri): Component(uri), x1(0), x2(1), x3(2) {}

            inline void setup() {
                new_output_port(x1,"x1");
                new_input_port(x2,"x2");
                new_input_port(x3,"x3");
            }

            inline void step() {
                x1 = x2 + x3;
                log_info("sum = " + std::to_string(x1));
            }

        private:
            int x1, x2, x3;
    };

This example only includes integer ports but any type can be used.  The input and output ports default value can be set during a the constructor.
The step() computes the sum and the setup() function is responsible for associating the internal component variables with the corresponding ports.
Once this association is made the ports automatically update (send or receive) the inputs and output values at each step.
The component does not need to do update the ports because, for any component, the framework performs the following generic step:

.. code-block:: cpp

    void Component::generic_step() {
        read_setup();
        read_inputs();
        step();
        write_outputs();
    }

Connecting Components
---------------------

The great thing about components is that they can be easily connected with other components to perform a much more complex function.
A connector is a link that enables communication between components. A connector simply means that runtime component instances can communicate using their ports.

Let's create a component type called Inc with just one output port. This component has an initial value that is incremented at every step.
If the internal counter is reset every 1000 steps.

.. code-block:: bash

               Inc
           ----------
           |        |
        <--|x1      |      x1 = x1+1
           |        |
           |        |
           |        |
           ----------


This is the equivalent c++ class:

.. code-block:: cpp

    class Inc : public Component<Component_Conf>
    {
        public:
            inline Inc(std::string uri): Component(uri), x1(0) {}

            inline void setup() {
                new_output_port(x1,"x1");
            }

            inline void step() {
                x1++;
                if (x1>1000) x1=0;
                log_info("x1 = " + std::to_string(x1));
            }

        private:
            int x1;
    };

Now we can create several instances of the Inc and Sum component classes and, connecting inputs and outputs, we could create a component network like this:

.. code-block:: bash

                       sum1                inc1
      sum2            +---+               +---+
      +---+           |   |<--------------|   |
      |   |<----------| + |               +---+
   <--| + |           |   |<-------+
      |   |<-----+    +---+        |       inc2
      +---+      |                 |      +---+
                 |                 +------|   |
                 |                        +---+
                 |
                 |                         inc3
                 |                        +---+
                 +------------------------|   |
                                          +---+

Note that this diagram includes five **connectors** between components. These connectors allow to move data between components.
At each step, if the connector is setup, the output of each component instance is sent to the next component in the connector.
It's important to know that **each component does not know if connectors exist**: they do not have references to other components.
Each component reads its inputs and proceeds according these values to run the step and write the corresponding outputs.
Components will run even when their ports are not connected to other components.

Running the example components
------------------------------

In one terminal open a log client so

.. code-block:: bash

    $ gds log_service client gmt
    log client subscribed at port tcp://127.0.0.1:9995
    subscription topic: gmt

In a second terminal run the example components

.. code-block:: bash

    $ run_example_components

After this command the example components will start to send log messages to the log service so we should see something like:

.. code-block:: bash

    $ gds log_service client gmt
    log client subscribed at port tcp://127.0.0.1:9995
    subscription topic: gmt
    [INF] 2016-09-06T16:24:38.730Z [gmt://examples/my_app:7000] starting
    [INF] 2016-09-06T16:24:38.731Z [gmt://examples/my_app:7000] init
    [INF] 2016-09-06T16:24:38.731Z [gmt://examples/simple_component:8000] starting
    [INF] 2016-09-06T16:24:38.731Z [gmt://examples/component_inc1:8001] starting
    [INF] 2016-09-06T16:24:38.731Z [gmt://examples/component_inc2:8002] starting
    [INF] 2016-09-06T16:24:38.731Z [gmt://examples/component_inc3:8003] starting
    [INF] 2016-09-06T16:24:38.731Z [gmt://examples/component_sum1:8004] starting
    [INF] 2016-09-06T16:24:38.731Z [gmt://examples/component_sum2:8005] starting
    [INF] 2016-09-06T16:24:38.732Z [gmt://examples/component_inc2:8002] x1 = 1
    [INF] 2016-09-06T16:24:38.732Z [gmt://examples/simple_component:8000] hello, component world
    [INF] 2016-09-06T16:24:38.732Z [gmt://examples/component_inc3:8003] x1 = 1
    [INF] 2016-09-06T16:24:38.732Z [gmt://examples/component_inc1:8001] x1 = 1
    [INF] 2016-09-06T16:24:38.732Z [gmt://examples/my_app:7000] app step num = 0
    [INF] 2016-09-06T16:24:38.732Z [gmt://examples/component_sum1:8004] sum = 3
    [INF] 2016-09-06T16:24:38.732Z [gmt://examples/component_sum2:8005] sum = 3
    [INF] 2016-09-06T16:24:39.732Z [gmt://examples/component_inc2:8002] x1 = 2
    [INF] 2016-09-06T16:24:39.732Z [gmt://examples/simple_component:8000] hello, component world

At the startup the components are not setup and ports can't be connected so each component is running alone.
For example, let's see what the component sum2 is doing:

.. code-block:: bash

    $ gds log_service client sum2
    log client subscribed at port tcp://127.0.0.1:9995
    subscription topic: sum2
    [INF] 2016-09-06T16:34:57.300Z [gmt://examples/component_sum2:8005] starting
    [INF] 2016-09-06T16:34:57.301Z [gmt://examples/component_sum2:8005] sum = 3
    [INF] 2016-09-06T16:34:58.301Z [gmt://examples/component_sum2:8005] sum = 3
    [INF] 2016-09-06T16:34:59.301Z [gmt://examples/component_sum2:8005] sum = 3
    [INF] 2016-09-06T16:35:00.301Z [gmt://examples/component_sum2:8005] sum = 3
    [INF] 2016-09-06T16:35:01.301Z [gmt://examples/component_sum2:8005] sum = 3

In this case the component sum2 is still not receiving values from other components so it's only processing the default inputs.
As the ports x2 and x3 are always:

::
                        sum2.x2 = 1   (see default constructor for Sum class components)
                        sum2.x3 = 2

then

::
                        sum2.x1 = 3

In order to receive data from other components all components ports shall be setup.

Setup Components and Ports
--------------------------

When a components starts it will automatically run its step() loop at the required rate.
On each step() it checks its inputs, process the inputs and sets the outputs. At the init phase only the setup port is ready.
It can receive the following component setup:

    - tcpip_port num
    - Rate (can be dinamically changed)
    - Inputs: list of ports
    - Outputs: list of ports
    - Per each port: name, protocol (push/pull …), url, mode, step_rate

Any component can receive its setup using:

    *Usage:*  ``gds setup -m [model] -e [element]``

For example, *component_sum1* has the following configuration in:

::

    Component "component_inc1",
        default:
            properties:
                uri_path: 'gmt://examples/component_inc1'
                host: '127.0.0.1'
                port: 8001
                rate: 1000000
                acl: 'private'

            state_vars: []

            input_ports: []

            output_ports: [
                { name: 'x1', protocol: 'push',  url: 'tcp://127.0.0.1:9001', blocking_mode: 'async', max_rate: 1 }
            ]

To connect all the example components as described in the previous section the following commands can be used:

.. code-block:: bash

    $ gds setup -m runtime -e component_inc1
    $ gds setup -m runtime -e component_inc2
    $ gds setup -m runtime -e component_inc3
    $ gds setup -m runtime -e component_sum1
    $ gds setup -m runtime -e component_sum2

and in terminal two we can see that the the component A2 is num processing the other component inputs.

.. code-block:: bash

    $ gds log_service client sum2
    log client subscribed at port tcp://127.0.0.1:9995
    subscription topic: sum2
    [INF] 2016-09-06T17:21:57.385Z [gmt://examples/component_sum2:8005] sum = 3
    [INF] 2016-09-06T17:21:58.385Z [gmt://examples/component_sum2:8005] sum = 3
    [INF] 2016-09-06T17:21:59.385Z [gmt://examples/component_sum2:8005] sum = 3
    [INF] 2016-09-06T17:22:00.386Z [gmt://examples/component_sum2:8005] new setup received
    [INF] 2016-09-06T17:22:00.386Z [gmt://examples/component_sum2:8005] new port [x1 -> tcp://127.0.0.1:9005/push~async@1]
    [INF] 2016-09-06T17:22:00.386Z [gmt://examples/component_sum2:8005] new port [x2 -> tcp://127.0.0.1:9004/pull~async@1]
    [INF] 2016-09-06T17:22:00.386Z [gmt://examples/component_sum2:8005] new port [x3 -> tcp://127.0.0.1:9003/pull~async@1]
    [INF] 2016-09-06T17:22:00.386Z [gmt://examples/component_sum2:8005] sum = 3
    [INF] 2016-09-06T17:22:01.386Z [gmt://examples/component_sum2:8005] sum = 22
    [INF] 2016-09-06T17:22:02.386Z [gmt://examples/component_sum2:8005] sum = 63
    [INF] 2016-09-06T17:22:03.386Z [gmt://examples/component_sum2:8005] sum = 63
    [INF] 2016-09-06T17:22:04.386Z [gmt://examples/component_sum2:8005] sum = 63
    [INF] 2016-09-06T17:22:05.386Z [gmt://examples/component_sum2:8005] sum = 63
    [INF] 2016-09-06T17:22:06.386Z [gmt://examples/component_sum2:8005] sum = 63
    [INF] 2016-09-06T17:22:07.386Z [gmt://examples/component_sum2:8005] sum = 63
    [INF] 2016-09-06T17:22:08.386Z [gmt://examples/component_sum2:8005] sum = 63
    [INF] 2016-09-06T17:22:09.386Z [gmt://examples/component_sum2:8005] sum = 63
    [INF] 2016-09-06T17:22:10.386Z [gmt://examples/component_sum2:8005] sum = 63
    [INF] 2016-09-06T17:22:11.386Z [gmt://examples/component_sum2:8005] sum = 73
    [INF] 2016-09-06T17:22:12.386Z [gmt://examples/component_sum2:8005] sum = 93
    [INF] 2016-09-06T17:22:13.386Z [gmt://examples/component_sum2:8005] sum = 93

All components, except the application, run at the 1s rate. The application has a 3s step rate.
The application will run for 1000 steps and then will automatically finish.

Controllers Example
-------------------

Controllers are the components responsible to control the different telescope or instruments devices.
Usually controllers are connected to the external sensors and actuators using the Hardware Adapters that we have seen before.
Many different kinds of controllers will be created for the telescope depending on the type of equipment to be controlled.
In order to know or maintain the state of the system under control the controllers will use State Variables.

A simple controller with a temperature state variable (simulated, with no real hardware) is provided.
Note that the controller implementation c++ code is split in three different files: general, setup and step.
This is for code generation purposes and because it's easy to focus development only on the step() loop.

The configuration for this controller is:

.. code-block:: bash

    Controller "simple_controller",
        default:
            properties:
                uri_path: 'gmt://examples/simple_controller'
                host: '127.0.0.1'
                port: 8001
                rate: 1000000
                acl: 'private'

            state_vars: [
                { type: 'float', init: 23.4, min: 2.0, max: 26.1, url: 'tcp://127.0.0.1:9011' }
            ]

            input_ports: [
                { name: 'temperature_setpoint', protocol: 'pull',  url: 'tcp://127.0.0.1:9001', blocking_mode: 'async', max_rate: 1 }
            ]
            output_ports: [
                { name: 'temperature_monitor', protocol: 'push',  url: 'tcp://127.0.0.1:9011', blocking_mode: 'async', max_rate: 1000 }
            ]

In order to run this controller open a log_service client in a different terminal and in other terminal use:

.. code-block:: bash

  $ run_simple_controller &

then send the setup using:

  $ gds setup -m runtime -e simple_controller

Check Monitored Values
----------------------

Telemetry is a distributed service than can be started independently on each component as:

  *Usage:*  ``gds telemetry_service monitor -m model -e element``

This command will start the telemetry adapters for this component and they will send monitored values to the server.
The telemetry service will:

        - store data points in the database
        - send the values to subscribers
        - accept queries from clients

Note that monitors are the values of the output ports.

For example, the following command will start sending 'temperature' data points to the telemetry service.

.. code-block:: bash

  $ gds telemetry_service monitor -m runtime -e simple_controller

To check the monitored values a telemetry client can be started in another unix terminal:

  *Usage:*  ``gds telemetry_service client monitor_name``

In the case will have:

    $ gds telemetry_service client temperature

And the monitored values shall be received on this terminal.

A query can also be sent to the telemetry service like this:

  *Usage:*  ``gds telemetry_service query monitor_name num``

For example, the following will list the last 12 temperature values (from newest to oldest) from this controller:

.. code-block:: bash

  $ gds telemetry_service query gmt://examples/simple_controller/temperature_monitor 10

All telemetry queries have to be done on specific monitors.

Send a Command to a Controller
------------------------------

Some numeric values can be sent directly to ports of any controller like this:

  *Usage:*  ``gds push_value port_name value -m model -e element``


For example:

.. code-block:: bash

  $ gds push_value temperature_setpoint 23.2 -m runtime -e simple_controller

This will send a temperature setpoint of 23.2 to the controller which should react to this change.
Monitored values should change on the telemetry client.
When the temperature values are out the the defined range the log client should show the corresponding warning messages.

.. note::

    In order to finish the example the run_simple_controller process shall be killed.
