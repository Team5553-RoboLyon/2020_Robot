/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "subsystems/Drivetrain.h"

#include <frc/smartdashboard/SmartDashboard.h>

Drivetrain::Drivetrain() {
  m_moteurDroite.RestoreFactoryDefaults();
  m_moteurGauche.RestoreFactoryDefaults();
  m_moteurDroiteFollower.RestoreFactoryDefaults();
  m_moteurGaucheFollower.RestoreFactoryDefaults();

  m_moteurDroite.SetIdleMode(kIdleMode);
  m_moteurDroiteFollower.SetIdleMode(kIdleMode);
  m_moteurGauche.SetIdleMode(kIdleMode);
  m_moteurGaucheFollower.SetIdleMode(kIdleMode);

  m_moteurDroite.SetOpenLoopRampRate(kOpenLoopRampeRate);
  m_moteurGauche.SetOpenLoopRampRate(kOpenLoopRampeRate);
  m_moteurDroiteFollower.SetOpenLoopRampRate(kOpenLoopRampeRate);
  m_moteurGaucheFollower.SetOpenLoopRampRate(kOpenLoopRampeRate);

  m_moteurDroiteFollower.Follow(m_moteurDroite);
  m_moteurGaucheFollower.Follow(m_moteurGauche);

  m_moteurGauche.SetInverted(true);

  ResetEncodeurs();
  SetVitesse(GearRatio::kHigh);

  EnableLogFile(false);
}

void Drivetrain::Periodic() {
  frc::SmartDashboard::PutNumber("Droite1", m_encodeurDroite1.GetPosition());
  frc::SmartDashboard::PutNumber("Droite2", m_encodeurDroite2.GetPosition());
  frc::SmartDashboard::PutNumber("Gauche1", m_encodeurGauche1.GetPosition());
  frc::SmartDashboard::PutNumber("Gauche2", m_encodeurGauche2.GetPosition());

  if (m_isLogFileEnabled) {
    m_logFile->Log(GetEncodeurDroit(), GetEncodeurGauche());
  }
}

void Drivetrain::EnableLogFile(bool enable) {
  if (!m_isLogFileEnabled && enable) {
    m_logFile = new CSVLogFile("log", "Droite", "Gauche");
  } else if (m_isLogFileEnabled && !enable) {
    delete m_logFile;
  }
  m_isLogFileEnabled = enable;
}

void Drivetrain::SetVitesse(GearRatio gearRatio) {
  if (gearRatio == GearRatio::kLow) {
    m_ballshifter.Set(frc::DoubleSolenoid::Value::kForward);
  } else {
    m_ballshifter.Set(frc::DoubleSolenoid::Value::kReverse);
  }
  m_gearRatio = gearRatio;
  SetPositionConversionFactor(gearRatio);
}

void Drivetrain::ChangerVitesse() {
  if (m_gearRatio == GearRatio::kLow) {
    SetVitesse(GearRatio::kHigh);
  } else {
    SetVitesse(GearRatio::kLow);
  }
}

void Drivetrain::Drive(double droite, double gauche) {
  m_moteurDroite.Set(droite);
  m_moteurGauche.Set(gauche);
}

void Drivetrain::ResetEncodeurs() {
  m_encodeurDroite1.SetPosition(0.0);
  m_encodeurDroite2.SetPosition(0.0);
  m_encodeurGauche1.SetPosition(0.0);
  m_encodeurGauche2.SetPosition(0.0);
}

void Drivetrain::SetPositionConversionFactor(GearRatio gearRatio) {
  if (gearRatio == GearRatio::kLow) {
    m_encodeurDroite1.SetPositionConversionFactor(kLowGearPositionConversionFactor);
    m_encodeurDroite2.SetPositionConversionFactor(kLowGearPositionConversionFactor);
    m_encodeurGauche1.SetPositionConversionFactor(kLowGearPositionConversionFactor);
    m_encodeurGauche2.SetPositionConversionFactor(kLowGearPositionConversionFactor);
  } else {
    m_encodeurDroite1.SetPositionConversionFactor(kHighGearPositionConversionFactor);
    m_encodeurDroite2.SetPositionConversionFactor(kHighGearPositionConversionFactor);
    m_encodeurGauche1.SetPositionConversionFactor(kHighGearPositionConversionFactor);
    m_encodeurGauche2.SetPositionConversionFactor(kHighGearPositionConversionFactor);
  }
}

units::meter_t Drivetrain::GetEncodeurDroit() {
  return units::meter_t((m_encodeurDroite1.GetPosition() + m_encodeurDroite2.GetPosition()) / 2.0);
}

units::meter_t Drivetrain::GetEncodeurGauche() {
  return units::meter_t((m_encodeurGauche1.GetPosition() + m_encodeurGauche2.GetPosition()) / 2.0);
}