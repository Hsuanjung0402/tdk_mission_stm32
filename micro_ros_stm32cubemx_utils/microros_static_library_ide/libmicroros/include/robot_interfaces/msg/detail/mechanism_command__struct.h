// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robot_interfaces:msg/MechanismCommand.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_INTERFACES__MSG__DETAIL__MECHANISM_COMMAND__STRUCT_H_
#define ROBOT_INTERFACES__MSG__DETAIL__MECHANISM_COMMAND__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'command_name'
// Member 'arg_json'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/MechanismCommand in the package robot_interfaces.
typedef struct robot_interfaces__msg__MechanismCommand
{
  uint16_t command_id;
  rosidl_runtime_c__String command_name;
  rosidl_runtime_c__String arg_json;
} robot_interfaces__msg__MechanismCommand;

// Struct for a sequence of robot_interfaces__msg__MechanismCommand.
typedef struct robot_interfaces__msg__MechanismCommand__Sequence
{
  robot_interfaces__msg__MechanismCommand * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__msg__MechanismCommand__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_INTERFACES__MSG__DETAIL__MECHANISM_COMMAND__STRUCT_H_
