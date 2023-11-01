
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

    # Configure the node
    dir_kin = Node(
        package='control',
        executable='dir_kin',
        # output='screen',
    )
    inv_kin = Node(
        package='control',
        executable='inv_kin',
        # output='screen',
    )
    controller = Node(
        package='control',
        executable='controller',
        output='screen',
    )
    sim_control = Node(
        package='control',
        executable='sim_control',
        output='screen',
    )
    sim_pub = Node(
        package='control',
        executable='sim_pub',
        # output='screen',
    )

    pkg_aubo=get_package_share_directory('aubo')
    aubo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_aubo, 'launch', 'aubo.launch.py'),
        )
    )

    return LaunchDescription([
        aubo,
        dir_kin,
        inv_kin,
        controller,
        sim_control,
        sim_pub
    ])
