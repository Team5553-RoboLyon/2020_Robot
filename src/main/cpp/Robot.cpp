/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

#include <frc2/command/CommandScheduler.h>
#include "cameraserver/CameraServer.h"
#include "frc/smartdashboard/SmartDashboard.h"

void Robot::RobotInit() {
  camera = frc::CameraServer::GetInstance()->StartAutomaticCapture();
  camera.SetResolution(320, 240);
  camera.SetFPS(30);
}

void Robot::RobotPeriodic() {
  frc2::CommandScheduler::GetInstance().Run();
  frc::SmartDashboard::PutNumber("Angle Hood :", m_container.m_adjustableHoodAngle);
}

void Robot::DisabledInit() {
  m_container.m_drivetrain.SetIdleMode(rev::CANSparkMax::IdleMode::kCoast);
  // Remet à zéro le controlleur le PID ( dont le setpoint ) du volet
  m_container.m_adjustableHood.Disable();
}

void Robot::DisabledPeriodic() {}

void Robot::AutonomousInit() {
  m_container.m_intake.Close();
  m_container.m_drivetrain.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
  m_container.m_adjustableHood.ResetEncoder();
}

void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() {
  m_container.m_intake.Close();
  m_container.m_drivetrain.SetIdleMode(rev::CANSparkMax::IdleMode::kBrake);
  m_container.m_adjustableHood.ResetEncoder();
}

void Robot::TeleopPeriodic() {}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
