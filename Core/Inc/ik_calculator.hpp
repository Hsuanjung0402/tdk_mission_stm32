/*
 * ik_calculator.hpp
 *
 *  Created on: Jul 25, 2026
 *      Author: jessielin
 */

#ifndef INC_IK_CALCULATOR_HPP_
#define INC_IK_CALCULATOR_HPP_

struct ArmAngles;
ArmAngles calculateIK(float target_X, float target_Y, float phi_deg = -90.0f) ;
void move_straight_down() ;


#endif /* INC_IK_CALCULATOR_HPP_ */
