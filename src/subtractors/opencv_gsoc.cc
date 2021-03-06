#include "opencv_gsoc.h"
#include <QJsonObject>

constexpr auto CAMERA_MOTION_COMPENSATION{ "CameraMotionCompensation" };
constexpr auto NUMBER_OF_SAMPLES{ "NumberOfSamples" };
constexpr auto REPLACE_RATE{ "ReplaceRate" };
constexpr auto PROPAGATION_RATE{ "PropagationRate" };
constexpr auto HITS_THRESHOLD{ "HitsThreshold" };

Subtractor::GSOC::GSOC(QJsonObject const &a_config)
  : m_cameraMotionCompensation{ a_config[CAMERA_MOTION_COMPENSATION].toInt() }
  , m_numberOfSamples{ a_config[NUMBER_OF_SAMPLES].toInt() }
  , m_replaceRate{ a_config[REPLACE_RATE].toDouble() }
  , m_propagationRate{ a_config[PROPAGATION_RATE].toDouble() }
  , m_hitsThreshold{ a_config[HITS_THRESHOLD].toInt() }

{
  m_backgroundSubtractor = cv::bgsegm::createBackgroundSubtractorGSOC(
      m_cameraMotionCompensation, m_numberOfSamples, m_replaceRate, m_propagationRate, m_hitsThreshold);
}

cv::Mat Subtractor::GSOC::process(cv::Mat &a_src)
{
  m_backgroundSubtractor->apply(a_src, m_background);
  return m_background;
}
