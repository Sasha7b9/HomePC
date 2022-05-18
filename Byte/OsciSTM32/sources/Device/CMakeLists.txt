project (Device)

include_directories(../common/MDK_KEIL/CMSIS/
                    src
                    src/Hardware/HAL
                    ../common/stm32CubeF7/Drivers/STM32F7xx_HAL_Driver/Inc/
                    ../common/stm32CubeF7/Drivers/CMSIS/Device/ST/STM32F7xx/Include/
                    ../common/stm32CubeF7/Drivers/CMSIS/Include/
)

add_definitions(-DSTM32F746xx -DDEVICE -DVS_KEIL)

add_compile_options(/wd4100)
add_compile_options(/wd4242)
add_compile_options(/wd4244)
add_compile_options(/wd4365)
add_compile_options(/wd4459)
add_compile_options(/wd4464)
add_compile_options(/wd4514)
add_compile_options(/wd4820)
add_compile_options(/wd5045)
add_compile_options(/wd5219)
add_compile_options(/wd5220)
add_compile_options(/wd26812)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")


file(GLOB SRC            src/*.*)
file(GLOB DISPLAY        src/Display/*.*)
file(GLOB DISPLAY_FONT   src/Display/Font/*.*)
file(GLOB HARDWARE       src/Hardware/*.*)
file(GLOB HARDWARE_HAL   src/Hardware/HAL/*.*)
file(GLOB HARDWARE_TOUCH src/Hardware/Touch/*.*)
file(GLOB LIBS_HAL       ../common/stm32CubeF7/Drivers/STM32F7xx_HAL_Driver/Src/*.*
                         ../common/stm32CubeF7/Drivers/STM32F7xx_HAL_Driver/Inc/*.*)

source_group(src            FILES ${SRC})
source_group(Display        FILES ${DISPLAY})
source_group(Display/Font   FILES ${DISPLAY_FONT})
source_group(Hardware       FILES ${HARDWARE})
source_group(Hardware/HAL   FILES ${HARDWARE_HAL})
source_group(Hardware/Touch FILES ${HARDWARE_TOUCH})
source_group(libs/HAL       FILES ${LIBS_HAL})

set_source_files_properties(${LIBS_HAL} PROPERTIES HEADER_FILE_ONLY TRUE)

add_library(${PROJECT_NAME} ${SRC}
                            ${DISPLAY} ${DISPLAY_FONT}
                            ${HARDWARE} ${HARDWARE_HAL} ${HARDWARE_TOUCH}
                            ${LIBS_HAL}
)
