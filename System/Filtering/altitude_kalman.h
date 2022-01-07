// All these are from Python
// and are fixed per-DT
// TODO how should we do this?

// These numbers are for a dt of 0.01513 seconds, Q of diag([0.5, 1]).^2 and R of diag([10]).^2
static constexpr double DEFAULT_KALMAN_GAIN[2] = {0.1657, 0.0934}; 

typedef struct {
    double estimatedVelocity;
    double estimatedAltitude;
} AltitudeKalmanOutput_t;


/*

==== Introduction ====

A Kalman filter is a filter that can combine multiple sensor readings using the math
that describes the motion of a thing (it's dynamics). In our case, we
know the upwards acceleration of the rocket, and how that affects its position and velocity. We
can also directly measure it's position. We can write the following equation, which would give
us a new position and velocity given a known old position, old velocity, and upwards acceleration:

new position = old position + velocity * time + 1/2 acceleration time^2
new velocity = old velocity + acceleration * time

In the ~modern controls nomenclature~, this means we have a "system" with two "states"
(position and velocity), one "input" (acceleration), and one "output" (altitude).
In this
nomenclature, the input is what we put into the system (thrust) and the output is what we can
measure (barometric pressure -> position). This also means we can estimate the values of states
we can't directly measure, like velocity!

Nomenclature:
- state: a thing that describes what our "system" is currently doing. For example, our rocket's
	position and velocity.
- input: the thing(s) we can "put into" the system to make it do what we want. For us, acceleration
- output: Something we can measure. For example, we can measure the altitude of our rocket, but
 	it's more difficult to measure its velocity
- x: a column vector describing the system's current state. One entry for each state.
	for us, that's [[position], [velocity]]. It's dimensions are states x 1.
- x-hat: our state, but there's a hat to tell us it's just an estimate of our true state
- u, the control input vector. For us, that's just [[acceleration]]. It's dimensions
	are inputs x 1.
- y: output vector, with one element for each thing we can measure. For us, [[position]].
	It's dimensions are outputs x 1.
- System: A general term for the whole thing we're concerned with. In our case, it's the rocket and its dynamics

References:
https://file.tavsys.net/control/controls-engineering-in-frc.pdf
https://docs.wpilib.org/en/stable/docs/software/advanced-controls/state-space/state-space-observers.html

==== Kalman Filters ====

In a Kalman filter there are two steps -- prediction and correction. In predict, the estimate
of where we are (x-hat) is updated based on the current inputs we're giving our system. In
~state-space notation~, we can write it as one of two forms, where _k denotes the current
timestep and _k+1 the next one.

- Continuous-time, where x-dot (the time derivative of the state x) is given as x-dot = Ax + Bu
- Discrete-time, where x_k+1 = A x_k + B u_k

A is a states x states matrix that describes how our system would evolve over time without
control input, and B is a states x inputs matrix that describes how our system responds to a
control input u. For controls people it's convienent to write systems first in continuous-time
form, but we don't have to. There's some cool math that falls out of it too, but we don't actually
care about that either.

==== Prediction ====

In our Kalman filter, we use the discrete-time form in the predict step to
update our state estimate x with what's happened over the last 15-ish milliseconds. We just have to
keep running predict() forever to make sure our estimate stays up to date!

Let's use the equations from way above to write it in matrix form. We combine our position and velocity
into one column vector for x:

x_k+1 = [new pos] = [old pos + vel * dt] + [1/2 acceleration dt^2]
        [new vel]   [old vel           ]   [acceleration * dt    ]

And factor out the constants:

x_k+1 = [new pos] = [1 dt] * [pos] + [1/2 dt^2] * [accel]
        [new vel]   [0  1]   [vel]   [dt      ]

==== Correction ===

The next bit of the Kalman filter is correction. The equation needed to get the "output"
vector of our system is given below, where y is a vector of each output we can measure:
    y = Cx + Du

Where C relates the state x to the output y, and D relates the input u to the output y.
We said we could measure position, so:

y = [position] = [1, 0] * [pos] + [0] * [acceleration]
                          [vel]

The correct step of the Kalman filter takes this output, compares it to what we thought
our output should have been, and changes our state estimate a bit to push us towards
what we measured out output to be. How much we're "pushed" is given by the kalman gain
matrix, which has dimensions states x outputs.

New state estimate = old state estimate + (kalman gain) * (difference between measured output and predicted output)

x_corrected = x-hat + kalman_gain * [y - (Cx + Du)]

x_corrected = [pos] + [k1] * [ [barometric altitude] - [1, 0] * [estimated pos] ]
              [vel]   [k2]                                      [estimated vel]

Let's analyze this in our case. It's saying if our measured altitude is above the estimated one,
we should increase our estimated position and velocity "a bit"; the opposite holds true if we think
we're too high. The discrete-time kalman gain is unique for a given delta-time.

The kalman gain is computed by "balancing" how certain we are about each state and each input to find an
optional gain. One of the nice things about this Kalman filter is that it's constant, so we can just
compute it ahead of time. We can change Q and R to change how much we "trust" our state estimate
or our measurements, respectively,, and Python can spit out a K that reflects those ratios:

Q = [position state uncertainty^2, 0]
    [0, velocity state uncertainty^2]
R = [measurement uncertainty^2]

sysc = ct.ss(np.array([[0,1],[0,0]]), np.array([[0],[1]]), np.array([1,0]), np.array([0]))
sysd = sysc.sample(dt)  # Convert to discrete-time form described above
kalman_gain, P_steady = kalmd(sysd, Q, R).

If we want to trust the barometer less, easy! Just increase the corrosponding entry in the R matrix
and run it again to spit out a new kalman_gain matrix. We know we have two states and one output, so our
kalman_gain is a 2x1 matrix. For example, if I set dt to 0.015 seconds, Q to diag([0.5, 1].^2) and
R to diag([10].^2) [numbers found by guess-and-check-what-works], the kalman_gain we spit out is:

kalman_gain = [k1] = [0.166]
              [k2]   [0.091]

In summary: for each iteration of the filter:
(1) Predict() to project our state estimate x-hat forward one timestep. This does the physics described above
(2) Correct() with our barometer measurements to incorperate our measurement of where we actually are

*/
class AltitudeKalman {
public:
    AltitudeKalman(double dt);

    // Project our state estimate x forward in time by one timestep
    // See above, but this does x_new = Ax_old + Bu
    void Predict(const double az);

    // Correct our estimate by sorta "nudging it a bit" towards where
    // we measured ourselves to actually be
    void Correct(const double baroAltitude, const double kalman_gain[2]);

    // Get the current estimated position/velocity
    const AltitudeKalmanOutput_t GetXhat() const;

    void SetDt(const double dt);

private:
    // The estimated state of the rocket. We assume we start at (0,0).
    AltitudeKalmanOutput_t xHat = {0, 0};
    
    // TODO should we calculate this every loop?
    double m_dt = 0.015;
};
