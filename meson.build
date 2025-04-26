project('andoe-framework', 'cpp', default_options: ['cpp_std=c++17'], version: '0.1')

compiler = meson.get_compiler('cpp')
ws2_32 = compiler.find_library('ws2_32', required: true)

inc_framework = include_directories('include/andoe')
inc_app = include_directories('include')

src_framework = [
  './src/server.cpp',
  './src/framework.cpp',
  './src/socket.cpp'
]

headers = files(
  'include/andoe/server.hpp',
  'include/andoe/framework.hpp',
  'include/andoe/socket.hpp'
)

server_lib = static_library('andoe-framework', src_framework, include_directories: inc_framework, install: true)

install_headers(headers, subdir: 'uwu')

subdir('app')
