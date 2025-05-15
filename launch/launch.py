from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    pkg_share = get_package_share_directory('gui_command')

    return LaunchDescription([
        Node(
            package='gui_command',
            executable='gui_command',
            name='gui_command',
            parameters=[{'config_path': pkg_share}],
        )
    ])