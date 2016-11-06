#OpenGLJet
=========
Generates an OpenGL model from Wavefront .OBJ and .MTL files.

##Compile on OSX (requires OSX Dev tools)
=========
Compile with:
```bash
cp Makefile.osx Makefile
make
```
Ignore any Deprecation warnings if compiling on OSX 10.9.

##Compile on Linux (requires OpenGL Libraries)
=========
Compile with:
```bash
cp Makefile.linux Makefile
make
```

##Usage (loads jet.obj and jet.mtl)
========
To run:
```bash
./jet
```

Keyboard Shortcuts:
'x', 'y', and 'z' are for rotation about their respective axis.
'S' zooms in and 's' zooms out (no quotes)
'r' resets the rotation angles
'q' exits the application
