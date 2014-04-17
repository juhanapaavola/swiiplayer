# SWiiPlayerUI - Connecting WiiMote to Symbian phone
# Copyright (C) 2010 Juhana Paavola <juhana.paavola@gmail.com>
#
# This file is part of SWiiPlayerUI.
#
# SWiiPlayerUI is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SWiiPlayerUI is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# You should have received a copy of the GNU Lesser General Public License
# along with SWiiPlayerUI.  If not, see <http://www.gnu.org/licenses/>.

QT       += core gui

TARGET = SWiiPlayer
TEMPLATE = app

ICON = swiiplayer_tiny.svg

SOURCES += main.cpp\
        swiiplayer.cpp \
    controllerwrapper.cpp \
    waitingdialog.cpp \
    connectinfodialog.cpp \
    configuredialog.cpp \
    helpdialog.cpp \
    helpviewerdialog.cpp

HEADERS  += swiiplayer.h \
    controllerwrapper.h \
    waitingdialog.h \
    connectinfodialog.h \
    configuredialog.h \
    helpdialog.h \
    helpviewerdialog.h

FORMS    += swiiplayer.ui \
    waitingdialog.ui \
    connectinfodialog.ui \
    configuredialog.ui \
    helpdialog.ui \
    helpviewerdialog.ui

CONFIG += mobility
MOBILITY = 

symbian {
    SOURCES += ccontroller.cpp
    HEADERS += ccontroller.h
    INCLUDEPATH += ../BTController/BTControllerServer/inc
    INCLUDEPATH += ../BTController/BTControllerClient/inc
    TARGET.UID3 = 0xe037da3b
    TARGET.CAPABILITY += ReadUserData \
        LocalServices \
        WriteUserData \
        UserEnvironment
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 \
        0x800000
    LIBS += -lBTControllerClient.dll \
        -lesock \
        -lbluetooth

    server.sources += "$(EPOCROOT)Epoc32/release/$(PLATFORM)/$(TARGET)/BTControllerServer.exe"
    server.sources += "$(EPOCROOT)Epoc32/release/$(PLATFORM)/$(TARGET)/BTControllerClient.dll"
    server.path = !:/sys/bin
    serverprivate.sources += "../btcontroller/group/defaultkeys.xml"
    serverprivate.sources += "../btcontroller/group/blacklist.xml"
    serverprivate.path = !:/private/ecf6d1a8

    defaultconfig.sources += "../btcontroller/group/defaultkeys.xml"
    defaultconfig.path = !:/data/WiiControllerConfig

    vendorinfo = "; Localised and non-localised vendor names"
    vendorinfo+=    "%{\"Juhana Paavola\"}" \
                    ":\"Juhana Paavola\""

    help.sources += "uidconf.txt"
    help.sources += "loadconf.txt"
    help.sources += "../about.txt"
    help.sources += "../LICENSE.LGPL"

    workaround = "[0x1028315F],0,0,0,{\"S60ProductID\"}";

    swiiplayer_deployment.pkg_prerules += vendorinfo
    swiiplayer_deployment.pkg_prerules += workaround

    DEPLOYMENT += swiiplayer_deployment
    DEPLOYMENT += serverprivate
    DEPLOYMENT += server
    DEPLOYMENT += defaultconfig
    DEPLOYMENT += help
}
simulator:DEFINES += _SIMULATOR_STUB
unix:DEFINES += _SIMULATOR_STUB

OTHER_FILES += \
    uidconf.txt \
    loadconf.txt \
    LICENSE.LGPL \
    ../about.txt
