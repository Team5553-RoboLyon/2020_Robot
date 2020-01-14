/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once
#include <frc/GenericHID.h>

#include <frc2/command/button/Button.h>

namespace frc2 {
/**
 * A class used to bind command scheduling to joystick POV presses.  Can be
 * composed with other buttons with the operators in Trigger.
 *
 * @see Trigger
 */
class POVButton2 : public Button {
 public:
  /**
   * Creates a POVButton2 that commands can be bound to.
   *
   * @param joystick The joystick on which the button is located.
   * @param angle The angle of the POV corresponding to a button press.
   * @param povNumber The number of the POV on the joystick.
   */
  POVButton2(frc::GenericHID* joystick, int angle, int povNumber = 0)
      : Button([joystick, angle, povNumber] { m_joystick->GetPOV(m_povNumber) == m_angle; }) {}
};
}  // namespace frc2
