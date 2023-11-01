
import os

from ament_index_python.packages import get_package_share_directory


from launch import LaunchDescription
from launch.actions import ExecuteProcess, IncludeLaunchDescription, RegisterEventHandler
from launch.event_handlers import OnProcessExit
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch.substitutions import PathJoinSubstitution, Command
from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import Node

import xacro


def generate_launch_description():

    gazebo = IncludeLaunchDescription(
                PythonLaunchDescriptionSource([os.path.join(
                   get_package_share_directory('gazebo_ros'), 'launch'), '/gazebo.launch.py']),
            )

    gazebo_ros2_control_demos_path = os.path.join(get_package_share_directory('aubo'))

    xacro_file = os.path.join(gazebo_ros2_control_demos_path,
                              'urdf',
                              'aubo_argo.urdf')

    doc = xacro.parse(open(xacro_file))
    xacro.process_doc(doc)
    params = {'robot_description': doc.toxml()}

    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        # namespace="aubo",
        output='screen',
        parameters=[params]
    )

    spawn_entity = Node(package='gazebo_ros', executable='spawn_entity.py',
             # name="spawnentity2",
             # namespace="aubo",
                        arguments=['-topic', 'robot_description',
                                   '-entity', 'aubo','-x','-1.4','-y','0.0','-z','0.85'],
                        output='screen')

    load_joint_State_controller =ExecuteProcess(
        cmd=['ros2', 'control', 'load_controller', '--set-state', 'active', 'joint_state_broadcaster' ],
        output='screen'
    )

    load_arm_controller =ExecuteProcess(
            cmd=['ros2','control','load_controller','--set-state','active','arm_controller'],
            output='screen'
        )


    return LaunchDescription([
          RegisterEventHandler(
              event_handler=OnProcessExit(
                  target_action=spawn_entity,
                  on_exit=[load_joint_State_controller],
              )
          ),
          RegisterEventHandler(
              event_handler=OnProcessExit(
                  target_action=load_joint_State_controller,
                  on_exit=[load_arm_controller],
              )
          ),
         gazebo,

         node_robot_state_publisher,
         spawn_entity,

    ])
