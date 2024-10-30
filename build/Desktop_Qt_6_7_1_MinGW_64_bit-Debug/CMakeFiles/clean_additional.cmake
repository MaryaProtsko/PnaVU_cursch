# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\CWLibrary_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CWLibrary_autogen.dir\\ParseCache.txt"
  "CWLibrary_autogen"
  )
endif()
