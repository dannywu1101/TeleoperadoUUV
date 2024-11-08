find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  sensor_msgs
)

catkin_package(
  CATKIN_DEPENDS roscpp std_msgs sensor_msgs
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(nodo_teleoperado src/teleop.cpp)  # Asegúrate de que el nombre del archivo coincide

target_link_libraries(nodo_teleoperado
  ${catkin_LIBRARIES}
)
