#include <ros/ros.h>

#include "lwr_ros_client/action_client_cmd_interface.h"
#include "lwr_ros_client/kuka_action_client.h"
#include "lwr_ros_client/ros_param_parser.h"
#include "lwr_ros_action/joint_action.h"
#include "planning/simple_actions/linear_cart_action.h"

/**
  *     Client Action node (simple example)
  *
  *     This .cpp file encodes the client ROS node of the action server. It is in this node
  *     that you the name of your actions with associated goal parameters. This list of
  *     of tuples [name,goal] is can the be called via three methods;
  *         1) service       : rosservice call /control_cmd_interface/kuka_cmd 'name'"
  *         2) voice         :
  *         3) cmd interface : terminal
  *
  *     When a name is selected the client contacts the Action server node with [goal] information.
  *     The action server will then proceed to run the action with the goal parameters specified in
  *     [goal].
 */

int main(int argc, char** argv)
{

    ros::init(argc, argv,"action_client");
    ros::NodeHandle nh("action_client");

    std::string node_name = ros::this_node::getName();

    std::map<std::string,std::string> param_name_value;
    param_name_value[node_name + "/speech_topic"]           = "";
    param_name_value[node_name + "/action_service_name"]    = "";
    param_name_value[node_name + "/cmd_service_name"]       = "";
    param_name_value[node_name + "/action_server_name"]     = "";

    if(!pps::Parser::parser_string(nh,param_name_value)){
        ROS_ERROR("failed to parse all parameters!");
        return -1;
    }

    std::string speech_topic          =  param_name_value[node_name + "/speech_topic"];
    std::string action_serivce_name   =  param_name_value[node_name + "/action_service_name"];
    std::string cmd_service_name      =  param_name_value[node_name + "/cmd_service_name"];
    std::string action_server_name    =  param_name_value[node_name + "/action_server_name"];


    /** ------------- Initialise Action Client & Set Action-Goals -------------

      The Simple_action client is initialsed. A set of actions and goals are defined
      add added to the action clients container which is a map. The key of
      the map is the name of the action and the value is the Goal.

    **/

    ac::Kuka_action_client kuka_action_client;
    std::map<std::string,ac::Base_action*> actions;


    /** ------------- Defining goals -------------
     *
     *  The action client registers in a map, goals["action_name"] = goal.
     *  The goal object holds sepcific variables for the policy that will
     *  execut this goal.
     *
     *  For instance goal can hold a target cartesian position, a target
     *  joint position, target stiffness values, etc..
     *
     *  It is important that in the action server node (server_action_node.cpp) there exists a
     *  a policy which has been registered with a type matching that of goal.action_type.
     *
     */
      std::array<double,7> des_position;
    
      ac::Joint_action joint_go_right(nh);      
      // des_position  =  {{-0.308, 0.749, -0.658, -1.118, 0.475, 1.382, 0.559}};
      // des_position  =  {{-1.0641, 0.8109, 0.8423, -1.2189, -0.6116, 1.3073, -0.233}};
      des_position  =  {{-0.838, 0.614, 0.388, -1.072, -0.1988, 1.478, -0.606}};
      joint_go_right.set_joint_values(des_position,ac::Joint_action::MESSAGE_TYPE::JOINT_POSITION);
      joint_go_right.debug_print = true;
      actions["go_right"] = &joint_go_right;
      
      ac::Joint_action joint_go_left(nh);   
      // des_position  =  {{0.858, 0.819, -0.476, -0.7953, 0.2931, 1.528, 0.512}};   
      // des_position  =  {{0.96716, 0.899, -0.7492, -0.849, 0.502, 1.484, 1.9653}};
      des_position  =  {{0.52, 0.802, 0.292, -0.826, -0.289, 1.457, 0.7755}};
      joint_go_left.set_joint_values(des_position,ac::Joint_action::MESSAGE_TYPE::JOINT_POSITION);
      joint_go_left.debug_print = true;
      actions["go_left"]        = &joint_go_left;  
      
      ac::Joint_action joint_go_home(nh);
      // des_position  =  {{0.529, 0.243, -0.4973, -1.3239, 0.0973, 1.5216, 0.0611}};
      des_position  =  {{-0.102, 0.226, 0.14, -1.456, -0.025, 1.407, -0.001}};
      joint_go_home.set_joint_values(des_position,ac::Joint_action::MESSAGE_TYPE::JOINT_POSITION);
      joint_go_home.debug_print = true;
      actions["go_home"]        = &joint_go_home;

      ac::Joint_action joint_go_nullspace(nh);
      des_position  =  {{-0.0045, 0.7503, -0.0598, -1.652, 0.0603, 0.7602, 1.538}};
      joint_go_nullspace.set_joint_values(des_position,ac::Joint_action::MESSAGE_TYPE::JOINT_POSITION);
      joint_go_nullspace.debug_print = true;
      actions["go_nullspace"]        = &joint_go_nullspace;

      ac::Joint_action go_candle(nh);
      des_position  =  {{0,0,0,0,0,0,0}};
      go_candle.set_joint_values(des_position,ac::Joint_action::MESSAGE_TYPE::JOINT_POSITION);
      go_candle.debug_print = true;
      actions["go_candle"]         = &go_candle;

      simple_actions::Linear_cart_action linear_cart_action(nh);
      actions["linear"]         = &linear_cart_action;

    /**
      * Here we register all the goals with the action client. This wil make them available to
      * be called with the three different methods mentioned above (service,voice,cmd interface)
      *
      **/

      kuka_action_client.push_back(actions);


    /**  ------------- Initialise Service, Voice & Cmd interface  -------------
     *  The control command interface is an interface to the action client.
     *  It provied a ros service and a voice command interface such to
     *  command the client server to send desired action requests to the action server.
     */
     ac::Action_client_cmd_interface action_cmd_interface(nh,kuka_action_client,action_serivce_name,cmd_service_name);
     action_cmd_interface.init_nl_subscriber(speech_topic);

     ROS_INFO("action CLIENT started!");
     ros::spin();

    return 0;
}
