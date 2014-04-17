SWiiPlayer - Copyright (C) 2010 Juhana Paavola <juhana.paavola@gmail.com>

What is ?
---------

SWiiPlayer is Symbian implementation of connecting Nintendo WiiMote to Symbian 
phone. Program consists of two parts, server and client. Server part is the 
worker; it handles Bluetooth connection, data handling and key events to Symbian 
framework. While client is just starting the server and handling UI.

Compile 
------- 

To compile you need Nokia S60 5.0 SDK and Nokia Qt SDK for Symbian. Also 
Symbian^3 0.9 SDK can be used but initial development is done with S60 5.0 
version. First import BTController\group\bld.inf in Carbide, compile it with 
SBSv1. Makefiles will copy needed libraries and include headers to Qt SDK, those 
will be needed when compiling client. Then switch to Qt SDK, import 
SWiiPlayerUI\swiiplayer.pro, run qmake and compile. Nokia QtSDK will create .SIS 
file to install. To build and install fully working version, you will need 
certificate with SwEvent capability. If you don't have, you still can work with 
one exception; not able to send keyevents. See 
BTController\BTControllerServer\inc\buildflags.hrh how to disable keyevents.

Configuring SWiiPlayer
----------------------

Configuring is quite easy. Open BTController\group\defaultkeys.xml for help. 
Create new .XML file, use button names as tags with parameter key. Tag is the 
WiiMote button name and key-parameter is defining Symbian key as hex code. This 
key code is used to generate keyevent to Symbian framework. Save file, copy it 
to phone memory or memory card, now you can open new config from client UI. Also 
there is more automatic way to load new configs, use programs UID as filename 
for XML and it will be loaded instantly when program is started. For example; 
programs UID is 0x1234ABCD, save config file named as 
\WiiControllerConfigs\1234ABCD.xml.

You also can put applications to blacklist, server also checks blacklist UID's 
with foreground application. If app is listed in blacklist, no keyevents will be 
sent. This is safety, by default blacklist has calling and messaging blacklisted 
so you won't be able to use WiiMote for calling.

Warranty
--------

Absolutely no warranty, not any kind. As this is free software, I will not be 
responsible if you break or damage you, hardware or something else somehow. 

License
-------

LGPLv3, see LICENSE.LPGL file.