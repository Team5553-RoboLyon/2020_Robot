/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/driving/Drive.h"

#include "lib/Utils.h"

#define TRACKWIDTH 0.61f
#define HALF_TRACKWIDTH (TRACKWIDTH / 2.0f)

#define AMAX 5.1  // Acceleration Max  au PIF .. à définir aux encodeurs
#define VMAX 3.4  // vitesse Max  théorique (3,395472 sur JVN-DT) .. à vérifier aux encodeurs
#define WMAX                     \
  (((2.0 * VMAX) / TRACKWIDTH) / \
   1.7)  // vitesse angulaire Max theorique	.. à modifier avec Garice

#define NABS(a) (((a) < 0) ? -(a) : (a))      // VALEUR ABSOLUE
#define NMAX(a, b) (((a) > (b)) ? (a) : (b))  // Max
#define NMIN(a, b) (((a) < (b)) ? (a) : (b))  // Min

Drive::Drive(std::function<double()> forward, std::function<double()> turn, Drivetrain* drivetrain)
    : m_forward(forward), m_turn(turn), m_drivetrain(drivetrain) {
  AddRequirements(m_drivetrain);
}

void Drive::Initialize() {}

void Drive::Execute() {
  double forward = utils::Deadband(m_forward());
  double turn = utils::Deadband(m_turn());

  /*
  double c = 0.35 * (turn * 5.0 * (std::abs(turn) + 1) / (std::abs(forward) + 1));
  if (turn < 0.0) {
    m_drivetrain->Drive(forward * ((c + 1) / (1 - c)), forward);

  } else {
    m_drivetrain->Drive(forward, forward * ((1 - c) / (c + 1)));
  }*/

  double v = forward * VMAX;
  double w = turn * WMAX;

  // w = m_drivetrain->CalculateTurn(forward, w);

  double lwheel = v + (w * HALF_TRACKWIDTH);
  double rwheel = v - (w * HALF_TRACKWIDTH);

  double k;
  k = 1.0 / (NMAX(VMAX, NMAX(NABS(lwheel), NABS(rwheel))));
  lwheel *= k;
  rwheel *= k;

  m_drivetrain->Drive(lwheel, rwheel);
  // m_drivetrain->Drive(forward + 0.5 * turn, forward - 0.5 * turn);
}

void Drive::End(bool interrupted) {}

bool Drive::IsFinished() { return false; }
