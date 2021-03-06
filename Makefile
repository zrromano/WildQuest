ExeName		=WildQuest.exe
Headers 	=Animation.hpp Game.hpp Image.hpp MediaManager.hpp Player.hpp Projectile.hpp SceneState.hpp Sprite.hpp
Sources		=MediaManager.cpp
SDLPATH		=C:\SDL2-2.0.9\i686-w64-mingw32
SDLInclude	=$(SDLPATH)\include\SDL2
LDPATH		=$(SDLPATH)\lib
CFlags		=-static -I $(SDLInclude) -L $(LDPATH) -g
LDFlags		=-lmingw32 -lSDL2main -lSDL2.dll

..\$(ExeName): source.cpp $(Headers) $(Sources)
	g++  $(CFlags) -o $@ $< $(Sources) $(LDFlags)

clean:
	rm $(ExeName)
