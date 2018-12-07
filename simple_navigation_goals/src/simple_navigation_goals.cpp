#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/Int32.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int selection=1;

void selectionCallback(const std_msgs::Int32::ConstPtr& msg)
{
  std::cout << "CALLBACK!" << std::endl;

  int sel_aux = msg->data;
  
  if(sel_aux==1 or sel_aux==2 or sel_aux==3) //Si se ha publicado una seleccion de pose valida
  {
    selection = sel_aux;
    std::cout << "CALLBACK - OK!" << std::endl;
  }
  else
  {
    std::cout << "FAILED!" << std::endl;
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "simple_navigation_goals");
  
  //Subscriber to the action selection
  ros::NodeHandle nh;
 	ros::Subscriber action_getter = nh.subscribe("/actionSelect", 1, selectionCallback);

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //wait for the action server to come up
  while(!ac.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  //Declarate possible goals
  const float pose1[3] = {5, 5, 1};
  const float pose2[3] = {2, 8, 1};
  const float pose3[3] = {7, 9, 1};

  //General goal configuration
  goal.target_pose.header.frame_id = "/map"; //Indicate that the reference frame will be the map
  goal.target_pose.header.stamp = ros::Time::now();

  while (ros::ok())
  {
      std::cout << "IF!" << std::endl;

      //Move to the chosen goal
      switch (selection)
      {
        case 1:
          goal.target_pose.pose.position.x = pose1[0];
          goal.target_pose.pose.position.y = pose1[1];
          goal.target_pose.pose.orientation.w = pose1[2];
        break;
        case 2:
          goal.target_pose.pose.position.x = pose2[0];
          goal.target_pose.pose.position.y = pose2[1];
          goal.target_pose.pose.orientation.w = pose2[2];
        break;
        case 3:
          goal.target_pose.pose.position.x = pose3[0];
          goal.target_pose.pose.position.y = pose3[1];
          goal.target_pose.pose.orientation.w = pose3[2];
        break;
        default:
          std::cout << "[!] ERROR: Illegal option introduced. Valid options: '1', '2', '3'" << std::endl;
          return 0;
        break;
      }
      

      ROS_INFO("[*] Sending goal...");
      ac.sendGoal(goal);

      ac.waitForResult();

      if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("[*] Success! Goal archived");
      else
        ROS_INFO("The base failed to move to the goal for some reason");

      ros::spinOnce();
  }

  return 0;
}
