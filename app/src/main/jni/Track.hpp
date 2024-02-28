#pragma once

#include <string>
#include <memory>

namespace test {

    struct PipelineData;

    class GstreamerTrack {
    public:
        GstreamerTrack(
            int trackID,
            const std::string& filepath
        );

        ~GstreamerTrack();

        void nextFrame();

    private:
        void initialize(int trackID, const std::string& filepath);

    private:
        std::unique_ptr<PipelineData> m_pipelineData;
    };

}