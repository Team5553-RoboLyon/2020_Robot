#pragma once

#include <frc2/command/SubsystemBase.h>

#include "Constants.h"

class Shooter : public frc2::SubsystemBase {
 public:

    Shooter();

    void Shoot(double puissance);
    void Periodic();
 private:
};