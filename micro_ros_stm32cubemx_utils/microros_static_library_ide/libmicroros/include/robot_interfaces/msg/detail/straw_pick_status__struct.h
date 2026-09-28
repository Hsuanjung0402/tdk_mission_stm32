// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robot_interfaces:msg/StrawPickStatus.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_INTERFACES__MSG__DETAIL__STRAW_PICK_STATUS__STRUCT_H_
#define ROBOT_INTERFACES__MSG__DETAIL__STRAW_PICK_STATUS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Constant 'PHASE_IDLE'.
enum
{
  robot_interfaces__msg__StrawPickStatus__PHASE_IDLE = 0
};

/// Constant 'PHASE_PICKING'.
enum
{
  robot_interfaces__msg__StrawPickStatus__PHASE_PICKING = 1
};

/// Constant 'PHASE_ERROR'.
enum
{
  robot_interfaces__msg__StrawPickStatus__PHASE_ERROR = 2
};

/// Struct defined in msg/StrawPickStatus in the package robot_interfaces.
typedef struct robot_interfaces__msg__StrawPickStatus
{
  /// PHASE_*
  uint8_t phase;
  /// 開機以來夾取完成的累計數（只增不減，uint8 溢位沒關係）
  uint8_t pick_count;
  /// 目前是否允許「碰到開關就自動夾取」
  bool armed;
  /// 前方極限開關原始狀態
  bool switch_pressed;
} robot_interfaces__msg__StrawPickStatus;

// Struct for a sequence of robot_interfaces__msg__StrawPickStatus.
typedef struct robot_interfaces__msg__StrawPickStatus__Sequence
{
  robot_interfaces__msg__StrawPickStatus * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__msg__StrawPickStatus__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_INTERFACES__MSG__DETAIL__STRAW_PICK_STATUS__STRUCT_H_
