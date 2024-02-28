#include <thread>
#include <mutex>

#include "Track.hpp"
#include "PipelineData.hpp"

#include <gio/gio.h>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>

#include <android/log.h>

namespace test {

    GstreamerTrack::GstreamerTrack(
        int trackID,
        const std::string& filepath
    ) {
        initialize(trackID, filepath);

        __android_log_print(
            ANDROID_LOG_INFO,
            "Native",
            "GstreamerTrack: track [%s] initialized",
            filepath.c_str()
        );
    }

    GstreamerTrack::~GstreamerTrack() {
        // Cleanup
        gst_element_set_state(m_pipelineData->pipeline, GST_STATE_NULL);
        gst_object_unref(m_pipelineData->pipeline);
    }

    void GstreamerTrack::initialize(int trackID, const std::string& filepath) {
        m_pipelineData = std::make_unique<PipelineData>();

        // Create a GStreamer pipeline with playbin
        m_pipelineData->pipeline = gst_element_factory_make("playbin", "pipeline");

        if (!m_pipelineData->pipeline) {
            __android_log_print(ANDROID_LOG_ERROR, "Native", "Failed to create playbin pipeline");
            return;
        }

        // Create an appsink element
        m_pipelineData->appsink = gst_element_factory_make("appsink", "mysink");

        if (!m_pipelineData->appsink) {
            __android_log_print(ANDROID_LOG_ERROR, "Native", "Failed to create appsink element");
            gst_object_unref(m_pipelineData->pipeline);
            return;
        }

        // Set the appsink as the video-sink property
        g_object_set(m_pipelineData->pipeline, "video-sink", m_pipelineData->appsink, NULL);

        std::string path = "file:///" + filepath;

        // Set the URI property (replace with your media file path)
        g_object_set(m_pipelineData->pipeline, "uri", path.c_str(), NULL);


        // Set pipeline to paused state
        GstStateChangeReturn ret = gst_element_set_state(m_pipelineData->pipeline, GST_STATE_PAUSED);
        if (ret == GST_STATE_CHANGE_FAILURE) {
            __android_log_print(ANDROID_LOG_ERROR, "Native", "Unable to set the pipeline to the paused state");
            gst_object_unref(m_pipelineData->pipeline);
            return;
        }
    }

    void GstreamerTrack::nextFrame() {

        // make a step to next frame
        gst_element_send_event(
            m_pipelineData->appsink,
            gst_event_new_step(
                GST_FORMAT_BUFFERS,
                1,
                ABS(1.0),
                TRUE,
                FALSE
            )
        );

        // Pull a sample from the appsink
        GstSample *sample =
            gst_app_sink_pull_sample(
                GST_APP_SINK(m_pipelineData->appsink)
            );

        if (sample != NULL) {
            GstBuffer *buffer = gst_sample_get_buffer(sample);

            // Get the PTS (Presentation Timestamp) of the frame
            GstClockTime pts = GST_BUFFER_PTS(buffer);

            // Get the width, height, and format of the frame
            GstCaps *caps = gst_sample_get_caps(sample);
            GstStructure *capsStruct = gst_caps_get_structure(caps, 0);
            gint width, height;
            gst_structure_get_int(capsStruct, "width", &width);
            gst_structure_get_int(capsStruct, "height", &height);

            GstMapInfo map;
            gst_buffer_map(buffer, &map, GST_MAP_READ);

            __android_log_print(
                ANDROID_LOG_INFO,
                "Native",
                "Frame obtained: pts=%d, width=%d, height=%d",
                (int)pts,
                width,
                height
            );

            gst_buffer_unmap(buffer, &map);
            gst_sample_unref(sample);
        } else {
            __android_log_print(ANDROID_LOG_ERROR, "Native", "Failed to get next frame");
        }
    }
}