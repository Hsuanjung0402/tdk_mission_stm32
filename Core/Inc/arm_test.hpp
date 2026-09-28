/*
 * arm_test.h
 *
 *  Created on: Jul 24, 2026
 *      Author: jessielin
 */

#ifndef INC_ARM_TEST_HPP_
#define INC_ARM_TEST_HPP_

// 一般 C 函數宣告
int arm_init(void);
void arm_homing(void);
void arm_init_servo(void);
int arm_test(void);
void arm_script(int);

// 👉 正確的 C 語言外銷護照 (處理 C 與 C++ 混編)
#ifdef __cplusplus
extern "C" {
#endif

void cpp_arm_init(void);
void cpp_arm_init_servo(void);
void cpp_arm_homing(void);
void cpp_arm_test(void);
void cpp_arm_update(void);
void cpp_arm_script(int);

#ifdef __cplusplus
}
#endif

#endif /* INC_ARM_TEST_HPP_ */
