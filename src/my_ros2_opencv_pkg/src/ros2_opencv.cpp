#include "rclcpp/rclcpp.hpp"
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.hpp>
#include <opencv2/opencv.hpp>

int main() {
  // 加载预训练的人脸识别器模型
  cv::CascadeClassifier face_cascade;
  if (!face_cascade.load("/home/jiu/OPENCV_WS/src/my_ros2_opencv_pkg/src/"
                         "haarcascade_frontalface_default.xml")) {
    std::cerr << "Error loading face cascade.\n";
    return -1;
  }

  // 打开摄像头或读取视频文件
  cv::VideoCapture cap(
      0); // 使用摄像头，如果是视频文件，将文件路径传递给构造函数

  if (!cap.isOpened()) {
    std::cerr << "Error opening camera.\n";
    return -1;
  }

  cv::Mat frame;

  while (cap.read(frame)) {
    // 将帧转换为灰度图像，人脸检测对灰度图像效果更好
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // 在灰度图像中检测人脸
    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.3, 5);

    // 在图像中绘制检测到的人脸
    for (const auto &face : faces) {
      cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
      cv::putText(frame, "man", cv::Point2f(face.x, face.y - 10),
                  cv::FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2);
    }

    // 显示带有人脸矩形的图像
    cv::imshow("Face Recognition", frame);

    // 检测键盘输入，按下ESC键退出循环
    if (cv::waitKey(1) == 27) {
      break;
    }
  }

  // 释放摄像头
  cap.release();
  cv::destroyAllWindows();

  return 0;
}
