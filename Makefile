# Define where the compiled plugin binaries will be stored
input-plugins-dir := VigasocoSDL/input 
audio-plugins-dir := VigasocoSDL/audio
video-plugins-dir := VigasocoSDL/video

# The default goal: creates directories then builds all sub-modules
all: | $(input-plugins-dir) $(audio-plugins-dir) $(video-plugins-dir)
	cd SDLInputKeyboardPlugin && make
	cd SDLVideoPlugins && make
	cd SDLAudioPlugin && make
	cd NULLAudioPlugin && make
	cd VigasocoSDL && make

# Rules to create the directories if they are missing
$(input-plugins-dir):
	mkdir $(input-plugins-dir)

$(audio-plugins-dir):
	mkdir $(audio-plugins-dir)

$(video-plugins-dir):
	mkdir $(video-plugins-dir)

# Clean up build artifacts by calling 'make clean' in each subdirectory
clean:
	cd SDLInputKeyboardPlugin && make clean
	cd SDLVideoPlugins && make clean
	cd SDLAudioPlugin && make clean
	cd NULLAudioPlugin && make clean
	cd VigasocoSDL && make clean
