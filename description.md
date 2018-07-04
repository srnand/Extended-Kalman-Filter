# Extended Kalman Filter

Applied EKF on a simple example of a bug moving at an angle from the sink at a constant velocity.

The state of the System is a vector consisting of 3 elements. 
1. Lateral distance of the bug from the sink.
2. The angle at which it is moving
3. The velocity of the bug.

The velocity and the angle is constant, the only parameter changing is the lateral distance of the bug from the sink and the task is to estimate that.

The lateral distance is a function of previous lateral distance, the velocity and the cosine of the angle which in all makes the system non-linear. So we have to use EKF and not KF.

![state](http://latex.codecogs.com/gif.latex?x%28t%29%20%3D%20%5Cbegin%7Bbmatrix%7D%20d%28t%29%5C%5C%20%5CTheta%20%5C%5C%20v%20%5Cend%7Bbmatrix%7D)

![next_state](http://latex.codecogs.com/gif.latex?x%28t%29%20%3D%20%5Cbegin%7Bbmatrix%7D%20d%28t&plus;1%29%3Dd%28t%29&plus;v*%5CDelta%20t*cos%28%5CTheta%20%29%5C%5C%20%5CTheta%20%5C%5C%20v%20%5Cend%7Bbmatrix%7D)

Since the equations are non-linear, we need to calculate Jacobian based on the three equations of the state.

![Jacobian](http://latex.codecogs.com/gif.latex?J%3D%5Cbegin%7Bbmatrix%7D%201%20%26-v*%5CDelta%20t*sin%28%5CTheta%20%29%20%26%5CDelta%20t*cos%28%5CTheta%20%29%20%5C%5C%200%261%20%260%20%5C%5C%200%26%200%26%201%20%5Cend%7Bbmatrix%7D)

And since the only parameter to be predicted is the lateral distance, the H matrix will be a 1x3 matrix with first value 1 and rest 0s.

As in figure, the blue dot is the bug moving at a constant angle and velocity.
<img src="bug.png" width=600 height=400 >
<br/>
References:

[Simple Example of Applying Extended Kalman Filter](https://www.researchgate.net/publication/273381901_Simple_Example_of_Applying_Extended_Kalman_Filter?enrichId=rgreq-e2f865965156e5c797628f76986b5d07-XXX&enrichSource=Y292ZXJQYWdlOzI3MzM4MTkwMTtBUzoyMDU2NzkyOTkxNzQ0MDlAMTQyNjA0OTE3NDY1MQ%3D%3D&el=1_x_2&_esc=publicationCoverPdf)
<br/>
[Kalman Filter: Predict, Measure, Update, Repeat](https://medium.com/@tjosh.owoyemi/kalman-filter-predict-measure-update-repeat-20a5e618be66)
