# WIP Godot 4.2 port of Cyberreality's 6DOF input device support 
### - Bundled library is only compiled for windows, feel free to contribute libraries for other platforms.
### - only supports perspective mode, I have disabled inputs while in orthogonal projection mode due to bugs
### - Updates will be slow, as I had no GDExtension experience prior to this project.

## Building from source 
You need Scons, godot-cpp (GDExtension) and the HIDAPI library

Update the bundeled Sconstruct to locate the HIDAPI library you downloaded, and build the godot-cpp repo to create the headers for GDExtension.

Then build the Spacemouse sconstruct.

The library will be placed inside the addons/bin ready to be used.

lastly update the spacemouse.GDExtension to include the path for the library suited for your platform. 

## LICENSE

MIT License

Copyright (c) 2024 Marco André Hansen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
