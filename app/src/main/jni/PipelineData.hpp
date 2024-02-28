#pragma once

#include <gst/gst.h>

namespace test {

    struct PipelineData {
        GstElement *pipeline;
        GstElement *appsink;
    };

}