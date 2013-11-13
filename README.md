![Alt text](https://raw.github.com/hypebeast/OSCShark/master/assets/logo.png)
<hr>

OSC Shark is a tool for monitoring and analysing OSC (Open Sound Control) packets.

## Overview

OSC Shark is a tool for monitoring and analysing OSC (Open Sound Control) packets. It's written in C++ and QT. It runs on Windows, Linux and Mac OS.

#### [Downloads](#releases)

![Alt text](https://raw.github.com/hypebeast/OSCShark/master/assets/screenshots/screenshot_1.png)

## Features

OSC Shark provides the following features:

* Cross-Platform (Windows, Linux and Mac OS)
* No external dependencies
* Listening for incoming OSC messages on multiple ports
* Enable/Disable specific listening ports
* OSC Address filtering
* Timestamps for incoming OSC messages
* Log only OSC messages from updated OSC addresses
* Export logged OSC messages to a CSV file

## Installation

OSC Shark is written in C++ and QT. It should run on every major platform: Windows, Linux and Mac OS.

The easiest way to install OSC Shark is to download one of the prebuild packages.

**Note**: Currently prebuild packages are only available for Mac OS. Packages for Windows and Linux will be provided in the future.

If you want to build OSC Shark by yourself see the 'For Developer' chapter for further instructions.

## Usage

The usage is quite simple:

1. Start OSC Shark
2. Add one or more listening ports with the **Add Port** button
3. Start listening on a port with a double click

## Releases

### 0.0.1

Initial release.

**Downloads**

* Mac OS: [OSCShark v0.0.1.dmg](http://sebastianruml.com/downloads/projects/OSCShark/OSCShark%20v0.0.1.dmg)

## For Developers

### Requirements

* QT >= 5.1.1 (every QT 5 release should work too)

### Build

Brief guide:

    $ git clone https://github.com/hypebeast/OSCShark.git
    $ cd OSCShark
    $ qmake OSCShark.pro
    $ make

Now you should find an executable your build directory.

### Generate a DMG Image on Mac OS

There is a script for building a DMG image that can be distributed to end-users. In order to build the DMG image you need to execute the following commands:

    $ cd OSCShark
    $ qmake
    $ cd scripts
    $ ./pack_installer_mac.sh

Once the script is finished, you should find the DMG image in the *scripts* directory.

## Credits

Thanks to [Kasper Kamperman](http://www.kasperkamperman.com/) for the initial idea. You can find his OSC Datamonitor [http://www.kasperkamperman.com/blog/osc-datamonitor/](http://www.kasperkamperman.com/blog/osc-datamonitor/).

For receiving OSC messages [OscPkt](http://gruntthepeon.free.fr/oscpkt/) is used. Thanks Julien Pommier for this wonderful library.

## License

OSC Shark is licensed under the terms of The MIT License (MIT).

    The MIT License (MIT)

    Copyright (c) <2013> <Sebastian Ruml>

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
