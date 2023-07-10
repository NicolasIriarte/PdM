add_executable(${PROJECT_NAME} main.cpp
  ${EXTRA_SOURCES_FILES})



target_link_options(${PROJECT_NAME} PUBLIC
  -T ${CMAKE_CURRENT_SOURCE_DIR}/STM32F429ZITX_FLASH.ld
  -T ${CMAKE_CURRENT_SOURCE_DIR}/STM32F429ZITX_RAM.ld)

message(STATUS "Extra include paths: ${EXTRA_INCLUDE_PATH}")
message(STATUS "Extra source files:" ${EXTRA_SOURCES_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  ${EXTRA_INCLUDE_PATH}
)

target_compile_options(${PROJECT_NAME} PUBLIC
  -mcpu=cortex-m4 -Wl,-Map="PW1_Exercise_1.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group)

# target_link_libraries(${PROJECT_NAME})

