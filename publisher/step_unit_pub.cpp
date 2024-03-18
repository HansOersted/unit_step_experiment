#include "rclcpp/rclcpp.hpp"
#include "autoware_auto_control_msgs/msg/ackermann_control_command.hpp"
#include <limits>
#include <chrono>

class ControlCommandPublisher : public rclcpp::Node
{
public:
    ControlCommandPublisher() 
    : Node("control_command_publisher"), start_time_(this->now())
    {
        publisher_ = this->create_publisher<autoware_auto_control_msgs::msg::AckermannControlCommand>("/control/trajectory_follower/control_cmd", 10);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&ControlCommandPublisher::publish_control_command, this));
    }

private:
    void publish_control_command()
    {
        auto control_command = autoware_auto_control_msgs::msg::AckermannControlCommand();
        rclcpp::Time now = this->get_clock()->now();

        if ((now - start_time_).seconds() < 5.0) {
            // For the first 10 seconds, publish zeros
            control_command.lateral.steering_tire_angle = 0.0;
            control_command.lateral.steering_tire_rotation_rate = 0.0;
            control_command.longitudinal.acceleration = 0.0;
            control_command.longitudinal.jerk = 0.0;
            control_command.longitudinal.speed = 0.0;
        } else {
            // After 10 seconds, publish the actual values
            control_command.lateral.steering_tire_angle = 1.0;
            control_command.lateral.steering_tire_rotation_rate = std::numeric_limits<double>::infinity();
            control_command.longitudinal.acceleration = 0.0; // Set to your desired value
            control_command.longitudinal.jerk = 0.0; // Set to your desired value
            control_command.longitudinal.speed = 0.0; // Set to your desired value
        }

        // Set the time stamp
        control_command.stamp = now;
        control_command.lateral.stamp = now;
        control_command.longitudinal.stamp = now;

        RCLCPP_INFO(this->get_logger(), "Publishing control command at time '%s'", control_command.stamp.to_string().c_str());
        publisher_->publish(control_command);
    }

    rclcpp::Publisher<autoware_auto_control_msgs::msg::AckermannControlCommand>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Time start_time_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ControlCommandPublisher>());
    rclcpp::shutdown();
    return 0;
}
