cmake_minimum_required(VERSION 3.8)
project(gui_command)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(sources
"src/gui_command.cpp"
"src/CommandApp.cpp")

set(includes
"include/CommandApp.hpp")

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(PkgConfig)
pkg_check_modules(GTK gtk4)
find_package(custom_msg REQUIRED) 

add_executable(gui_command ${sources} ${includes})

ament_target_dependencies(gui_command
  rclcpp
  GTK
  custom_msg
)

target_include_directories(gui_command PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  GTK_INCLUDE_DIRS
  "../custom_msg/install/custom_msg/include/custom_msg"
  )
target_compile_features(gui_command PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

target_link_libraries(gui_command ${GTK_LIBRARIES})

install(TARGETS gui_command
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
