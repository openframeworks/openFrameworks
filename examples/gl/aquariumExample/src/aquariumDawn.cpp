#include "ofApp.h"

#if !OF_USE_DAWN

bool aquariumDawnPrepare(ofApp &) { return false; }
void aquariumDawnDraw(ofApp &) {}

#else

#include "ofWebGPURenderer.h"

#include <unordered_map>
#include <vector>

namespace {

constexpr uint32_t kStride = 512;
constexpr uint32_t kSlots = 40000;

enum Pipe {
	PipeLit = 0,
	PipeFish,
	PipeSeaweed,
	PipeInner,
	PipeOuter,
	PipeRay,
	PipeBubble,
	PipeCount
};

struct GpuMesh {
	wgpu::Buffer pos, nor, uv, tan, bin, color, index;
	uint64_t posBytes = 0, norBytes = 0, uvBytes = 0, tanBytes = 0, binBytes = 0, colorBytes = 0, indexBytes = 0;
	uint32_t indexCount = 0;
	uint32_t vertexCount = 0;
	bool indexed = false;
	wgpu::BindGroup group[3];
};

float * at(float * u, int offset) {
	return u + offset;
}

void writeMat(float * u, int offset, const aq::Mat4 & m) {
	std::memcpy(at(u, offset), m.m, sizeof(m.m));
}

void write4(float * u, int offset, float x, float y, float z, float w) {
	float * p = at(u, offset);
	p[0] = x;
	p[1] = y;
	p[2] = z;
	p[3] = w;
}

void write1(float * u, int offset, float v) {
	u[offset] = v;
}

struct State {
	bool ready = false;
	wgpu::Device device;
	wgpu::Queue queue;
	wgpu::BindGroupLayout layout;
	wgpu::RenderPipeline pipeline[PipeCount];
	wgpu::Buffer uniform[3];
	wgpu::Sampler repeatSampler;
	wgpu::Sampler clampSampler;
	wgpu::Texture whiteTex;
	wgpu::TextureView whiteView;
	wgpu::Texture cubeTex;
	wgpu::TextureView cubeView;
	std::vector<wgpu::Texture> textures;
	std::unordered_map<std::string, wgpu::TextureView> views;
	std::unordered_map<const ofApp::Mesh *, GpuMesh> meshes;
	GpuMesh ray;
	GpuMesh bubble;
	int ring = 0;
	bool loggedDraw = false;
};

State g;

const char * kWGSL = R"WGSL(
struct U {
  viewProjection: mat4x4f,
  world: mat4x4f,
  worldInverseTranspose: mat4x4f,
  viewInverse: mat4x4f,
  lightWorldPos: vec4f,
  lightColor: vec4f,
  ambient: vec4f,
  specular: vec4f,
  fogColor: vec4f,
  shininess: f32,
  specularFactor: f32,
  uNormal: f32,
  uReflect: f32,
  uNormalBias: f32,
  uFog: f32,
  fogPower: f32,
  fogMult: f32,
  fogOffset: f32,
  refractionFudge: f32,
  eta: f32,
  tankColorFudge: f32,
  time: f32,
  scale: f32,
  fishLength: f32,
  fishWaveLength: f32,
  fishBendAmount: f32,
  pad0: f32,
  pad1: f32,
  pad2: f32,
  colorMult: vec4f,
  worldPosition: vec4f,
  nextPosition: vec4f,
}
@group(0) @binding(0) var<uniform> u: U;
@group(0) @binding(1) var diffuseTex: texture_2d<f32>;
@group(0) @binding(2) var normalTex: texture_2d<f32>;
@group(0) @binding(3) var reflectTex: texture_2d<f32>;
@group(0) @binding(4) var skyTex: texture_cube<f32>;
@group(0) @binding(5) var sampRepeat: sampler;
@group(0) @binding(6) var sampClamp: sampler;

struct LitIn {
  @location(0) pos: vec3f,
  @location(1) normal: vec3f,
  @location(2) uv: vec2f,
  @location(3) tangent: vec3f,
  @location(4) binormal: vec3f,
}
struct SeaIn {
  @location(0) pos: vec3f,
  @location(1) normal: vec3f,
  @location(2) uv: vec2f,
}
struct TexIn {
  @location(0) pos: vec3f,
  @location(1) uv: vec2f,
}
struct BubIn {
  @location(0) pos: vec3f,
  @location(1) uv: vec2f,
  @location(2) color: vec4f,
}
struct LitOut {
  @builtin(position) clip: vec4f,
  @location(0) v_position: vec4f,
  @location(1) uv: vec2f,
  @location(2) tangent: vec3f,
  @location(3) binormal: vec3f,
  @location(4) normal: vec3f,
  @location(5) toLight: vec3f,
  @location(6) toView: vec3f,
}
struct TexOut {
  @builtin(position) clip: vec4f,
  @location(0) uv: vec2f,
}
struct BubOut {
  @builtin(position) clip: vec4f,
  @location(0) uv: vec2f,
  @location(1) color: vec4f,
}

fn lit(l: f32, h: f32, m: f32) -> vec4f {
  let spec = select(0.0, pow(max(h, 0.0), m), l > 0.0);
  return vec4f(1.0, max(l, 0.0), spec, 1.0);
}

@vertex fn vsStatic(in: LitIn) -> LitOut {
  var o: LitOut;
  let wp = u.world * vec4f(in.pos, 1.0);
  o.uv = in.uv;
  o.v_position = u.viewProjection * wp;
  o.normal = (u.worldInverseTranspose * vec4f(in.normal, 0.0)).xyz;
  o.tangent = (u.worldInverseTranspose * vec4f(in.tangent, 0.0)).xyz;
  o.binormal = (u.worldInverseTranspose * vec4f(in.binormal, 0.0)).xyz;
  o.toLight = u.lightWorldPos.xyz - wp.xyz;
  o.toView = (u.viewInverse[3] - wp).xyz;
  o.clip = o.v_position;
  return o;
}

@vertex fn vsFish(in: LitIn) -> LitOut {
  var o: LitOut;
  let pos = in.pos;
  let vz = normalize(u.worldPosition.xyz - u.nextPosition.xyz);
  let vx = normalize(cross(vec3f(0.0, 1.0, 0.0), vz));
  let vy = cross(vz, vx);
  let orient = mat4x4f(vec4f(vx, 0.0), vec4f(vy, 0.0), vec4f(vz, 0.0), vec4f(u.worldPosition.xyz, 1.0));
  let sm = mat4x4f(
    vec4f(u.scale, 0.0, 0.0, 0.0),
    vec4f(0.0, u.scale, 0.0, 0.0),
    vec4f(0.0, 0.0, u.scale, 0.0),
    vec4f(0.0, 0.0, 0.0, 1.0));
  let world = orient * sm;
  let len = max(u.fishLength, 0.001);
  let mult = select(-pos.z / len * 2.0, pos.z / len, pos.z > 0.0);
  let s = sin(u.time + mult * u.fishWaveLength);
  let offset = mult * mult * s * u.fishBendAmount;
  let bent = vec4f(pos.x + offset, pos.y, pos.z, 1.0);
  let wp = world * vec4f(pos, 1.0);
  o.uv = in.uv;
  o.v_position = u.viewProjection * (world * bent);
  o.normal = (world * vec4f(in.normal, 0.0)).xyz;
  o.tangent = (world * vec4f(in.tangent, 0.0)).xyz;
  o.binormal = (world * vec4f(in.binormal, 0.0)).xyz;
  o.toLight = u.lightWorldPos.xyz - wp.xyz;
  o.toView = (u.viewInverse[3] - wp).xyz;
  o.clip = o.v_position;
  return o;
}

@vertex fn vsSeaweed(in: SeaIn) -> LitOut {
  var o: LitOut;
  let toCamera = normalize(u.viewInverse[3].xyz - u.world[3].xyz);
  let yAxis = vec3f(0.0, 1.0, 0.0);
  let xAxis = cross(yAxis, toCamera);
  let newWorld = mat4x4f(vec4f(xAxis, 0.0), vec4f(yAxis, 0.0), vec4f(xAxis, 0.0), u.world[3]);
  let swayT = in.pos.y * 0.07;
  let sway = swayT * swayT;
  let swayed = vec4f(in.pos, 1.0) + vec4f(sin(u.time * 0.5) * sway, -4.0, 0.0, 0.0);
  let wp = u.world * vec4f(in.pos, 1.0);
  o.uv = in.uv;
  o.v_position = (u.viewProjection * newWorld) * swayed;
  o.normal = (newWorld * vec4f(in.normal, 0.0)).xyz;
  o.tangent = vec3f(1.0, 0.0, 0.0);
  o.binormal = vec3f(0.0, 0.0, 1.0);
  o.toLight = u.lightWorldPos.xyz - wp.xyz;
  o.toView = (u.viewInverse[3] - wp).xyz;
  o.clip = o.v_position;
  return o;
}

@vertex fn vsTex(in: TexIn) -> TexOut {
  var o: TexOut;
  o.clip = u.viewProjection * u.world * vec4f(in.pos, 1.0);
  o.uv = in.uv;
  return o;
}

@vertex fn vsBubble(in: BubIn) -> BubOut {
  var o: BubOut;
  o.clip = u.viewProjection * vec4f(in.pos, 1.0);
  o.uv = in.uv;
  o.color = in.color;
  return o;
}

fn fogMix(color: vec4f, clip: vec4f) -> vec4f {
  if (u.uFog < 0.5) {
    return color;
  }
  let ndc = max(clip.z / clip.w, 0.0);
  let fog = clamp(pow(ndc, u.fogPower) * u.fogMult - u.fogOffset, 0.0, 1.0);
  return mix(color, vec4f(u.fogColor.rgb, color.a), fog);
}

@fragment fn fsLit(in: LitOut) -> @location(0) vec4f {
  let diffuseColor = textureSample(diffuseTex, sampRepeat, in.uv);
  var normalSpec = vec4f(0.0);
  var normal = normalize(in.normal);
  if (u.uNormal > 0.5) {
    normalSpec = textureSample(normalTex, sampRepeat, in.uv);
    var tangentNormal = normalSpec.xyz - vec3f(0.5);
    if (u.uNormalBias > 0.5) {
      tangentNormal = normalize(tangentNormal + vec3f(0.0, 0.0, 2.0));
    }
    normal = normalize(mat3x3f(in.tangent, in.binormal, in.normal) * tangentNormal);
  }
  let surfaceToLight = normalize(in.toLight);
  let surfaceToView = normalize(in.toView);
  let halfVector = normalize(surfaceToLight + surfaceToView);
  let litR = lit(dot(normal, surfaceToLight), dot(normal, halfVector), u.shininess);
  let rgb = (u.lightColor * (diffuseColor * litR.y + diffuseColor * u.ambient + u.specular * litR.z * u.specularFactor * normalSpec.a)).rgb;
  var outColor = vec4f(rgb, diffuseColor.a);
  if (u.uReflect > 0.5) {
    let reflection = textureSample(reflectTex, sampRepeat, in.uv);
    let skyColor = textureSample(skyTex, sampClamp, -reflect(surfaceToView, normal));
    outColor = vec4f(mix(skyColor, outColor, 1.0 - reflection.r).rgb, diffuseColor.a);
  }
  return fogMix(outColor, in.v_position);
}

@fragment fn fsSeaweed(in: LitOut) -> @location(0) vec4f {
  let diffuseColor = textureSample(diffuseTex, sampRepeat, in.uv);
  if (diffuseColor.a < 0.3) {
    discard;
  }
  let normal = normalize(in.normal);
  let surfaceToLight = normalize(in.toLight);
  let surfaceToView = normalize(in.toView);
  let halfVector = normalize(surfaceToLight + surfaceToView);
  let litR = lit(dot(normal, surfaceToLight), dot(normal, halfVector), u.shininess);
  let rgb = (u.lightColor * (diffuseColor * litR.y + diffuseColor * u.ambient + u.specular * litR.z * u.specularFactor)).rgb;
  return fogMix(vec4f(rgb, diffuseColor.a), in.v_position);
}

@fragment fn fsInner(in: LitOut) -> @location(0) vec4f {
  let diffuseColor = textureSample(diffuseTex, sampRepeat, in.uv) + vec4f(u.tankColorFudge, u.tankColorFudge, u.tankColorFudge, 1.0);
  let refraction = textureSample(reflectTex, sampRepeat, in.uv);
  var normal = normalize(in.normal);
  if (u.uNormal > 0.5) {
    let normalSpec = textureSample(normalTex, sampRepeat, in.uv);
    var tangentNormal = normalSpec.xyz - vec3f(0.5);
    tangentNormal = normalize(tangentNormal + vec3f(0.0, 0.0, u.refractionFudge));
    normal = normalize(mat3x3f(in.tangent, in.binormal, in.normal) * tangentNormal);
  }
  let surfaceToView = normalize(in.toView);
  let refractionVec = refract(surfaceToView, normal, u.eta);
  let skyColor = textureSample(skyTex, sampClamp, refractionVec);
  let outColor = vec4f(mix(skyColor * diffuseColor, diffuseColor, refraction.r).rgb, diffuseColor.a);
  return fogMix(outColor, in.v_position);
}

@fragment fn fsOuter(in: LitOut) -> @location(0) vec4f {
  let diffuseColor = textureSample(diffuseTex, sampRepeat, in.uv);
  let reflection = textureSample(reflectTex, sampRepeat, in.uv);
  var normal = normalize(in.normal);
  if (u.uNormal > 0.5) {
    let normalSpec = textureSample(normalTex, sampRepeat, in.uv);
    let tangentNormal = normalSpec.xyz - vec3f(0.5);
    normal = normalize(mat3x3f(in.tangent, in.binormal, in.normal) * tangentNormal);
  }
  let surfaceToView = normalize(in.toView);
  let skyColor = textureSample(skyTex, sampClamp, -reflect(surfaceToView, normal));
  let fudge = skyColor.rgb * vec3f(1.1);
  let bright = min(1.0, fudge.r * fudge.g * fudge.b);
  let reflectColor = mix(vec4f(skyColor.rgb, bright), diffuseColor, 1.0 - reflection.r);
  let r = abs(dot(surfaceToView, normal));
  return vec4f(mix(skyColor, reflectColor, (r + 0.3) * reflection.r).rgb, 1.0 - r);
}

@fragment fn fsTex(in: TexOut) -> @location(0) vec4f {
  return textureSample(diffuseTex, sampRepeat, in.uv) * u.colorMult;
}

@fragment fn fsBubble(in: BubOut) -> @location(0) vec4f {
  return textureSample(diffuseTex, sampRepeat, in.uv) * in.color;
}
)WGSL";

wgpu::Buffer makeBuffer(uint64_t size, wgpu::BufferUsage usage) {
	wgpu::BufferDescriptor desc = {};
	desc.size = std::max<uint64_t>(size, 4);
	desc.usage = usage;
	return g.device.CreateBuffer(&desc);
}

void writeBuffer(wgpu::Buffer buffer, const void * data, uint64_t size) {
	if (!buffer || !data || size == 0) {
		return;
	}
	g.queue.WriteBuffer(buffer, 0, data, size);
}

int mipCountFor(int w, int h) {
	int n = 1;
	while (w > 1 || h > 1) {
		w = std::max(1, w / 2);
		h = std::max(1, h / 2);
		++n;
	}
	return n;
}

std::vector<unsigned char> downsample(const std::vector<unsigned char> & src, int w, int h, int & nw, int & nh) {
	nw = std::max(1, w / 2);
	nh = std::max(1, h / 2);
	std::vector<unsigned char> dst(static_cast<size_t>(nw) * static_cast<size_t>(nh) * 4);
	for (int y = 0; y < nh; ++y) {
		for (int x = 0; x < nw; ++x) {
			const int x0 = std::min(x * 2, w - 1);
			const int y0 = std::min(y * 2, h - 1);
			const int x1 = std::min(x0 + 1, w - 1);
			const int y1 = std::min(y0 + 1, h - 1);
			unsigned char * d = dst.data() + (static_cast<size_t>(y) * nw + x) * 4;
			for (int c = 0; c < 4; ++c) {
				const int s00 = src[(static_cast<size_t>(y0) * w + x0) * 4 + c];
				const int s10 = src[(static_cast<size_t>(y0) * w + x1) * 4 + c];
				const int s01 = src[(static_cast<size_t>(y1) * w + x0) * 4 + c];
				const int s11 = src[(static_cast<size_t>(y1) * w + x1) * 4 + c];
				d[c] = static_cast<unsigned char>((s00 + s10 + s01 + s11) / 4);
			}
		}
	}
	return dst;
}

std::vector<unsigned char> toRGBA(const ofPixels & pix) {
	const int w = pix.getWidth();
	const int h = pix.getHeight();
	const int channels = pix.getNumChannels();
	std::vector<unsigned char> rgba(static_cast<size_t>(w) * static_cast<size_t>(h) * 4, 255);
	if (w <= 0 || h <= 0 || !pix.getData() || channels < 1) {
		return {};
	}
	const unsigned char * src = pix.getData();
	for (int i = 0; i < w * h; ++i) {
		const unsigned char * p = src + static_cast<size_t>(i) * channels;
		unsigned char * d = rgba.data() + static_cast<size_t>(i) * 4;
		if (channels == 1) {
			d[0] = d[1] = d[2] = p[0];
		} else if (channels == 2) {
			d[0] = d[1] = d[2] = p[0];
			d[3] = p[1];
		} else if (channels == 3) {
			d[0] = p[0];
			d[1] = p[1];
			d[2] = p[2];
		} else {
			d[0] = p[0];
			d[1] = p[1];
			d[2] = p[2];
			d[3] = p[3];
		}
	}
	return rgba;
}

wgpu::TextureView upload2D(const std::vector<unsigned char> & rgba, int w, int h) {
	if (rgba.empty() || w <= 0 || h <= 0) {
		return g.whiteView;
	}
	const int mips = mipCountFor(w, h);
	wgpu::TextureDescriptor desc = {};
	desc.usage = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst;
	desc.dimension = wgpu::TextureDimension::e2D;
	desc.size = { static_cast<uint32_t>(w), static_cast<uint32_t>(h), 1 };
	desc.format = wgpu::TextureFormat::RGBA8Unorm;
	desc.mipLevelCount = static_cast<uint32_t>(mips);
	wgpu::Texture tex = g.device.CreateTexture(&desc);
	std::vector<unsigned char> level = rgba;
	int lw = w;
	int lh = h;
	for (int mip = 0; mip < mips; ++mip) {
		const uint32_t bpr = (static_cast<uint32_t>(lw) * 4u + 255u) & ~255u;
		std::vector<unsigned char> padded(static_cast<size_t>(bpr) * static_cast<size_t>(lh));
		for (int y = 0; y < lh; ++y) {
			std::memcpy(padded.data() + static_cast<size_t>(y) * bpr, level.data() + static_cast<size_t>(y) * lw * 4, static_cast<size_t>(lw) * 4);
		}
		wgpu::TexelCopyTextureInfo dst = {};
		dst.texture = tex;
		dst.mipLevel = static_cast<uint32_t>(mip);
		dst.aspect = wgpu::TextureAspect::All;
		wgpu::TexelCopyBufferLayout layout = {};
		layout.bytesPerRow = bpr;
		layout.rowsPerImage = static_cast<uint32_t>(lh);
		wgpu::Extent3D size = { static_cast<uint32_t>(lw), static_cast<uint32_t>(lh), 1 };
		g.queue.WriteTexture(&dst, padded.data(), padded.size(), &layout, &size);
		if (mip + 1 < mips) {
			int nw = 0;
			int nh = 0;
			level = downsample(level, lw, lh, nw, nh);
			lw = nw;
			lh = nh;
		}
	}
	g.textures.push_back(tex);
	return tex.CreateView();
}

wgpu::TextureView viewNamed(const std::string & name) {
	if (name.empty()) {
		return g.whiteView;
	}
	auto found = g.views.find(name);
	if (found == g.views.end()) {
		return g.whiteView;
	}
	return found->second;
}

wgpu::BindGroup makeGroup(int slot, wgpu::TextureView diffuse, wgpu::TextureView normal, wgpu::TextureView reflect) {
	wgpu::BindGroupEntry entries[7] = {};
	entries[0].binding = 0;
	entries[0].buffer = g.uniform[slot];
	entries[0].offset = 0;
	entries[0].size = kStride;
	entries[1].binding = 1;
	entries[1].textureView = diffuse ? diffuse : g.whiteView;
	entries[2].binding = 2;
	entries[2].textureView = normal ? normal : g.whiteView;
	entries[3].binding = 3;
	entries[3].textureView = reflect ? reflect : g.whiteView;
	entries[4].binding = 4;
	entries[4].textureView = g.cubeView;
	entries[5].binding = 5;
	entries[5].sampler = g.repeatSampler;
	entries[6].binding = 6;
	entries[6].sampler = g.clampSampler;
	wgpu::BindGroupDescriptor desc = {};
	desc.layout = g.layout;
	desc.entryCount = 7;
	desc.entries = entries;
	return g.device.CreateBindGroup(&desc);
}

void finishGroups(GpuMesh & mesh, wgpu::TextureView diffuse, wgpu::TextureView normal, wgpu::TextureView reflect) {
	for (int slot = 0; slot < 3; ++slot) {
		mesh.group[slot] = makeGroup(slot, diffuse, normal, reflect);
	}
}

wgpu::Buffer uploadArray(const std::vector<float> & data, uint64_t & bytes) {
	bytes = data.size() * sizeof(float);
	wgpu::Buffer buffer = makeBuffer(std::max<uint64_t>(bytes, 4), wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst);
	if (!data.empty()) {
		writeBuffer(buffer, data.data(), bytes);
	}
	return buffer;
}

bool uploadModel(const ofApp::Mesh & mesh, const ofApp::Model & model) {
	if (mesh.indices.empty() || mesh.pos.empty()) {
		return false;
	}
	GpuMesh gpu;
	gpu.indexed = true;
	gpu.indexCount = static_cast<uint32_t>(mesh.indices.size());
	gpu.vertexCount = static_cast<uint32_t>(mesh.pos.size() / 3);
	gpu.pos = uploadArray(mesh.pos, gpu.posBytes);
	gpu.nor = uploadArray(mesh.nor, gpu.norBytes);
	gpu.uv = uploadArray(mesh.uv, gpu.uvBytes);
	gpu.tan = uploadArray(mesh.tan, gpu.tanBytes);
	gpu.bin = uploadArray(mesh.bin, gpu.binBytes);
	gpu.indexBytes = mesh.indices.size() * sizeof(uint32_t);
	gpu.index = makeBuffer(gpu.indexBytes, wgpu::BufferUsage::Index | wgpu::BufferUsage::CopyDst);
	writeBuffer(gpu.index, mesh.indices.data(), gpu.indexBytes);
	finishGroups(gpu, viewNamed(model.diffuse), viewNamed(model.normalMap), viewNamed(model.reflectionMap));
	g.meshes[&mesh] = std::move(gpu);
	return true;
}

wgpu::RenderPipeline makePipeline(const char * vs, const char * fs, int kind, bool blend, bool additive, bool depthWrite, wgpu::CullMode cull) {
	wgpu::VertexAttribute attrs[5] = {};
	wgpu::VertexBufferLayout layouts[5] = {};
	int n = 0;
	auto add = [&](int location, wgpu::VertexFormat format, uint64_t stride) {
		attrs[n].format = format;
		attrs[n].offset = 0;
		attrs[n].shaderLocation = static_cast<uint32_t>(location);
		layouts[n].arrayStride = stride;
		layouts[n].stepMode = wgpu::VertexStepMode::Vertex;
		layouts[n].attributeCount = 1;
		layouts[n].attributes = &attrs[n];
		++n;
	};
	if (kind == PipeBubble) {
		add(0, wgpu::VertexFormat::Float32x3, 12);
		add(1, wgpu::VertexFormat::Float32x2, 8);
		add(2, wgpu::VertexFormat::Float32x4, 16);
	} else if (kind == PipeRay) {
		add(0, wgpu::VertexFormat::Float32x3, 12);
		add(1, wgpu::VertexFormat::Float32x2, 8);
	} else if (kind == PipeSeaweed) {
		add(0, wgpu::VertexFormat::Float32x3, 12);
		add(1, wgpu::VertexFormat::Float32x3, 12);
		add(2, wgpu::VertexFormat::Float32x2, 8);
	} else {
		add(0, wgpu::VertexFormat::Float32x3, 12);
		add(1, wgpu::VertexFormat::Float32x3, 12);
		add(2, wgpu::VertexFormat::Float32x2, 8);
		add(3, wgpu::VertexFormat::Float32x3, 12);
		add(4, wgpu::VertexFormat::Float32x3, 12);
	}

	wgpu::BlendState blendState = {};
	if (additive) {
		blendState.color.srcFactor = wgpu::BlendFactor::SrcAlpha;
		blendState.color.dstFactor = wgpu::BlendFactor::One;
		blendState.alpha.srcFactor = wgpu::BlendFactor::SrcAlpha;
		blendState.alpha.dstFactor = wgpu::BlendFactor::One;
	} else if (blend) {
		blendState.color.srcFactor = wgpu::BlendFactor::SrcAlpha;
		blendState.color.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
		blendState.alpha.srcFactor = wgpu::BlendFactor::SrcAlpha;
		blendState.alpha.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
	} else {
		blendState.color.srcFactor = wgpu::BlendFactor::One;
		blendState.color.dstFactor = wgpu::BlendFactor::Zero;
		blendState.alpha.srcFactor = wgpu::BlendFactor::One;
		blendState.alpha.dstFactor = wgpu::BlendFactor::Zero;
	}
	blendState.color.operation = wgpu::BlendOperation::Add;
	blendState.alpha.operation = wgpu::BlendOperation::Add;

	wgpu::ColorTargetState target = {};
	target.format = wgpu::TextureFormat::BGRA8Unorm;
	target.blend = &blendState;
	target.writeMask = wgpu::ColorWriteMask::All;

	wgpu::FragmentState fragment = {};
	fragment.module = nullptr;
	wgpu::ShaderSourceWGSL wgsl;
	wgsl.code = kWGSL;
	wgpu::ShaderModuleDescriptor shaderDesc;
	shaderDesc.nextInChain = &wgsl;
	wgpu::ShaderModule module = g.device.CreateShaderModule(&shaderDesc);
	fragment.module = module;
	fragment.entryPoint = fs;
	fragment.targetCount = 1;
	fragment.targets = &target;

	wgpu::DepthStencilState depth = {};
	depth.format = wgpu::TextureFormat::Depth24Plus;
	depth.depthWriteEnabled = depthWrite ? wgpu::OptionalBool::True : wgpu::OptionalBool::False;
	depth.depthCompare = wgpu::CompareFunction::LessEqual;

	wgpu::RenderPipelineDescriptor pipe = {};
	pipe.layout = nullptr;
	wgpu::PipelineLayoutDescriptor layoutDesc;
	layoutDesc.bindGroupLayoutCount = 1;
	layoutDesc.bindGroupLayouts = &g.layout;
	wgpu::PipelineLayout layout = g.device.CreatePipelineLayout(&layoutDesc);
	pipe.layout = layout;
	pipe.vertex.module = module;
	pipe.vertex.entryPoint = vs;
	pipe.vertex.bufferCount = static_cast<uint32_t>(n);
	pipe.vertex.buffers = layouts;
	pipe.fragment = &fragment;
	pipe.primitive.topology = wgpu::PrimitiveTopology::TriangleList;
	pipe.primitive.cullMode = cull;
	pipe.primitive.frontFace = wgpu::FrontFace::CCW;
	pipe.depthStencil = &depth;
	pipe.multisample.count = 1;
	return g.device.CreateRenderPipeline(&pipe);
}

void fillShared(float * u, ofApp & app, const aq::Mat4 & viewProjection) {
	std::memset(u, 0, kStride);
	writeMat(u, 0, viewProjection);
	writeMat(u, 48, app.viewInverse);
	write4(u, 64, app.light[0], app.light[1], app.light[2], 0.f);
	write4(u, 68, 1.f, 1.f, 1.f, 1.f);
	write4(u, 72, app.ambient[0], app.ambient[1], app.ambient[2], 1.f);
	write4(u, 76, 1.f, 1.f, 1.f, 1.f);
	write4(u, 80, app.fogRgb[0], app.fogRgb[1], app.fogRgb[2], 1.f);
	write1(u, 90, app.fogPower);
	write1(u, 91, app.fogMult);
	write1(u, 92, app.fogOffset);
	write1(u, 93, app.refractionFudge);
	write1(u, 94, app.eta);
	write1(u, 95, app.tankColorFudge);
}

struct Item {
	int pipe = PipeLit;
	const GpuMesh * mesh = nullptr;
	float u[kStride / sizeof(float)];
};

void pushModel(std::vector<Item> & items, ofApp & app, const ofApp::Model & model, const aq::Mat4 & world, int pipe, float specular, float useNormal, float useReflect, float bias, float fog, float time) {
	auto found = g.meshes.find(&model.mesh);
	if (found == g.meshes.end() || found->second.indexCount == 0) {
		return;
	}
	Item item;
	item.pipe = pipe;
	item.mesh = &found->second;
	fillShared(item.u, app, app.viewProjection);
	writeMat(item.u, 16, world);
	aq::Mat4 inv;
	aq::Mat4 invT;
	aq::inverse(inv, world);
	aq::transpose(invT, inv);
	writeMat(item.u, 32, invT);
	write1(item.u, 84, pipe == PipeFish ? 5.f : 50.f);
	write1(item.u, 85, pipe == PipeFish ? 0.3f : specular);
	write1(item.u, 86, useNormal);
	write1(item.u, 87, useReflect);
	write1(item.u, 88, bias);
	write1(item.u, 89, fog);
	write1(item.u, 96, time);
	items.push_back(item);
}

void bindMesh(wgpu::RenderPassEncoder pass, const GpuMesh & mesh, int pipe) {
	if (pipe == PipeRay) {
		pass.SetVertexBuffer(0, mesh.pos, 0, mesh.posBytes);
		pass.SetVertexBuffer(1, mesh.uv, 0, mesh.uvBytes);
		return;
	}
	if (pipe == PipeBubble) {
		pass.SetVertexBuffer(0, mesh.pos, 0, mesh.posBytes);
		pass.SetVertexBuffer(1, mesh.uv, 0, mesh.uvBytes);
		pass.SetVertexBuffer(2, mesh.color, 0, mesh.colorBytes);
		return;
	}
	if (pipe == PipeSeaweed) {
		pass.SetVertexBuffer(0, mesh.pos, 0, mesh.posBytes);
		pass.SetVertexBuffer(1, mesh.nor, 0, mesh.norBytes);
		pass.SetVertexBuffer(2, mesh.uv, 0, mesh.uvBytes);
		return;
	}
	pass.SetVertexBuffer(0, mesh.pos, 0, mesh.posBytes);
	pass.SetVertexBuffer(1, mesh.nor, 0, mesh.norBytes);
	pass.SetVertexBuffer(2, mesh.uv, 0, mesh.uvBytes);
	pass.SetVertexBuffer(3, mesh.tan, 0, mesh.tanBytes);
	pass.SetVertexBuffer(4, mesh.bin, 0, mesh.binBytes);
	if (mesh.indexed) {
		pass.SetIndexBuffer(mesh.index, wgpu::IndexFormat::Uint32, 0, mesh.indexBytes);
	}
}

} // namespace

bool aquariumDawnPrepare(ofApp & app) {
	auto renderer = std::dynamic_pointer_cast<ofWebGPURenderer>(ofGetCurrentRenderer());
	if (!renderer || !renderer->getDevice()) {
		ofLogError("aquarium") << "dawn device missing";
		return false;
	}
	g.device = renderer->getDevice();
	g.queue = renderer->getQueue();

	const unsigned char whitePx[4] = {255, 255, 255, 255};
	g.whiteView = upload2D(std::vector<unsigned char>(whitePx, whitePx + 4), 1, 1);
	for (const auto & image : app.images) {
		const ofPixels & pix = image.second.getPixels();
		std::vector<unsigned char> rgba = toRGBA(pix);
		if (!rgba.empty()) {
			g.views[image.first] = upload2D(rgba, pix.getWidth(), pix.getHeight());
		}
	}

	const char * faces[6] = {
		"assets/GlobeOuter_EM_positive_x.jpg",
		"assets/GlobeOuter_EM_negative_x.jpg",
		"assets/GlobeOuter_EM_positive_y.jpg",
		"assets/GlobeOuter_EM_negative_y.jpg",
		"assets/GlobeOuter_EM_positive_z.jpg",
		"assets/GlobeOuter_EM_negative_z.jpg"
	};
	std::vector<std::vector<unsigned char>> faceRGBA(6);
	int cw = 0;
	int ch = 0;
	bool cubeOk = true;
	for (int i = 0; i < 6; ++i) {
		ofImage face;
		face.setUseTexture(false);
		if (!face.load(faces[i])) {
			cubeOk = false;
			break;
		}
		if (i == 0) {
			cw = face.getWidth();
			ch = face.getHeight();
		} else if (face.getWidth() != cw || face.getHeight() != ch) {
			cubeOk = false;
			break;
		}
		faceRGBA[i] = toRGBA(face.getPixels());
	}
	if (!cubeOk || cw <= 0 || ch <= 0) {
		cw = ch = 1;
		for (int i = 0; i < 6; ++i) {
			faceRGBA[i] = {255, 255, 255, 255};
		}
	}
	const int cubeMips = mipCountFor(cw, ch);
	wgpu::TextureDescriptor cubeDesc = {};
	cubeDesc.usage = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst;
	cubeDesc.dimension = wgpu::TextureDimension::e2D;
	cubeDesc.size = { static_cast<uint32_t>(cw), static_cast<uint32_t>(ch), 6 };
	cubeDesc.format = wgpu::TextureFormat::RGBA8Unorm;
	cubeDesc.mipLevelCount = static_cast<uint32_t>(cubeMips);
	g.cubeTex = g.device.CreateTexture(&cubeDesc);
	for (int face = 0; face < 6; ++face) {
		std::vector<unsigned char> level = faceRGBA[face];
		int lw = cw;
		int lh = ch;
		for (int mip = 0; mip < cubeMips; ++mip) {
			const uint32_t bpr = (static_cast<uint32_t>(lw) * 4u + 255u) & ~255u;
			std::vector<unsigned char> padded(static_cast<size_t>(bpr) * static_cast<size_t>(lh));
			for (int y = 0; y < lh; ++y) {
				std::memcpy(padded.data() + static_cast<size_t>(y) * bpr, level.data() + static_cast<size_t>(y) * lw * 4, static_cast<size_t>(lw) * 4);
			}
			wgpu::TexelCopyTextureInfo dst = {};
			dst.texture = g.cubeTex;
			dst.mipLevel = static_cast<uint32_t>(mip);
			dst.origin = { 0, 0, static_cast<uint32_t>(face) };
			dst.aspect = wgpu::TextureAspect::All;
			wgpu::TexelCopyBufferLayout layout = {};
			layout.bytesPerRow = bpr;
			layout.rowsPerImage = static_cast<uint32_t>(lh);
			wgpu::Extent3D size = { static_cast<uint32_t>(lw), static_cast<uint32_t>(lh), 1 };
			g.queue.WriteTexture(&dst, padded.data(), padded.size(), &layout, &size);
			if (mip + 1 < cubeMips) {
				int nw = 0;
				int nh = 0;
				level = downsample(level, lw, lh, nw, nh);
				lw = nw;
				lh = nh;
			}
		}
	}
	wgpu::TextureViewDescriptor cubeViewDesc = {};
	cubeViewDesc.dimension = wgpu::TextureViewDimension::Cube;
	cubeViewDesc.format = wgpu::TextureFormat::RGBA8Unorm;
	cubeViewDesc.mipLevelCount = static_cast<uint32_t>(cubeMips);
	cubeViewDesc.arrayLayerCount = 6;
	g.cubeView = g.cubeTex.CreateView(&cubeViewDesc);

	wgpu::SamplerDescriptor repeatDesc = {};
	repeatDesc.addressModeU = wgpu::AddressMode::Repeat;
	repeatDesc.addressModeV = wgpu::AddressMode::Repeat;
	repeatDesc.addressModeW = wgpu::AddressMode::Repeat;
	repeatDesc.magFilter = wgpu::FilterMode::Linear;
	repeatDesc.minFilter = wgpu::FilterMode::Linear;
	repeatDesc.mipmapFilter = wgpu::MipmapFilterMode::Linear;
	g.repeatSampler = g.device.CreateSampler(&repeatDesc);
	wgpu::SamplerDescriptor clampDesc = repeatDesc;
	clampDesc.addressModeU = wgpu::AddressMode::ClampToEdge;
	clampDesc.addressModeV = wgpu::AddressMode::ClampToEdge;
	clampDesc.addressModeW = wgpu::AddressMode::ClampToEdge;
	g.clampSampler = g.device.CreateSampler(&clampDesc);

	for (int slot = 0; slot < 3; ++slot) {
		g.uniform[slot] = makeBuffer(static_cast<uint64_t>(kSlots) * kStride, wgpu::BufferUsage::Uniform | wgpu::BufferUsage::CopyDst);
	}

	wgpu::BindGroupLayoutEntry layoutEntries[7] = {};
	layoutEntries[0].binding = 0;
	layoutEntries[0].visibility = wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment;
	layoutEntries[0].buffer.type = wgpu::BufferBindingType::Uniform;
	layoutEntries[0].buffer.hasDynamicOffset = true;
	layoutEntries[0].buffer.minBindingSize = kStride;
	for (int binding = 1; binding <= 3; ++binding) {
		layoutEntries[binding].binding = static_cast<uint32_t>(binding);
		layoutEntries[binding].visibility = wgpu::ShaderStage::Fragment;
		layoutEntries[binding].texture.sampleType = wgpu::TextureSampleType::Float;
		layoutEntries[binding].texture.viewDimension = wgpu::TextureViewDimension::e2D;
	}
	layoutEntries[4].binding = 4;
	layoutEntries[4].visibility = wgpu::ShaderStage::Fragment;
	layoutEntries[4].texture.sampleType = wgpu::TextureSampleType::Float;
	layoutEntries[4].texture.viewDimension = wgpu::TextureViewDimension::Cube;
	layoutEntries[5].binding = 5;
	layoutEntries[5].visibility = wgpu::ShaderStage::Fragment;
	layoutEntries[5].sampler.type = wgpu::SamplerBindingType::Filtering;
	layoutEntries[6].binding = 6;
	layoutEntries[6].visibility = wgpu::ShaderStage::Fragment;
	layoutEntries[6].sampler.type = wgpu::SamplerBindingType::Filtering;
	wgpu::BindGroupLayoutDescriptor layoutDesc = {};
	layoutDesc.entryCount = 7;
	layoutDesc.entries = layoutEntries;
	g.layout = g.device.CreateBindGroupLayout(&layoutDesc);

	g.pipeline[PipeLit] = makePipeline("vsStatic", "fsLit", PipeLit, false, false, true, wgpu::CullMode::Back);
	g.pipeline[PipeFish] = makePipeline("vsFish", "fsLit", PipeFish, true, false, true, wgpu::CullMode::Back);
	g.pipeline[PipeSeaweed] = makePipeline("vsSeaweed", "fsSeaweed", PipeSeaweed, true, false, true, wgpu::CullMode::Back);
	g.pipeline[PipeInner] = makePipeline("vsStatic", "fsInner", PipeInner, false, false, true, wgpu::CullMode::Back);
	g.pipeline[PipeOuter] = makePipeline("vsStatic", "fsOuter", PipeOuter, true, false, true, wgpu::CullMode::Back);
	g.pipeline[PipeRay] = makePipeline("vsTex", "fsTex", PipeRay, true, true, false, wgpu::CullMode::None);
	g.pipeline[PipeBubble] = makePipeline("vsBubble", "fsBubble", PipeBubble, true, false, false, wgpu::CullMode::None);

	int uploaded = 0;
	for (const ofApp::Scene & scene : app.scenes) {
		for (const ofApp::Model & model : scene.models) {
			if (uploadModel(model.mesh, model)) {
				++uploaded;
			}
		}
	}

	std::vector<float> rp, ru;
	const float corner[4][3] = {{-0.5f, 0.f, 0.f}, {0.5f, 0.f, 0.f}, {0.5f, 1.f, 0.f}, {-0.5f, 1.f, 0.f}};
	const float coord[4][2] = {{0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}};
	const int order[6] = {0, 1, 2, 0, 2, 3};
	for (int index : order) {
		rp.push_back(corner[index][0]);
		rp.push_back(corner[index][1]);
		rp.push_back(corner[index][2]);
		ru.push_back(coord[index][0]);
		ru.push_back(coord[index][1]);
	}
	g.ray.vertexCount = 6;
	g.ray.pos = uploadArray(rp, g.ray.posBytes);
	g.ray.uv = uploadArray(ru, g.ray.uvBytes);
	finishGroups(g.ray, viewNamed("LightRay.png"), g.whiteView, g.whiteView);
	finishGroups(g.bubble, viewNamed("bubble.png"), g.whiteView, g.whiteView);

	g.ready = g.pipeline[PipeLit] && g.pipeline[PipeFish] && uploaded > 0;
	std::cout << "aquarium dawn meshes=" << uploaded
			  << " textures=" << g.views.size()
			  << " pipelines=" << (g.ready ? "ok" : "failed") << std::endl;
	ofLogNotice("aquarium") << "dawn meshes=" << uploaded
							<< " textures=" << g.views.size()
							<< " pipelines=" << (g.ready ? "ok" : "failed");
	return g.ready;
}

void aquariumDawnDraw(ofApp & app) {
	if (!g.ready) {
		return;
	}
	auto renderer = std::dynamic_pointer_cast<ofWebGPURenderer>(ofGetCurrentRenderer());
	if (!renderer) {
		return;
	}
	wgpu::RenderPassEncoder pass = renderer->getRenderPass();
	if (!pass) {
		return;
	}
	const float width = static_cast<float>(std::max(ofGetWidth(), 1));
	const float height = static_cast<float>(std::max(ofGetHeight(), 1));
	pass.SetViewport(0.f, 0.f, width, height, 0.f, 1.f);

	std::vector<Item> items;
	items.reserve(static_cast<size_t>(app.fishTotal) + 128);
	auto drawGroup = [&](const std::string & group, float specular) {
		int index = 0;
		for (const ofApp::Object & obj : app.objects) {
			const ofApp::Scene & scene = app.scenes[obj.scene];
			if (scene.group != group) {
				continue;
			}
			int pipe = PipeLit;
			if (scene.program == "inner") {
				pipe = PipeInner;
			} else if (scene.program == "outer") {
				pipe = PipeOuter;
			} else if (scene.program == "seaweed") {
				pipe = PipeSeaweed;
			}
			const float fog = (app.optFog && scene.fog) ? 1.f : 0.f;
			for (const ofApp::Model & model : scene.models) {
				float useNormal = 0.f;
				float useReflect = 0.f;
				if (scene.program != "diffuse" && scene.program != "seaweed" && app.optNormal && !model.normalMap.empty()) {
					useNormal = 1.f;
				}
				if (scene.program == "auto" && app.optReflect && !model.reflectionMap.empty()) {
					useReflect = 1.f;
				}
				pushModel(items, app, model, obj.world, pipe, specular, useNormal, useReflect, 0.f, fog, app.clock + static_cast<float>(index));
			}
			++index;
		}
	};
	drawGroup("base", 1.f);

	app.resetPseudo();
	for (auto & kind : app.fish) {
		if (kind.num <= 0 || kind.scene < 0 || app.scenes[kind.scene].models.empty()) {
			continue;
		}
		const ofApp::Model & model = app.scenes[kind.scene].models[0];
		const float useNormal = (app.optNormal && !model.normalMap.empty()) ? 1.f : 0.f;
		const float useReflect = (app.optReflect && !model.reflectionMap.empty()) ? 1.f : 0.f;
		const float fog = app.optFog ? 1.f : 0.f;
		const float baseClock = app.clock * app.fishSpeed;
		const float tail = kind.tailSpeed * app.fishTailSpeed;
		const float height = app.fishHeight + kind.heightOffset;
		const float heightRange = app.fishHeightRange * kind.heightRange;
		for (int i = 0; i < kind.num; ++i) {
			const float fishClock = baseClock + static_cast<float>(i) * app.fishOffset;
			const float speed = kind.speed + app.pseudo() * kind.speedRange;
			const float scale = 1.f + app.pseudo();
			const float xRadius = kind.radius + app.pseudo() * kind.radiusRange;
			const float yRadius = 2.f + app.pseudo() * heightRange;
			const float zRadius = kind.radius + app.pseudo() * kind.radiusRange;
			const float speedClock = fishClock * speed;
			const float px = std::sin(speedClock * app.fishXClock) * xRadius;
			const float py = std::sin(speedClock * app.fishYClock) * yRadius + height;
			const float pz = std::cos(speedClock * app.fishZClock) * zRadius;
			const float nx = std::sin(speedClock * app.fishXClock - 0.04f) * xRadius;
			const float ny = std::sin(speedClock * app.fishYClock - 0.01f) * yRadius + height;
			const float nz = std::cos(speedClock * app.fishZClock - 0.04f) * zRadius;
			const float time = std::fmod((app.clock + static_cast<float>(i)) * tail * speed, static_cast<float>(PI) * 2.f);
			auto found = g.meshes.find(&model.mesh);
			if (found == g.meshes.end()) {
				continue;
			}
			Item item;
			item.pipe = PipeFish;
			item.mesh = &found->second;
			fillShared(item.u, app, app.viewProjection);
			write1(item.u, 84, 5.f);
			write1(item.u, 85, 0.3f);
			write1(item.u, 86, useNormal);
			write1(item.u, 87, useReflect);
			write1(item.u, 88, 1.f);
			write1(item.u, 89, fog);
			write1(item.u, 96, time);
			write1(item.u, 97, scale);
			write1(item.u, 98, kind.length);
			write1(item.u, 99, kind.wave);
			write1(item.u, 100, kind.bend);
			write4(item.u, 108, px, py, pz, 1.f);
			write4(item.u, 112, nx, ny, nz, 1.f);
			items.push_back(item);
		}
	}
	if (app.optTank) {
		drawGroup("inner", 1.f);
	}
	drawGroup("seaweed", 1.f);
	if (app.optMuseum) {
		drawGroup("outside", 0.f);
	}
	if (app.optBubbles && !app.bubbles.empty() && g.bubble.group[0]) {
		float right[3];
		float upv[3];
		aq::axis(right, app.viewInverse, 0);
		aq::axis(upv, app.viewInverse, 1);
		const float quad[6][2] = {{-1, -1}, {1, -1}, {1, 1}, {-1, -1}, {1, 1}, {-1, 1}};
		const float quv[6][2] = {{0, 0}, {1, 0}, {1, 1}, {0, 0}, {1, 1}, {0, 1}};
		std::vector<float> pos, uv, col;
		pos.reserve(app.bubbles.size() * 18);
		uv.reserve(app.bubbles.size() * 12);
		col.reserve(app.bubbles.size() * 24);
		for (const ofApp::Bubble & bubble : app.bubbles) {
			const float t = bubble.age;
			const float cx = bubble.x + bubble.vx * t;
			const float cy = bubble.y + bubble.vy * t + 0.5f * bubble.ay * t * t;
			const float cz = bubble.z + bubble.vz * t;
			const float u = std::min(1.f, t / bubble.life);
			const float size = bubble.size0 + (bubble.size1 - bubble.size0) * u;
			const float alpha = u < 0.8f ? 1.f : (1.f - u) / 0.2f;
			const float half = size * 0.5f;
			for (int v = 0; v < 6; ++v) {
				pos.push_back(cx + right[0] * quad[v][0] * half + upv[0] * quad[v][1] * half);
				pos.push_back(cy + right[1] * quad[v][0] * half + upv[1] * quad[v][1] * half);
				pos.push_back(cz + right[2] * quad[v][0] * half + upv[2] * quad[v][1] * half);
				uv.push_back(quv[v][0]);
				uv.push_back(quv[v][1]);
				col.push_back(0.7f);
				col.push_back(0.8f);
				col.push_back(1.f);
				col.push_back(alpha);
			}
		}
		const uint64_t posBytes = pos.size() * sizeof(float);
		const uint64_t uvBytes = uv.size() * sizeof(float);
		const uint64_t colBytes = col.size() * sizeof(float);
		if (g.bubble.posBytes < posBytes) {
			g.bubble.pos = makeBuffer(posBytes, wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst);
			g.bubble.posBytes = g.bubble.pos.GetSize();
		}
		if (g.bubble.uvBytes < uvBytes) {
			g.bubble.uv = makeBuffer(uvBytes, wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst);
			g.bubble.uvBytes = g.bubble.uv.GetSize();
		}
		if (g.bubble.colorBytes < colBytes) {
			g.bubble.color = makeBuffer(colBytes, wgpu::BufferUsage::Vertex | wgpu::BufferUsage::CopyDst);
			g.bubble.colorBytes = g.bubble.color.GetSize();
		}
		writeBuffer(g.bubble.pos, pos.data(), posBytes);
		writeBuffer(g.bubble.uv, uv.data(), uvBytes);
		writeBuffer(g.bubble.color, col.data(), colBytes);
		g.bubble.vertexCount = static_cast<uint32_t>(pos.size() / 3);
		Item item;
		item.pipe = PipeBubble;
		item.mesh = &g.bubble;
		fillShared(item.u, app, app.viewProjection);
		items.push_back(item);
	}
	if (app.optRays && g.ray.vertexCount > 0) {
		for (const ofApp::Ray & ray : app.rays) {
			const float lerp = ray.duration > 0.f ? ray.timer / ray.duration : 0.f;
			const float y = std::max(70.f, std::min(120.f, 50.f + app.eyeHeight));
			aq::Mat4 spin, shift, placed, scale, world;
			aq::rotationZ(spin, ray.rot + lerp * 0.2f);
			aq::translation(shift, ray.x, y, 0.f);
			aq::mul(placed, spin, shift);
			aq::scaling(scale, 10.f, -100.f, 10.f);
			aq::mul(world, scale, placed);
			Item item;
			item.pipe = PipeRay;
			item.mesh = &g.ray;
			fillShared(item.u, app, app.viewProjectionNoRot);
			writeMat(item.u, 16, world);
			const float alpha = std::sin(lerp * static_cast<float>(PI));
			write4(item.u, 104, 1.f, 1.f, 1.f, alpha);
			items.push_back(item);
		}
	}
	if (app.optTank) {
		drawGroup("outer", 1.f);
	}

	if (items.size() > kSlots) {
		items.resize(kSlots);
	}
	const int slot = g.ring;
	g.ring = (g.ring + 1) % 3;
	std::vector<unsigned char> bytes(items.size() * kStride);
	for (size_t i = 0; i < items.size(); ++i) {
		std::memcpy(bytes.data() + i * kStride, items[i].u, kStride);
	}
	if (!bytes.empty()) {
		g.queue.WriteBuffer(g.uniform[slot], 0, bytes.data(), bytes.size());
	}
	int currentPipe = -1;
	const GpuMesh * currentMesh = nullptr;
	for (size_t i = 0; i < items.size(); ++i) {
		const Item & item = items[i];
		if (!item.mesh || !g.pipeline[item.pipe] || !item.mesh->group[slot]) {
			continue;
		}
		if (currentPipe != item.pipe) {
			pass.SetPipeline(g.pipeline[item.pipe]);
			currentPipe = item.pipe;
			currentMesh = nullptr;
		}
		if (currentMesh != item.mesh) {
			bindMesh(pass, *item.mesh, item.pipe);
			currentMesh = item.mesh;
		}
		uint32_t offset = static_cast<uint32_t>(i * kStride);
		pass.SetBindGroup(0, item.mesh->group[slot], 1, &offset);
		if (item.mesh->indexed) {
			pass.DrawIndexed(item.mesh->indexCount);
		} else {
			pass.Draw(item.mesh->vertexCount);
		}
	}
	if (!g.loggedDraw) {
		g.loggedDraw = true;
		std::cout << "aquarium dawn draws=" << items.size() << std::endl;
		ofLogNotice("aquarium") << "dawn draws=" << items.size();
	}
}

#endif
