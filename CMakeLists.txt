cmake_minimum_required(VERSION 3.31)

project(AOC_2025)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  ${CMAKE_MODULE_PATH})


set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization

if (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(generator_config "DEBUG")
elseif(CMAKE_BUILD_TYPE MATCHES "Release" OR CMAKE_BUILD_TYPE MATCHES "MinSizeRel" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebugInfo")
	set(generator_config "RELEASE")
endif()

if(MSVC) 
	# add_compile_options(/arch:AVX2) #make sure SIMD optimizations take place
	# add_compile_options(/W4) # too aggresive
	add_compile_options(/WX)
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
	# 	add_compile_options(/fsanitize=address)
	# 	add_link_options(/fsanitize=address)
	endif()
else()
	add_compile_options(-Wall)
	add_compile_options(-Wextra) 
	add_compile_options(-Wpedantic)
	add_compile_options(-Werror)
	# add_compile_options(-Wconversion)
	# add_compile_options(-Wdouble-promotion)
	add_compile_options(-Wshadow)
	add_compile_options(-Wno-unused-parameter)
	add_compile_options(-Wno-unused-function)
	add_compile_options(-Wno-unused-but-set-variable)
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		add_compile_options(-fsanitize=address)
		add_compile_options(-fno-omit-frame-pointer)
		add_link_options(-fsanitize=address)
		add_link_options(-fno-omit-frame-pointer)
	endif()
endif()




if(MSVC)
	SET(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	# SET(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

set (bin_dir "${CMAKE_SOURCE_DIR}/bin")
set (output_dir "${bin_dir}/${CMAKE_SYSTEM_NAME}-$<CONFIG>")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${output_dir})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${output_dir})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${output_dir}/lib)
set (CMAKE_WORKING_DIRECTORY        ${CMAKE_SOURCE_DIR})


add_subdirectory(ludutils)

add_subdirectory(day1-complete)

set_target_properties(2025_01_01 PROPERTIES FOLDER "DAY01")
set_target_properties(2025_01_02 PROPERTIES FOLDER "DAY01")

add_subdirectory(day2-complete)

set_target_properties(2025_02_01 PROPERTIES FOLDER "DAY02")
set_target_properties(2025_02_02 PROPERTIES FOLDER "DAY02")

add_subdirectory(day3-complete)

set_target_properties(2025_03_01 PROPERTIES FOLDER "DAY03")
set_target_properties(2025_03_02 PROPERTIES FOLDER "DAY03")

add_subdirectory(day4-complete)

set_target_properties(2025_04_01 PROPERTIES FOLDER "DAY04")
set_target_properties(2025_04_02 PROPERTIES FOLDER "DAY04")

add_subdirectory(day5-complete)

set_target_properties(2025_05_01 PROPERTIES FOLDER "DAY05")
set_target_properties(2025_05_02 PROPERTIES FOLDER "DAY05")

add_subdirectory(day6-complete)

set_target_properties(2025_06_01 PROPERTIES FOLDER "DAY06")
set_target_properties(2025_06_02 PROPERTIES FOLDER "DAY06")
