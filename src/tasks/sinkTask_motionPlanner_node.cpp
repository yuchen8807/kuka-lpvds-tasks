#include "ros/ros.h"
#include "sinkTaskMotionPlanner.h"
#include "lwr_ros_client/action_client_cmd_interface.h"
#include <vector>


int main(int argc, char **argv)
{
  ros::init(argc, argv, "sinkTask_motionPlanner_node");

  ros::NodeHandle nh;
  double frequency = 500.0;
  ros::ServiceClient joint_cmd_client = nh.serviceClient<lwr_ros_client::String_cmd>("/action_client/kuka_action_cmd");

  // Parameters
  std::string          input_pose_topic_name;
  std::string          input_ds1_topic_name;
  std::string          input_ds2_topic_name;
  std::string          output_vel_topic_name;
  std::string          output_pick_topic_name;
  std::vector<double>  attractors_pick;
  std::vector<double>  attractor_sink;
  bool                 sim(false);
  

  if (!nh.getParam("input_pose_topic_name", input_pose_topic_name))   {
    ROS_ERROR("Couldn't retrieve the topic name for the input. ");
    // return -1;
  }

  if (!nh.getParam("input_ds1_topic_name", input_ds1_topic_name))   {
    ROS_ERROR("Couldn't retrieve the topic name for the input. ");
    // return -1;
  }

  if (!nh.getParam("input_ds2_topic_name", input_ds2_topic_name))   {
    ROS_ERROR("Couldn't retrieve the topic name for the input. ");
    // return -1;
  }

  if (!nh.getParam("output_vel_topic_name", output_vel_topic_name))   {
    ROS_ERROR("Couldn't retrieve the topic name for the output. ");
    // return -1;
  }

  if (!nh.getParam("output_pick_topic_name", output_pick_topic_name))   {
    ROS_ERROR("Couldn't retrieve the topic name for the output. ");
    // return -1;
  }

  if (!nh.getParam("attractors_pick", attractors_pick))   {
    ROS_ERROR("Couldn't retrieve the topic name for the output. ");
    // return -1;
  }

  if (!nh.getParam("attractor_sink", attractor_sink))   {
    ROS_ERROR("Couldn't retrieve the topic name for the output. ");
    // return -1;
  }


  if (!nh.getParam("sim", sim))   {
    ROS_ERROR("Couldn't retrieve sim value. ");
    // return -1;
  }

  sinkTaskMotionPlanner sinkTask_MotionPlanner_(nh, frequency,
                                         input_pose_topic_name,
                                         input_ds1_topic_name,
                                         input_ds2_topic_name,
                                         output_vel_topic_name,
                                         output_pick_topic_name,
                                         attractors_pick,
                                         attractor_sink,
                                         sim);
  
  if (!sinkTask_MotionPlanner_.Init()) 
    return -1;
  else 
    sinkTask_MotionPlanner_.Run();


  /* Before closing the node, send robot to go_left joint command */
  lwr_ros_client::String_cmd joint_srv;
  joint_srv.request.cmd = "go_left";
  joint_cmd_client.call(joint_srv);

  ros::Duration(2.0).sleep(); // wait

  joint_srv.request.cmd = "go_home";
  joint_cmd_client.call(joint_srv);

  ros::shutdown();

  return 0;
}
