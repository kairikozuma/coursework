#!/usr/bin/env python

'''
Copyright (c) 2015, Mark Silliman
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
'''

# A very basic TurtleBot script that moves TurtleBot forward indefinitely. Press CTRL + C to stop.  To run:
# On TurtleBot:
# roslaunch turtlebot_bringup minimal.launch
# On work station:
# python goforward.py

import roslib
import rospy
from geometry_msgs.msg import Twist
from kobuki_msgs.msg import BumperEvent
from math import radians

class Move():
    STATE_STOPPED = 0
    STATE_FORWARD = 1
    STATE_TURN    = 2
    STATE_BACKWARD = 3

    def __init__(self):
        # initiliaze
        rospy.init_node('Move', anonymous=False)

	#monitor kobuki's bumper events
	rospy.Subscriber("/mobile_base/events/bumper",BumperEvent,self.BumperEventCallback)

	# tell user how to stop TurtleBot
	rospy.loginfo("To stop TurtleBot CTRL + C")

        # What function to call when you ctrl + c    
        rospy.on_shutdown(self.shutdown)
        
	# Create a publisher which can "talk" to TurtleBot and tell it to move
        # Tip: You may need to change cmd_vel_mux/input/navi to /cmd_vel if you're not using TurtleBot2
        self.cmd_vel = rospy.Publisher('cmd_vel_mux/input/navi', Twist, queue_size=10)
     
	#TurtleBot will stop if we don't keep telling it to move.  How often should we tell it to move? 10 HZ
        r =rospy.Rate(10);

        # Twist is a datatype for velocity
        move_cmd = Twist()
	# let's go forward at 0.2 m/s
        move_cmd.linear.x = 0.2
	# let's turn at 0 radians/s
	move_cmd.angular.z = 0
	
        #stop_cmd: By default, new instance has velocity 0
	stop_cmd = Twist()
        
        #turn_cmd: let's turn at 45 deg/s
        turn_cmd = Twist()
        turn_cmd.linear.x = 0
        turn_cmd.angular.z = radians(45) #45 deg/s in radians/s
 
        # backward_cmd
        backward_cmd = Twist()
        # backward speed at -0.2 m/s
        backward_cmd.linear.x = -0.2
        # let's turn at 0 radians/s
        backward_cmd.angular.z = 0

        # self. refers to an instance of a class
        # Move. refers to the class variable
	self.BUMPER_STATE = Move.STATE_FORWARD

	while not rospy.is_shutdown():
		# publish the velocity
		if (self.BUMPER_STATE == Move.STATE_FORWARD):
                        rospy.loginfo("In moving state")
                        self.cmd_vel.publish(move_cmd)
                        r.sleep()
		elif (self.BUMPER_STATE == Move.STATE_STOPPED):
			for x in range(0,20):
                            self.cmd_vel.publish(stop_cmd)
			    rospy.loginfo("Waiting ...")
	  		    r.sleep()
                        self.BUMPER_STATE = Move.STATE_BACKWARD
		elif (self.BUMPER_STATE == Move.STATE_TURN):
                        # turn 180 degrees
	                rospy.loginfo("Turning")
                        for x in range(0,25):
                            self.cmd_vel.publish(turn_cmd)
                            r.sleep()  
                        self.BUMPER_STATE = Move.STATE_FORWARD
                        for x in range(0,10):
                            r.sleep()
                else:
                        # STATE_BACKWARD
                        # move back for 0.2 second
                        for x in range(0,2):
                            self.cmd_vel.publish(backward_cmd)
                            r.sleep()
                        for x in range(0,10):
                            # wait for 0.2 s after backing off
                            r.sleep()
                        self.BUMPER_STATE = Move.STATE_TURN
		r.sleep()
                        
    def BumperEventCallback(self,data):
		if (data.state == BumperEvent.PRESSED):
			# sleep for 2 seconds, if the bumper is pressed
			# publish the velocity
			r =rospy.Rate(1)
			rospy.loginfo("Bumper")
			self.BUMPER_STATE = Move.STATE_STOPPED
			#for x in range(0,2):
			#    rospy.loginfo("Waiting ...")
	  		#    r.sleep()
			#self.BUMPER_STATE = Move.STATE_BACKWARD
       
    def shutdown(self):
        # stop turtlebot
        rospy.loginfo("Stop TurtleBot")
	# a default Twist has linear.x of 0 and angular.z of 0.  So it'll stop TurtleBot
        self.cmd_vel.publish(Twist())
	# sleep just makes sure TurtleBot receives the stop command prior to shutting down the script
        rospy.sleep(1)
 
if __name__ == '__main__':
    try:
        Move()
    except:
        rospy.loginfo("Move node terminated.")

