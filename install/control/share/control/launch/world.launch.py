
import os

from ament_index_python.packages import get_package_share_directory


from launch import LaunchDescription
from launch.actions import ExecuteProcess, IncludeLaunchDescription, RegisterEventHandler
from launch.event_handlers import OnProcessExit
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch.substitutions import PathJoinSubstitution , FindExecutable
from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import Node
from launch.actions import TimerAction
import xacro


def generate_launch_description():
	gazebo = IncludeLaunchDescription(
				PythonLaunchDescriptionSource([os.path.join(
					get_package_share_directory('gazebo_ros'), 'launch'), '/gazebo.launch.py']),
				)

	pkg_aubo=get_package_share_directory('aubo')
	aubo = IncludeLaunchDescription(
		PythonLaunchDescriptionSource(
		    os.path.join(pkg_aubo, 'launch', 'aubo.launch.py'),
		)
	    )
	    
	pkg_argo=get_package_share_directory('argo')
	argo = IncludeLaunchDescription(
		PythonLaunchDescriptionSource(
		    os.path.join(pkg_argo, 'launch', 'argo.launch.py'),
		)
	    )
        
	ExecuteProcess(
		cmd=[[
			FindExecutable(name='ros2'),
			"/apply_link_wrench",
			"gazebo_msgs/srv/ApplyLinkWrench",
 			'"{link_name: "shoulder_Link", reference_frame: "", reference_point: { x: 100, y: 0, z: 0 }, wrench: { force: { x: 10, y: 0, z: 0 }, torque: { x: 0, y: 0, z: 0 } }, start_time: {sec: 0, nanosec: 0}, duration: {sec: -1, nanosec: 0} }"',
		]],
    )
     

	return LaunchDescription([
		gazebo,
		argo,
		TimerAction(period=15.0,
		actions=[aubo])
    	])
