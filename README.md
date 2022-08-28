# pMINK MNO
##### *Project MINK framework + Mobile Network Operator Components*
----
[![Build Status](https://travis-ci.org/dfranusic/pmink.svg?branch=master)](https://travis-ci.org/dfranusic/pmink)

- Core components
  - [**libr14p**](#r14p-protocol-library) - R14P protocol library      
  - [**libpmcfg**](#configuration-parser-library) - Configuration parser library                                                                       
  - [**configd**](#configuration-daemon) - Configuration daemon                                                                               
  - [**cli_service**](#cli-client-shell) - CLI client shell                                                                                   
  - [**routingd**](#r14p-routing-daemon) - R14P Routing daemon                                                                                
  - [**configdc**](#config-daemon-client) - Config daemon client                                                                               
  - [**r14ptrapc**](#r14p-trap-client) - R14P trap client                                                                                   
  - [**asn1c**](#asn1-c-compiler) - ASN.1 c++ compiler                                                                                 

- Mobile Network Operator components
  - [**sgnd**](#signalling-gateway-node-daemon) - Signalling Gateway Node daemon
  - [**stpd**](#signal-transfer-point-daemon) - Signal Transfer Point daemon                                                                        
  - [**drd**](#data-retention-daemon) - Data Retention daemon                                                                               
  - [**fgnd**](#filtering-gateway-node-daemon) - Filtering Gateway Node daemon                                                                       
  - [**pdnd**](#pattern-detection-node-daemon) - Pattern Detection Node daemon

- [License](#license)

### R14P protocol library
---
[R14P](http://www.iana.org/assignments/sctp-parameters/sctp-parameters.xml#sctp-parameters-25) is 
a stateful stream based application layer protocol used for data communication between various pMINK nodes. 
It is [defined](http://github.com/dfranusic/pmink/blob/master/src/asn1/r14p/r14p.asn) using 
[ASN.1](http://en.wikipedia.org/wiki/Abstract_Syntax_Notation_One) syntax notation and carried by wire 
in binary [Basic Encoding Rules (BER)](https://en.wikipedia.org/wiki/X.690) format. At the moment, R14P 
uses  **[SCTP](https://en.wikipedia.org/wiki/Stream_Control_Transmission_Protocol)** for reliable data transfer, 
although it can be easily ported to TCP. Correct order of packets and statefulness is maintained by both 
SCTP and R14P to ensure maximum quality of packet delivery.

The overall design of the protocol was influenced by [TCAP](https://en.wikipedia.org/wiki/Stream_Control_Transmission_Protocol) 
but with various enhancements and differences. R14P supports **label** based application layer routing; 
every **end-point** is defined by a two part string label. The first part, named **id**, is a unique 
pMINK node identifier string, similar to IP address of a machine. The second part is a **type** string, 
used mainly for grouping of various nodes for redundancy and load balancing purposes. pMINK framework 
comes with a specialized node called **Routing daemon (routingd)** which is used mainly for R14P 
label-based routing and [Weighted round robin (WRR)](https://en.wikipedia.org/wiki/Weighted_round_robin) 
load balancing.

All R14P messages consist of two basic parts; a **Header** and a **Body**. 
Header is mandatory and is handled by the **libr14p** library and mostly not 
used by the end-user. The body of the message is where the actual data resides; 
there are many different types of message bodies and most of them are specialized 
and used internally by pMINK framework. User generated data is transferred in 
a **ServiceMessage** body; every service message stream contains a user defined
**service-id** value used for data classification.

```asn1
-- ============
-- R14P Message
-- ============
-- header       - packet header
-- body         - packet body
R14PMessage ::= SEQUENCE {
        header          Header,
        body            Body OPTIONAL,
        ...
}

-- ===========
-- R14P Header
-- ===========
-- version       - R14P version
-- source        - source information
-- destination   - destination information
-- uuid          - universally unique identifier (UUID)
-- sequence-num  - sequence number
-- sequence-flag - packet sequence information (stateful/stateless/etc.)
-- enc-info      - encryption information
-- hop-info      - hop counter
-- status        - error code
Header ::= SEQUENCE {
        version       [0] INTEGER,
        source        [1] EndPointDescriptor,
        destination   [2] EndPointDescriptor,
        uuid          [3] OCTET STRING,
        sequence-num  [4] INTEGER,
        sequence-flag [5] SequenceFlag,
        enc-info      [6] EncryptionInfo OPTIONAL,
        hop-info      [7] HopInfo OPTIONAL,
        status        [8] ErrorCode OPTIONAL,
        ...
}

-- =========
-- R14P Body
-- =========
-- encrypted-data        - Encrypted R14P body, used only when content is encrypted (header.encryption)
-- packet-fwd            - General packet forwarding, used for routing and failovers
-- filter                - Filtering service, mostly used but not limited to SMS
-- data-retention        - Data retention service, used for DB storage
-- general               - Reserved for custom daemons and/or future use
-- conf                  - Configuration daemon service
-- stats                 - Statistical data exchange
-- auth                  - Authentication messages, used for daemon authentication
-- reg                   - Registration messages, used for daemon discovery and various 
--                         registration procedures
-- ntfy                  - Various notification/alarm/etc. messages
-- data                  - payload data exchange
-- routing               - routing related messages
-- service-msg           - service related messages
-- state-msg             - statefulness related messages
Body ::= CHOICE {
        encrypted-data  [1] OCTET STRING,
        packet-fwd      [2] PacketFwdMessage,
        filter          [3] FilterMessage,
        data-retention  [4] DataRetentionMessage,
        conf            [6] ConfigMessage,
        stats           [7] StatsMessage,
        auth            [8] AuthMessage,
        reg             [9] RegistrationMessage,
        ntfy            [10] NotifyMessage,
        data            [11] DataMessage,
        routing         [12] RoutingMessage,
        service-msg     [13] ServiceMessage,
        state-msg       [14] StateMessage,
        ...
}

```

### Configuration parser library
---
pMINK Configuration parser library is a core component used primarily by **Configuration daemon (configd)**. 
When designing a system on top of pMINK framework, configuration definition is always a good starting point. There
are two types of configuration files used by configd; **definition** and **contents**. 

Definition file is used to describe the structure of data stored in the contents file. Proper definition ensures data
integrity and avoids unnecessary implementations of custom data validators in user specific daemons. Both types
of configuration files share a similar structure and are parsed using the same grammar. Default extension for pMINK
configuration files is ``.pmink``; a standard naming convention used in this project is to add a `_def` suffix to
base name of definition file (e.g. ``pmink_def.pmcfg`` for definition and ``pmink.pmcfg`` for contents).

The following example shows a simple configuration showing **3 main types** of configuration items:

* **primitive**
  - single field container whose type is defined in the **TYPES** section of configuration definition file
* **block** 
  - a storage container similar to a file system directory, defined with **CONST** keyword and used mostly for grouping
* **dynamic block**
  - special storage container identified by a **"*"** (star) symbol; serves a purpose of being a **template** node for 
    dynamic creation of block nodes

###### Configuration definition "pmink_def.pmcfg"
```c
// data types   
TYPES {         
  // each type is defined by perl-type regex
  "INT"     PTRN\d+PTRN  "Number type" 
  "STRING"  PTRN.*PTRN   "Alphanumeric type"
}                               
                                        
// config structure                     
CONFIG {                                        
  // global parameter                           
  test_global  "INT"  "Global INT parameter"            
                                                                
  // static sub node                                           
  local CONST  "Local group" {                
    test_local  "INT"  "Local INT parameter"    
  }                             

  // dynamic nodes
  dynamic CONST "Dynamic container node" {
    dynamic_child * "STRING" "Dynamic child node" {
      test "INT" "Test INT parameter"
    }
  }
}
```

###### Configuration contents "pmink.pmcfg"
```c
test_global "10"
local {
  test_local "666"
}
dynamic {
  node_1 {
    test "10"
  }
  node_2 {
    test "20"
  }
}
```
### Configuration daemon
---
Configuration daemon is a pMINK daemon used for configuration management and is a central most important service
of this framework. These are the main features of configd:

* configuration file management
* replication of configuration data 
* **ACID** like properties for configuration transactions (**COMMIT/DISCARD/ROLLBACK**)
* real-time distribution of configuration data to all affected daemons 
  - each pMINK daemon attaches itself to its own subset of configuration nodes; smart configuration definition will play a
    vital role in minimizing bandwith usage for configuration data 
  - when configuration data is changed in some way (using pMINK Command Line Interface), all connected daemons 
    that are affected with that particular modification, are notified
  - configuration data can grow quite a bit  and is therefore advised for each daemon to attach itself to its own subset of 
    configuration defintion (e.g. STP daemon should not get any configuration data related to R14P routing)

###### configd command line arguments
```bash
config_daemon - pMINK Configuration daemon
Copyright (c) 2012 Release14.org

Options:
 -?	help
 -i	unique daemon id
 -p	R14P inbound port
 -d	configuration definition file
 -c	configuration contents file
 -r	routing daemon address (ipv4:port)
 -n	other config daemon id
 -t	user timeout in seconds
 -D	debug mode
 -R	enable routing

R14P Options:
=============
 --r14p-streams		R14P Session stream pool		(default = 1000)
 --r14p-stimeout	R14P Stream timeout in seconds		(default = 5
```

###### configd example
```bash
$ ./configd -i cfg1 -p 10000 -d /tmp/test_def.pmcfg -c /tmp/test.pmcfg -R
```
This will start **configd** with the following parameters set:

- pMINK unique daemon id = **cfg1**
- SCTP listening port = **10000**
- configuration definition file = **/tmp/test_def.pmcfg**
- configuration contents file = **/tmp/test.pmcfg** 
- **-R** flag is used when configd should accept connections directly and not via **routingd** 

### CLI client shell
---
pMINK Command Line Service (CLI) is a specialized shell used for interaction with configd; it features a powerful
TAB activated auto-complete and was inspired by [CISCO](http://www.cisco.com) and [Vyatta](http://vyos.io/) command line interfaces.

![pmink cli image](http://www.release14.org/wp-content/uploads/2016/12/cli.jpg "pMINK CLI")

###### cli_service command line arguments
```bash
cli_service - pMINK CLI service
Copyright (c) 2012 Release14.org

Options:
 -i	unique service id
 -f	cli definition file
 -c	config daemon address (ipv4:port)
```

###### cli_service example
```bash
$ ./cli_service -i cli1 -f /tmp/cli_default.pmcfg -c 127.0.0.1:10000
```

This will start **cli_service** with the following parameters set:

- pMINK unique daemon id = **cli1**
- CLI definition file = **/tmp/cli_default.pmcfg**
- configd IP address and listening port = **127.0.0.1:10000**

CLI definition file is used for defining commands and modules available to the user.  At the moment, **cli_service** uses only one 
available plugin; **pm\_plg\_cli\_cfg.so** module is used for **cli <---> configd** communication. 
The file [cli\_default.pmcfg](http://github.com/dfranusic/pmink/blob/master/src/cli/cli_default.pmcfg) used in this example is a default 
CLI definition; it is created during installation (**make install**)

### R14P Routing daemon
---
Routing daemon is a general purpose R14P router; it is used for **label** based R14P routing and load balancing. The most common type
of setup assumes the following:

- two routing daemons (or one if redundancy is not an issue)
- one config daemon
- any number of user daemons

In this setup, **routingd** connects directly to **configd** while other daemons involved in this setup connect only to **routingd**. 
Routing daemon facilitates all **daemon-to-daemon** communication, takes care of **load balancing** (if used), and creates an extra 
layer of security for **configd**. Direct connection to **configd** is established only by **routing**, all other daemons communicate 
with **configd** via **routingd**.

###### routingd command line arguments
```bash
routingd - pMINK Routing daemon
Copyright (c) 2012 Release14.org

Options:
 -?	help
 -i	unique daemon id
 -c	config daemon address (ipv4:port)
 -p	R14P inbound port
 -D	start in debug mode

R14P Options:
=============
 --r14p-streams		R14P Session stream pool		(default = 1000)
 --r14p-stimeout	R14P Stream timeout in seconds		(default = 5)
```

###### routingd example
```bash
$ ./routingd -i rtr1 -c 127.0.0.1:10000 -p 15000
```

This will start **routingd** with the following parameters set:

- pMINK unique daemon id = **rtr1**
- configd IP address and listening port = **127.0.0.1:10000**
- SCTP listening port = **15000**

### Config daemon client
---
Configuration daemon client is an alternative interface to **configd**. Unlike **cli_service** which is interactive, **configdc** 
is a script based client used by 3rd party applications like WEB GUIs and shell scripts. Script format used by **configdc** is a 
simple text file containing a list of commands to be sent to **configd**. Commands are processed sequentially and results are 
outputted to standard output. The resulting output is generated for each command found in a script file; commands are outputted 
with a **: (colon)**  prefix  and their results follow in the next line, unmodified. If an error occurs, **configd** will do an 
automatic **discard** of the whole transaction, and the resulting output will start with **"ERROR:"**, followed by the 
actual error message generated by **configd**.


###### configdc command line arguments
```bash
cfgdc - pMINK Config daemon client
Copyright (c) 2012 Release14.org

Options:
========
 -?	help
 -i	unique daemon id
 -f	config daemon command script file
 -r	routing daemon address (ipv4:port)
 -D	start in debug mode

R14P Options:
=============
 --r14p-streams		R14P Session stream pool		(default = 1000)
 --r14p-stimeout	R14P Stream timeout in seconds		(default = 5)
```

###### configdc example (success)
```bash
$ cat /tmp/test.txt
configuration
show dyn node_1 test

$ ./configdc -i cfgc1 -r 127.0.0.1:10000 -f /tmp/test.txt
: configuration
test_global "10"
local {
	test_local "666"
}
dynamic {
	node_1 {
		test "10"
	}
	node_2 {
		test "20"
	}
}

: show dyn node_1 test
test = 10
```

This will start **configdc** with the following parameters set:

- pMINK unique daemon id = **cfgc1**
- configd (or routingd) IP address and listening port = **127.0.0.1:10000**
- script file with the list of commands to send to configd = **/tmp/test.txt**


###### configdc example (error)
```bash
$ cat /tmp/test_err.txt
configuration
show dyn node_1 testbla

$ ./configdc -i cfgc1 -r 127.0.0.1:10000 -f /tmp/test_err.txt
: configuration
test_global "10"
local {
	test_local "666"
}
dynamic {
	node_1 {
		test "10"
	}
	node_2 {
		test "20"
	}
}

: show dyn node_1 testbla
ERROR: Unknown item or command "testbla"!
```

This will start **configdc** with the following parameters set:

- pMINK unique daemon id = **cfgc1**
- configd (or routingd) IP address and listening port = **127.0.0.1:10000**
- script file with the list of commands to send to configd = **/tmp/test_err.txt**

### R14P trap client
---
R14P Trap client is a tool used for fetching pMINK performance counters. Daemons can expose various
counters which are usually required for performance tracking and trouble shooting. pMINK framework uses
its own system for counters; it is an R14P protocol extension quite similar but not as powerful as
[SNMP](https://en.wikipedia.org/wiki/Simple_Network_Management_Protocol). Most production environments
use their own 3rd party monitoring tools; creating wrapper scripts to convert **r14ptrapc** output to 
SNMP compatible data should not be a big issue.

###### r14ptrapc command line arguments

```bash
r14ptrapc - pMINK R14P trap client
Copyright (c) 2012 Release14.org

Options:
 -c	target daemon address (ipv4:port)
 -t	target daemon type
 -i	target daemon id
 -s	target trap id (0 for ALL)
 -a	unique client id

R14P Options:
=============
 --r14p-streams		R14P Session stream pool		(default = 10)
 --r14p-stimeout	R14P Stream timeout in seconds		(default = 5)
 --r14p-smsg-pool	R14P Service message pool		(default = 10)
 --r14p-sparam-pool	R14P Service message parameter pool	(default = 1000)
```


###### r14ptrapc example (specific counter)

```bash

$ ./r14ptrapc -c 127.0.0.1:15000 -t routingd -i rtr1 -s R14P_IN_config_daemon_cfg1_BYTES -a trapc1
                         Trap Id                    Trap Value
-------------------------------------------------------------
R14P_IN_config_daemon_cfg1_BYTES                           140

```

This will start **r14ptrapc** with the following parameters set:

- routingd IP address and listening port = **127.0.0.1:10000**
- **type label** of target daemon to fetch counters from = **routingd**
- **id label** of target daemon to fetch counters from = **rtr1**
- counter id (multiple **-s** supported) to fetch = **R14P_IN_config_daemon_cfg1_BYTES**
- trap client's unique daemon id = **trapc1**


###### r14ptrapc example (all counters)

```bash
$ ./r14ptrapc -c 127.0.0.1:15000 -t routingd -i rtr1 -s 0 -a trapc1
                                      Trap Id                    Trap Value
----------------------------------------------------------------------------
                 R14P_IN_%routingd_%rtr1_BYTES                           441
             R14P_IN_%routingd_%rtr1_DISCARDED                             0
             R14P_IN_%routingd_%rtr1_MALFORMED                             0
               R14P_IN_%routingd_%rtr1_PACKETS                             5
              R14P_IN_%routingd_%rtr1_POOL_ERR                             0
            R14P_IN_%routingd_%rtr1_SOCKET_ERR                             0
               R14P_IN_%routingd_%rtr1_STREAMS                             1
          R14P_IN_%routingd_%rtr1_STREAM_BYTES                           192
            R14P_IN_%routingd_%rtr1_STREAM_ERR                             0
       R14P_IN_%routingd_%rtr1_STREAM_LOOPBACK                             0
        R14P_IN_%routingd_%rtr1_STREAM_TIMEOUT                             0
           R14P_IN_r14ptrapc_trapc117815_BYTES                             0
       R14P_IN_r14ptrapc_trapc117815_DISCARDED                             0
       R14P_IN_r14ptrapc_trapc117815_MALFORMED                             0
         R14P_IN_r14ptrapc_trapc117815_PACKETS                             0
        R14P_IN_r14ptrapc_trapc117815_POOL_ERR                             0
      R14P_IN_r14ptrapc_trapc117815_SOCKET_ERR                             0
         R14P_IN_r14ptrapc_trapc117815_STREAMS                             0
    R14P_IN_r14ptrapc_trapc117815_STREAM_BYTES                             0
      R14P_IN_r14ptrapc_trapc117815_STREAM_ERR                             0
 R14P_IN_r14ptrapc_trapc117815_STREAM_LOOPBACK                             0
  R14P_IN_r14ptrapc_trapc117815_STREAM_TIMEOUT                             0
                R14P_OUT_%routingd_%rtr1_BYTES                           815
            R14P_OUT_%routingd_%rtr1_DISCARDED                             0
            R14P_OUT_%routingd_%rtr1_MALFORMED                             0
              R14P_OUT_%routingd_%rtr1_PACKETS                             7
             R14P_OUT_%routingd_%rtr1_POOL_ERR                             0
           R14P_OUT_%routingd_%rtr1_SOCKET_ERR                             0
              R14P_OUT_%routingd_%rtr1_STREAMS                             0
         R14P_OUT_%routingd_%rtr1_STREAM_BYTES                             0
           R14P_OUT_%routingd_%rtr1_STREAM_ERR                             0
      R14P_OUT_%routingd_%rtr1_STREAM_LOOPBACK                             0
       R14P_OUT_%routingd_%rtr1_STREAM_TIMEOUT                             0
          R14P_OUT_r14ptrapc_trapc117815_BYTES                             0
      R14P_OUT_r14ptrapc_trapc117815_DISCARDED                             0
      R14P_OUT_r14ptrapc_trapc117815_MALFORMED                             0
        R14P_OUT_r14ptrapc_trapc117815_PACKETS                             0
       R14P_OUT_r14ptrapc_trapc117815_POOL_ERR                             0
     R14P_OUT_r14ptrapc_trapc117815_SOCKET_ERR                             0
        R14P_OUT_r14ptrapc_trapc117815_STREAMS                             0
   R14P_OUT_r14ptrapc_trapc117815_STREAM_BYTES                             0
     R14P_OUT_r14ptrapc_trapc117815_STREAM_ERR                             0
R14P_OUT_r14ptrapc_trapc117815_STREAM_LOOPBACK                             0
 R14P_OUT_r14ptrapc_trapc117815_STREAM_TIMEOUT                             0

```

This will start **r14ptrapc** with the following parameters set:

- routingd IP address and listening port = **127.0.0.1:10000**
- **type label** of target daemon to fetch counters from = **routingd**
- **id label** of target daemon to fetch counters from = **rtr1**
- counter id (multiple **-s** supported) to fetch = **0**
- trap client's unique daemon id = **trapc1**

### ASN.1 c++ compiler
---
pMINK ASN.1 c++ compiler translates [ASN.1](http://en.wikipedia.org/wiki/Abstract_Syntax_Notation_One) syntax notation into c++ soure code.
The generated code is intended to be used with pMINK asn.1 library, but it
can also be of value to 3rd party applications and/or libraries. R14P protocol
definition ([r14p.asn](http://github.com/dfranusic/pmink/blob/master/src/asn1/r14p/r14p.asn)) 
was also compiled using **asn1c**. 

Project MINK MNO (Mobile Network Operator) implementation consists of many ASN.1 defined protocols, some most important ones are 
[Transaction Capabilities Application Part (TCAP)](http://en.wikipedia.org/wiki/Transaction_Capabilities_Application_Part) 
and [Mobile Application Part (MAP)](http://en.wikipedia.org/wiki/Mobile_Application_Part). All of these were compiled using **asn1c**, only 
minor modifications were needed. ASN.1 syntax notation is sometimes difficult to parse due to its complexity; pMINK's **asn1c**
compiler tries to cover most of syntax variations but it is not 100% ASN.1 compatible. Minor tweaks are sometimes needed in order
to accommodate for syntax variations.


###### asn1c command line arguments
```bash
asn1c - pMINK ASN.1 C++ compiler
Copyright (c) 2012 Release14.org
Usage: asn1c [options]

Options:
 -f	specify asn.1 input file
 -p	print asn.1 structure
 -g	generate c++ code
 -t	redirect code generation to stdout
 -c	syntax check asn.1 definition
 -o	specify output directory
 -?	help

```

###### asn1c example
```bash
$ ./asn1c -f src/asn1/r14p/r14p.asn -g -o /tmp
Generating header file "r14p.h"...
Finished generating header file "/tmp/r14p.h"

Generating src file "/tmp/r14p.h"...
Finished generating src file "/tmp/r14p.h"
```

This will start **asn1c** with the following parameters set:

- asn.1 input file = **src/asn1/r14p/r14p.asn**
- **-g** = generate c++ header and source file
- output directory for c++ generated files = **/tmp**

### Signalling Gateway Node daemon
---
Project MINK's implementation of [Signal Transfer Point (STP)](https://en.wikipedia.org/wiki/Signal_Transfer_Point) 
deviates from a standard STP blueprint in which a single software process is used for both connection handling and routing.

Signalling Gateway Node (SGN) and Signal Transfer Point (STP) included in pMINK framework utilize a different approach; 
the former is responsible for connection management (sockets and state machines), while the latter specializes in data
routing and translations.

There are two types of signalling categories available in pMINK; **internal** and **external**. External signalling is 
context dependant and can vary greatly; pMINK platform uses **SGN** as the main data entry point and signalling converter.
After successful conversion, external signalling is classified as internal signalling and transferred freely between 
various nodes using context free **R14P** protocol.

This type of approach creates a layer of abstraction around various external signalling protocols, and allows pMINK user
daemons (e.g. STP and FGN) to focus on implementation of logic, and not data formats and conversions.


###### The following types of connections are currently implemented in SGN:
- [M3UA](http://en.wikipedia.org/wiki/M3UA) - MTP Level 3 (MTP3) User Adaptation Layer
- [SMPP](http://en.wikipedia.org/wiki/Short_Message_Peer-to-Peer) - Short Message Peer-to-Peer         
- PCAP - Passive packet capture (M3UA and SMPP)
  - both libpcap and [PF_RING](http://github.com/ntop/PF\_RING) are supported (``--with-pfring`` configure flag)

###### User data protocols currently supported:
- [SCCP](https://en.wikipedia.org/wiki/Signalling_Connection_Control_Part) - Signalling Connection Control Part
  - Unitdata (UDT) and Extended Unitdata (XUDT)
- [TCAP](https://en.wikipedia.org/wiki/Transaction_Capabilities_Application_Part) - Transaction Capabilities Application Part
- [GSM MAP 3GPP TS 29.002](https://en.wikipedia.org/wiki/Mobile_Application_Part) - GSM Mobile Application Part
  - Supported application contexts:
    - 0.4.0.0.1.0.25.3 - shortMsgMT-RelayContext-v3
    - 0.4.0.0.1.0.25.2 - shortMsgMT-RelayContext-v2
    - 0.4.0.0.1.0.20.3 - shortMsgGatewayContext-v3
    - 0.4.0.0.1.0.20.2 - shortMsgGatewayContext-v2
    - 0.4.0.0.1.0.20.1 - shortMsgGatewayContext-v1
    - 0.4.0.0.1.0.21.3 - shortMsgMO-RelayContext-v3
    - 0.4.0.0.1.0.21.2 - shortMsgMO-RelayContext-v2
    - 0.4.0.0.1.0.21.1 - shortMsgRelayContext-v1
  - Supported opcodes:
    - 44 - mt-forward-SM
    - 46 - mo-forward-SM
    - 45 - sendRoutingInfoForSM
- [SMS TPDU 3GPP TS 23.040](https://en.wikipedia.org/wiki/GSM_03.40)
- [SMPP v3.4](http://en.wikipedia.org/wiki/Short_Message_Peer-to-Peer) - Short Message Peer-to-Peer 

###### M3UA notes:
Definition from Wikipedia:

>M3UA stands for MTP Level 3 (MTP3) User Adaptation Layer as defined by the IETF SIGTRAN working group in RFC 4666
(which replaces and supersedes RFC 3332). M3UA enables the SS7 protocol's User Parts (e.g. ISUP, SCCP and TUP) to
run over IP instead of telephony equipment like ISDN and PSTN. It is recommended to use the services of SCTP to transmit M3UA.

- Signalling Gateway Node (SGN) supports M3UA carried by SCTP; user part of M3UA indicated by **Service Indicator** is 
currently limited to SCCP (**SI = 3**)

###### Connection configuration
SGN connection configuration is organized using M3UA principles and terminology; 
[Application Server Process (ASP)](https://tools.ietf.org/html/rfc3332#section-1.2) and
[Application Server (AS)](https://tools.ietf.org/html/rfc3332#section-1.2) are used for both SMPP and M3UA configuration.

AS Definition from M3UA RFC:
>Application Server (AS) - A logical entity serving a specific Routing
Key.  An example of an Application Server is a virtual switch element
handling all call processing for a unique range of PSTN trunks,
identified by an SS7 SIO/DPC/OPC/CIC_range.  Another example is a
virtual database element, handling all HLR transactions for a
particular SS7 DPC/OPC/SCCP_SSN combination.  The AS contains a set
of one or more unique Application Server Processes, of which one or
more is normally actively processing traffic.  Note that there is a
1:1 relationship between an AS and a Routing Key.


ASP Definition from M3UA RFC:
>Application Server Process (ASP) - A process instance of an
Application Server. An Application Server Process serves as an active
or backup process of an Application Server (e.g., part of a
distributed virtual switch or database).  Examples of ASPs are
processes (or process instances) of MGCs, IP SCPs or IP HLRs.  An ASP
contains an SCTP endpoint and may be configured to process signalling
traffic within more than one Application Server.

###### sgnd command line arguments

```bash
sgnd - pMINK Signalling daemon
Copyright (c) 2012 Release14.org

Options:
 -?	help
 -i	unique daemon id
 -t	daemon type override
 -r	routing daemon address (ipv4:port)
 -D	start in debug mode

R14P Options:
=============
 --r14p-streams		R14P Session stream pool		(default = 10000)
 --r14p-stimeout	R14P Stream timeout in seconds		(default = 5)
 --r14p-smsg-pool	R14P Service message pool		(default = 1000)
 --r14p-sparam-pool	R14P Service message parameter pool	(default = 100000)

Dev Options:
=============
 --dev-m3ua-sm		Enable/Disable m3ua state machine	(0 - Disable, 1 - Enable)
 --dev-smpp-sm		Enable/Disable smpp state machine	(0 - Disable, 1 - Enable)
```

###### sgnd example 

```bash
$ ./sgnd -i sgn-test-01 -r 127.0.0.1:10000
```

This will start **sgnd** with the following parameters set:

- routingd IP address and listening port = **127.0.0.1:10000**
- pMINK unique daemon id = **sgn-test-01**

#### M3UA Connection
##### ASP/AS configuration example
The following configuration excerpts show a simple **1 AS with 1 ASP** setup; ASP **TEST-ASP-01**
is assigned a **192.168.0.10:2906** local ip/port combination, and is connected to a **192.168.0.100:2906**
remote peer. SCTP timers in this example are also defined explicitly, and M3UA **opc/dpc** is set to
**1234/5678**.

Each Application Server Process has to be assigned to one or more Application Servers; in this example,
we have created an ASP named **TEST-ASP-01** and an AS named **TEST-AS-01**. The second step needed to 
properly activate an ASP, is to add it to the Application Server's list of active Application Server Processes.

###### ASP configuration subset (mno/sgn/sgn-test-01/m3ua/asp)
```c
TEST-ASP-01 {
  sctp {
    local {
      ip1  "192.168.0.10"
      port "2906"
    }     
    remote {
      ip1  "192.168.0.100"
      port "2906"
    }     
    timers {
      hb-interval         "3000"
      max-init-retransmit "8"
      path-max-retrans    "5"   
      rto-initial         "100" 
      rto-max             "500" 
      rto-min             "150" 
      sack-timeout        "0"   
      sack-freq           "1"   
      valid-cookie-life   "60000"
    }     
  }     
  m3ua {
    opc   "1234"
    dpc   "5678"
    hbeat "1000"
  }     
  mode        "0"   
  shutdown    "1"   
  description "Testing ASP TEST-ASP-01"
} 
```

###### AS configuration subset (mno/sgn/sgn-test-01/m3ua/as)
```c
TEST-AS-01 {
  asp {
    TEST-ASP-01 {
      active "1"
    }
  }
  routing-key {
    routing-context "111"
  }
  traffic-mode {
    type "2"
  }
  description "Testing AS TEST-AS-01"
}
```

###### Step by step ASP/AS activation process using the [pMINK CLI](#cli-client-shell):
1. Create new ASP named **TEST-ASP-01** on an SGN node named **sgn-test-01**
   - create a new empty ASP node: `set mno sgn sgn-test-01 m3ua asp TEST-ASP-01`
   - set a new root node for easier editing: `edit mno sgn sgn-test-01 m3ua asp TEST-ASP-01`
   - set ASP connection parameters:

     ```c
     set sctp local ip1 192.168.0.10
     set sctp local port 2906
     set sctp remote ip1 192.168.0.100
     set sctp remote port 2906
     set sctp timers hb-interval 3000
     set sctp timers max-init-retransmit 8
     set sctp timers path-max-retrans 5
     set sctp timers rto-initial 100
     set sctp timers rto-max 500
     set sctp timers rto-min 150
     set sctp timers sack-timeout 0
     set sctp timers sack-freq 1
     set sctp timers valid-cookie-life 60000
     set m3ua opc 1234
     set m3ua dpc 5678
     set m3ua hbeat 1000
     set mode 0
     set shutdown 1
     set description "Testing ASP TEST-ASP-01"
     ```
  - commit changes: `commit`
2. Create new AS named **TEST-AS-01** on an SGN node named **sgn-test-01**
  - create a new empty AS node: `set mno sgn sgn-test-01 m3ua as TEST-AS-01`
  - set a new root node for easier editing: `edit mno sgn sgn-test-01 m3ua as TEST-AS-01`
  - set AS parameters and link it with ASP:

    ```c
    set asp TEST-ASP-01 active 1
    set routing-key routing-context 111
    set traffic-mode type 2
    set description "Testing AS TEST-AS-01"
    ```
  - commit changes: `commit`

#### SMPP Connection
##### ASP/AS configuration example
The following configuration excerpts show a simple **1 AS with 1 ASP** setup; ASP **TEST-SMPP-ASP-01**
is assigned a **192.168.0.10:2775** local ip/port combination, and is connected to a **192.168.0.100:2775**
remote peer. SMPP BIND method, username and password used in the following example, are respectively 
**Bind_transceiver**, **testuser** and **12345678**.

Each Application Server Process has to be assigned to one or more Application Servers; in this example,
we have created an ASP named **TEST-SMPP-ASP-01** and an AS named **TEST-SMPP-AS-01**. The second step needed to 
properly activate an ASP, is to add it to the Application Server's list of active Application Server Processes.

###### ASP configuration subset (mno/sgn/sgn-test-01/smpp/asp)
```c
TEST-SMPP-ASP-01 {
  tcp {
    local {
      ip   "192.168.0.10"
      port "2775"
    }
    remote {
      ip   "192.168.0.100"
      port "2775"
    }     
  }     
  smpp {
    timers {
      enquire_link_timer "10"
    }
    users {
      testuser {
        password "12345678"
      }
    }
  }     
  mode        "0"   
  description "Testing ASP TEST-SMPP-ASP-01"
} 
```

###### AS configuration subset (mno/sgn/sgn-test-01/smpp/as)
```c
TEST-SMPP-AS-01 {
  asp {
    TEST-SMPP-ASP-01 {
      active "1"
    }
  }
  description "Testing AS TEST-SMPP-AS-01"
}
```

###### Step by step ASP/AS activation process using the [pMINK CLI](#cli-client-shell):
1. Create new ASP named **TEST-SMPP-ASP-01** on an SGN node named **sgn-test-01**
   - create a new empty ASP node: `set mno sgn sgn-test-01 smpp asp TEST-SMPP-ASP-01`
   - set a new root node for easier editing: `edit mno sgn sgn-test-01 smpp asp TEST-SMPP-ASP-01`
   - set ASP connection parameters:

     ```c
     set tcp local ip 192.168.0.10
     set tcp local port 2775
     set tcp remote ip 192.168.0.100
     set tcp remote port 2775
     set smpp bind_method 9
     set smpp timers enquire_link_timer 10
     set smpp users testuser password 12345678
     set mode 0
     set description "Testing ASP TEST-SMPP-ASP-01"
     ```
  - commit changes: `commit`
2. Create new AS named **TEST-SMPP-AS-01** on an SGN node named **sgn-test-01**
  - create a new empty AS node: `set mno sgn sgn-test-01 smpp as TEST-SMPP-AS-01`
  - set a new root node for easier editing: `edit mno sgn sgn-test-01 smpp as TEST-SMPP-AS-01`
  - set AS parameters and link it with ASP:

    ```c
    set asp TEST-SMPP-ASP-01 active "1"
    set description "Testing AS TEST-SMPP-AS-01"
    ```
  - commit changes: `commit`

### Signal Transfer Point daemon
---
Signal Transfer Point (STP) is a **rule based** routing engine used for packet routing and translations.
Some implementation details were already mentioned in the previous chapter; STP was designed to be
used in combination with SGN, the former focuses on routing while the latter keeps connections alive
and deals with sockets, state machines and data conversions.

Although Filter Gateway Node (FGN) and Signal Transfer Point (STP) both share the same rule engine, FGN
capabilities exceed basic routing and offer increased flexibility and customization. More information
regarding STP's rule engine can be found in 
[FGN user's manual](http://github.com/dfranusic/pmink/blob/master/doc/smsf.pdf); please note that unlike
FGN, STP support is limited to **basic matching** and section **8.2.4** of the manual is not supported.

###### Packet routing protocols available to STP's routing engine:
- [M3UA](http://en.wikipedia.org/wiki/M3UA)
- [SCCP](http://en.wikipedia.org/wiki/Short_Message_Peer-to-Peer)
- [TCAP](https://en.wikipedia.org/wiki/Transaction_Capabilities_Application_Part)
- [GSM MAP 3GPP TS 29.00](https://en.wikipedia.org/wiki/Mobile_Application_Part)
    - Supported application contexts:
      - 0.4.0.0.1.0.25.3 - shortMsgMT-RelayContext-v3
      - 0.4.0.0.1.0.25.2 - shortMsgMT-RelayContext-v2
      - 0.4.0.0.1.0.20.3 - shortMsgGatewayContext-v3
      - 0.4.0.0.1.0.20.2 - shortMsgGatewayContext-v2
      - 0.4.0.0.1.0.20.1 - shortMsgGatewayContext-v1
      - 0.4.0.0.1.0.21.3 - shortMsgMO-RelayContext-v3
      - 0.4.0.0.1.0.21.2 - shortMsgMO-RelayContext-v2
      - 0.4.0.0.1.0.21.1 - shortMsgRelayContext-v1
    - Supported opcodes:
      - 44 - mt-forward-SM
      - 46 - mo-forward-SM
      - 45 - sendRoutingInfoForSM
- [SMS TPDU 3GPP TS 23.040](https://en.wikipedia.org/wiki/GSM_03.40)
- [SMPP v3.4](http://en.wikipedia.org/wiki/Short_Message_Peer-to-Peer)

###### stpd command line arguments

```bash
stpd - pMINK Signalling Transfer Point daemon
Copyright (c) 2012 Release14.org

Options:
 -?	help
 -i	unique daemon id
 -r	routing daemon address (ipv4:port)
 -D	start in debug mode

R14P Options:
=============
 --r14p-streams		R14P Session stream pool		(default = 10000)
 --r14p-stimeout	R14P Stream timeout in seconds		(default = 5)
 --r14p-smsg-pool	R14P Service message pool		(default = 1000)
 --r14p-sparam-pool	R14P Service message parameter pool	(default = 100000)
```

###### stpd example 

```bash
$ ./stpd -i stp-test-01 -r 127.0.0.1:10000
```

This will start **stpd** with the following parameters set:

- routingd IP address and listening port = **127.0.0.1:10000**
- pMINK unique daemon id = **stp-test-01**


#### STP Routing
##### Configuration example
The following configuration excerpt shows a simple **1 rule** routing; all traffic
having an **SCCP GT Calling Address** set to **12345678** will be routed back to 
**Application Server (AS)** named **TEST-AS-01**, configured on pMINK daemon
whose daemon type is set to **sgnd**. 

Routing destinations consist of **priority** value and **two destinations**; 
**R14P (Level 1 routing)** and **AS (Level 2 routing)**. They are processed in the following way:

**1. Level 1 routing**

  - destinations are sorted by their `priority` value and processed sequentially; if data
    transmission fails, next destination will be used until successful transmission is achieved or
    there are no more available destinations
  - data is first sent to a pMINK daemon whose **daemon_type** (configurable with `-t` switch
    for some daemons, e.g. SGN) is set to a value pointed out by the `r14p` configuration field


**2. Level 2 routing**

  - SGN daemon will receive the data and examine the destination (`dest_1`) which 
    was used for current data transfer
  - destination used consists of **two destinations** (`r14p` and `as`); the former
    was already consumed by the sending daemon, and the latter will be processed by the receiving SGN daemon
  - SGN daemon will forward the data to the **Application Server (AS)** pointed
    out by the `as` field of the current destination (`dest_1`)
  - if data transmission fails while sending the data to the destination **AS** (`as`),
    SGN daemon will use the next available destination and repeat the whole **Level 1 routing** 
    procedure

###### Routing rules configuration subset (mno/stp/stp-test-01/routing)
```c
rule_1 {
  hunt-stop   "1"
  description "Test rule #1"
  priority    "0"           
  route {        
    match {
      sccp {
        cgpa {
          gt {
            address "12345678"
          }
        }
      }
    }
    destination {
      dest_1 {
        priority   "0"
        as         "TEST-AS-01"
        r14p       "sgnd"
      }
    }
  }        
}

```

###### Step by step STP Rule activation process using the [pMINK CLI](#cli-client-shell):
1. Create new rule named **rule_1** on an STP node named **stp-test-01**
   - create empty rule node: `set mno stp stp-test-01 routing rule_1`
   - set a new root node for easier editing: `edit mno stp stp-test-01 routing rule_1`
   - set basic parameters:

     ```c
     set hunt-stop 1
     set description "Test rule #1"
     set priority 0
     ```
2. Create **match** section for **rule_1** on an STP node named **stp-test-01**
   - set a new root node for easier editing: `edit route`
   - create match section

     ```c
     set match sccp cgpa gt address 12345678
     ```
3. Create **destination** section for **rule_1** with **dest_1** destination
   - create new empty destination node: `set destination dest_1`
   - set a new root node for easier editing: `edit destination dest_1`
   - set destination parameters

     ```c
     set priority 0
     set as TEST-AS-01
     set r14p sgnd
     ```

   - commit changes: `commit`


#### STP Translations
##### Configuration example
Routing of traffic is one of the two most prominent features of Signal Transfer Point daemon; the second
one is the ability to modify live traffic, or in terms of STP, to do translations. More information
about the relationship between **match** and **translate** sections of routing rules can be found in 
chapters **8.2** and **8.2.5** of
the [FGN user's manual](http://github.com/dfranusic/pmink/blob/master/doc/smsf.pdf).

The following configuration excerpt shows a modified version of **rule_1** with one extra **translation**
added to the rule definition. **SCCP** translation used in this example will cause the packet to be **re-encoded**
with **Calling GT Address** changed to **99999999**.


###### Routing rules configuration subset (mno/stp/stp-test-01/routing/rule_1/route)
```c
match {
  sccp {
    cgpa {
      gt {
        address "12345678"
      }
    }
  }
}
translate {
  sccp {
    cgpa {
      gt {
        address "99999999"
      }
    }
  }
}
destination {
  dest_1 {
    priority   "0"
    as         "TEST-AS-01"
    r14p       "sgnd"
  }
}
```

###### Step by step Rule translation activation using the [pMINK CLI](#cli-client-shell):
1. Create new **SCCP translation** in **rule_1** on an STP node named **stp-test-01**
   - set a new root node for easier editing: `edit mno stp stp-test-01 routing rule_1 route translate`
   - set translation parameters:
   
    ```c
    set sccp cgpa gt address 99999999
    ``` 

   - commit changes: `commit`


### Data Retention daemon
---
Project MINK offers **two** solutions for passive traffic monitoring; **Data Retention daemon (DRD)**
and **Signalling Gateway Node (SGN)**. Although they both provide a useful set of features when it
comes to passive monitoring, there are some major differences to consider:

- SGN supports both SIGTRAN and SMPP; DRD is, for the moment, limited to SIGTRAN 
- SGN send passive data to STP; additional rule based routing is required on STP in order to 
  forward the data to some other user daemon responsible for data storage
- SGN uses libpcap or PF_RING to capture data; DRD can only receive data via R14P (e.g. from SGN)
- DRD was designed for passive SMS TPDU 3GPP TS 23.04 and SRI-for-SM monitoring
- DRD was designed to work with SGN when M3UA ASPs are used for receiving a **copy of live traffic**; this
  feature is provided by some 3d party STPs and is usually used for debugging
- DRD uses [MySQL](http://www.mysql.com) database for data storage
- DRD implements various levels of data correlation and tracking (TCAP, GSM MAP, SCCP); it
  is perfect for [Qos](https://en.wikipedia.org/wiki/Quality_of_service) monitoring

##### Data Retention daemon (DRD) data fields for SMS:

```
+------------------------------------------------------------------------+
| Field name            | Field value                                    |
+------------------------------------------------------------------------+
| DirectionId           | MO, MT                                         |
| SmsSizeTypeId         | SINGLE, CONCATENATED                           |
| SmsStatusId           | OK, Error                                      |
| CalledGt              | SCCP Called Party GT                           |
| CallingGt             | SCCP Calling Party GT                          |
| Scda                  | GSM MAP Service Centre Destination address     |
| Scoa                  | GSM MAP Service Centre Originating address     |
| Imsi                  | GSM MAP IMSI                                   |
| Msisdn                | GSM MAP MSISDN                                 |
| SmsDestination        | SMS TPDU TP-Destination-Address                |
| SmsOriginating        | SMS TPDU TP-Originating-Address                |
| SmsText               | SMS TPDU TP-User-Data                          |
| SmsTextEncId          | GSM 7bit, 8bit, UCS2                           |
| SmsDestinationEncId   | SMS TPDU TP-Destination-Address Type-Of-Number |
| SmsOriginatingEncId   | SMS TPDU TP-Originating-Address Type-Of-Number |
| DestinationPointCode  | M3UA DPC                                       |
| OriginatingPointCode  | M3UA OPC                                       |
| TcapSid               | TCAP Source Transaction ID                     |
| TcapDid               | TCAP Destination Transaction ID                |
| AppCtxOid             | TCAP Dialogue application context OID          |
| SmsPartnum            | CONCATENATED SMS Part number                   |
| SmsParts              | CONCATENATED SMS Total parts                   |
| SmsMessageId          | CONCATENATED SMS Message ID                    |
| ErrorTypeId           | Error type                                     |
| ErrorCode             | Error code                                     |
+------------------------------------------------------------------------+
```

###### Data Retention daemon (DRD) data fields for SRI-for-SM:

```
+--------------------------------------------------------------+
| Field name           | Field value                           |
+--------------------------------------------------------------+
| OriginatingPointCode | M3UA OPC                              |
| DestinationPointCode | M3UA DPC                              |
| CalledGt             | SCCP Called Party GT                  |
| CallingGt            | SCCP Calling Party GT                 |
| TcapSid              | TCAP Source Transaction ID            |
| TcapDid              | TCAP Destination Transaction ID       |
| AppCtxOid            | TCAP Dialogue application context OID |
| Imsi                 | GSM MAP IMSI                          |
| Msisdn               | GSM MAP MSISDN                        |
| Nnn                  | GSM MAP Network Node Number           |
| An                   | GSM MAP Additional Number             |
| Sca                  | GSM MAP Service Centre Address        |
| ErrorTypeId          | Error type                            |
| ErrorCode            | Error code                            |
+--------------------------------------------------------------+
```
            
Data Retention daemon (DRD) offers detailed error tracking for **SMS TPDU 3GPP TS 23.04** and **SRI-for-SM**;
the following error types are available:

```
+--------------------------------------------+
| Error type                                 |
+--------------------------------------------+
| TCAP Component error national              |
| TCAP Component error private               |
| TCAP P-Abort error                         |
| TCAP Dialogue error user                   |
| TCAP Dialogue error service provider       |
| TCAP Component Reject-General problem      |
| TCAP Component Reject-Invoke problem       |
| TCAP Component Reject-ReturnResult problem |
| TCAP Component Reject-ReturnError problem  |
| GSM MAP Error                              |
| No reply                                   |
| SMPP Error                                 |
| SCCP Error                                 |
| Unknown error                              |
+--------------------------------------------+
```

### Filtering Gateway Node daemon
---
Filtering Gateway Node is a versatile rule based packet filtering system currently used for 
[SMS](http://en.wikipedia.org/wiki/Short_Message_Service) ([SIGTRAN](http://en.wikipedia.org/wiki/SIGTRAN) and 
[SMPP](http://en.wikipedia.org/wiki/Short_Message_Peer-to-Peer)) filtering. 

- FGN documentation is distributed separately due to its complexity and size; user manual can be found 
here: [**smsf.pdf**](http://github.com/dfranusic/pmink/blob/master/doc/smsf.pdf)

### Pattern Detection Node daemon
---
Still in development, documentation coming soon...



### License
----
This software is licensed under the GPL v3 license
