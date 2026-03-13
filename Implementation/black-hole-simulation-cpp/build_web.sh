emcc src/main.cpp -o ./../black-hole-simulation-web/index.html \
  -I ./src \
  -I ./include \
  -s USE_SDL=2 \
  -s USE_SDL_IMAGE=2 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -O2