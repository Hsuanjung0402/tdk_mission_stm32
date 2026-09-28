cmake_minimum_required(VERSION 3.8)
project(robot_interfaces LANGUAGES C CXX)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/MechanismCommand.msg"
  "msg/StrawPickStatus.msg"
)

ament_export_dependencies(rosidl_default_runtime)
ament_package()
