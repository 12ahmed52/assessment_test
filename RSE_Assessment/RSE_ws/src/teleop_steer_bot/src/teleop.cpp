#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <termios.h>
#include <unistd.h>

#define MAX_LIN_VEL 1.0
#define MAX_ANG_VEL 0.4

#define LIN_VEL_STEP_SIZE 0.1
#define ANG_VEL_STEP_SIZE 0.1

char getLastKey() {
    char c;
    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);  // Add stdin to the set

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    if (select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0) {
        read(STDIN_FILENO, &c, 1);  // Read the key if available
    } else {
        c = '\0';  // Return null character if no key is pressed
    }
    return c;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "steer_bot_teleop");
    ros::NodeHandle nh;
    ros::Publisher twist_pub = nh.advertise<geometry_msgs::Twist>("/steer_bot/ackermann_steering_controller/cmd_vel", 100, true); // Latched publisher
    ros::Rate loop_rate(10);

    double target_linear_vel = 0.0;
    double target_angular_vel = 0.0;

    printf("Control Your Steer Bot!\n");
    printf("---------------------------\n");
    printf("Moving around:\n");
    printf("        w\n");
    printf("   a    s    d\n");
    printf("        x\n");
    printf("\n");
    printf("w/x : increase/decrease linear velocity (max: %f)\n", MAX_LIN_VEL);
    printf("a/d : increase/decrease angular velocity (max: %f)\n", MAX_ANG_VEL);
    printf("\n");
    printf("space key, s : force stop\n");
    printf("\n");
    printf("CTRL-C to quit\n");

    while (ros::ok()) {
        char key = getLastKey();
        if (key == 'w') {
            target_linear_vel = std::min(target_linear_vel + LIN_VEL_STEP_SIZE, MAX_LIN_VEL);
            printf("currently:\tlinear vel %f\t angular vel %f\n", target_linear_vel, target_angular_vel);
        } else if (key == 'x') {
            target_linear_vel = std::max(target_linear_vel - LIN_VEL_STEP_SIZE, -MAX_LIN_VEL);
            printf("currently:\tlinear vel %f\t angular vel %f\n", target_linear_vel, target_angular_vel);
        } else if (key == 'a') {
            target_angular_vel = std::min(target_angular_vel + ANG_VEL_STEP_SIZE, MAX_ANG_VEL);
            printf("currently:\tlinear vel %f\t angular vel %f\n", target_linear_vel, target_angular_vel);
        } else if (key == 'd') {
            target_angular_vel = std::max(target_angular_vel - ANG_VEL_STEP_SIZE, -MAX_ANG_VEL);
            printf("currently:\tlinear vel %f\t angular vel %f\n", target_linear_vel, target_angular_vel);
        } else if (key == ' ' || key == 's') {
            target_linear_vel = 0.0;
            target_angular_vel = 0.0;
            printf("currently:\tlinear vel %f\t angular vel %f\n", target_linear_vel, target_angular_vel);
        } else {
            if (key == '\x03') break;
        }

        geometry_msgs::Twist twist;
        twist.linear.x = target_linear_vel;
        twist.angular.z = target_angular_vel;

        twist_pub.publish(twist);
        loop_rate.sleep();
    }

    geometry_msgs::Twist twist;
    twist.linear.x = 0.0;
    twist.angular.z = 0.0;
    twist_pub.publish(twist);

    return 0;
}

