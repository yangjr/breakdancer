project(breakdancer-max)

set(SOURCES
    BreakDancerMax.cpp
    BreakDancerMax.h
)

set(EXECUTABLE_NAME breakdancer-max)
add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_link_libraries(${EXECUTABLE_NAME} breakdancer)
set_target_properties(${EXECUTABLE_NAME} PROPERTIES PACKAGE_OUTPUT_NAME ${EXECUTABLE_NAME}${EXE_VERSION_SUFFIX})
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin/)
