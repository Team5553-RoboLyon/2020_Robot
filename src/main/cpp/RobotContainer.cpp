/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "RobotContainer.h"

#include "commands/Drive.h"

RobotContainer::RobotContainer() {
  ConfigureButtonBindings();
  m_drivetrain.SetDefaultCommand(
      Drive([this] { return -m_driverController.GetY(frc::GenericHID::JoystickHand::kLeftHand); },
            [this] { return -m_driverController.GetX(frc::GenericHID::JoystickHand::kRightHand); },
            &m_drivetrain));
}

void RobotContainer::ConfigureButtonBindings() {}