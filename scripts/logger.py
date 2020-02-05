#!/usr/bin/env python
# Copyright 2017 Masahiro Kato
# Copyright 2017 Ryuichi Ueda
# Released under the BSD License.

import rospy, rosbag, rosparam
import math, sys, random, datetime
from geometry_msgs.msg import Twist
from std_srvs.srv import Trigger, TriggerResponse
from raspimouse_ros_2.msg import LightSensorValues, ButtonValues
from raspimouse_gamepad_teach_and_replay.msg import Event
from sensor_msgs.msg import LaserScan

#left_ang = [3, 15, 20, 30, 40, 45, 60, 75, 90, 120]
#right_ang = [3, 15, 20, 30, 40, 45, 60, 75, 90, 120]
#left_ang = [13, 40, 67, 93, 120]
#right_ang = [13, 40, 67, 93, 120]
#left_ang = [2, 5, 9, 12, 16, 19, 23, 26, 30, 33, 37, 40, 43, 47, 50, 54, 57, 61, 64, 68, 71, 75, 78, 82, 85, 89, 92, 96, 99, 103, 106, 110, 113, 117, 120]
#right_ang = [2, 5, 9, 12, 16, 19, 23, 26, 30, 33, 37, 40, 43, 47, 50, 54, 57, 61, 64, 68, 71, 75, 78, 82, 85, 89, 92, 96, 99, 103, 106, 110, 113, 117, 120]
left_ang = [2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38, 41, 44, 47, 50, 53, 56, 59, 62, 65, 68, 71, 74, 77, 81, 84, 87, 90, 93, 96, 99, 102, 105, 108, 111, 114, 117, 120]
right_ang = [2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38, 41, 44, 47, 50, 53, 56, 59, 62, 65, 68, 71, 74, 77, 81, 84, 87, 90, 93, 96, 99, 102, 105, 108, 111, 114, 117, 120]
#left_ang = [1, 3, 5, 7, 9, 11, 13.1, 15.1, 17.1, 19.2, 21.2, 23.2, 25.2, 27.2, 29.2, 31.3, 33.3, 35.3, 37.3, 39.3, 41.3, 43.4, 45.4, 47.4, 49.4, 51.4, 53.4, 55.5, 57.5, 59.5, 61.5, 63.5, 65.5, 67.6, 70, 71.6, 73.6, 75.6, 77.6, 79.7, 81.7, 83.7, 85.7, 87.7, 89.7, 91.8, 93.8, 95.8, 97.8, 99.8, 101.8, 103.9, 105.9, 107.9, 109.9, 111.9, 113.9, 116, 118, 120]
#right_ang = [1, 3, 5, 7, 9, 11, 13.1, 15.1, 17.1, 19.2, 21.2, 23.2, 25.2, 27.2, 29.2, 31.3, 33.3, 35.3, 37.3, 39.3, 41.3, 43.4, 45.4, 47.4, 49.4, 51.4, 53.4, 55.5, 57.5, 59.5, 61.5, 63.5, 65.5, 67.6, 70, 71.6, 73.6, 75.6, 77.6, 79.7, 81.7, 83.7, 85.7, 87.7, 89.7, 91.8, 93.8, 95.8, 97.8, 99.8, 101.8, 103.9, 105.9, 107.9, 109.9, 111.9, 113.9, 116, 118, 120]

class Logger():
    def __init__(self):
        self.sensor_values = LaserScan()
        self.cmd_vel = Twist()

        self._decision = rospy.Publisher('/event',Event,queue_size=100)
	rospy.Subscriber('/buttons', ButtonValues, self.button_callback, queue_size=1)
#        rospy.Subscriber('/lightsensors', LightSensorValues, self.sensor_callback)
        rospy.Subscriber('/cmd_vel', Twist, self.cmdvel_callback)
        rospy.Subscriber('/scan', LaserScan, self.sensor_callback)


	self.on = False
	self.bag_open = False

    def button_callback(self,msg):
        self.on = msg.front_toggle

    def sensor_callback(self,messages):
        self.sensor_values = messages

    def cmdvel_callback(self,messages):
        self.cmd_vel = messages

    def output_decision(self):
	if not self.on:
	    if self.bag_open:
		self.bag.close()
		self.bag_open = False
	    return
	else:
	    if not self.bag_open:
		filename = datetime.datetime.today().strftime("%Y%m%d_%H%M%S") + '.bag'
		rosparam.set_param("/current_bag_file", filename)
		self.bag = rosbag.Bag(filename, 'w')
		self.bag_open = True

	s = self.sensor_values
	a = self.cmd_vel
	e = Event()

	
	# rf = int((-math.pi*3.0/180 - s.angle_min)/s.angle_increment); 
	# lf = int((math.pi*3.0/180 - s.angle_min)/s.angle_increment); 
	# rs = int((-math.pi*45.0/180 - s.angle_min)/s.angle_increment); 
      	# ls = int((math.pi*45.0/180 - s.angle_min)/s.angle_increment); 
# 
      	# e.left_side = 500.0 if math.isnan(s.ranges[ls]) else s.ranges[ls]*1000;
      	# e.right_side = 500.0  if math.isnan(s.ranges[rs]) else s.ranges[rs]*1000;
      	# e.left_forward = 500.0 if math.isnan(s.ranges[lf]) else s.ranges[lf]*1000;
      	# e.right_forward = 500.0 if math.isnan(s.ranges[rf]) else s.ranges[rf]*1000;
      	# e.linear_x = a.linear.x
      	# e.angular_z = a.angular.z
	# r1 = int((-math.pi*3.0/180 - s.angle_min)/s.angle_increment); 
	# r2 = int((-math.pi*15.0/180 - s.angle_min)/s.angle_increment); 
	# r3 = int((-math.pi*20.0/180 - s.angle_min)/s.angle_increment); 
	# r4 = int((-math.pi*30.0/180 - s.angle_min)/s.angle_increment); 
	# r5 = int((-math.pi*40.0/180 - s.angle_min)/s.angle_increment); 
	# r6 = int((-math.pi*45.0/180 - s.angle_min)/s.angle_increment); 
	# r7 = int((-math.pi*60.0/180 - s.angle_min)/s.angle_increment); 
	# r8 = int((-math.pi*75.0/180 - s.angle_min)/s.angle_increment); 
	# r9 = int((-math.pi*90.0/180 - s.angle_min)/s.angle_increment); 
	# r10 = int((-math.pi*120.0/180 - s.angle_min)/s.angle_increment); 
	# l1 = int((math.pi*3.0/180 - s.angle_min)/s.angle_increment); 
	# l2 = int((math.pi*15.0/180 - s.angle_min)/s.angle_increment); 
	# l3 = int((math.pi*20.0/180 - s.angle_min)/s.angle_increment); 
	# l4 = int((math.pi*30.0/180 - s.angle_min)/s.angle_increment); 
	# l5 = int((math.pi*40.0/180 - s.angle_min)/s.angle_increment);
	# l6 = int((math.pi*45.0/180 - s.angle_min)/s.angle_increment); 
	# l7 = int((math.pi*60.0/180 - s.angle_min)/s.angle_increment); 
	# l8 = int((math.pi*75.0/180 - s.angle_min)/s.angle_increment); 
	# l9 = int((math.pi*90.0/180 - s.angle_min)/s.angle_increment); 
	# l10 = int((math.pi*120.0/180 - s.angle_min)/s.angle_increment);

	# r = [0] * len(right_ang)
	# l = [0] * len(left_ang)
	r = []
	l = []
	for angle in left_ang:
		n = int((math.pi*angle/180 - s.angle_min)/s.angle_increment)
		l.append(n)
	for angle in right_ang:
		n = int((-math.pi*angle/180 - s.angle_min)/s.angle_increment)
		r.append(n)
	#print(r4)

    # e.left_1 = 4100.0 if math.isnan(s.ranges[l1]) else s.ranges[l1]*1000;
    # e.left_2 = 4100.0 if math.isnan(s.ranges[l2]) else s.ranges[l2]*1000;
    # e.left_3 = 4100.0 if math.isnan(s.ranges[l3]) else s.ranges[l3]*1000;
    # e.left_4 = 4100.0 if math.isnan(s.ranges[l4]) else s.ranges[l4]*1000;
    # e.left_5 = 4100.0 if math.isnan(s.ranges[l5]) else s.ranges[l5]*1000;
    # e.left_6 = 4100.0 if math.isnan(s.ranges[l6]) else s.ranges[l6]*1000;
    # e.left_7 = 4100.0 if math.isnan(s.ranges[l7]) else s.ranges[l7]*1000;
    # e.left_8 = 4100.0 if math.isnan(s.ranges[l8]) else s.ranges[l8]*1000;
    # e.left_9 = 4100.0 if math.isnan(s.ranges[l9]) else s.ranges[l9]*1000;
    # e.left_10 = 4100.0 if math.isnan(s.ranges[l10]) else s.ranges[l10]*1000;
    # e.right_1 = 4100.0 if math.isnan(s.ranges[r1]) else s.ranges[r1]*1000;
    # e.right_2 = 4100.0 if math.isnan(s.ranges[r2]) else s.ranges[r2]*1000;
    # e.right_3 = 4100.0 if math.isnan(s.ranges[r3]) else s.ranges[r3]*1000;
    # e.right_4 = 4100.0 if math.isnan(s.ranges[r4]) else s.ranges[r4]*1000;
    # e.right_5 = 4100.0 if math.isnan(s.ranges[r5]) else s.ranges[r5]*1000;
    # e.right_6 = 4100.0 if math.isnan(s.ranges[r6]) else s.ranges[r6]*1000;
    # e.right_7 = 4100.0 if math.isnan(s.ranges[r7]) else s.ranges[r7]*1000;
    # e.right_8 = 4100.0 if math.isnan(s.ranges[r8]) else s.ranges[r8]*1000;
    # e.right_9 = 4100.0 if math.isnan(s.ranges[r9]) else s.ranges[r9]*1000;
    # e.right_10 = 4100.0 if math.isnan(s.ranges[r10]) else s.ranges[r10]*1000;
    # e.linear_x = a.linear.x
    # e.angular_z = a.angular.z

	#e.left.clear()
	#e.right.clear()
	for data in l:
		lf = 4100.0 if math.isnan(s.ranges[data]) else s.ranges[data]*1000
		if lf > 4100.0:
			lf = 4100.0
		e.left.append(lf)
	for data in r:
		rg = 4100.0 if math.isnan(s.ranges[data]) else s.ranges[data]*1000
		if rg > 4100.0:
			rg = 4100.0
		e.right.append(rg)
	
	e.linear_x = a.linear.x
	e.angular_z = a.angular.z

      	print(e)

      	self._decision.publish(e)
	self.bag.write('/event', e)

    def run(self):
        rate = rospy.Rate(10)
        data = Twist()

        while not rospy.is_shutdown():
            self.output_decision()
            rate.sleep()

if __name__ == '__main__':
    rospy.init_node('logger')
    Logger().run()
