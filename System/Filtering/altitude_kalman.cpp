#include "altitude_kalman.h"

AltitudeKalman::AltitudeKalman(double dt) : m_dt(dt) {}

void AltitudeKalman::Predict(const double az) {
    // x_k+1 = A x_k + B u_k
    // x_k+1 = [pos] = [old pos + vel * m_dt] + [1/2 acceleration m_dt^2]
    //         [vel]   [old vel           ]   [acceleration * m_dt    ]
    xHat.estimatedAltitude += xHat.estimatedVelocity * m_dt + 1/2 * az * m_dt * m_dt;
    xHat.estimatedVelocity += az * m_dt;
}

void AltitudeKalman::Correct(const double baroAltitude, const double kalman_gain[2]) {
    // xhat = xhat + K(y-Cx-Du)
    // x_corrected = [old pos] + [k1] * [ [barometric altitude] - [1, 0] * [estimated pos] ]
    //               [old vel]   [k2]   [                                  [estimated vel] ]

    // The delta between y (where we think we are) and xhat (where we actually are) with
    // all the zero terms omitted is actually pretty clean!
    double deltaAltitude = baroAltitude - xHat.estimatedAltitude;
    
    xHat.estimatedAltitude += kalman_gain[0] * deltaAltitude;
    xHat.estimatedVelocity += kalman_gain[1] * deltaAltitude;
}

const AltitudeKalmanOutput_t AltitudeKalman::GetXhat() const {
    return xHat;
}

void AltitudeKalman::SetDt(double dt) {
    m_dt = dt;
}