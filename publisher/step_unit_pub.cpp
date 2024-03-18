#include "rclcpp/rclcpp.hpp"
#include "autoware_auto_control_msgs/msg/ackermann_control_command.hpp"

class ControlCommandPublisher : public rclcpp::Node
{
public:
    ControlCommandPublisher() : Node("control_command_publisher")
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
        // set the lateral control command
        control_command.lateral.stamp = this->get_clock()->now();
        control_command.lateral.steering_tire_angle = 0.0;
        control_command.lateral.steering_tire_rotation_rate = 0.0;

        // set the longitudinal control command
        control_command.longitudinal.stamp = this->get_clock()->now();
        control_command.longitudinal.acceleration = 0.0;
        control_command.longitudinal.jerk = 0.0;
        control_command.longitudinal.speed = 0.0;

        // set the time stamp
        control_command.stamp = this->get_clock()->now();
        
        RCLCPP_INFO(this->get_logger(), "Publishing control command at time '%s'", control_command.stamp.to_string().c_str());
        publisher_->publish(control_command);
    }

    rclcpp::Publisher<autoware_auto_control_msgs::msg::AckermannControlCommand>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ControlCommandPublisher>());
    rclcpp::shutdown();
    return 0;
}
