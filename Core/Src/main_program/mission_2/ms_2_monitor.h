/*
 * ms_2_monitor.hpp
 *
 *  Created on: Aug 13, 2026
 *      Author: hsuanjung
 */

#ifndef SRC_MAIN_PROGRAM_MISSION_2_MS_2_MONITOR_HPP_
#define SRC_MAIN_PROGRAM_MISSION_2_MS_2_MONITOR_HPP_

#ifdef __cplusplus
extern "C"{
#endif

void MS_2_init();
void MS_2_CW_down();
void MS_2_CCW_down();
void MS_2_CW_rotate();
void MS_2_CCW_rotate();
void MS_2_middle();
void MS_2_open_pink();
void MS_2_open_blue();
void MS_2_middle();
void MS_2_close_pink();
void MS_2_close_blue();
void MS_2_CW_time(int);
void MS_2_CCW_time(int);

#ifdef __cplusplus
}
#endif



#endif /* SRC_MAIN_PROGRAM_MISSION_2_MS_2_MONITOR_HPP_ */
