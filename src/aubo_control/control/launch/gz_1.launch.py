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

    pkg_name = 'aubo'
    file_subpath = 'urdf/aubo_i5.urdf'


    # Use xacro to process the file
    xacro_file = os.path.join(get_package_share_directory(pkg_name),file_subpath)
    robot_description_raw = xacro.process_file(xacro_file).toxml()


    # Configure the node
    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[{'robot_description': robot_description_raw,
        'use_sim_time': True}] # add other parameters here if required
    )

    world_file = DeclareLaunchArgument(
        'world',
        default_value=os.path.join(
            get_package_share_directory('control'), 'worlds', 'gazebo_ros_joint_pose_trajectory_demo.world'),
        description='/home/sara/aubo_ws/src/aubo_control/control/worlds/gazebo_ros_joint_pose_trajectory_demo.world')

    # Launch Gazebo if it's available
    gazebo_launch = Node(
        package='gazebo_ros',
        executable='gazebo.launch.py',
        name='gazebo',
        output='screen',
        # condition=IfCondition(has_gazebo),
        arguments=[LaunchConfiguration('world')],
    )


    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory('gazebo_ros'), 'launch', 'gazebo.launch.py')
        ),
        launch_arguments={
            'world': LaunchConfiguration('world'),  # Pass the 'world' argument to the included launch
        }.items()
    )


    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([os.path.join(
            get_package_share_directory('gazebo_ros'), 'launch'), '/gazebo.launch.py']),
        )

    gazebo_world =ExecuteProcess(
        cmd=['gazebo','--verbose','/home/sara/gazebo_ros_joint_pose_trajectory_demo.world'],
        output='screen'
    )

    spawn_entity = Node(package='gazebo_ros', executable='spawn_entity.py',
                    arguments=['-topic', 'robot_description',
                                '-entity', 'my_bot','-x','-1.4','-y','0.0','-z','0.85'],
                    output='screen')

    load_joint_State_controller =ExecuteProcess(
        cmd=['ros2', 'control', 'load_controller', '--set-state', 'active','joint_state_broadcaster'],
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
        world_file,
        gazebo_launch,
        node_robot_state_publisher,
        spawn_entity
    ])


