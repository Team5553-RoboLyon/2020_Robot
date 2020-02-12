/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "subsystems/Feeder.h"

Feeder::Feeder() {

}

void Feeder::Activate() {
    m_moteur.Set(kFeederMoteurSpeed);
}

void Feeder::Stop() {
    m_moteur.StopMotor();
}