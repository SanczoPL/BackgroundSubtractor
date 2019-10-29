#include <backgroundsubtractor.h>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define floatqDebug() qDebug() << fixed << qSetRealNumberPrecision(2)

int main() {
  QJsonObject filterConfig{{"Name", "OpenCV_KNN"},
                           {"LearningRate", -1},
                           {"History", 200},
                           {"VarThreshold", 100},
                           {"DetectShadow", false}};
  QJsonObject filterConfig2{{"Name", "OpenCV_MOG2"},
                            {"LearningRate", -1},
                            {"History", 200},
                            {"VarThreshold", 100},
                            {"DetectShadow", false}};
  QJsonObject filterConfig3{{"Name", "OpenCV_CNT"},
                            {"MinPixelStability", 15},
                            {"MaxPixelStability", 60 * 15},
                            {"IsParallel", true},
                            {"UseHistory", true}};
  QJsonObject filterConfig4{{"Name", "OpenCV_GMG"},
                            {"InitializationFrames", 120},
                            {"DecisionThreshold", 0.8}};

  QJsonObject filterConfig5{
      {"Name", "OpenCV_GSOC"},   {"CameraMotionCompensation", 0},
      {"NumberOfSamples", 20},   {"ReplaceRate", 0.003},
      {"PropagationRate", 0.01}, {"HitsThreshold", 32}};

  QJsonObject filterConfig6{{"Name", "OpenCV_LSBP"},
                            {"CameraMotionCompensation", 0},
                            {"NumberOfSamples", 20},
                            {"LSBPRadius", 16}};

  QJsonObject filterConfig7{{"Name", "OpenCV_MOG"},
                            {"History", 200},
                            {"Nmixtures", 5},
                            {"BackgroundRatio", 0.7},
                            {"NoiseSigma", 0}};

  QJsonObject filterConfig8{{"Name", "User_Median"},
                            {"History", 20},
                            {"HistoryDelta", 1},
                            {"DecisionThreshold", 110}};

  QJsonObject filterConfig9{{"Name", "User_Median_V2"},
                            {"History", 20},
                            {"HistoryDelta", 1},
                            {"DecisionThreshold", 110}};

  QJsonArray filters{filterConfig8};
  QJsonArray filters2{filterConfig9};
  qDebug() << "filters" << filters;
  BackgroundSubtractor multiImageFilter{filters};
  BackgroundSubtractor multiImageFilter2{filters2};

  cv::VideoCapture cap;
  // cap.open(0);
  cap.open("/media/data/raw_video/1.avi");
  // cap.open("/home/gmatczak/tmp/000001%04d.png");
  if (!cap.isOpened()) {
    return -1;
  }
  cv::Mat frame;
  cap >> frame;
  cv::cvtColor(frame, frame, 6);
  cv::Mat bg = multiImageFilter.process(frame);
  cv::Mat bg2 = multiImageFilter2.process(frame);
  cv::TickMeter tm;
  tm.reset();

  for (int i = 0; i < 20; i++) {
    cap >> frame;
    cv::cvtColor(frame, frame, 6);
    if (frame.empty()) break;
    bg = multiImageFilter.process(frame);
    bg2 = multiImageFilter2.process(frame);
    floatqDebug() << "i:" << i << endl;
    cv::imshow("Before", frame);
    cv::imshow("After", bg);
    cv::imshow("After2", bg2);
    cv::waitKey(10);
  }

  double elapsedTime1 = multiImageFilter.getElapsedTimeSubtractor();
  double elapsedTime2 = multiImageFilter2.getElapsedTimeSubtractor();
  floatqDebug() << "elapsedTime1" << elapsedTime1 << endl;
  floatqDebug() << "elapsedTime2" << elapsedTime2 << endl;

  return 0;
}
