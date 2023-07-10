# set(
#   PROJECT_EXTRA_SRC
#   ${PROJECT_EXTRA_SRC}
#   Src/stm32f4xx_it.c
#   Src/syscalls.c
#   Src/sysmem.c)

file(GLOB_RECURSE EXTRA_SOURCES_FILES "STM32F4xx_Nucleo_144/*.c")

list(APPEND EXTRA_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/STM32F4xx_Nucleo_144")

# target_include_directories(${PROJECT_NAME} PRIVATE
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/STM32F4xx_Nucleo_144>
# )
