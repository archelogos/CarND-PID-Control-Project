# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

## Param tuning explanaition

In this project we have made use of a PID controller to drive a car.

To do that it has been implemented a C++ class (PID). The properties of this class
are the PID coefficients and errors.

The class also implements two methods. The first one, called UpdateError, updates the
proportional, differential and integral errors of the PID controller.

The proportional error is updated according with the new CTE value.
The differential error is the substraction between the previous CTE and the current one.
The integral error is the sum of the all CTE.

The TotalError method applies the following formula to calculate the total error of the controller.

error = -Kp * p_error - Kd * d_error - Ki * i_error

The key point in this project is to find the appropriate values for the coefficients.

Because of the total error it is calculated with three components, it is really important to
understand how each one affects to the resulting estimation.

If the proportional coefficient is too high, the car starts to oscillate and it cannot converge.

In other hand, the integral coefficient must to be the lowest because it represents the sum of all errors. If it is
too high, the car starts moving in circles.

Finally, the differential coefficient needs to be the greater one in order to detect rapid oscillations in the
CTE that could represent curves in the road.

Mentioned that, the final chosen values are:

```python
double Kp_pid_steering = 0.1;  // proportional coefficient
double Ki_pid_steering = 0.005;  // integral coefficient
double Kd_pid_steering = 4.0;  // differential coefficient
```

The car sometimes oscillates a little bit, but it quickly changes the steering angle and takes
the right direction again. Thanks to the high kd and ki values, it does not go out of the road,
even if the car is entering a curve fast.

It is also implemented a PID controller to set the throttle of the vehicle. It works fine, but not too much in comparision
with the constant throttle scenario.

Depending on the CTE and the given maximum speed, the algorithm defines a target speed. After that, the controller
calculates the error considering the CTE as the subtstraction between the current speed and the target speed.

Generally speaking, the car behaves properly the majority of the time and it does not go out of the road.
It also gets a faster speed, but it sometimes oscillates too much and it could not be considered a safe driving.

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets) == 0.13, but the master branch will probably work just fine
  * Follow the instructions in the [uWebSockets README](https://github.com/uWebSockets/uWebSockets/blob/master/README.md) to get setup for your platform. You can download the zip of the appropriate version from the [releases page](https://github.com/uWebSockets/uWebSockets/releases). Here's a link to the [v0.13 zip](https://github.com/uWebSockets/uWebSockets/archive/v0.13.0.zip).
  * If you run OSX and have homebrew installed you can just run the ./install-mac.sh script to install this
* Simulator. You can download these from the [project intro page](https://github.com/udacity/CarND-PID-Control-Project/releases) in the classroom.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`.

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./
