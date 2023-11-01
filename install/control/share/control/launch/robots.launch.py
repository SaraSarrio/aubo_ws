
import os

from ament_index_python.packages import get_package_share_directory
from launch.actions import DeclareLaunchArgument, LogInfo
# from launch.conditions import IfCondition
from launch.launch_description_sources import AnyLaunchDescriptionSource


from launch import LaunchDescription
from launch.actions import ExecuteProcess, IncludeLaunchDescription, RegisterEventHandler
from launch.event_handlers import OnProcessExit
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch.substitutions import PathJoinSubstitution, Command, LaunchConfiguration
from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import Node

import xacro


def generate_launch_description():

    # Specify the name of the package and path to xacro file within the package
    pkg_name = 'argo'
    file_subpath = 'urdf/argo.urdf'


    # Use xacro to process the file
    xacro_file = os.path.join(get_package_share_directory(pkg_name),file_subpath)
    robot_description_raw = xacro.process_file(xacro_file).toxml()


    # Configure the node
    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        namespace= 'argo',
        output='screen',
        parameters=[{'robot_description': robot_description_raw,
        'use_sim_time': True}], # add other parameters here if required
        remappings=[
                # Remap tf y tf_static según tus necesidades
                ('tf', 'tf_argo'),
                ('tf_static', 'tf_static_argo'),
            ],
    )

    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([os.path.join(
            get_package_share_directory('gazebo_ros'), 'launch'), '/gazebo.launch.py']),
        )

    spawn_entity = Node(package='gazebo_ros', executable='spawn_entity.py',
                    namespace= 'argo',
                    arguments=['-topic', 'robot_description_argo',
                                '-entity', 'argo','-x','0.0','-y','0.0','-z','0.0'],
                    output='screen')


    return LaunchDescription([
        # gazebo,
        node_robot_state_publisher,
        spawn_entity
    ])


