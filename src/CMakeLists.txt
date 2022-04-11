set(EXE_SOURCES
  main.cpp
  )

set(LIB_SOURCES
  gstmark.c
  core.cpp
  )

include_directories(
  ../include
  )

add_library (${CORE_LIBRARY} SHARED
  ${LIB_SOURCES}
  )
 
#building target executable
add_executable(${PROJECT_NAME} ${EXE_SOURCES})

#linking Gstreamer library with target executable
target_link_libraries(${PROJECT_NAME} 
  ${GLIB_LIBRARIES}
  ${GSTREAMER_LIBRARIES}
  ${CORE_LIBRARY}
  )
