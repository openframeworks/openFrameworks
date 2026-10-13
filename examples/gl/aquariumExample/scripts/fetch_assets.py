#!/usr/bin/env python3
"""Download the WebGL Aquarium runtime assets and pack the tdl meshes.

Source: https://github.com/WebGLSamples/WebGLSamples.github.io
License: BSD-3-Clause, Copyright 2009, Google Inc.
Skips aquarium/source_assets (Maya / TGA masters). The running demo uses
aquarium/assets plus two static textures.
"""

import array
import json
import os
import sys
import urllib.request

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
DATA = os.path.join(ROOT, "bin", "data")
ASSET_DIR = os.path.join(DATA, "assets")
MODEL_DIR = os.path.join(DATA, "models")
RAW = "https://raw.githubusercontent.com/WebGLSamples/WebGLSamples.github.io/master/aquarium/"
API = "https://api.github.com/repos/WebGLSamples/WebGLSamples.github.io/contents/aquarium/"

# Matches g_sceneInfo in aquarium.js. Unlisted models stay in the base group.
SCENE = {
    "SmallFishA": {"group": "fish", "program": "fish", "fog": True, "blend": True},
    "MediumFishA": {"group": "fish", "program": "fish", "fog": True, "blend": True},
    "MediumFishB": {"group": "fish", "program": "fish", "fog": True, "blend": True},
    "BigFishA": {"group": "fish", "program": "fish", "fog": True, "blend": True},
    "BigFishB": {"group": "fish", "program": "fish", "fog": True, "blend": True},
    "EnvironmentBox": {"group": "outside", "program": "diffuse", "fog": False, "blend": False},
    "GlobeBase": {"group": "base", "program": "diffuse", "fog": False, "blend": False},
    "GlobeInner": {"group": "inner", "program": "inner", "fog": True, "blend": False},
    "GlobeOuter": {"group": "outer", "program": "outer", "fog": True, "blend": True},
    "Skybox": {"group": "outside", "program": "diffuse", "fog": False, "blend": False},
    "SupportBeams": {"group": "outside", "program": "auto", "fog": False, "blend": False},
    "SeaweedA": {"group": "seaweed", "program": "seaweed", "fog": True, "blend": True},
    "SeaweedB": {"group": "seaweed", "program": "seaweed", "fog": True, "blend": True},
}

HAS_NORMAL = 1
HAS_UV = 2
HAS_TAN = 4
HAS_BIN = 8


def urlopen(url):
    req = urllib.request.Request(url, headers={"User-Agent": "openFrameworks-aquarium-example"})
    return urllib.request.urlopen(req, timeout=120)


def list_dir(path):
    with urlopen(API + path) as resp:
        return json.load(resp)


def download(url, dest, expect=None):
    if expect and os.path.isfile(dest) and os.path.getsize(dest) == expect:
        return
    os.makedirs(os.path.dirname(dest), exist_ok=True)
    print("get", os.path.basename(dest), flush=True)
    with urlopen(url) as resp:
        data = resp.read()
    with open(dest, "wb") as f:
        f.write(data)


def field(model, *names):
    fields = model.get("fields", {})
    for name in names:
        if name in fields:
            return fields[name]
    return None


def floats(field_info):
    if not field_info:
        return []
    return [float(v) for v in field_info["data"]]


def write_f32(f, values):
    buf = array.array("f", values)
    if sys.byteorder != "little":
        buf.byteswap()
    buf.tofile(f)


def write_u32(f, values):
    buf = array.array("I", values)
    if sys.byteorder != "little":
        buf.byteswap()
    buf.tofile(f)


def pack_model(js_path, aqm_path):
    with open(js_path, "r", encoding="utf-8") as f:
        data = json.load(f)
    models = data["models"]
    with open(aqm_path, "wb") as out:
        out.write(b"AQM1")
        write_u32(out, [len(models)])
        for model in models:
            textures = model.get("textures", {})
            keys = [(k, textures[k]) for k in ("diffuse", "normalMap", "reflectionMap") if k in textures]
            write_u32(out, [len(keys)])
            for key, filename in keys:
                kb = key.encode("utf-8")
                fb = filename.encode("utf-8")
                write_u32(out, [len(kb)])
                out.write(kb)
                write_u32(out, [len(fb)])
                out.write(fb)
            pos_f = field(model, "position", "positions", "vertexPositions")
            if not pos_f:
                raise SystemExit("no positions in " + js_path + " fields=" + ",".join(model.get("fields", {})))
            pos = floats(pos_f)
            ncomp = int(pos_f.get("numComponents", 3))
            nverts = len(pos) // ncomp
            if ncomp != 3:
                raise SystemExit("expected 3-component positions in " + js_path)
            nor = floats(field(model, "normal", "normals", "vertexNormals"))
            uv = floats(field(model, "texCoord", "texcoord", "uv", "vertexTextureCoords"))
            tan = floats(field(model, "tangent", "tangents"))
            binor = floats(field(model, "binormal", "binormals"))
            idx_f = field(model, "indices", "index")
            if idx_f:
                indices = [int(v) for v in idx_f["data"]]
            else:
                indices = list(range(nverts))
            flags = 0
            if nor:
                flags |= HAS_NORMAL
            if uv:
                flags |= HAS_UV
            if tan:
                flags |= HAS_TAN
            if binor:
                flags |= HAS_BIN
            write_u32(out, [nverts, len(indices), flags])
            write_f32(out, pos)
            if nor:
                write_f32(out, nor)
            if uv:
                write_f32(out, uv)
            if tan:
                write_f32(out, tan)
            if binor:
                write_f32(out, binor)
            write_u32(out, indices)
            print("  ", os.path.basename(js_path), "verts", nverts, "idx", len(indices), "flags", flags, flush=True)


def main():
    os.makedirs(ASSET_DIR, exist_ok=True)
    os.makedirs(MODEL_DIR, exist_ok=True)
    entries = list_dir("assets")
    kept = []
    for entry in entries:
        if entry.get("type") != "file":
            continue
        name = entry["name"]
        dest = os.path.join(ASSET_DIR, name)
        download(RAW + "assets/" + name, dest, entry.get("size"))
        kept.append(name)
    for name in ("beam.png", "bubble.png"):
        download(RAW + "static_assets/" + name, os.path.join(ASSET_DIR, name))
        kept.append(name)

    scenes = []
    for name in sorted(kept):
        if not name.endswith(".js") or name == "PropPlacement.js":
            continue
        stem = name[:-3]
        info = SCENE.get(stem, {"group": "base", "program": "auto", "fog": True, "blend": False})
        aqm = os.path.join(MODEL_DIR, stem + ".aqm")
        pack_model(os.path.join(ASSET_DIR, name), aqm)
        scenes.append({
            "name": stem,
            "file": "models/" + stem + ".aqm",
            "group": info["group"],
            "program": info["program"],
            "fog": info["fog"],
            "blend": info["blend"],
        })

    with open(os.path.join(ASSET_DIR, "PropPlacement.js"), "r", encoding="utf-8") as f:
        placement = json.load(f)
    scene_doc = {"scenes": scenes, "objects": placement["objects"]}
    with open(os.path.join(DATA, "scene.json"), "w", encoding="utf-8") as f:
        json.dump(scene_doc, f, separators=(",", ":"))
    print("scenes", len(scenes), "objects", len(placement["objects"]), flush=True)


if __name__ == "__main__":
    main()
