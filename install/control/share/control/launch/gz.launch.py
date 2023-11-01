
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
    gazebo_ros2_control_demos_path = os.path.join(get_package_share_directory('aubo'))

    xacro_file = os.path.join(gazebo_ros2_control_demos_path,
                              'urdf',
                              'aubo_i5.urdf')
    doc = xacro.parse(open(xacro_file))
    xacro.process_doc(doc)
    params = {'robot_description': doc.toxml(),'use_sim_time': True}

    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[params]
    )

    spawn_entity  = Node(package='gazebo_ros',
        executable='spawn_entity.py',
                        arguments=['-topic', 'robot_description',
                                   '-entity', 'aubo'],
                        output='screen')

    
    spawn_entity_1 = Node (package='gazebo_ros', executable='spawn_entity.py',
             name="spawnentity2",
                        arguments=['-topic', 'robot_description1',
                                   '-entity', 'aubo','-x','-1.4','-y','0.0','-z','0.85'],
                        output='screen')
                        
    return LaunchDescription([
       # gazebo ,
       node_robot_state_publisher ,
       spawn_entity ,
	
    ])
