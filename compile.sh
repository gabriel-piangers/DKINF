gcc src/main.c src/enemies.c src/ranking.c src/map.c src/player.c src/ui.c \
    -o game \
    -I./src \
    -lraylib -lm -lpthread -ldl -lrt -lX11