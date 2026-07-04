# ENV_CHECK.md

## colcon version-check

```
colcon-argcomplete 0.3.3: up-to-date
colcon-bash 0.5.0: up-to-date
colcon-cd 0.2.1: local version is newer than latest release (0.1.1)
colcon-cmake 0.2.29: newer version available (0.2.30)
colcon-core 0.20.1: newer version available (0.21.0)
colcon-defaults 0.2.9: up-to-date
colcon-devtools 0.3.0: up-to-date
colcon-installed-package-information 0.2.1: up-to-date
colcon-library-path 0.2.1: up-to-date
colcon-metadata 0.2.5: up-to-date
colcon-mixin 0.2.3: up-to-date
colcon-notification 0.3.1: up-to-date
colcon-output 0.2.14: up-to-date
colcon-override-check 0.0.1: up-to-date
colcon-package-information 0.4.1: up-to-date
colcon-package-selection 0.2.10: up-to-date
colcon-parallel-executor 0.3.0: newer version available (0.4.0)
colcon-pkg-config 0.1.0: up-to-date
colcon-powershell 0.5.0: up-to-date
colcon-python-setup-py 0.2.9: up-to-date
colcon-recursive-crawl 0.2.3: up-to-date
colcon-ros 0.5.0: up-to-date
colcon-test-result 0.3.8: up-to-date
colcon-zsh 0.5.0: up-to-date
```

## cmake --version

```
cmake version 3.28.3

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```

## python3 --version

```
Python 3.12.3
```

## ros2 --version OR apt show ros-jazzy-desktop 2>/dev/null | grep Version

```
Version: 0.11.0-1noble.20260616.084553
```

## printenv ROS_DISTRO

```
jazzy
```

## timeout 3 ros2 run demo_nodes_cpp talker || echo "done"

```
[INFO] [1783151109.099655886] [talker]: Publishing: 'Hello World: 1'
[INFO] [1783151110.099751398] [talker]: Publishing: 'Hello World: 2'
[INFO] [1783151110.836212664] [rclcpp]: signal_handler(SIGINT/SIGTERM)
done
```