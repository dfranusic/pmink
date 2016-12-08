# pMINK MNO
##### *Project MINK framework + Mobile Network Operator Compoents*
----
[![Build Status](https://travis-ci.org/dfranusic/pmink.svg?branch=master)](https://travis-ci.org/dfranusic/pmink)

- Core components
  - [**libr14p**](#libr14p) - R14P protocol library      
  - [**libpmcfg**](#libpmcfg) - Configuration parser library                                                                       
  - [**configd**](#configd) - Configuration daemon                                                                               
  - [**cli_service**](#cli_service) - CLI client shell                                                                                   
  - **routingd** - R14P Routing daemon                                                                                
  - **configdc** - Config daemon client                                                                               
  - **r14ptrapc** - R14P trap client                                                                                   
  - **libpmpcap** - Packet capture library                                                                             
  - **asn1c** - ASN.1 c++ compiler                                                                                 

- Mobile Network Operator components
  - **sgnd** - Signalling Gateway Node daemon
  - **stpd** - Signal Transfer Point daemon                                                                        
  - **drd** - Data Retention daemon                                                                               
  - **fgnd** - Filtering Gateway Node daemon                                                                       
  - **pdnd** - Pattern Detection Node daemon

- [License](#license)

### libr14p
---
[R14P](http://www.iana.org/assignments/sctp-parameters/sctp-parameters.xml#sctp-parameters-25) is 
a stateful stream based application layer protocol used for data communication between various pMINK nodes. 
It is [defined](http://github.com/dfranusic/pmink/blob/master/src/asn1/r14p/r14p.asn) using 
[ASN.1](https://en.wikipedia.org/wiki/Abstract_Syntax_Notation_One) syntax notation and carried by wire 
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

### libpmcfg
---
pMINK Configuration parser library is a core component used primarily by **Configuration daemon (configd)**. 
When designing a system on top of pMINK framework, configuration definition is always a good starting point. There
are two types of configuration files used by configd; **definition** and **contents**. 

Definition file is used to describe the structure of data stored in the contents file. Propper definition ensures data
integrity and avoids unecessary implementations of custom data validators in user specific daemons. Both types
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
### configd
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

### cli_service
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
- CLI definition file = **/tmp/cli_default.pmcfg **
- configd IP address and listening port = **127.0.0.1:10000**

CLI definition file is used for defining commands and modules available to the user.  At the moment, **cli_service** useses only one 
available plugin; **pm\_plg\_cli\_cfg.so** module is used for **cli <---> configd** communication. 
The file [cli\_default.pmcfg](http://github.com/dfranusic/pmink/blob/master/src/cli/cli_default.pmcfg) used in this example is a default 
CLI defintion; it is created during installation (**make install**)


### License
----
This software is licensed under the GPL v3 license
