# IO
Ardan is built to support multiple methods of input and output, called IO Modules. IO modules are responsible for displaying pixels, and taking keyboard and mouse input.

## The Buffer
Each IO Module will use the buffer to get pixel information to display. The buffer is an abstraction layer that all rendering methods will write to once they figure out what colour each pixel needs to be.

Termicast, for example, has a ray translator which turns ray journey data into pixel colour information and stores that information in the buffer for any IO module to use.

## IO Modules
Ardan currently has two different IO modules. Terminal IO, and SDL IO.

### Terminal
Terminal IO uses the support of 24-bit colour in modern terminals to display output.

### SDL2
The SDL2 IO module uses an SDL2 window to write pixels to a window on the screen and also uses SDL2's input methods to get keyboard and mouse input.