# Aquarium

The [WebGL Aquarium](https://webglsamples.org/aquarium/aquarium.html) (Gregg Tavares and Human Engines, Copyright 2009 Google Inc., BSD-3-Clause). This is the scene in the [MetalANGLE](https://github.com/kakashidinho/metalangle) readme: static fish meshes, one draw per fish, tail bend in the vertex shader, plus the tank, props, seaweed, light rays, and bubbles.

Assets are not in git. From this directory:

```
python3 scripts/fetch_assets.py
make Debug
```

The fetch script downloads `aquarium/assets` from WebGLSamples and packs the tdl meshes. It leaves out the Maya / TGA `source_assets` tree. See `bin/data/LICENSE-aquarium.txt`.

Defaults match the web page: 500 fish, inside view, fog on. Once the frame rate is above 40 the page turns normal maps and reflection on. This example does the same. `--pretty` starts in that mode.

Keys:

- `1`–`9`, `0` — fish counts 1, 100, 500, 1000, 5000, 10000, 15000, 20000, 25000, 30000. `8` is the 20 000 fish shot from the MetalANGLE readme.
- space — cycle the six camera views (the second is outside the globe)
- `n` normal maps, `r` reflection, `f` fog, `t` tank, `m` museum, `b` bubbles, `y` light rays, `l` shark lasers

```
bin/aquariumExample_debug.app/Contents/MacOS/aquariumExample_debug --fish 20000 --view 1 --pretty
```

`--novsync` turns vsync off. `--shot 120` saves `/tmp/aquarium-shot.png` and quits.
