#include "headposeestimator.h"

HeadPoseEstimator::HeadPoseEstimator(opengl_view *renderWidget, QObject *parent) : QObject(parent)
{
    //Initialize
    m_webcam.initialize(0);
    m_renderer.initialize(renderWidget);
    m_head_pose_detector.initialize();
    m_vertex_generator.initialize();
    m_snapchatdog_filter.initialize();
    m_fancyman_filter.initialize();
    m_faceswap_generator.initialize(QApplication::applicationDirPath()+"\\overlays\\Obama.png");

    if(m_face_detector.initialize()!=0)
    {
        return;
    }

    // Form processing chain
    m_webcam.setNextItem(&m_face_detector);
    m_face_detector.setNextItem(&m_head_pose_detector);
    m_head_pose_detector.setNextItem(&m_vertex_generator);
    m_vertex_generator.setNextItem(&m_snapchatdog_filter);
    m_snapchatdog_filter.setNextItem(&m_fancyman_filter);
    m_fancyman_filter.setNextItem(&m_faceswap_generator);
    m_faceswap_generator.setNextItem(&m_renderer);

    //Connect loop timer with run method
    connect(&m_loop_timer,SIGNAL(timeout()),this,SLOT(run()));
}

HeadPoseEstimator::~HeadPoseEstimator()
{
    m_webcam.stop();
}

void HeadPoseEstimator::processWebcam(bool process)
{
    if(process==true)
    {
        m_webcam.start();
        m_loop_timer.start(1);
    }
    else
    {
        m_webcam.stop();
        m_loop_timer.stop();
    }
}

void HeadPoseEstimator::showFacePoints(bool show)
{
    m_face_detector.setEnabled(show);
}

void HeadPoseEstimator::showHeadAngles(bool show)
{
    m_head_pose_detector.setEnabled(show);
}

void HeadPoseEstimator::showVertexModel(bool show)
{
    m_vertex_generator.setEnabled(show);
}

void HeadPoseEstimator::showSnapchatDogOverlay(bool show)
{
    m_snapchatdog_filter.setEnabled(show);
}

void HeadPoseEstimator::showFancyManOverlay(bool show)
{
    m_fancyman_filter.setEnabled(show);
}

void HeadPoseEstimator::swapFace(bool swap)
{
    m_faceswap_generator.setEnabled(swap);
}

void HeadPoseEstimator::run()
{
   m_webcam.process(m_result);
}