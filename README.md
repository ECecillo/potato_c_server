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

# Providing static file to the client 

## Serving HTML file based on URL

At each request we can extract the first line of the HTTP Request that correspond to the content that the client is asking.

With a little bit of parsing from our part we were able to extract the path and read into a buffer the content that the client was looking for.

We also provided an error page in case the client is asking an unknown content.

## Serving any type of content

Until now our only focus was serving HTML content to our Client.

However, as of 2023, we usually have some CSS or JS stored on the server side.

So we need a way to: 
- Detect the type of file that the client is asking. 
- Set the Content-Type in our Header.

For the Detection it was pretty straight forward, we just needed to extract it when parsing the client request.

Finally, setting the Content-Type will be possible when serving the content because in first place we had to get the Path and since we detect

the type of the file during this operation we just need to re-use the result of the detection for the build of the Header.

For simplicity, we created a struct to store the file_type and the path to it.

At this point, our potatoe web server is able to do a lot of stuff.


# Understanding what a web server does using Node

https://www.youtube.com/watch?v=DaU1-XoANig
https://medium.com/@hnasr/when-nodejs-i-o-blocks-327f8a36fbd4

This article and video, really helped me understand how a socket read data coming from the internet, how the kernel store 
and forward data to the application buffer and finally how ndoe really handle all of this as a single threaded process.

# Author

CECILLON Enzo
