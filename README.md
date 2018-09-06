# kuka-lpvds-compliant
This package implements the LPV-DS motion generator from Nadia's CoRL (2018) paper together with the passive-DS controller for the KUKA LWR 4+ robot in simulation (gazebo) and real scenarios. 

### Dependencies
To run this package you must install the following dependencies:
- [kuka-lwr-ros](https://github.com/epfl-lasa/kuka-lwr-ros.git) checkout 'nadia' branch
- [ds-motion-generator](https://github.com/epfl-lasa/ds_motion_generator.git) checkout 'nadia' branch

### Simulation
Bring up the kuka-lwr-ros controller and simulator:
```
$ roslaunch lwr_simple_example sim.launch force-interface:=true
```
To apply external forces during the execution of the passive-DS controller you can define the force in the following topicL
```
/lwr/joint_controllers/passive_ds_external_force
```
the force will be applied by manipulating the boolean command:
```
/lwr/joint_controllers/passive_ds_apply_force
```

### Real robot
Bring up the kuka-lwr-ros controller: 
```
$ roslaunch lwr_simple_example real.launch
```
and console
```
$ roslaunch lwr_fri lwr_fri_console.launch
```
