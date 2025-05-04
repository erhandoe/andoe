project('andoe-framework', 'cpp', default_options: ['cpp_std=c++17'], version: '0.1')

compiler = meson.get_compiler('cpp')
ws2_32 = compiler.find_library('ws2_32', required: true)

inc_framework = include_directories('include/andoe')
inc_app = include_directories('include')

src_framework = [
  './src/server.cpp',
  './src/framework.cpp',
  './src/socket.cpp',
  './src/router.cpp',
  './src/response.cpp',
  './src/request.cpp',
  './src/thread_pool.cpp',
  './src/htmlElement.cpp',
  './src/static.cpp'
]

headers = files(
  'include/andoe/server.hpp',
  'include/andoe/framework.hpp',
  'include/andoe/socket.hpp',
  'include/andoe/async_model.hpp',
  'include/andoe/router.hpp',
  'include/andoe/http_method.hpp',
  'include/andoe/request.hpp',
  'include/andoe/response.hpp',
  'include/andoe/thread_pool.hpp',
  'include/andoe/htmlElement.hpp',
  'include/andoe/andoe.hpp',
  'include/andoe/htmlTagFactories.hpp',
  'include/andoe/static.hpp'
)

server_lib = static_library('andoe-framework', src_framework, include_directories: inc_framework, install: true)

install_headers(headers, subdir: 'andoe')

subdir('app')
