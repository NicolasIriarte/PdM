# set(
#   PROJECT_EXTRA_SRC
#   ${PROJECT_EXTRA_SRC}
#   Src/stm32f4xx_it.c
#   Src/syscalls.c
#   Src/sysmem.c)

file(GLOB_RECURSE EXTRA_SOURCES_FILES "Src/*.c")

set(PROJECT_EXTRA_INC_PATHS ${PROJECT_EXTRA_INC_PATHS}
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc)
