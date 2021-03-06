# ******************************************************************************
# Copyright 2017-2020 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ******************************************************************************

set(TARGET_NAME conditional_compilation)

add_library(${TARGET_NAME} INTERFACE)

add_library(openvino::conditional_compilation ALIAS ${TARGET_NAME})

target_link_libraries(${TARGET_NAME} INTERFACE openvino::itt)

target_include_directories(${TARGET_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(SELECTIVE_BUILD STREQUAL "COLLECT")
    target_compile_definitions(${TARGET_NAME} INTERFACE SELECTIVE_BUILD_ANALYZER)
elseif(SELECTIVE_BUILD STREQUAL "ON")
    if(NOT DEFINED SELECTIVE_BUILD_STAT)
        message(FATAL_ERROR "In case SELECTIVE_BUILD is enabled, the SELECTIVE_BUILD_STAT variable should contain the path to the collected InelSEAPI statistics.\
 Usage: -DSELECTIVE_BUILD=ON -DSELECTIVE_BUILD_STAT=/path/*.csv")
    endif()

    file(GLOB STAT_FILES ${SELECTIVE_BUILD_STAT})

    target_compile_definitions(${TARGET_NAME} INTERFACE SELECTIVE_BUILD)

    set(GENERATED_HEADER ${CMAKE_CURRENT_BINARY_DIR}/conditional_compilation_gen.h)
    set(GENERATOR ${CMAKE_CURRENT_SOURCE_DIR}/scripts/ccheader.py)

    add_custom_command(OUTPUT ${GENERATED_HEADER}
                       COMMAND python3 ${GENERATOR} --stat ${SELECTIVE_BUILD_STAT} --out ${GENERATED_HEADER}
                       DEPENDS ${STAT_FILES})
    add_custom_target(conditional_compilation_gen DEPENDS ${GENERATED_HEADER})
    add_dependencies(${TARGET_NAME} conditional_compilation_gen)

    ov_force_include(${TARGET_NAME} INTERFACE ${GENERATED_HEADER})
endif()

file(GLOB_RECURSE hdrs "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
add_cpplint_target(${TARGET_NAME}_cpplint FOR_SOURCES ${hdrs})
