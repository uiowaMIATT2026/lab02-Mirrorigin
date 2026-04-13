set(EXE_NAME RegisterCircles)

add_executable(${EXE_NAME} RegisterCircles.cxx)

target_link_libraries(${EXE_NAME} ${ITK_LIBRARIES})

set_target_properties(${EXE_NAME} PROPERTIES OUTPUT_NAME "${MYHAWKID}_RegisterCircles")