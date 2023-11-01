
import os

from ament_index_python.packages import get_package_share_directory


from launch import LaunchDescription
from launch.actions import ExecuteProcess, IncludeLaunchDescription, RegisterEventHandler
from launch.event_handlers import OnProcessExit
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.actions import LogInfo
from launch.substitutions import LaunchConfiguration
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
from launch import Action
from launch_ros.actions import Node
from launch.actions import TimerAction


import xacro


def generate_launch_description():
    gazebo = IncludeLaunchDescription(
                PythonLaunchDescriptionSource([os.path.join(
                   get_package_share_directory('gazebo_ros'), 'launch'), '/gazebo.launch.py']),
            )
    gazebo_ros2_control_demos_path = os.path.join(get_package_share_directory('argo'))

    xacro_file = os.path.join(gazebo_ros2_control_demos_path,
                              'urdf',
                              'argo.urdf')
    doc = xacro.parse(open(xacro_file))
    xacro.process_doc(doc)
    params = {'robot_description': doc.toxml()}

    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name="publisher1",
        namespace="argo",
        output='screen',
        parameters=[params]
    )

    spawn_entity  = Node(package='gazebo_ros',
        executable='spawn_entity.py',
        name="spawnentity1",
        namespace="argo",
                        arguments=['-topic', 'robot_description',
                                   '-entity', 'argo','-x','0.0','-y','0.0','-z','0.0'],
                        output='screen')

 #   load_joint_state_controller = ExecuteProcess(
  #      cmd=['ros2', 'control', 'load_controller', '--set-state', 'active','joint_state_broadcaster'],
   #     output='screen'
    #)

    #load_arm_controller = ExecuteProcess(
     #   cmd=['ros2', 'control', 'load_controller', '--set-state', 'active', 'arm_controller'],
      #  output='screen'
    #)
    control_node = Node(package='controller_manager', executable='ros2_control_node',
                        arguments=[ ],
                        output='screen')

    control_node_2 = Node(package='controller_manager', executable='spawner',
                        arguments=["joint_trajectory_controller", "-c", "/controller_manager"],
                        output='screen')


    return LaunchDescription([
        # gazebo ,
        node_robot_state_publisher ,
        spawn_entity ,

        #TimerAction(period=10.0,
         #           actions=[aubo]),
        
    ])
