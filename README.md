# lowladb-winrt

This project provides a native implementation of LowlaDB packaged as a Windows Runtime Component for use on
Windows 8.1 and Windows Phone 8.1. Its primary use is to power the Cordova Plugin for LowlaDB but it may also be used as a standalone library.

# License
lowladb-winrt is available under the MIT license. See the LICENSE file for more info.

# Requirements
lowladb-winrt has been developed using Microsoft Visual Studio 2013 on Windows 8.1.

# Installation
The library depends on the core C++ liblowladb library and the build assumes that these projects are siblings:

```bash
mkdir lowla-dev
cd lowla-dev
git clone https://github.com/lowla/liblowladb.git
git clone https://github.com/lowla/lowladb-winrt.git
```

You should then be able to open the Visual Studio solution and build the project. The project contains unit
tests that you can run from the Tests menu in Visual Studio.

# Author
- Mark Dixon, mark_dixon@lowla.io

