####### Compiler, tools and options

# General options for all programs.
HEADER_DIR    = -Iincludes
# glfw3: don't use the static lib, use the dll
LIB_DIR       = -Ldean_dlls/glfw-3.3.8.bin.WIN64/lib-mingw-w64
LIBS          = -lglfw3

# x86 Windows Options for mingw.
# Create the window without the command line window behind it.
# Keep the cmd line window for debugging.
#WINDOWS_FLAGS = -mwindows

CXX      	  = g++
#CXXDEFINES    = -Dxx=yy -Dzz
CXXFLAGS 	  = -Wall \
                -Werror \
                -g \
                $(WINDOWS_FLAGS) \
                $(HEADER_DIR) \
                $(LIB_DIR) \
                $(LIBS)

####### Common files
SRC_GLAD    = src/glad.c
# For reading the jpg images
SRC_STB     = src/stb_image.cpp
# For drawing the axes of the world space
SRC_AXES    = includes/axes.cpp

####### Chapter src and out dirs
CH0_OUT     = out/0.dean
SRC_0_1     = src/0.dean/1.draw_circle/draw_circle.cpp $(SRC_GLAD)
SRC_0_2     = src/0.dean/2.access_neighboring_texels/access_neighboring_texels.cpp $(SRC_GLAD) $(SRC_STB)
RES_0_2     = src/0.dean/2.access_neighboring_texels/2.*
SRC_0_3     = src/0.dean/3.pyramid_texturing/pyramid_texturing.cpp $(SRC_GLAD) $(SRC_STB) $(SRC_AXES)
RES_0_3     = src/0.dean/3.pyramid_texturing/3.* includes/axes_shader.*
CH0_SRC     = $(SRC_0_1) $(SRC_0_2) $(SRC_0_3)

CH1_OUT     = out/1.getting_started
SRC_1_1_1   = src/1.getting_started/1.1.hello_window/hello_window.cpp $(SRC_GLAD)
SRC_1_1_2   = src/1.getting_started/1.2.hello_window_clear/hello_window_clear.cpp $(SRC_GLAD)

SRC_1_2_1   = src/1.getting_started/2.1.hello_triangle/hello_triangle.cpp $(SRC_GLAD)
SRC_1_2_2   = src/1.getting_started/2.2.hello_triangle_indexed/hello_triangle_indexed.cpp $(SRC_GLAD)
SRC_1_2_3   = src/1.getting_started/2.3.hello_triangle_exercise1/hello_triangle_exercise1.cpp $(SRC_GLAD)
SRC_1_2_4   = src/1.getting_started/2.4.hello_triangle_exercise2/hello_triangle_exercise2.cpp $(SRC_GLAD)
SRC_1_2_5   = src/1.getting_started/2.5.hello_triangle_exercise3/hello_triangle_exercise3.cpp $(SRC_GLAD)

SRC_1_3_1   = src/1.getting_started/3.1.shaders_uniform/shaders_uniform.cpp $(SRC_GLAD)
SRC_1_3_2   = src/1.getting_started/3.2.shaders_interpolation/shaders_interpolation.cpp $(SRC_GLAD)
SRC_1_3_3   = src/1.getting_started/3.3.shaders_class/shaders_class.cpp $(SRC_GLAD)
RES_1_3_3   = src/1.getting_started/3.3.shaders_class/3.3.*
SRC_1_3_4   = src/1.getting_started/3.4.shaders_exercise1/shaders_exercise1.cpp $(SRC_GLAD)
RES_1_3_4   = src/1.getting_started/3.4.shaders_exercise1/3.4.*
SRC_1_3_5   = src/1.getting_started/3.5.shaders_exercise2/shaders_exercise2.cpp $(SRC_GLAD)
RES_1_3_5   = src/1.getting_started/3.5.shaders_exercise2/3.5.*
SRC_1_3_6   = src/1.getting_started/3.6.shaders_exercise3/shaders_exercise3.cpp $(SRC_GLAD)
RES_1_3_6   = src/1.getting_started/3.6.shaders_exercise3/3.6.*

SRC_1_4_1   = src/1.getting_started/4.1.textures/textures.cpp $(SRC_GLAD) $(SRC_STB)
RES_1_4_1   = src/1.getting_started/4.1.textures/4.1.*
SRC_1_4_2   = src/1.getting_started/4.2.textures_combined/textures_combined.cpp $(SRC_GLAD) $(SRC_STB)
RES_1_4_2   = src/1.getting_started/4.2.textures_combined/4.2.*
SRC_1_4_4   = src/1.getting_started/4.4.textures_exercise2/textures_exercise2.cpp $(SRC_GLAD) $(SRC_STB)
# Note the number mismatch below.
RES_1_4_4   = src/1.getting_started/4.4.textures_exercise2/4.3.*
SRC_1_4_5   = src/1.getting_started/4.5.textures_exercise3/textures_exercise3.cpp $(SRC_GLAD) $(SRC_STB)
RES_1_4_5   = src/1.getting_started/4.5.textures_exercise3/4.4.*
SRC_1_4_6   = src/1.getting_started/4.6.textures_exercise4/textures_exercise4.cpp $(SRC_GLAD) $(SRC_STB)
RES_1_4_6   = src/1.getting_started/4.6.textures_exercise4/4.5.*

SRC_1_5_1   = src/1.getting_started/5.1.transformations/transformations.cpp $(SRC_GLAD) $(SRC_STB)
RES_1_5_1   = src/1.getting_started/5.1.transformations/5.1.*
SRC_1_5_2   = src/1.getting_started/5.2.transformations_exercise2/transformations_exercise2.cpp $(SRC_GLAD) $(SRC_STB)
RES_1_5_2   = src/1.getting_started/5.2.transformations_exercise2/5.2.*

SRC_1_6_1   = src/1.getting_started/6.1.coordinate_systems/coordinate_systems.cpp $(SRC_GLAD) $(SRC_STB)
RES_1_6_1   = src/1.getting_started/6.1.coordinate_systems/6.1.*
SRC_1_6_2   = src/1.getting_started/6.2.coordinate_systems_depth/coordinate_systems_depth.cpp $(SRC_GLAD) $(SRC_STB)
RES_1_6_2   = src/1.getting_started/6.2.coordinate_systems_depth/6.2.*
SRC_1_6_3   = src/1.getting_started/6.3.coordinate_systems_multiple/coordinate_systems_multiple.cpp $(SRC_GLAD) $(SRC_STB)
RES_1_6_3   = src/1.getting_started/6.3.coordinate_systems_multiple/6.3.*

SRC_1_7_1   = src/1.getting_started/7.1.camera_circle/camera_circle.cpp $(SRC_GLAD) $(SRC_STB)
RES_1_7_1   = src/1.getting_started/7.1.camera_circle/7.1.*
SRC_1_7_2   = src/1.getting_started/7.2.camera_keyboard_dt/camera_keyboard_dt.cpp $(SRC_GLAD) $(SRC_STB)
RES_1_7_2   = src/1.getting_started/7.2.camera_keyboard_dt/7.2.*
SRC_1_7_3   = src/1.getting_started/7.3.camera_mouse_zoom/camera_mouse_zoom.cpp $(SRC_GLAD) $(SRC_STB) $(SRC_AXES)
RES_1_7_3   = src/1.getting_started/7.3.camera_mouse_zoom/7.3.* includes/axes_shader.*
SRC_1_7_4   = src/1.getting_started/7.4.camera_class/camera_class.cpp $(SRC_GLAD) $(SRC_STB)
RES_1_7_4   = src/1.getting_started/7.4.camera_class/7.4.*
CH1_SRC     = $(SRC_1_1_1) $(SRC_1_1_2) \
              $(SRC_1_2_1) $(SRC_1_2_2) $(SRC_1_2_3) $(SRC_1_2_4) $(SRC_1_2_5) \
              $(SRC_1_3_1) $(SRC_1_3_2) $(SRC_1_3_3) $(SRC_1_3_4) $(SRC_1_3_5) $(SRC_1_3_6) \
              $(SRC_1_4_1) $(SRC_1_4_2) $(SRC_1_4_4) $(SRC_1_4_5) $(SRC_1_4_6) \
              $(SRC_1_5_1) $(SRC_1_5_2) \
              $(SRC_1_6_1) $(SRC_1_6_2) $(SRC_1_6_3) \
              $(SRC_1_7_1) $(SRC_1_7_2) $(SRC_1_7_3) $(SRC_1_7_4)

CH2_OUT     = out/2.lighting
SRC_2_1     = src/2.lighting/1.colors/colors.cpp $(SRC_GLAD)
RES_2_1     = src/2.lighting/1.colors/1.*

SRC_2_2_1   = src/2.lighting/2.1.basic_lighting_diffuse/basic_lighting_diffuse.cpp $(SRC_GLAD)
RES_2_2_1   = src/2.lighting/2.1.basic_lighting_diffuse/2.1.*
SRC_2_2_2   = src/2.lighting/2.2.basic_lighting_specular/basic_lighting_specular.cpp $(SRC_GLAD)
RES_2_2_2   = src/2.lighting/2.2.basic_lighting_specular/2.2.*
SRC_2_2_4   = src/2.lighting/2.4.basic_lighting_exercise2/basic_lighting_exercise2_code.cpp $(SRC_GLAD)
RES_2_2_4   = src/2.lighting/2.4.basic_lighting_exercise2/2.4.*
SRC_2_2_5   = src/2.lighting/2.5.basic_lighting_exercise3/basic_lighting_exercise3_code.cpp $(SRC_GLAD)
RES_2_2_5   = src/2.lighting/2.5.basic_lighting_exercise3/2.5.*

SRC_2_3_1   = src/2.lighting/3.1.materials/materials.cpp $(SRC_GLAD)
RES_2_3_1   = src/2.lighting/3.1.materials/3.1.*
SRC_2_3_2   = src/2.lighting/3.2.materials_exercise1/materials_exercise1.cpp $(SRC_GLAD)
RES_2_3_2   = src/2.lighting/3.2.materials_exercise1/3.2.*

SRC_2_4_1   = src/2.lighting/4.1.lighting_maps_diffuse_map/lighting_maps_diffuse.cpp $(SRC_GLAD) $(SRC_STB)
RES_2_4_1   = src/2.lighting/4.1.lighting_maps_diffuse_map/4.1.*
SRC_2_4_2   = src/2.lighting/4.2.lighting_maps_specular_map/lighting_maps_specular.cpp $(SRC_GLAD) $(SRC_STB)
RES_2_4_2   = src/2.lighting/4.2.lighting_maps_specular_map/4.2.*
CH2_SRC     = $(SRC_2_1) \
              $(SRC_2_2_1) $(SRC_2_2_2) $(SRC_2_2_4) $(SRC_2_2_5)\
              $(SRC_2_3_1) $(SRC_2_3_2) \
              $(SRC_2_4_1)

CH4_OUT     = out/4.advanced_opengl
SRC_4_1_1   = src/4.advanced_opengl/1.1.depth_testing/depth_testing.cpp $(SRC_GLAD) $(SRC_STB) $(SRC_AXES)
RES_4_1_1   = src/4.advanced_opengl/1.1.depth_testing/1.1.* includes/axes_shader.*
SRC_4_1_2   = src/4.advanced_opengl/1.2.depth_testing_view/depth_testing_view.cpp $(SRC_GLAD) $(SRC_STB)
RES_4_1_2   = src/4.advanced_opengl/1.2.depth_testing_view/1.2.*

SRC_4_2     = src/4.advanced_opengl/2.stencil_testing/stencil_testing.cpp $(SRC_GLAD) $(SRC_STB)
RES_4_2     = src/4.advanced_opengl/2.stencil_testing/2.*

SRC_4_3_1   = src/4.advanced_opengl/3.1.blending_discard/blending_discard.cpp $(SRC_GLAD) $(SRC_STB)
RES_4_3_1   = src/4.advanced_opengl/3.1.blending_discard/3.1.*
SRC_4_3_2   = src/4.advanced_opengl/3.2.blending_sort/blending_sorted.cpp $(SRC_GLAD) $(SRC_STB)
RES_4_3_2   = src/4.advanced_opengl/3.2.blending_sort/3.2.*

SRC_4_4     = src/4.advanced_opengl/4.face_culling_exercise1/face_culling_exercise1_code.cpp $(SRC_GLAD) $(SRC_STB)

SRC_4_5_1   = src/4.advanced_opengl/5.1.framebuffers/framebuffers.cpp $(SRC_GLAD) $(SRC_STB)
RES_4_5_1   = src/4.advanced_opengl/5.1.framebuffers/5.1.*
SRC_4_5_2   = src/4.advanced_opengl/5.2.framebuffers_exercise1/framebuffers_exercise1.cpp $(SRC_GLAD) $(SRC_STB)
RES_4_5_2   = src/4.advanced_opengl/5.2.framebuffers_exercise1/5.2.*

SRC_4_6_1   = src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp $(SRC_GLAD) $(SRC_STB) $(SRC_AXES)
RES_4_6_1   = src/4.advanced_opengl/6.1.cubemaps_skybox/6.1.* includes/axes_shader.*
SRC_4_6_2   = src/4.advanced_opengl/6.2.cubemaps_environment_mapping/cubemaps_environment_mapping.cpp $(SRC_GLAD) $(SRC_STB)
RES_4_6_2   = src/4.advanced_opengl/6.2.cubemaps_environment_mapping/6.2.*

SRC_4_8     = src/4.advanced_opengl/8.advanced_glsl_ubo/advanced_glsl_ubo.cpp $(SRC_GLAD) $(SRC_STB)
RES_4_8     = src/4.advanced_opengl/8.advanced_glsl_ubo/8.*

SRC_4_9_1   = src/4.advanced_opengl/9.1.geometry_shader_houses/geometry_shader_houses.cpp $(SRC_GLAD) $(SRC_STB)
RES_4_9_1   = src/4.advanced_opengl/9.1.geometry_shader_houses/9.1.*

SRC_4_10_1  = src/4.advanced_opengl/10.1.instancing_quads/instancing_quads.cpp $(SRC_GLAD) $(SRC_STB)
RES_4_10_1  = src/4.advanced_opengl/10.1.instancing_quads/10.1.*

SRC_4_11_1  = src/4.advanced_opengl/11.1.anti_aliasing_msaa/anti_aliasing_msaa.cpp $(SRC_GLAD) $(SRC_STB)
RES_4_11_1  = src/4.advanced_opengl/11.1.anti_aliasing_msaa/11.1.*
SRC_4_11_2  = src/4.advanced_opengl/11.2.anti_aliasing_offscreen/anti_aliasing_offscreen.cpp $(SRC_GLAD) $(SRC_STB)
RES_4_11_2  = src/4.advanced_opengl/11.2.anti_aliasing_offscreen/11.2.*
CH4_SRC     = $(SRC_4_1_1) $(SRC_4_1_2) \
              $(SRC_4_2) \
              $(SRC_4_3_1) $(SRC_4_3_2) \
			  $(SRC_4_4)\
              $(SRC_4_5_1) $(SRC_4_5_2) \
              $(SRC_4_6_1) $(SRC_4_6_2) \
              $(SRC_4_8) \
              $(SRC_4_9_1) \
              $(SRC_4_10_1) \
              $(SRC_4_11_1) $(SRC_4_11_2)


####### Build rules

#TODO, if I didn't edit anything, how to skip re-compiling?
.PHONY : all env ch0 ch1 ch2 ch4 clean

all : env ch0 ch1 ch2 ch4

env : Makefile
	@echo "LOGL_ROOT_PATH=$(LOGL_ROOT_PATH)"
	echo "const char * logl_root = \"D://LearnOpenGL\";" > includes/root_directory.h

ch0 : env Makefile $(CH0_SRC)
	mkdir -p $(CH0_OUT)
	cp dean_dlls/glfw-3.3.8.bin.WIN64/lib-mingw-w64/glfw3.dll $(CH0_OUT)/

ifneq (,$(filter $(BUILD_CHAPTER),4 all)) # skip build in it
	@echo "===== Build $(SRC_0_1) ===="
	$(CXX) $(SRC_0_1) $(CXXFLAGS) -o $(CH0_OUT)/SRC_0_1.exe

	@echo "===== Build $(SRC_0_2) ===="
	$(CXX) $(SRC_0_2) $(CXXFLAGS) -o $(CH0_OUT)/SRC_0_2.exe
	cp $(RES_0_2) $(CH0_OUT)/
endif
	@echo "===== Build $(SRC_0_3) ===="
	$(CXX) $(SRC_0_3) $(CXXFLAGS) -o $(CH0_OUT)/SRC_0_3.exe
	cp $(RES_0_3) $(CH0_OUT)/

ch1 : env Makefile $(CH1_SRC)
	mkdir -p $(CH1_OUT)
	cp dean_dlls/glfw-3.3.8.bin.WIN64/lib-mingw-w64/glfw3.dll $(CH1_OUT)/

	@echo "===== Build $(SRC_1_1_1) ===="
	$(CXX) $(SRC_1_1_1) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_1_1.exe
	@echo "===== Build $(SRC_1_1_2) ===="
	$(CXX) $(SRC_1_1_2) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_1_2.exe

	@echo "===== Build $(SRC_1_2_1) ===="
	$(CXX) $(SRC_1_2_1) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_2_1.exe
	@echo "===== Build $(SRC_1_2_2) ===="
	$(CXX) $(SRC_1_2_2) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_2_2.exe
	@echo "===== Build $(SRC_1_2_3) ===="
	$(CXX) $(SRC_1_2_3) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_2_3.exe
	@echo "===== Build $(SRC_1_2_4) ===="
	$(CXX) $(SRC_1_2_4) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_2_4.exe
	@echo "===== Build $(SRC_1_2_5) ===="
	$(CXX) $(SRC_1_2_5) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_2_5.exe

	@echo "===== Build $(SRC_1_3_1) ===="
	$(CXX) $(SRC_1_3_1) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_3_1.exe
	@echo "===== Build $(SRC_1_3_2) ===="
	$(CXX) $(SRC_1_3_2) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_3_2.exe
	@echo "===== Build $(SRC_1_3_3) ===="
	$(CXX) $(SRC_1_3_3) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_3_3.exe
	cp $(RES_1_3_3) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_3_4) ===="
	$(CXX) $(SRC_1_3_4) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_3_4.exe
	cp $(RES_1_3_4) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_3_5) ===="
	$(CXX) $(SRC_1_3_5) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_3_5.exe
	cp $(RES_1_3_5) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_3_6) ===="
	$(CXX) $(SRC_1_3_6) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_3_6.exe
	cp $(RES_1_3_6) $(CH1_OUT)/

	@echo "===== Build $(SRC_1_4_1) ===="
	$(CXX) $(SRC_1_4_1) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_4_1.exe
	cp $(RES_1_4_1) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_4_2) ===="
	$(CXX) $(SRC_1_4_2) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_4_2.exe
	cp $(RES_1_4_2) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_4_4) ===="
	$(CXX) $(SRC_1_4_4) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_4_4.exe
	cp $(RES_1_4_4) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_4_5) ===="
	$(CXX) $(SRC_1_4_5) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_4_5.exe
	cp $(RES_1_4_5) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_4_6) ===="
	$(CXX) $(SRC_1_4_6) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_4_6.exe
	cp $(RES_1_4_6) $(CH1_OUT)/

	@echo "===== Build $(SRC_1_5_1) ===="
	$(CXX) $(SRC_1_5_1) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_5_1.exe
	cp $(RES_1_5_1) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_5_2) ===="
	$(CXX) $(SRC_1_5_2) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_5_2.exe
	cp $(RES_1_5_2) $(CH1_OUT)/

	@echo "===== Build $(SRC_1_6_1) ===="
	$(CXX) $(SRC_1_6_1) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_6_1.exe
	cp $(RES_1_6_1) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_6_2) ===="
	$(CXX) $(SRC_1_6_2) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_6_2.exe
	cp $(RES_1_6_2) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_6_3) ===="
	$(CXX) $(SRC_1_6_3) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_6_3.exe
	cp $(RES_1_6_3) $(CH1_OUT)/

	@echo "===== Build $(SRC_1_7_1) ===="
	$(CXX) $(SRC_1_7_1) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_7_1.exe
	cp $(RES_1_7_1) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_7_2) ===="
	$(CXX) $(SRC_1_7_2) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_7_2.exe
	cp $(RES_1_7_2) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_7_3) ===="
	$(CXX) $(SRC_1_7_3) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_7_3.exe
	cp $(RES_1_7_3) $(CH1_OUT)/
	@echo "===== Build $(SRC_1_7_4) ===="
	$(CXX) $(SRC_1_7_4) $(CXXFLAGS) -o $(CH1_OUT)/SRC_1_7_4.exe
	cp $(RES_1_7_4) $(CH1_OUT)/

ch2 : env Makefile $(CH2_SRC)
	mkdir -p $(CH2_OUT)
	cp dean_dlls/glfw-3.3.8.bin.WIN64/lib-mingw-w64/glfw3.dll $(CH2_OUT)/

	@echo "===== Build $(SRC_2_1) ===="
	$(CXX) $(SRC_2_1) $(CXXFLAGS) -o $(CH2_OUT)/SRC_2_1.exe
	cp $(RES_2_1) $(CH2_OUT)/

	@echo "===== Build $(SRC_2_2_1) ===="
	$(CXX) $(SRC_2_2_1) $(CXXFLAGS) -o $(CH2_OUT)/SRC_2_2_1.exe
	cp $(RES_2_2_1) $(CH2_OUT)/
	@echo "===== Build $(SRC_2_2_2) ===="
	$(CXX) $(SRC_2_2_2) $(CXXFLAGS) -o $(CH2_OUT)/SRC_2_2_2.exe
	cp $(RES_2_2_2) $(CH2_OUT)/
	@echo "===== Build $(SRC_2_2_4) ===="
	$(CXX) $(SRC_2_2_4) $(CXXFLAGS) -o $(CH2_OUT)/SRC_2_2_4.exe
	cp $(RES_2_2_4) $(CH2_OUT)/
	@echo "===== Build $(SRC_2_2_5) ===="
	$(CXX) $(SRC_2_2_5) $(CXXFLAGS) -o $(CH2_OUT)/SRC_2_2_5.exe
	cp $(RES_2_2_5) $(CH2_OUT)/

	@echo "===== Build $(SRC_2_3_1) ===="
	$(CXX) $(SRC_2_3_1) $(CXXFLAGS) -o $(CH2_OUT)/SRC_2_3_1.exe
	cp $(RES_2_3_1) $(CH2_OUT)/
	@echo "===== Build $(SRC_2_3_2) ===="
	$(CXX) $(SRC_2_3_2) $(CXXFLAGS) -o $(CH2_OUT)/SRC_2_3_2.exe
	cp $(RES_2_3_2) $(CH2_OUT)/

	@echo "===== Build $(SRC_2_4_1) ===="
	$(CXX) $(SRC_2_4_1) $(CXXFLAGS) -o $(CH2_OUT)/SRC_2_4_1.exe
	cp $(RES_2_4_1) $(CH2_OUT)/
	@echo "===== Build $(SRC_2_4_2) ===="
	$(CXX) $(SRC_2_4_2) $(CXXFLAGS) -o $(CH2_OUT)/SRC_2_4_2.exe
	cp $(RES_2_4_2) $(CH2_OUT)/

ch4 : env Makefile $(CH4_SRC)
	mkdir -p $(CH4_OUT)
	cp dean_dlls/glfw-3.3.8.bin.WIN64/lib-mingw-w64/glfw3.dll $(CH4_OUT)/

	@echo "===== Build $(SRC_4_1_1) ===="
	$(CXX) $(SRC_4_1_1) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_1_1.exe
	cp $(RES_4_1_1) $(CH4_OUT)/
	@echo "===== Build $(SRC_4_1_2) ===="
	$(CXX) $(SRC_4_1_2) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_1_2.exe
	cp $(RES_4_1_2) $(CH4_OUT)/

	@echo "===== Build $(SRC_4_2) ===="
	$(CXX) $(SRC_4_2) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_2.exe
	cp $(RES_4_2) $(CH4_OUT)/

	@echo "===== Build $(SRC_4_3_1) ===="
	$(CXX) $(SRC_4_3_1) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_3_1.exe
	cp $(RES_4_3_1) $(CH4_OUT)/
	@echo "===== Build $(SRC_4_3_2) ===="
	$(CXX) $(SRC_4_3_2) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_3_2.exe
	cp $(RES_4_3_2) $(CH4_OUT)/

	@echo "===== Build $(SRC_4_4) ===="
	$(CXX) $(SRC_4_4) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_4.exe

	@echo "===== Build $(SRC_4_5_1) ===="
	$(CXX) $(SRC_4_5_1) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_5_1.exe
	cp $(RES_4_5_1) $(CH4_OUT)/
	@echo "===== Build $(SRC_4_5_2) ===="
	$(CXX) $(SRC_4_5_2) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_5_2.exe
	cp $(RES_4_5_2) $(CH4_OUT)/

	@echo "===== Build $(SRC_4_6_1) ===="
	$(CXX) $(SRC_4_6_1) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_6_1.exe
	cp $(RES_4_6_1) $(CH4_OUT)/
	@echo "===== Build $(SRC_4_6_2) ===="
	$(CXX) $(SRC_4_6_2) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_6_2.exe
	cp $(RES_4_6_2) $(CH4_OUT)/

	@echo "===== Build $(SRC_4_8) ===="
	$(CXX) $(SRC_4_8) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_8.exe
	cp $(RES_4_8) $(CH4_OUT)/

	@echo "===== Build $(SRC_4_9_1) ===="
	$(CXX) $(SRC_4_9_1) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_9_1.exe
	cp $(RES_4_9_1) $(CH4_OUT)/

	@echo "===== Build $(SRC_4_10_1) ===="
	$(CXX) $(SRC_4_10_1) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_10_1.exe
	cp $(RES_4_10_1) $(CH4_OUT)/

	@echo "===== Build $(SRC_4_11_1) ===="
	$(CXX) $(SRC_4_11_1) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_11_1.exe
	cp $(RES_4_11_1) $(CH4_OUT)/
	@echo "===== Build $(SRC_4_11_2) ===="
	$(CXX) $(SRC_4_11_2) $(CXXFLAGS) -o $(CH4_OUT)/SRC_4_11_2.exe
	cp $(RES_4_11_2) $(CH4_OUT)/

clean :
	@echo "===== Clean ====="
	rm -rf $(CH0_OUT) $(CH1_OUT) $(CH2_OUT) $(CH4_OUT)
	rm -f includes/root_directory.h
