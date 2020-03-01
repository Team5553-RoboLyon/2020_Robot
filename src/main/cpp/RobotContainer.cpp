/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "RobotContainer.h"

#include <frc2/command/CommandScheduler.h>

#include "commands/driving/Drive.h"
#include "commands/controlpanel/PositionControl.h"
#include "commands/controlpanel/RotationControl.h"
#include "commands/endgame/DropHook.h"
#include "commands/endgame/DropRobot.h"
#include "commands/endgame/LiftRobot.h"
#include "commands/endgame/RaiseHook.h"
#include "commands/scoring/RetractHood.h"
#include "commands/scoring/PrepShoot.h"
#include "commands/scoring/Shoot.h"
#include "commands/scoring/Feed.h"
#include "commands/scoring/FeederUnblock.h"
#include "commands/intake/ChangeIntakePosition.h"
#include "commands/intake/TakeCell.h"
#include "commands/intake/EmergencyIntake.h"
#include "commands/scoring/ShootGroup.h"
#include "commands/scoring/AutomatedShoot.h"
#include "commands/scoring/MoveTurret.h"

RobotContainer::RobotContainer() {
  m_drivetrain.SetDefaultCommand(
      Drive([this] { return -m_driverController.GetY(frc::GenericHID::JoystickHand::kLeftHand); },
            [this] { return m_driverController.GetX(frc::GenericHID::JoystickHand::kRightHand); },
            &m_drivetrain));

  ConfigureControls();
}

void RobotContainer::ConfigureControls() {
  //########## Xbox Controller ##########
  // Intake buttons
  j_bumperRightButton.WhenPressed(ChangeIntakePosition(&m_intake, &m_drivetrain));
  j_axisRightTrigger.WhileActiveContinous(TakeCell(&m_intake));

  // Shooter buttons
  // j_axisRightTrigger.WhileActiveContinous(ShootGroup(&m_shooter, &m_feeder, &m_drivetrain,
  //                                                   &m_intake, &m_controlPanelManipulator,
  //                                                   &m_turret, &m_adjustableHood, 1));
  j_axisLeftTrigger.WhileActiveContinous(frc2::ParallelCommandGroup(Feed(&m_feeder, &m_intake)));

  j_bumperLeftButton.ToggleWhenActive(
      frc2::ParallelCommandGroup(PrepShoot(1, &m_shooter, &m_feeder, &m_drivetrain, &m_intake,
                                           &m_controlPanelManipulator, &m_adjustableHood),
                                 MoveTurret(&m_turret)));

  // AdjustableHood buttons
  // ------  desactiver pour maintenance Volet
  j_POV0Deg.WhenPressed(RetractHood(&m_adjustableHood, 40));
  j_POV90Deg.WhenPressed(RetractHood(&m_adjustableHood, 30));
  j_POV180Deg.WhenPressed(RetractHood(&m_adjustableHood, 0));
  // ------  desactiver pour maintenance Volet

  // Winch Buttons
  j_yButton.WhenPressed(LiftRobot(&m_winch));
  j_xButton.WhileHeld(DropRobot(&m_winch));

  // TelescopicArm Buttons
  j_aButton.WhileHeld(DropHook(&m_telescopicArm, &m_intake));
  j_bButton.WhileHeld(RaiseHook(&m_telescopicArm, &m_intake));

  // AutomatedShoot Buttons
  // j_aButton.WhileActiveOnce(AutomatedShoot(&m_drivetrain));

  //########## Panel ##########
  // p_redButton.WhileHeld(EmergencyIntake(&m_intake, &m_drivetrain), false);
  // p_yellowButton.WhileHeld(FeederUnblock(&m_feeder));

  p_redButton.WhileHeld([this] {
    if (m_adjustableHoodAngle < 45) {
      m_adjustableHoodAngle += 0.5;
    } else {
      m_adjustableHoodAngle = 45.0;
    }
  });
  p_whiteButton.WhileHeld([this] {
    if (m_adjustableHoodAngle > 0) {
      m_adjustableHoodAngle -= 0.5;
    } else {
      m_adjustableHoodAngle = 0.0;
    }
  });
  p_yellowButton.WhenPressed(
      [this] {
        m_adjustableHood.Enable();
        m_adjustableHood.SetSetpoint(m_adjustableHoodAngle);
      },
      {&m_adjustableHood});

  (p_whiteButton && p_redButton).WhenActive([this] { m_adjustableHoodAngle = 0.0; });
}
