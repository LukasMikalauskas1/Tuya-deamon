Task. First daemon program

In this task we will create a daemon process, which will communicate with Tuya IoT cloud.

Before you start, you need to create a Tuya account. You can create an account here. Use your Teltonika email address to create an account. Now use this link here to reach the Tuya IoT platform, which will be used to connect your device to the cloud.

Before you start to develop the program, you need to find a way how to communicate with the platform. The best solution would be to use official SDK or Library, if the platform has one. Inform your internship manager/coordinator about which tool you will be using for communication. 

IMPORTANT! When you have found a way how to connect your program to the cloud, you can start developing your program. 

It is important, that SDK or library, which will be used to develop the program, must build SHARED libraries, not STATIC libraries.

Your program must send information to the cloud. What information to send to the cloud it's your choice. It can be memory information about your device/computer, or it can be some kind of random data or number.

Your program must use ARGP library to pass arguments to the program. Your program must accept these parameters:

    -Device id
    -Device secret
    -Product id

Syslog must be used to log program messages.

A makefile must be written to compile your program.

You can use Device debugging feature to debug if data is received from the device, which is located Product -> Device debugging on Tuya IoT platform. 

Extra task

Implement an action which would allow to change something on the device from the cloud. It could be as simple as writing something to a file or change some kind of parameter on the device. 

Use device debugging feature to test this action. 