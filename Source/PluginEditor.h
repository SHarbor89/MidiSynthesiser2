#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MidiSynthesiserAudioProcessorEditor : public juce::AudioProcessorEditor, juce::Timer
{
public:
    MidiSynthesiserAudioProcessorEditor(MidiSynthesiserAudioProcessor&);
    ~MidiSynthesiserAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    void addSlider(juce::Slider& slider, int width, int height, double minValue, double maxValue, double midPoint);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiSynthesiserAudioProcessor& audioProcessor;
    juce::MidiKeyboardComponent keyboardComponent;
    juce::AudioDeviceManager deviceManager;
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    //juce::Slider detuneSlider;
    juce::Slider masterVolumeSlider;

    juce::Label attackLabel;
    juce::Label decayLabel;
    juce::Label sustainLabel;
    juce::Label releaseLabel;
    juce::Label masterVolumeLabel;

    juce::ToggleButton oscillator1Toggle;
    juce::ToggleButton oscillator2Toggle;
    juce::ToggleButton oscillator3Toggle;
    juce::ToggleButton noiseToggle;

    juce::Slider gainSlider1;
    juce::Label gainLabel1;
    juce::Slider gainSlider2;
    juce::Label gainLabel2;
    juce::Slider gainSlider3;
    juce::Label gainLabel3;
    juce::Slider noiseGainSlider;
    juce::Label noiseGainLabel;

    juce::Slider waveTableSlider1;
    juce::Label waveTableLabel1;

    juce::Slider waveTableSlider2;
    juce::Label waveTableLabel2;

    juce::Slider waveTableSlider3;
    juce::Label waveTableLabel3;

    juce::Label oscillatorLabel1;
    juce::Label oscillatorLabel2;
    juce::Label oscillatorLabel3;
    juce::Label noiseLabel;

    void timerCallback() override;
    juce::String waveTableTypes[4] = {
        "Sine",
        "Square",
        "Saw",
        "Triangle",
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiSynthesiserAudioProcessorEditor)
};