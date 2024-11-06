# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Demo_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Demo_autogen.dir\\ParseCache.txt"
  "Demo_autogen"
  )
endif()
