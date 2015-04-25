cmake_minimum_required(VERSION 2.6)

project(rasterizer)

add_executable (rasterizer 
  Color.h
  evector.h
  FrameBuffer.h
  Image_png.h
  Light.h
  Material.h
  mathematics.h
  matrix.h
  pngconf.h
  png.h
  pnglibconf.h
  Terrain.h
  VertexBuffer.h
  zlib.h

  Color.cpp
  evector.cpp
  Image_png.cpp
  Material.cpp
  mathematics.cpp
  matrix4.cpp
  matrix.cpp
  Projection.cpp
  Terrain.cpp
  VertexBuffer.cpp)


find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries(rasterizer ${ZLIB_LIBRARIES})
endif(ZLIB_FOUND)


find_package(PNG 1.6 REQUIRED)
if (PNG_FOUND)
  include_directories(${PNG_INCLUDE_DIR})
  target_link_libraries(rasterizer ${PNG_LIBRARY})
endif(PNG_FOUND)
