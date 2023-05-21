# Creating a TCP/IP web server from "scratch" using C 🔨.

# Why ?

This is a personnal project that aims to learn and understand how web server are built, see how TCP work ...

For this I chose to follow the following article which explain everything nicely 

https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa

# HTTP

Explain and show how it work

# Transport Layer and TCP

same

# Socket

Communication Channels that enable unrelated processes to exchange data locally or/and across networks.
A single socket is one end point of a two-way communication channel.

See schema on socket_client_server.png

## File descriptors
[Source](https://www.computerhope.com/jargon/f/file-descriptor.htm)

Number that uniquely identifies an open file in a computer OS. It describes a data resource, and how that resource
may be accessed.

When a program asks to open a file — or another data resource, like a network socket — the kernel:

- Grants access.
- Creates an entry in the global file table.
- Provides the software with the location of that entry.

## Socket Communication Domains

source : https://www.ibm.com/docs/en/aix/7.2?topic=sockets-socket-communication-domains

Socket sharing the same communication preperties, such as naming conventions and
protocols address formats, are grouped into communication domains.

A communication domain is sometimes referred to as name or address space.

The communication domain includes the following:
- Rules for manipulating and interpreting names
- Collection of related address formats that comprise an address family
- Set of protocols, called the protocol family

Example of address family:
- [AF_INET](https://www.ibm.com/docs/en/i/7.4?topic=family-af-inet-address) : 
  - For IP, provides interprocess communication between processes that run on the same system or on different systems.
- [AF_INET6](): IPv6, 
- [AF_UNIX](): local channel, similar to pipes
- [AF_ISO](): ISO protocols, 
- [AF_NS](): Xeros Network Systems protocols

## Socket Type of Service

Can be chose according to the address family we are using.

## Socket Protocol

Indicate a specific protocol to use in supporting the sockets operation.
We can see it has a variation of the type of service. 

# Compiling and executing the code

```bash
gcc server.c -o server && gcc client.c -o client
```

# Author

CECILLON Enzo
