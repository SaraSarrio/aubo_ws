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
    pkg_name = 'aubo'
    file_subpath = 'urdf/aubo_argo.urdf'


    # Use xacro to process the file
    xacro_file = os.path.join(get_package_share_directory(pkg_name),file_subpath)
    robot_description_raw = xacro.process_file(xacro_file).toxml()


    # Configure the node
    node_robot_state_publisher_aubo = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        namespace='aubo',
        output='screen',
        parameters=[{'robot_description': robot_description_raw,
        'use_sim_time': True}] # add other parameters here if required
    )

    spawn_entity_aubo = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        namespace='aubo',
        arguments=['-topic', 'robot_description',
                                '-entity', 'my_bot'],
        output='screen')

    pkg_name_argo = 'argo'
    file_subpath_argo = 'urdf/argo.urdf'


    # Use xacro to process the file
    xacro_file_argo = os.path.join(get_package_share_directory(pkg_name_argo),file_subpath_argo)
    robot_description_raw_argo = xacro.process_file(xacro_file_argo).toxml()


    # Configure the node
    node_robot_state_publisher_argo = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        namespace='argo',
        output='screen',
        parameters=[{'robot_description': robot_description_raw_argo,
        'use_sim_time': True}] # add other parameters here if required
    )

    spawn_entity_argo = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        namespace='argo',
        arguments=['-topic', 'robot_description',
                                '-entity', 'my_bot'],
        output='screen')
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([os.path.join(
            get_package_share_directory('gazebo_ros'), 'launch'), '/gazebo.launch.py']),
        )

    load_joint_State_controller =ExecuteProcess(
        cmd=['ros2', 'control', 'load_controller', '--set-state', 'active','joint_state_broadcaster'],
        output='screen'
    )

    load_arm_controller =ExecuteProcess(
            cmd=['ros2','control','load_controller','--set-state','active','arm_controller'],
            output='screen'
        )

    return LaunchDescription([
        # RegisterEventHandler(
        #     event_handler=OnProcessExit(
        #         target_action=spawn_entity_aubo,
        #         on_exit=[load_joint_State_controller],
        #     )
        # ),
        # RegisterEventHandler(
        #     event_handler=OnProcessExit(
        #         target_action=load_joint_State_controller,
        #         on_exit=[load_arm_controller],
        #     )
        # ),
        gazebo,
        node_robot_state_publisher_argo,
        spawn_entity_argo,
        node_robot_state_publisher_aubo,
        spawn_entity_aubo
    ])


