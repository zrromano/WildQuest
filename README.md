# VideoGameDesign
Add a path in your build command so the .exe gets saved to the main directory when compiling.

My build command as an example:
g++ 
-I C:\Users\crazy\Code\SDL\SDL2-2.0.9\i686-w64-mingw32\include\SDL2 
-L C:\Users\crazy\Code\SDL\SDL2-2.0.9\i686-w64-mingw32\lib 
-g 
-o 
C:\Users\crazy\Code\GitHub\VideoGameDesign\%e 
%f 
-lmingw32 
-lSDL2main 
-lSDL2.dll

This is the important bit:
C:\Users\crazy\Code\GitHub\VideoGameDesign\%e

Just take your main directory path and add \%e to the end.

Make sure to do the same to your execute command or it won't be able to find the .exe