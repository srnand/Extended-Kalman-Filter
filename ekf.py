import numpy as np
import random
import math
import matplotlib.pyplot as plt

def true_data():
	dl=0.1
	theta=45.*np.pi/180.
	vel=1.67

	data=[[dl,theta,vel]]
	for i in range(1,50):
		dl= data[i-1][0]
		theta = data[i-1][1]
		vel = data[i-1][2]
		dt = 1./30.
		dl+= vel*dt*math.cos(theta)
		data.append([dl,theta,vel])

	return data

def sense(data):
	sensor_data=[]
	for i in data:
		sensor_data.append(i[0]+random.uniform(-1,1)/5.)
	return sensor_data

def initial_state(data):
	P = np.diag([0.1, 10.*np.pi/180., 0.5])**2
	state = [data[0], 0, 0]

	return [state, P]

def predict(state):
	dt = 1./30.

	# print state
	theta = state[1]

	vel = state[2]

	new_state = [state[0]+dt*vel*math.cos(theta),theta,vel]

	temp = [1, (dt*vel*-1.*math.sin(theta)),(dt*math.cos(theta))]

	J = [temp,[0,1,0],[0,0,1]]

	return [new_state, J]

def sysh(state):
	H=[1, 0, 0]
	return [np.array(H)*np.array(state),np.array(H)]

# print sense(true_data())

Qk = np.diag(np.array([0.1, 10.*np.pi/180., 0.5])**2*1./30)
R = np.array([0.05])
# print Qk


data = true_data()
ground_truth=[data[0][0]]
sensor_data = sense(data)
[state, P]= initial_state(sensor_data)
predicted=[state[0]]
# print state

for i in range(1,50):
	[new_state, J] = predict(state)
	[pred_Z,H] = sysh(new_state)
	newP = np.array(J)*np.array(P)*np.transpose(np.array(J)) + Qk

	residual = sensor_data[i] - pred_Z

	# print H*(newP*np.transpose(np.matrix(H)))

	K = np.matrix(newP) * np.transpose(np.matrix(H)) * np.linalg.inv(np.matrix(H)*np.matrix(newP)*np.transpose(np.matrix(H)) + np.matrix(R))

	# print K

	P_updated = np.identity(len(K)) - np.matrix(K) * np.matrix(H)*np.matrix(newP)

	state_updated = np.array(new_state) + np.matrix(K) * residual

	state = np.array(state_updated[0])[0]
	# print state_updated
	# print state.tolist()[0],sensor_data[i]
	predicted.append(state.tolist()[0])
	ground_truth.append(data[i][0])
	P = P_updated

# plt.plot(ground_truth)
plt.plot(sensor_data[1:],label="sensor_data")
plt.plot(predicted,label="predicted_values")
plt.ylabel("distance")
plt.legend()
plt.show()


