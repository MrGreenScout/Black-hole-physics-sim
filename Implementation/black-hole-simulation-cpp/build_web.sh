emcc src/main.cpp -o ./../../docs/index.html \
  -I ./src \
  -I ./include \
  -s USE_SDL=2 \
  -s USE_SDL_IMAGE=2 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -O2