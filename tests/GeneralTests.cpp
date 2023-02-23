#include "doctest.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <AudioFile.h>

//=============================================================
TEST_SUITE ("General Tests")
{
    //=============================================================
    const std::string projectBuildDirectory = PROJECT_BINARY_DIR;

    //=============================================================
    template <typename S>
    void checkFilesAreExactlyTheSame (AudioFile<S>& a, AudioFile<S>& b)
    {
        CHECK (a.getSampleRate() == b.getSampleRate());
        CHECK (a.getBitDepth() == b.getBitDepth());
        CHECK (a.getNumChannels() == b.getNumChannels());
        CHECK (a.getNumSamplesPerChannel() == b.getNumSamplesPerChannel());
        CHECK (a.getLengthInSeconds() == b.getLengthInSeconds());
        
        size_t numSamplesToTest = std::min (static_cast<size_t> (20000), a.samples[0].size());
        
        for (size_t channel = 0; channel < a.samples.size(); channel++)
        {
            for (size_t i = 0; i < numSamplesToTest; i++)
            {
                CHECK_EQ (a.samples[channel][i], b.samples[channel][i]);
            }
        }
    }

    //=============================================================
    TEST_CASE ("GeneralTests::CopyConstructor")
    {
        AudioFile<float> a;
        a.load (projectBuildDirectory + "/test-audio/aiff_stereo_16bit_44100.aif");

        AudioFile<float> b (a);
        
        checkFilesAreExactlyTheSame<float> (a, b);
    }

    //=============================================================
    TEST_CASE ("GeneralTests::CopyAssignmentOperator")
    {
        AudioFile<float> a;
        a.load (projectBuildDirectory + "/test-audio/aiff_stereo_16bit_44100.aif");

        AudioFile<float> b;
        b = a;
        
        checkFilesAreExactlyTheSame<float> (a, b);
    }

    //=============================================================
    TEST_CASE ("GeneralTests::MoveAssignmentOperator")
    {
        AudioFile<float> a;
        a.load (projectBuildDirectory + "/test-audio/aiff_stereo_16bit_44100.aif");

        AudioFile<float> b;
        AudioFile<float> c;
        b = a;
        c = std::move (a);
        
        checkFilesAreExactlyTheSame<float> (b, c);
    }

    //=============================================================
    TEST_CASE ("GeneralTests::MoveConstructor")
    {
        AudioFile<float> a;
        a.load (projectBuildDirectory + "/test-audio/aiff_stereo_16bit_44100.aif");

        AudioFile<float> b;
        b = a;
        
        AudioFile<float> c (std::move (a));
        
        checkFilesAreExactlyTheSame<float> (b, c);
    }

    //=============================================================
    TEST_CASE ("GeneralTests::ConstructFromPath")
    {
        std::string filePath = projectBuildDirectory + "/test-audio/aiff_stereo_16bit_44100.aif";
        
        AudioFile<float> a;
        a.load (filePath);

        AudioFile<float> b (filePath);
            
        checkFilesAreExactlyTheSame<float> (a, b);
    }

    //=============================================================
    TEST_CASE ("GeneralTests::SingleIntegerResampling")
    {
        // check downsampling 16bit to 8bit
        CHECK_EQ (resampleIntegerSample<int16_t, int8_t>(32767), 127);
        CHECK_EQ (resampleIntegerSample<int16_t, int8_t>(0), 0);
        CHECK_EQ (resampleIntegerSample<int16_t, int8_t>(-32767), -128);

        // check upsampling 8bit to 16bit
        // Note: upsampling is particularly lossy and does not make perfect
        // use of sample's range of values.
        CHECK_EQ (resampleIntegerSample<int8_t, int16_t>(127), 32512);
        CHECK_EQ (resampleIntegerSample<int16_t, int8_t>(0), 0);
        CHECK_EQ (resampleIntegerSample<int8_t, int16_t>(-128), -32768);
    }

    //=============================================================
    TEST_CASE ("GeneralTests::SingleByteToSample")
    {
        AudioFile<double> dummy;

        CHECK_EQ (dummy.singleByteToSample((uint8_t)128), 0);
        CHECK_EQ (dummy.singleByteToSample((int8_t)0), 0);

        CHECK (dummy.singleByteToSample((uint8_t)255) == doctest::Approx (1).epsilon (0.05));
        CHECK (dummy.singleByteToSample((uint8_t)0) == doctest::Approx (-1).epsilon (0.05));
    }
    //=============================================================
    TEST_CASE ("GeneralTests::IntegerFormat")
    {
        std::string filePath = projectBuildDirectory + "/test-audio/aiff_stereo_16bit_44100.aif";
        
        AudioFile<int16_t> a;
        a.load (filePath);

        AudioFile<int16_t> b (filePath);
            
        checkFilesAreExactlyTheSame<int16_t> (a, b);
    }
}
