cmake_minimum_required(VERSION 3.16)
project(libparserutils)

set(LIBPARSERUTILS_SOURCES
    source/src/charset/codecs/8859_tables.h
    source/src/charset/codecs/codec_8859.c
    source/src/charset/codecs/codec_ascii.c
    source/src/charset/codecs/codec_ext8.c
    source/src/charset/codecs/codec_impl.h
    source/src/charset/codecs/codec_utf8.c
    source/src/charset/codecs/codec_utf16.c
    source/src/charset/codecs/ext8_tables.h
    source/src/charset/encodings/utf8.c
    source/src/charset/encodings/utf8impl.h
    source/src/charset/encodings/utf16.c
    source/src/charset/aliases.c
    source/src/charset/aliases.h
    source/src/charset/codec.c
    source/src/input/filter.c
    source/src/input/filter.h
    source/src/input/inputstream.c
    source/src/utils/buffer.c
    source/src/utils/endian.h
    source/src/utils/errors.c
    source/src/utils/stack.c
    source/src/utils/utils.h
    source/src/utils/vector.c
)
    

add_library(libparserutils STATIC ${LIBPARSERUTILS_SOURCES})
add_library(libparserutils::libparserutils ALIAS libparserutils)
target_include_directories(libparserutils PUBLIC source/include)
target_include_directories(libparserutils PUBLIC source/src)
target_compile_features(libparserutils PUBLIC c_std_99)
