project('libtess2', 'c',
version : '0.12.0',
default_options : [
  'buildtype=release',
  'optimization=3',
  'default_library=static',
  'prefer_static=true',
  'b_ndebug=if-release',
  'c_args=-pipe',
  'cpp_args=-pipe',
]
)

inc = include_directories(
  'Include',
  'Source'
)

install_headers('Include/tesselator.h')

sources = files(
  'Source/bucketalloc.c',
  'Source/dict.c',
  'Source/geom.c',
  'Source/mesh.c',
  'Source/priorityq.c',
  'Source/sweep.c',
  'Source/tess.c',
)

lib = library('tess2',
  sources,
  include_directories: inc,
  install: true)

libtess2_dep = declare_dependency(include_directories: 'Include', link_with: lib)

# This is to make meson projects using this one as subproject find the dependency
# automagically when doing "dependency('libtess2')"
meson.override_dependency('libtess2', libtess2_dep)

pkg = import('pkgconfig')
pkg.generate(lib)
