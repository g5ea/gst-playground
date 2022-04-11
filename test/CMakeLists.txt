set(SOURCES
  test_main.cpp
  test_parser.cpp
  test_pipeline.cpp
  )

include_directories(
  ../include
  ./lib/Catch2/single_include/catch2
  )

#building target executable
add_executable(test-${PROJECT_NAME} ${SOURCES})

#linking Gstreamer library with target executable
target_link_libraries(test-${PROJECT_NAME} 
  ${GLIB_LIBRARIES}
  ${GSTREAMER_LIBRARIES}
  ${CORE_LIBRARY}
  )
