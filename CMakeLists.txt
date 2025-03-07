# CMake 4 no longer supports compatibility modes below 3.10.
cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Several examples contain UTF-8 comments.  Make MSVC interpret their source
# files as UTF-8 instead of using the machine's legacy Windows code page.
if(MSVC)
  add_compile_options(/utf-8)
endif()

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/lib)

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
# find_package(SOIL REQUIRED)
# message(STATUS "Found SOIL in ${SOIL_INCLUDE_DIR}")
# find_package(GLEW REQUIRED)
# message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")
if(APPLE)
  find_package(Freetype REQUIRED)
  message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIRS}")
endif(APPLE)

if(WIN32)
  set(LIBS glfw3 opengl32 assimp freetype irrKlang)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(UNIX AND NOT APPLE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  find_package(OpenGL REQUIRED)
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)
  # note that the order is important for setting the libs
  # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
  set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARY})
  set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
  INCLUDE_DIRECTORIES(/System/Library/Frameworks)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
  FIND_LIBRARY(IOKit_LIBRARY IOKit)
  FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
  MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
  SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
  SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES})
  set(LIBS ${LIBS} ${APPLE_LIBS})
else()
  set(LIBS )
endif(WIN32)

set(CHAPTERS
    1.getting_started
    2.lighting
    3.model_loading
    4.advanced_opengl
    5.advanced_lighting
    6.pbr
    7.in_practice
)

set(1.getting_started
    1.1.hello_window
    1.2.hello_window_clear
    2.1.hello_triangle
    2.2.hello_triangle_indexed
    2.3.hello_triangle_exercise1
    2.4.hello_triangle_exercise2
    2.5.hello_triangle_exercise3
    3.1.shaders_uniform
    3.2.shaders_interpolation
    3.3.shaders_class
    4.1.textures
    4.2.textures_combined
    4.4.textures_exercise2
    4.5.textures_exercise3
    4.6.textures_exercise4
    5.1.transformations
    5.2.transformations_exercise2
    6.1.coordinate_systems
    6.2.coordinate_systems_depth
    6.3.coordinate_systems_multiple
    7.1.camera_circle
    7.2.camera_keyboard_dt
    7.3.camera_mouse_zoom
    7.4.camera_class
)

set(2.lighting
    1.colors
    2.1.basic_lighting_diffuse
    2.2.basic_lighting_specular
    3.1.materials
    3.2.materials_exercise1
    4.1.lighting_maps_diffuse_map
    4.2.lighting_maps_specular_map
    4.4.lighting_maps_exercise4
    5.1.light_casters_directional
    5.2.light_casters_point
    5.3.light_casters_spot
    5.4.light_casters_spot_soft
    6.multiple_lights
)

set(3.model_loading
    1.model_loading
)

set(4.advanced_opengl
    1.1.depth_testing
    1.2.depth_testing_view
    2.stencil_testing
    3.1.blending_discard
    3.2.blending_sort
    5.1.framebuffers
    5.2.framebuffers_exercise1
    6.1.cubemaps_skybox
    6.2.cubemaps_environment_mapping
    8.advanced_glsl_ubo
    9.1.geometry_shader_houses
    9.2.geometry_shader_exploding
    9.3.geometry_shader_normals
    10.1.instancing_quads
    10.2.asteroids
    10.3.asteroids_instanced
    11.1.anti_aliasing_msaa
    11.2.anti_aliasing_offscreen
)

set(5.advanced_lighting
    1.advanced_lighting
    2.gamma_correction
    3.1.1.shadow_mapping_depth
    3.1.2.shadow_mapping_base
    3.1.3.shadow_mapping
    3.2.1.point_shadows
    3.2.2.point_shadows_soft
    4.normal_mapping
    5.1.parallax_mapping
    5.2.steep_parallax_mapping
    5.3.parallax_occlusion_mapping
    6.hdr
    7.bloom
    8.1.deferred_shading
    8.2.deferred_shading_volumes
    9.ssao
)

set(6.pbr
    1.1.lighting
    1.2.lighting_textured
    2.1.1.ibl_irradiance_conversion
    2.1.2.ibl_irradiance
    2.2.1.ibl_specular
    2.2.2.ibl_specular_textured
)

set(7.in_practice
    1.debugging
    2.text_rendering
    #3.2d_game
)

set(GUEST_ARTICLES
	8.guest/2020/oit
	8.guest/2020/skeletal_animation
	8.guest/2021/1.scene/1.scene_graph
	8.guest/2021/1.scene/2.frustum_culling
	8.guest/2021/2.csm
	8.guest/2021/3.tessellation/terrain_gpu_dist
	8.guest/2021/3.tessellation/terrain_cpu_src
	8.guest/2021/4.dsa
	8.guest/2022/5.computeshader_helloworld
	8.guest/2022/6.physically_based_bloom
	8.guest/2022/7.area_lights/1.area_light
	8.guest/2022/7.area_lights/2.multiple_area_lights
)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# first create relevant static libraries required for other projects
add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

macro(makeLink src dest target)
  add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  DEPENDS  ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()

function(create_project_from_sources chapter demo)
	file(GLOB SOURCE
            "src/${chapter}/${demo}/*.h"
            "src/${chapter}/${demo}/*.cpp"
            "src/${chapter}/${demo}/*.vs"
            "src/${chapter}/${demo}/*.fs"
            "src/${chapter}/${demo}/*.tcs"
            "src/${chapter}/${demo}/*.tes"
            "src/${chapter}/${demo}/*.gs"
            "src/${chapter}/${demo}/*.cs"
    )
	if (demo STREQUAL "")
		SET(replaced "")
		string(REPLACE "/" "_" replaced ${chapter})
		set(NAME ${replaced})
	else()
		set(NAME "${chapter}__${demo}")
	endif()
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} ${LIBS})
    if(MSVC)
		target_compile_options(${NAME} PRIVATE /std:c++17 /MP)
        target_link_options(${NAME} PUBLIC /ignore:4099)
    endif(MSVC)
    if(WIN32)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
        set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}/Debug")
    elseif(UNIX AND NOT APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
    elseif(APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${chapter}")
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${chapter}")
    endif(WIN32)
    # copy shader files to build directory
    file(GLOB SHADERS
             "src/${chapter}/${demo}/*.vs"
             # "src/${chapter}/${demo}/*.frag"
             "src/${chapter}/${demo}/*.fs"
             "src/${chapter}/${demo}/*.tcs"
             "src/${chapter}/${demo}/*.tes"
             "src/${chapter}/${demo}/*.gs"
             "src/${chapter}/${demo}/*.cs"
    )
	# copy dlls
	file(GLOB DLLS "dlls/*.dll")
    foreach(SHADER ${SHADERS})
        if(WIN32)
            # configure_file(${SHADER} "test")
            add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
			add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLLS} $<TARGET_FILE_DIR:${NAME}>)
        elseif(UNIX AND NOT APPLE)
            file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/${chapter})
        elseif(APPLE)
            # create symbolic link for *.vs *.fs *.gs
            get_filename_component(SHADERNAME ${SHADER} NAME)
            makeLink(${SHADER} ${CMAKE_SOURCE_DIR}/bin/${chapter}/${SHADERNAME} ${NAME})
        endif(WIN32)
    endforeach(SHADER)
    # if compiling for visual studio, also use configure file for each project (specifically to set up working directory)
    if(MSVC)
        configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.vcxproj.user @ONLY)
    endif(MSVC)
endfunction()

# then create a project file per tutorial
foreach(CHAPTER ${CHAPTERS})
    foreach(DEMO ${${CHAPTER}})
		create_project_from_sources(${CHAPTER} ${DEMO})
    endforeach(DEMO)
endforeach(CHAPTER)
foreach(GUEST_ARTICLE ${GUEST_ARTICLES})
	create_project_from_sources(${GUEST_ARTICLE} "")
endforeach(GUEST_ARTICLE)

# This locally extended guest example draws the coordinate axes via
# dean/includes/utils.h. Keep that personal include path target-local.
target_include_directories(8.guest_2020_skeletal_animation PRIVATE
    "${CMAKE_SOURCE_DIR}/dean/includes")

include_directories(${CMAKE_SOURCE_DIR}/includes)

# Personal experiments are kept outside the upstream tutorial tree. Register
# each one explicitly so unfinished dean examples are never built by accident.
file(GLOB DEAN_DLLS "${CMAKE_SOURCE_DIR}/dlls/*.dll")
function(add_dean_example target source_dir source_file)
    add_executable(${target} "${source_dir}/${source_file}")
    target_include_directories(${target} PRIVATE "${CMAKE_SOURCE_DIR}/dean/includes")
    target_link_libraries(${target} ${LIBS})
    if(MSVC)
        target_compile_options(${target} PRIVATE /std:c++17 /MP)
        target_link_options(${target} PUBLIC /ignore:4099)
    endif()
    string(REGEX REPLACE "__.*$" "" dean_chapter "${target}")
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/dean/${dean_chapter}")

    file(GLOB shaders
        "${source_dir}/*.vs" "${source_dir}/*.fs" "${source_dir}/*.gs"
        "${source_dir}/*.tcs" "${source_dir}/*.tes" "${source_dir}/*.cs")
    foreach(shader ${shaders})
        add_custom_command(TARGET ${target} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${shader} $<TARGET_FILE_DIR:${target}>)
    endforeach()
    if(WIN32)
        add_custom_command(TARGET ${target} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DEAN_DLLS} $<TARGET_FILE_DIR:${target}>)
    endif()
endfunction()

set(DEAN_EXAMPLES_DIR "${CMAKE_SOURCE_DIR}/dean/src/0.examples")
add_dean_example(0.examples__1.draw_circle
    "${DEAN_EXAMPLES_DIR}/1.draw_circle" draw_circle.cpp)
add_dean_example(0.examples__2.access_neighboring_texel
    "${DEAN_EXAMPLES_DIR}/2.access_neighboring_texel" access_neighboring_texel.cpp)
add_dean_example(0.examples__3.pyramid_texturing
    "${DEAN_EXAMPLES_DIR}/3.pyramid_texturing" pyramid_texturing.cpp)
add_dean_example(0.examples__4.draw_point
    "${DEAN_EXAMPLES_DIR}/4.draw_point" draw_point.cpp)
add_dean_example(0.examples__5.use_utils
    "${DEAN_EXAMPLES_DIR}/5.use_utils" use_utils.cpp)

# Examples 6 and 7 read camera frames through a local OpenCV build. They are
# opt-in so the rest of the project configures without OpenCV installed.
option(DEAN_BUILD_OPENCV_EXAMPLES "Build dean examples 6 and 7" OFF)
if(DEAN_BUILD_OPENCV_EXAMPLES)
set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/dean/3rdparty/opencv-msvc/x64/vc17/lib")
find_package(OpenCV 4.12 REQUIRED COMPONENTS core videoio)
set(DEAN_OPENCV_DLL_DIR
    "${CMAKE_SOURCE_DIR}/dean/3rdparty/opencv-msvc/x64/vc17/bin")
file(GLOB DEAN_OPENCV_DLLS "${DEAN_OPENCV_DLL_DIR}/*.dll")

add_dean_example(0.examples__6.fisheye_filter
    "${DEAN_EXAMPLES_DIR}/6.fisheye_filter" fisheye_filter.cpp)
add_dean_example(0.examples__7.dof_filter
    "${DEAN_EXAMPLES_DIR}/7.dof_filter" dof_filter.cpp)
foreach(opencv_target
        0.examples__6.fisheye_filter
        0.examples__7.dof_filter)
    target_include_directories(${opencv_target} PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${opencv_target} ${OpenCV_LIBS})
    if(WIN32)
        add_custom_command(TARGET ${opencv_target} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${DEAN_OPENCV_DLLS} $<TARGET_FILE_DIR:${opencv_target}>)
    endif()
endforeach()
endif()

# glPong is a multi-file application with a bundled Dear ImGui copy and its
# own STB_IMAGE_IMPLEMENTATION in program.cpp.
# ported from https://github.com/nephewtom/glpong/tree/main
#   commented out all the irrklang code in game.cpp
#   commented out the "TASK_DO_FOR(blink, 0.5f)" in line 289 in game.cpp
#   fixed the "not may not appear in macro parameter list" error in game.h
set(GLPONG_DIR "${DEAN_EXAMPLES_DIR}/8.glpong")
set(GLPONG_GLFW_DIR
    "${CMAKE_SOURCE_DIR}/dean/3rdparty/glfw-3.3.8.bin.WIN64")
file(GLOB GLPONG_SOURCES
    "${GLPONG_DIR}/src/*.cpp"
    "${GLPONG_DIR}/src/util/*.cpp"
    "${GLPONG_DIR}/external/imgui/*.cpp")
set(GLPONG_TARGET 0.examples__8.glpong)
add_executable(${GLPONG_TARGET} ${GLPONG_SOURCES})
target_include_directories(${GLPONG_TARGET} PRIVATE
    "${GLPONG_DIR}/src"
    "${GLPONG_DIR}/src/util"
    "${GLPONG_DIR}/external/imgui")
target_include_directories(${GLPONG_TARGET} BEFORE PRIVATE
    "${GLPONG_GLFW_DIR}/include")
target_compile_definitions(${GLPONG_TARGET} PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_GLAD
    GLFW_EXPOSE_NATIVE_WGL)
target_link_libraries(${GLPONG_TARGET}
    "${GLPONG_GLFW_DIR}/glfw3dll.lib"
    opengl32 freetype irrKlang GLAD)
if(MSVC)
    target_compile_options(${GLPONG_TARGET} PRIVATE /std:c++17 /MP)
    target_link_options(${GLPONG_TARGET} PUBLIC /ignore:4099)
endif()
set_target_properties(${GLPONG_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/dean/0.examples"
    VS_DEBUGGER_WORKING_DIRECTORY
        "${CMAKE_SOURCE_DIR}/bin/dean/0.examples/Debug")
foreach(resource_dir shaders textures fonts)
    add_custom_command(TARGET ${GLPONG_TARGET} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${GLPONG_DIR}/${resource_dir}"
            "$<TARGET_FILE_DIR:${GLPONG_TARGET}>/${resource_dir}")
endforeach()
if(WIN32)
    add_custom_command(TARGET ${GLPONG_TARGET} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DEAN_DLLS} $<TARGET_FILE_DIR:${GLPONG_TARGET}>)
    add_custom_command(TARGET ${GLPONG_TARGET} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${GLPONG_GLFW_DIR}/glfw3.dll"
            $<TARGET_FILE_DIR:${GLPONG_TARGET}>)
endif()

add_dean_example(0.examples__9.shadertoy_hot
    "${DEAN_EXAMPLES_DIR}/9.shadertoy_hot" shadertoy_hot.cpp)
add_dean_example(0.examples__10.shadertoy_basic
    "${DEAN_EXAMPLES_DIR}/10.shadertoy_basic" shadertoy_basic.cpp)
add_dean_example(0.examples__11.compute_shader_basic
    "${DEAN_EXAMPLES_DIR}/11.compute_shader_basic" compute_shader_basic.cpp)
add_dean_example(0.examples__12.particle_system_cpu
    "${DEAN_EXAMPLES_DIR}/12.particle_system_cpu" particle_system_cpu.cpp)
add_dean_example(0.examples__13.particle_system_gpu
    "${DEAN_EXAMPLES_DIR}/13.particle_system_gpu" particle_system_gpu.cpp)
add_dean_example(0.examples__14.rt_in_one_weekend
    "${DEAN_EXAMPLES_DIR}/14.rt_in_one_weekend" main.cc)
add_dean_example(0.examples__15.rt_in_one_weekend_gpu
    "${DEAN_EXAMPLES_DIR}/15.rt_in_one_weekend_gpu" rt_in_one_weekend_gpu.cpp)

set(DEAN_GETTING_STARTED_DIR "${CMAKE_SOURCE_DIR}/dean/src/1.getting_started")
add_dean_example(1.getting_started__3.4.shaders_exercise1
    "${DEAN_GETTING_STARTED_DIR}/3.4.shaders_exercise1" shaders_exercise1.cpp)
add_dean_example(1.getting_started__3.5.shaders_exercise2
    "${DEAN_GETTING_STARTED_DIR}/3.5.shaders_exercise2" shaders_exercise2.cpp)
add_dean_example(1.getting_started__3.6.shaders_exercise3
    "${DEAN_GETTING_STARTED_DIR}/3.6.shaders_exercise3" shaders_exercise3.cpp)
add_dean_example(1.getting_started__7.5.camera_exercise1
    "${DEAN_GETTING_STARTED_DIR}/7.5.camera_exercise1" camera_exercise1.cpp)
add_dean_example(1.getting_started__7.6.camera_exercise2
    "${DEAN_GETTING_STARTED_DIR}/7.6.camera_exercise2" camera_exercise2.cpp)

set(DEAN_LIGHTING_DIR "${CMAKE_SOURCE_DIR}/dean/src/2.lighting")
add_dean_example(2.lighting__2.3.basic_lighting_exercise1
    "${DEAN_LIGHTING_DIR}/2.3.basic_lighting_exercise1" basic_lighting_exercise1.cpp)
add_dean_example(2.lighting__2.4.basic_lighting_exercise2
    "${DEAN_LIGHTING_DIR}/2.4.basic_lighting_exercise2" basic_lighting_exercise2.cpp)
add_dean_example(2.lighting__2.5.basic_lighting_exercise3
    "${DEAN_LIGHTING_DIR}/2.5.basic_lighting_exercise3" basic_lighting_exercise3.cpp)
add_dean_example(2.lighting__4.3.lighting_maps_exercise2
    "${DEAN_LIGHTING_DIR}/4.3.lighting_maps_exercise2" lighting_maps_exercise2.cpp)

set(DEAN_ADVANCED_OPENGL_DIR "${CMAKE_SOURCE_DIR}/dean/src/4.advanced_opengl")
add_dean_example(4.advanced_opengl__1.3.depth_testing_fixed
    "${DEAN_ADVANCED_OPENGL_DIR}/1.3.depth_testing_fixed" depth_testing_fixed.cpp)
add_dean_example(4.advanced_opengl__4.face_culling_exercise1
    "${DEAN_ADVANCED_OPENGL_DIR}/4.face_culling_exercise1" face_culling_exercise1.cpp)
add_dean_example(4.advanced_opengl__9.4.geometry_shader_exploding
    "${DEAN_ADVANCED_OPENGL_DIR}/9.4.geometry_shader_exploding" geometry_shader_exploding.cpp)
add_dean_example(4.advanced_opengl__9.5.geometry_shader_normals
    "${DEAN_ADVANCED_OPENGL_DIR}/9.5.geometry_shader_normals" normal_visualization.cpp)

set(DEAN_ADVANCED_LIGHTING_DIR "${CMAKE_SOURCE_DIR}/dean/src/5.advanced_lighting")
add_dean_example(5.advanced_lighting__1.1.advanced_lighting
    "${DEAN_ADVANCED_LIGHTING_DIR}/1.1.advanced_lighting" advanced_lighting.cpp)
add_dean_example(5.advanced_lighting__3.1.4.shadow_mapping
    "${DEAN_ADVANCED_LIGHTING_DIR}/3.1.4.shadow_mapping" shadow_mapping.cpp)
add_dean_example(5.advanced_lighting__3.1.5.shadow_mapping_perspective
    "${DEAN_ADVANCED_LIGHTING_DIR}/3.1.5.shadow_mapping_perspective" shadow_mapping_perspective.cpp)
add_dean_example(5.advanced_lighting__3.2.3.point_shadows
    "${DEAN_ADVANCED_LIGHTING_DIR}/3.2.3.point_shadows" point_shadows.cpp)
add_dean_example(5.advanced_lighting__8.3.deferred_shading
    "${DEAN_ADVANCED_LIGHTING_DIR}/8.3.deferred_shading" deferred_shading.cpp)

# Breakout is a complete multi-file program rather than a single tutorial
# source. Reuse the project-wide GLAD/STB_IMAGE targets instead of compiling
# the duplicate implementations shipped in its source directory.
set(BREAKOUT_DIR "${CMAKE_SOURCE_DIR}/src/7.in_practice/3.2d_game/0.full_source")
file(GLOB BREAKOUT_SOURCES "${BREAKOUT_DIR}/*.cpp" "${BREAKOUT_DIR}/*.h")
list(FILTER BREAKOUT_SOURCES EXCLUDE REGEX "/(glad|stb_image)\\.(c|cpp)$")

set(BREAKOUT_TARGET "7.in_practice__3.2d_game__0.full_source")
add_executable(${BREAKOUT_TARGET} ${BREAKOUT_SOURCES})
target_include_directories(${BREAKOUT_TARGET} PRIVATE "${BREAKOUT_DIR}")
target_link_libraries(${BREAKOUT_TARGET} ${LIBS})
if(MSVC)
    target_compile_options(${BREAKOUT_TARGET} PRIVATE /std:c++17 /MP)
    target_link_options(${BREAKOUT_TARGET} PUBLIC /ignore:4099)
endif()
set_target_properties(${BREAKOUT_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/7.in_practice/3.2d_game"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/7.in_practice/3.2d_game/Debug")

file(GLOB BREAKOUT_SHADERS "${BREAKOUT_DIR}/*.vs" "${BREAKOUT_DIR}/*.fs")
foreach(shader ${BREAKOUT_SHADERS})
    add_custom_command(TARGET ${BREAKOUT_TARGET} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${shader} $<TARGET_FILE_DIR:${BREAKOUT_TARGET}>)
endforeach()
if(WIN32)
    add_custom_command(TARGET ${BREAKOUT_TARGET} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DEAN_DLLS} $<TARGET_FILE_DIR:${BREAKOUT_TARGET}>)
endif()
