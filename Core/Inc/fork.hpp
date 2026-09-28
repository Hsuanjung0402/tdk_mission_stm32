/*
 * fork.hpp
 *
 *  Created on: Sep 26, 2026
 *      Author: hsuanjung
 */

#ifndef INC_FORK_HPP_
#define INC_FORK_HPP_

void fork_init(void);
void fork_homing(void);
void fork_pos(int);


#ifdef __cpluspluc
extern "C"{
#endif

void cpp_fork_init(void);
void cpp_fork_homing(void);
void cpp_fork_pos(int);

#ifdef __cplusplus
}
#endif


#endif /* INC_FORK_HPP_ */
