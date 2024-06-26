.. _release_1:

Release 1
=========

Contents of release 1.14.0
--------------------------

- Core Frameworks

  - Updated Node.js Core Framework

    - Step Groups

      - The step function of the Component class supports a new 'group' parameter. When used, only the Component Behaviors and the Feature Behaviors matching that group will be executed.
      - Behaviors have a new 'group' attribute. Their 'apply' function is only executed if the 'group' argument matches the specified Behavior group.
      - Connectors have a new 'group' attribute. The group is propagated to the associated EndPoints Behaviors, so communication between components can also be orchestrated.
      - CoreContainer can now invoke the step function of its Components. This allows the orchestration of the behaviors from different Components using the group attribute.
      - The new 'scan_mode' property allows to specify whether a Component triggers its own step or is triggered externally as part of a step group.

    - Sampling Policies

      - Added a new 'sampling_policies' property with a set of predefined default sampling policies (silent, standby, norm, debug).
      - Added a new 'sampling_mode' property that can be used to set the sampling policies for all Component DataIOs.

    - Query sorting

      - Core Services now support a 'sort' expression when invoking a query
      - Updated 'gas db' command to support sorting arguments

    - Added new unit tests using node:test
    - Added new parameter depth to 'grs inspect' to display large, nested structures
    - 'grs inspect' can now show the status of Component behaviors, proxies and handlers
    - Minor bug fixes

  - Updated C/C++ Core Framework

    - Step Groups

      - generic_step changed to run_step, and now accepts a group parameter
      - group added to behaviors, which will not execute if the group passed as the _apply parameter is not the behavior group
      - group added to connectors
      - added the Dispatcher class, which can be used to orchestrate the execution of components
      - added scan_mode property in Component

    - Added ability to lock a ValueClassifier, or set a State Variable to not be controllable, preventing external updates
    - Updated Alarm and Fault Event fields, synchronized with Node.js implementation (#281, #288) 
    - Fixed gmt_issues #201: Alarm acknowledgement
    - Fixed gmt_issues #237: The component execution will no longer be affected by setting a priority on a non-rt kernel

  - Updated Python Core Framework

    - Server Proxy and Data Adapters
    - Fixed issues with Sampling behavior (#284)
    - Fixed gmt_issues #248: Component class init default properties (#287)

  - Updated C/C++ IO Framework

    - EtherCAT SDO/PDO handling optimization
    - Minor bug fixes


Contents of release 1.13.0
--------------------------

- Core Frameworks
  
  - Updated C/C++ Core Framework

    - DataIOs now are part of collection fields. For example, to access the property `acl` now, one should use `properties->acl->value`. Same thing for inputs, outputs, state_vars, alarms and faults.
    - Added all the remaining core adapters to the C++ frameworks: conf, data, job, supervisor and alarm.
    - Added HealthSupervisingBehavior
    - Changed telemetry field from `data` to `value` to be consistent with other services

  - Updated Coffee Core Framework

    - Added initial support to the UI framework WebSockets

  - Updated Python Core Framework

    - Added `new_async_input_handler` method to Python DataIOs.
    - Added `is_step_counter` method to PeriodicThreads and, therefore, Components
    - Changed telemetry field from `data` to `value` to be consistent with other services
    - Bugfixes in the Periodic Behavior

  - Updated Development Framework

    - Fixed `gds info` command


Contents of release 1.12.0
--------------------------

- Core Frameworks
  
  - Updated C/C++ Core Framework

    - Added support for log messages containing formatted strings with parameters.
    - Added a class hierarchy to compute several kinds of moving averages.
    - Added convenience methods to the TimingAnalyzer class for getting measured start and end times.
    - Added an optional start-time delay in PeriodicThread class.
    - Fixed a minor issue with incoming Fault connectors.

  - Updated Python Core Framework

    - Improved error handling.
 
  - Updated C/C++ Development Framework

    - Fixed a code generation issue caused by a limitation in MsgPack macros when a struct had too many fields.
    - Code generation of Hardware Adapters now generate a parameter for DataIO during data objects creation, allowing the use of an async forward handler to automatically write SDOs. 
    - Fixed gmt_issues #218: C++ applications are now only created for C++ components.
    - Fixed gmt_issues #181: The module.mk file will no longer be overwritten.
    - Fixed gmt_issues #195: Corrected CaMeLiZation of variables and methods for inherited packages.

  - Updated Python Development Framework

    - Fixed issue where code generation did not call the Component setup method at the appropriate time.


Contents of release 1.11.0
--------------------------

- Core Frameworks

  - Updated C/C++ Core Framework

    - Added a forwarding asynchronous handler. This handler will automatically copy the data of an async readable feature to a writeable feature. Optionally, it will trigger an async send of the recipient feature.
    - Telemetry time stamps are now stored per step instead of individually for each telemetry value. This change optimizes space usage in the database and synchronizes timestamps for data collected within the same step.

  - Updated Python Core Framework

    - Minor bug fixes and example updates.
    - Added send() method to dataio implementation
    - Updates to preliminary Test Framework implementation, including the ability to skip certain tests and specifying a host runner.
    - Fixed issue with python ComponentProxy not working with C++ components (fixes GMTO/gmt_issues#227)

  - Updated C/C++ I/O Framework

    - Fixed minor issue in EtherCAT where the first PDO (ID 0) was not being sent correctly


Contents of release 1.10.0
--------------------------

- Core Frameworks

  - Updated C/C++ Core Framework

    - Added timers to the step function to assess performance
    - Fixed adding scan_rate in the properties declaration
    - Fixed issue with telemetry sampling rate that caused erroneous sampling rates in some cases

  - Updated Development Framework

    - Fixed issue #212: Type ‘float’ is now mapped to ‘double’ in C++, per modeling guidelines
    - Removed stale zeromq dependency

  - Updated C/C++ IO Framework

    - EtherCAT HW Adapter refactoring

      - Fixed names, namespaces and includes
      - Improved robustness of several data structures and decision trees
      - Updated to follow code formatting guidelines
      - Improved encapsulation of etherlab library method calls
      - Fixed errors and typos in type name identifiers and descriptions

    - EtherCAT HW Adapter updates

      - EtherCAT bus configuration is now loaded and applied during the setup, before the step loop starts
      - The state machine of the EtherCAT master has been changed to a StateMachine Behavior
      - The SDO read/write mechanisms have been changed, as the previous strategy led to non-deterministic errors on sdo read or write. Now the framework provides:

        - Methods to read/write SDO during the setup execution, before starting the RT process. These methods are blocking and ensure that all read/write operations are executed before the step
        - Methods to read/write SDO during the step execution. These methods are non-blocking. The operations will take more than one cycle to be completed; upon completion, a user-provided callback is executed

  - Updated UI Framework

    - Improved build environment for continuous integration and dependencies on other modules
    - Improved error logging
    - Fixed issue with multi-dimensional array parsing


Contents of release 1.9.0
-------------------------

- Core Frameworks

  - Updated Core Framework

    - Implemented Python Component life-cycle, including 
      - Resource allocation/deallocation
      - op_state_sv state machine compliance
    - Implemented Python Component Distributed Communication, including
      - Reading and writing remote data
      - Asynchronous communication
      - Pseudo synchronous communication
      - Publishing and receiving data streams
    - Implemented Python Component Behaviors, including
      - Periodic behavior
      - Dynamic behavior
      - Reusable behaviors
      - OpState behavior
    - Implemented Python Component Service Access (Container), including
      - Log events
      - Telemetry
      - Local logging
    - Fixed minor issues in the service data ports

  - Updated IO Framework

    - Fixed small issues in EtherCAT hardware adapter

      - Vendor_id type changed from uint8 to uint32
      - Updated support for addressing slaves by alias
      - Set slave to PRE_OP after firmware update
      - Re-read SDOs on slave when transitioning from IDLE to RUNNING
      - Added an error log when the master goes into ERROR state
      - #116: Removed erroneous "Failed to get slave info: invalid argument" error message on hw_adapter_app start-up
      - #124: Added slave type info to get_ethercat_slave_info functions
      - #125: Added exception checking in get_ethercat_slave_info functions for invalid values (slave does not exist) 
      - #126: Prevent application from crashing if booting without no slaves connected on the bus (but defined in the config) 

  - Updated Development Framework

    - Added OCS Module code generation in Python
    - Added OCS Module building support for Python platform
    - Implemented Model definition mapping to Python

  - Updated UI Framework

    - Split framework into separate libraries with separate concerns to make future development easier

    - Visualization Package development with dynamic rendering
  
      - visualization packages can be run as standalone apps using Navigator as the launcher
      - single source of modules in GMT_LOCAL/node_modules
      - visualization packages can register data with Navigator context
      
  - React library implementation

    - provides a simple and systematic way to retrieve and display connector data
    - provides interaction primitives that can be used to build complex visualization packages

  - UI Styles update

    - unified look and feel through CSS styling
    - updated widgets for buttons and data structures

- Navigator Application
    
    - Stability and performance improvements
    
    - Updated tab system
      
      - drag & drop support
      - tab groups
      - complex layouts
      - faster tab switching/rendering
    
    - Create new inspect instances by pasting comands grs inspect commands
    
    - Load standalone vis packages
    
    - Develop vis packages with live-reload

- Documentation: Software Development

  - Updated :ref:`installation`

    - Fixed commandline examples
    - Added installation instructions for Python and related libraries

  - Updated :ref:`upgrade`

    - Added instructions for upgrading from v1.8 to v1.9

- Known issues

  - Missing documentation for Python model-language mapping
  - Missing documentatino for updated widget library


Contents of release 1.8.1
-------------------------

- Core Frameworks

  - Updated UI Framework

    - Added React framework library, allowing vis panel developers to use the same library used by Navigator.
    - Consolidated CSS styles, allowing vis panels to inherit Navigator's look and feel
    - Added new developer tools to support realtime vis panel rendering during development

- Navigator Application

  - Updated data rendering
  
    - better representation of on/off states
    - distinct colors for data types
    - fixed rendering issues for complex data types
    - cleaner and more consistent design
    - fail gracefully on unknown types

  - Updated data controls

    - allow selection between connecting to service data ports or sockets
    - play/stop interface to start and stop data streaming from components
    - show errors on connection failure

  - Improved interface for sending values to component features

    - notification on successfully sending a value
    - allow storing and editing a list of commonly used values per feature
    - auto saves last value sent to list of commonly used values

  - Improved toolboxes

    - selecting enums and state machines will show all possible values
    - selecting structs will show properties and types

  - Auto-generated intance tabs

    - selecting an instance now opens a more comprehensive view with inputs/outputs/state_vars

  - Preliminary plots

    - 2D plot view of a single scalar value

  - New window tiling system with improved panel editor

    - add/remove new panels to the grid
    - resize and re-order panels
    - Add panels from different component instances to the same view

  - Camera views

    - GMT site cameras included in the list of pre-defined camera feeds that can be added from the panel menu

  - Vis panel developer tools

    - allows switching between production and development modes
    - allows mock data to be received by the UI, simulating a component
    - realtime vis panel rendering during development

  - Improved error displays when failing to render

    - provide full stacktrace for better debugging
    - automated attempts to recover from error

  - Performance improvements

    - threaded component proxy instances
    - rendering improvements
    - more memoization, more fun 

- Implementation Examples

  - The HDK example has been updated to be synchronized with Core Frameworks version 1.8

- Documentation: Software Development

  - Updated :ref:`gds_guide`

    - Removed support for "gds clone" command. Developers are requested to use the "git clone" command directly.

  - Updated :ref:`grs_guide`

    - Added description of "grs db" command for Database operations when the instance implements a database server
    - Added more examples of grs commands

  - Updated :ref:`mapping_model_to_coffee`

    - Updated diagrams for Fault FSM and Alarm FSM
    - Updated description for StructType, Enum and StateMachine Data Types
    - Updated description of communication between components to be consistent with Core Frameworks version 1.8
    - Added section describing the ComponentProxy

  - Updated :ref:`modeling_guidelines`

    - Updated to be consistent with Core Frameworks version 1.8
    - New examples and diagrams

  - Updated :ref:`test_guidelines`

    - Updated examples to use “op_state_value” instead of “ops_state_value” to be consistent with the version 1.8 of the core frameworks

  - Updated :ref:`ui_fwk` documentation

Contents of release 1.8.0
-------------------------

- Release distribution and installation:

  - Updated mechanism for packaging and distributing the Navigator application for MacOS and Linux

- Third-party libraries and Applications

  - Updated to Node 12.16.1
  - Updated the following node modules to newer versions:

    - coffeescript 2.5.1
    - mongodb 4.2
    - nanomsg 4.1.0
    - zeromq 5.2.0

- Development Framework

  - New Data I/O and Connector definitions implemented on Node.js and C++
  - Update ICD model and docgen templates for generating ICD documents
  - Add generation of automated interface tests

  - Code Generation updates on Node.js:
 
    - Update code generation for the new Data I/O and connector model definitions
    - Add code generation for StateMachine and StructType data types
    - Add code generation for Enums (#178)
    - Fixed issue where code generation created duplicate “require” statements (#177)
    - Fixed issue in code generation where components were included in an application that did not match the target language (#175)

  - Code Generation updates on C++:

    - Update code generation for the new Data I/O and connector definitions
    - Update code generation for Faults and Alarms
    - Updated code generation of HW Adapter to allow having data objects that are primitive types or array elements instead of structs when the "field" parameter of a data object in the data_object_map collection is left empty (#169)
    - Fixed issue in C++ code generation of default values for arrays of state machines (#156)
    - Fixed issue in C++ code generation of path values for data types (#163)
    - Fixed issue in C++ code generation of default values for data types defined in the IO Framework (#164)
    - Fixed issue in C++ code generation where the dimensions of a 2D array was reversed (#167)
    - Removed GMT namespace from I/O Framework state machines (#172)
    - Fixed issue where enums in C++ were not properly initialized (#176)

  - Configuration files:

    - Updated generation of configuration files for new Data I/O and connector definitions
    - Fixed issue in generation of port numbers in configuration files (#157)
    - Fixed issue in generation of state variable ports in configuration files (#165)
    - Fixed issue in generation of large default values in configuration files (#168)
 
  - Improved error handling in ‘grs compile’ when the URI has not been specified properly in the model (#143)
  - Update ‘grs compile’ to compile all the configuration files of a module
  - Added check option in ‘grs compile’ to check the consistency of the compiled files

- Core Frameworks

  - Updated Node.js implementation of Core Framework:

    - New implementation of Data I/O and Connector strategy
    - Added timeout option to grs get, set and inspect commands
    - Added monitoring of computing resources, such as CPU and memory
    - CPU and memory performance optimizations
    - Updated model definition and implementation of Enum data types
    - Updated control_mode_sv state machine implementation
    - Updated OpStateFSM, AlarmFSM and FaultFSM model representation to match current implementation
    - Added performance metrics for Component Behaviors (step execution average, jitter, etc)
    - Added Query API to service adapters

  - Updated C++ implementation of Core Framework:

    - New implementation of Data I/O and Connector strategy, updated to match Node.js Core Framework
    - Added Fault Management
    - Added Alarm Management
    - Component distributed asynchronous communication
    - Updated op_state_sv state machine implementation
    - Updated Component Service Access (container) handling of alarm, fault and configuration events

  - Added Node.js implementation of OPC-UA Adapter with client and server implementation examples 
  - Added new database command to the `grs` tool to query and update the Core Services databases

  - EtherCAT Hardware Adapter updates:

    - Support the ability to set any EtherCAT slave state from the master (including OFF, PREOP, OP, SAFEOP, INIT and BOOT)
    - Added ability to update slave firmware from the master
    - Support definition of domains with different data rates 
    - Fixed PDO mapping

  - Control Framework updated to sync with Node.js Core Framework

  - Updated Persistence Framework:

    - Updated mongodb driver
    - Added database connection monitoring

  - Test Framework updated to sync with Node.js Core Framework

  - UI Framework implementation:

    - Abstraction of UI framework components from the Navigator application
    - Updated data connectors to synchronize with Core Frameworks
    - Added basic 2-D Plot widget for time-series display
    - Data Caching for widgets that are temporarily not visible
    - Added a basic Telemetry Viewer UI Element with filtering capability

- Core Services

    - Add support for Query API to Core Service Servers

- Implementation examples

  - The HDK example has been updated to sync with the new Core Frameworks
  - The ISample example has been updated to sync with the new Core Frameworks

- Documentation: Software Development

  - Minor updates to the ``Installing the SDK`` and ``Upgrade`` pages
  - Replaced the online version of the Software and Controls Standards with a download link for the released version of Rev.A

- Known Issues:

  - Validate command has not been updated yet to correctly validate the new Connector implementation


Contents of release 1.7.0
-------------------------

- Release distribution and installation:

  - Support added for CentOS 8

- Core Frameworks:

  - Development Framework:

    - Added C++17 support
    - Added SerialAdapter compiler flags to module.mk
    - Fixed issue where make did not parallelize the build correctly (#139)

  - Core Framework (Node.js):

    - Fixed issue where a scan rate < 1 caused an issue in periodic execution (#152)

  - Core Framework (C++):

    - Added StateMachine implementation
    - Implemented Operational State Machine

  - I/O Framework:

    - Updated query mechanism for EtherCAT slave state
    - Alias addressing on the EtherCAT bus

- Navigator Application:

  - Complete integration with latest version of the Node.js frameworks, including Service Data Ports
  - Added a basic log viewer UI Element with filtering capability
  - Added a basic telemetry viewer UI Element
  - Added ability to get and set state variables, inputs and outputs via the user interface
  - Updated packaging and distribution for MacOS and Linux

- Documentation: Software Development

  - Updated instructions for installing and running the Navigator application in MacOS and Linux
  - Updated installation and upgrade instructions for CentOS 8
  - Update Virtual Machine installation guide for CentOS 8
  - Developer Guide for UI Framework

- Implementation Examples:

  - Updated version of the HDK with Visualization Package

- Known Issues:

  - In C++ controllers, auto-generated configuration files need to be updated by hand to define the correct inputs and outputs for the goals and values of the state variables
  - System reboot may be needed after losing connection to EtherCAT modules

Contents of release 1.6.2
-------------------------

- Bug Fixes:
  
  - Fixed issue with connectors in Node.js where components incorrectly determined whether a connection is active/inactive
  - Unable to reproduce issue in v1.6.0 with generating test skeletons using "gds gen -t test". Marking as fixed.
  - Fixed issue in v1.6.0 with sending SDOs during runtime using the EtherCAT Hardware Adapter.


Contents of release 1.6.1
-------------------------

- Core Frameworks:

  - Development Framework:

    - Improved component configuration generation
    - Fixed type generation case in which no types are defined
    - Fixed names of inputs and outputs in config file
    - Preserving StateMachine and Component generated files
    - Preserving module.mk when generating code
    - Improved codegen for Node.js applications
    - Fixed dependency linking in Makefile for compiling apps, examples and tests with parallel option (-j)

  - Core Framework (Node.js):

    - Updated State Machine implementation 
    - Updated Tree validation and improved validation rules
    - Improved Fault and Alarm State Machines
    - Added state elapsed time and timeout functionality

  - Core Framework (C++):

    - Improved compilation time due to external template declaration and explicit template instantiation
    - Restructured Service Data code to reduce compilation time
    - Removed unused Data I/O (i.e.: heartbeat, etc)

  - Control Framework:
    - Improved compilation time due to external template declaration and explicit template instantiation

  - Test Framework:

    - Performance and Functionality improvements

- Core Services:

  - Log, Alarm, Telemetry and Configuration Services:

    - Fixed empty parent specification in fault section of the configuration files 

- OCS Application System:

  - Added preview of Core Service Server improvements

- OCS Supervisory System:

  - Updated Fault and Alarm Tree specifications

- OCS Sequencing System:

  - Initial implementation and examples for the OCS Sequencer

- Documentation: Software Development

  - Added description of the mapping between the Model Definition Files and Coffeescript source code (:ref:`mapping_model_to_coffee`).
  - Added Version 1.5 to Version 1.6 migration guide
  - Updated page ``gds documentation`` to add section on ``gds validate`` command
  - Updated page ``Core Services user guide`` to use fix argument description for ``--records`` command.
  - Updated page ``Model specification guide document``, section ``Component Specification``, to update ``faults`` and ``alarms`` descriptions.

- Implementation Examples:

  - Updated ISample connector specification


Contents of release 1.6.0
-------------------------

- Release distribution and installation

  - The Navigator application is now distributed as a binary instead of a tar file (Supported on MacOS). 

- Third-party libraries and Applications

  - Updated from Node 8 to Node 10
  - Updated the following node modules to newer versions:

    - coffeescript 2.4
    - mongodb 3.2
    - nanomsg 4.0.2
    - zeromq 5.1

  - Updated to msgpack version 3.1.1
  - Updated to nanomsg (C++) version 1.1.5

- Development Framework

  - Add Node.js code generation
  - Code Generation updates on C++:

    - Update code generation to sync with Core and I/O Frameworks
    - Change code preservation mechanism
    - Add realtime-specific code generation

  - Fixed minor issues in "gds info" and "validate" commands
  - Fixed issue where default values for State Variables were not generated in the config files
  - Updated OPC-UA data model generator

- Core Frameworks

  - Updated Node.js implementation of Core Framework:

    - Refactored Service Data Ports
    - Added HealthSupervisory behavior
    - Added Fault Management and propagation
    - Fault Tree evaluation per Component
    - Support of DataIO paths
    - Implementation of connectors between DataIO paths
    - Component to Component communication without dedicated ports
    - Added Alarm Management and propagation
    - Compilation and loading of configuration files from the file system
    - Added new runtime tool to inspect and communicate with running components (grs)
    - Added ComponentProxy to communicate with other components given their instance name
    - Added Views for command line state visualization
    - Added support for distributed goal sequencing
    - Telemetry decimation

  - Refactored C++ Core Framework to sync with Node.js Core Framework:

    - Added Service Data Ports
    - Added support for loading component configurations from file
    - Added Asynchronous ports
    - Added real-time support
    - Telemetry decimation
    - Command Line support for C++ applications

  - Updated C++ Control Framework to sync with Core Framework
  - EtherCAT Hardware Adapter updates:

    - Support dynamic updates to PDO mapping during runtime
    - EtherCAT ring topology support
    - Read the slave state (OP, PREOP, SAFEOP, etc)
    - Fixed issue found when etherCAT bus nominal rate was less than the component scan rate

  - Added basic Serial Communications Hardware Adapter (does not support Serial over EtherCAT yet)
  - Persistence Framework updated to sync with Node.js Core Framework:

    - Added option to define the number of records to return on a query
    - Added database connection and disconnection fault reporting

- Core Services

  - Refactored all core services to sync with Node.js Core Framework
  - Update options to the core services command line tools (See updated documentation) 

- Navigator Application

  - Updated Look & Feel

- Implementation examples

  - The HDK example has been updated to sync with the new Core Frameworks

    - Configurations are read from file and the command line
    - Realtime priorities added
    - Changes in class layout (each Component has a Base with autogenerated code and a derived class with user-added code)
    - Using CoreContainer and CoreApplication

  - The ISample example has been updated to sync with the new Core Frameworks

    - Model files have been cleaned up
    - Removed Heartbeat dedicated port as this is now managed transparently by the component supervisor
    - Removed unsupported components from the Model
    - Changed async ports to sync

- Documentation: Software Development

  - New page ``grs documentation``, contains a user guide for the new grs (GMT Runtime System) Tool. This utility allows interaction with running instances of remote components.
  - Updated page ``Core Services user guide``, to reflect recent changes to the Core Services Applications.
  - Updated ``ISample Example`` and ``HDK example`` pages to reflect recent changes to the commands for interacting with the core service applications.
  - Updated ``UI Framework`` page to simplify installation instructions for the Navigator application binary.

- Known Issues:

  - Generating test skeletons with "gds gen -t test" does not currently work
  - The UI Framework has not been updated to work with the new Core Frameworks yet. This updated functionality will be included in an updated release in the next 2 months, along with more examples on creating User Interface panels.
  - Functionality added to send SDOs during runtime using the EtherCAT Hardware Adapter does not work as expected. At this time, no SDOs can be sent to the slave, either during start-up or runtime. This will be fixed in a patch as soon possible.
  - The issue connecting the IgH Master to EL7201-0010 and EL7211-0010 modules has been fixed, but the known issue with sending SDO values to the slave affects this functionality as well.


Contents of release 1.5.0
-------------------------

- Release distribution and installation

  - Support added for Fedora 28
  - Installation instructions updated for creating either a Fedora 28 Server or a MacOS Workstation
  - Instructions added for installing and running the SDK and Navigator application on MacOS 

- Development Framework

  - Added Model validation with ```gds validate``` command
  - Added Test plugin for generating and executing module tests
  - Fixed ```gds new``` command (issue #108)

- Core Frameworks

  - C++ components generate heartbeats using timestamps instead of 0 values
  - EtherCAT support

    - Added ability to send SDOs to slaves during runtime and not just during initialization
    - Fixed issue with sending SDOs to multiple slaves with the same name

  - Added Ethernet TCP/IP Hardware Adapter
  - Initial release of the UI Framework for building User Interface panels

    - Navigator Application for viewing Engineering UI panels and custom UI Panels
    - Model files are loaded automatically for configured modules to build Engineering UI Panels
    - Custom UI panels can be defined in the Visualization package of the module

  - Initial release of the Test Framework for generating and running tests on the Component level 

- Implementation examples

  - HDK components have been updated to provide visibility to data for the UI
  - Documentation for the HDK example has been updated to include UI components.

- Documentation: Software Development

  - New page ``UI Framework``, contains a user guide for UI Framework.
  - New page ``OCS Test Guidelines``, contains a user guide for the Test Framework.
  - Updated page ``HDK example``, with instructions on running the Engineering UI and building custom
    UI panels.


Contents of release 1.4.1
-------------------------

- Release distribution and installation

  - A new folder ```doc``` has been created in ```$GMT_GLOBAL``` with the PDF version of the documentation.

- Development Framework

  - Updated configuration files
  - Improvements in the C++ code generation:

    - Properties-related code is now generated.
    - Inherited class member variables are not re-defined in the generated
      code for derived classes.
    - Type mapping improvements.
    - Fixes to handle correctly some rare cases in code generation.

- Core Frameworks

  - Add database support for logging and telemetry.
  - Changed C++ BaseComponent class member variables according the model.
  - Component scan_rate is now a frequency (in Hz), not a period.
  - Port rates are now true frequencies, not cycle counts.
  - Fixed instabilities in the EtherCAT IO framework.

- Implementation examples

  - Documentation for the HDK example has been added.

- Documentation: Software Development

  - New page ``gds documentation``, with the user manual of the *gds* tool.
  - New page ``Model specification guide``, with the description of the
    model files syntax.
  - New page ``Model-language mapping``, with the mapping between the model
    files and the implementation languages.
  - New page ``Core Services user guide``, with the user manual of the
    core services.
  - New page ``HDK example``, with a tutorial to download, build and
    execute the HDK example.

Contents of release 1.4.0
-------------------------

- Release distribution and installation

  - The OCS Software Release is no longer distributed as a fully configured ISO file with multiple RPM packages to be installed. The Software Development Kit (SDK) is now distributed as a single TAR file. The Operating System must be installed independently.
  - Instructions are provided to install the Operating System, set up the development platform, configure applicable system services, install external dependencies, install the SDK and use the Development Tools for software development.
  - Dependency management is built into the SDK platform instead of being managed by external tools in order to maintain control of specific versions used.

- Development Framework

  - The single repository containing model files and development tools has been reorganized into individual modules according to the new Work Breakdown Structure (WBS). The SDK supports the full life-cycle of each module independently.
  - Folder organization and tools and processes for working within the development environment have been standardized across all modules.
  - Development tools have been added to configure the development environment, integrate modules and build/deploy software in a standardized way.
  - The build system is improved and simplified.
  - The code generator supports c++ and coffee targets, with python planned on subsequent releases.
  - The code generator includes now support for scalar, structured and multidimensional array types.
  - A preliminary test automation framework is included with this release.

- Core Frameworks

  - An improved version of the c++ implementation of the core frameworks is included. The major improvements are the correct handling of the configuration properties, the possibility to define default values for the input and output ports and the standardization of the telemetry generation.
  - A new nodejs implementation of the core frameworks is included and provides the foundation for the Core Services.

- Core Services

  - A new improved implementation of the core services is included —currently, logging, telemetry, alarm and supervisory services are included.
  - All the services provide event consumer filtering.
  - The server and test client applications support new command line options and help.

- Implementation examples

  - Two reference Device Control System implementations are included: hdk_dcs and isample_dcs.
  - The model specifications of both subsystem have been updated
  - The code generated from the specification can be compiled and executed.
  - Both examples are distributed directly from git

- Documentation: Software Development

  - ``Installation`` page rewritten to reflect new OCS Software Release procedure:

    - Install the Operating System and configure system functions
    - Configure the Development Platform

  - Install the Software Development Kit (SDK)
  - ``Upgrade`` page rewritten to provide instructions for upgrading from version 1.3 to 1.4.
  - ``Installing a Virtual Machine`` page changed with instructions and images for installing a standard Fedora server instead of a distributed GMT iso file.
  - ``ISample Example`` page updated to reflect new Development Procedure using the SDK.

- Known Issues

  - A new implementation of the EtherCAT IO framework is included and has some stability problems while loading the fieldbus configuration.
  - The persistent functionality of the core services has been revised and it is disabled in this release.
  - The project is working in the known issues and the release will be updated once a patch is available.

Follow the :ref:`upgrade procedure <upgrade>`.

Contents of release 1.3
-----------------------

- Upgraded OS to Fedora 26
- Improvements to port communication mechanism using msgpack and nanomsg
- Added support for float and double data objects in the Ethercat Adapter
- Fully implemented testing port push/pull using gds
- Fixed known issues with code generation
- Defined the development environment file structure and added commands and scripts for easy configuration
- Added Module Configuration Management
- Added dynamic loading of submodules into gds/gmt
- Made significant improvements to the code generator, including automatic port assignments based on the model
- Moved ISample Example DCS to a new GitHub repository
- Updated :ref:`ISample Example <Isample_example>` documentation to reflect the new development workflow


Contents of release 1.2
-----------------------

- Minor bug fixes.
- The code generation tools now support c++14.
- Improved :ref:`ISample Example <Isample_example>` documentation.
- New guide on setting up a :ref:`Virtual Machine <virtual_machine>` development environment.

Contents of release 1.1
-----------------------

- Miscellaneous fixes and improvements. Follow the :ref:`upgrade procedure <upgrade>`.

Contents of release 1.0
-----------------------

- A set of common frameworks that provide software components that address similar
  problems with a :ref:`unified architecture <dcs_reference_architecture>`. The common frameworks encapsulate the implementation
  details allowing the developers to focus in the solving the domain specific programming tasks.
  These release includes a first implementation of the following frameworks:

   - The :ref:`Core Framework <core_framework>` implements a component model and distributed
     real-time communication protocols between components. Software components
     may be deployed in the same execution thread, different processes or different machines.

   - The :ref:`IO framework<IO_framework>` provides adapter components that enable GMT software components
     to communicate with external control and data acquisition hardware.
     In this release the IO framework provides adapters for EtherCAT and OPC UA.

   - The :ref:`Control Framework<device_control_framework>` includes the main building blocks of a control system.
     These real-time control components address the problems of state estimation,
     goal estimation and state control and define a set of standard state variables
     and associated state machines (e.g. operation state, simulation mode and control mode).

   - The :ref:`Persistence Framework<persistence_framework>` provides a way to store telemetry data streams. The
     current implementation uses MongoDB.

- A set of :ref:`Core Services<observatory_services>` that allows subsystem developers to test their software/hardware
  components in an environment similar to the one they will find at the observatory.
  This release includes an initial implementation of the telemetry, configuration,
  persistence and logging services.

- An :ref:`example instrument control system implementation (ISample) <dcs_spec_example>`. This example provides
  a template that instrument developers can use as a model.

- A formal specification and modeling language for the description of software interfaces.
  Interface test programs will be generated automatically from this specification to
  guarantee consistency between specification and implementation and to facilitate
  continuous integration and testing through the life of the project.

- A set of code generation tools that create subsystem scaffolds that conform to
  the reference architecture. These scaffolds reduce dramatically the time necessary
  to have an initial working system by generating automatically repetitive and tedious
  parts of code. They also provide a way to separate application logic from infrastructure
  logic. The code generation tools support c++11, python and `Coffeescript <http://coffeescript.org>`_ (Javascript dialect).

- The documentation of the GMT control reference architecture and the corresponding
  development tools.


.. note::

  The scope of v1.0 development documentation is currently limited to
  describing how to configure, start and monitor services (using logging and
  telemetry as examples), how to establish a communication network, and finally,
  how to setup a device control system. Future versions of this document will add
  other information as the development progresses.
