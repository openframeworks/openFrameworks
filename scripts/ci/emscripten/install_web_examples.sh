#!/bin/bash

docker exec -i emscripten apt update
docker exec -i emscripten apt install -y rsync
docker exec -i emscripten sh -c "echo \$PATH"

