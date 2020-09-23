#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiSynthesiserAudioProcessorEditor::MidiSynthesiserAudioProcessorEditor(MidiSynthesiserAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    keyboardComponent(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{

    addAndMakeVisible(oscillatorLabel1);
    addAndMakeVisible(oscillatorLabel2);
    addAndMakeVisible(oscillatorLabel3);
    addAndMakeVisible(noiseLabel);

    addAndMakeVisible(masterVolumeSlider);
    addAndMakeVisible(keyboardComponent);
    addAndMakeVisible(waveTableSlider1);
    addAndMakeVisible(waveTableSlider2);
    addAndMakeVisible(waveTableSlider3);
    addAndMakeVisible(gainSlider1);
    addAndMakeVisible(gainSlider2);
    addAndMakeVisible(gainSlider3);
    addAndMakeVisible(noiseGainSlider);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(masterVolumeLabel);
    addAndMakeVisible(waveTableLabel1);
    addAndMakeVisible(waveTableLabel2);
    addAndMakeVisible(waveTableLabel3);
    addAndMakeVisible(gainLabel1);
    addAndMakeVisible(gainLabel2);
    addAndMakeVisible(gainLabel3);
    addAndMakeVisible(noiseGainLabel);
    addAndMakeVisible(attackLabel);
    addAndMakeVisible(decayLabel);
    addAndMakeVisible(sustainLabel);
    addAndMakeVisible(releaseLabel);
    addAndMakeVisible(oscillator1Toggle);
    addAndMakeVisible(oscillator2Toggle);
    addAndMakeVisible(oscillator3Toggle);
    addAndMakeVisible(noiseToggle);

    masterVolumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    masterVolumeSlider.setRange(0, 0.7);
    masterVolumeSlider.setValue(audioProcessor.getMasterVolume());
    masterVolumeSlider.setSkewFactorFromMidPoint(0.1);
    masterVolumeSlider.onValueChange = [this]() { audioProcessor.setMasterVolume(masterVolumeSlider.getValue()); };

    masterVolumeLabel.setText("Master Volume", juce::NotificationType::dontSendNotification);

    waveTableSlider1.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    waveTableSlider1.setRange(0, 3, 1);
    waveTableSlider1.setValue(0);
    waveTableSlider1.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    waveTableSlider1.onValueChange =
        [this]()
    {
        audioProcessor.synthAudioSource.setWaveTableOsc1(waveTableSlider1.getValue());
        waveTableLabel1.setText(waveTableTypes[(int)waveTableSlider1.getValue()], juce::NotificationType::dontSendNotification);
    };

    waveTableLabel1.setText(waveTableTypes[(int)waveTableSlider1.getValue()], juce::NotificationType::dontSendNotification);

    waveTableSlider2.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    waveTableSlider2.setRange(0, 3, 1);
    waveTableSlider2.setValue(0);
    waveTableSlider2.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    waveTableSlider2.onValueChange =
        [this]()
    {
        audioProcessor.synthAudioSource.setWaveTableOsc2(waveTableSlider2.getValue());
        waveTableLabel2.setText(waveTableTypes[(int)waveTableSlider2.getValue()], juce::NotificationType::dontSendNotification);
    };

    waveTableLabel2.setText(waveTableTypes[(int)waveTableSlider2.getValue()], juce::NotificationType::dontSendNotification);

    waveTableSlider3.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    waveTableSlider3.setRange(0, 3, 1);
    waveTableSlider3.setValue(0);
    waveTableSlider3.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    waveTableSlider3.onValueChange =
        [this]()
    {
        audioProcessor.synthAudioSource.setWaveTableOsc3(waveTableSlider3.getValue());
        waveTableLabel3.setText(waveTableTypes[(int)waveTableSlider3.getValue()], juce::NotificationType::dontSendNotification);
    };

    waveTableLabel3.setText(waveTableTypes[(int)waveTableSlider3.getValue()], juce::NotificationType::dontSendNotification);

    gainSlider1.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    gainSlider1.setRange(0, 1);
    gainSlider1.setValue(1);
    gainSlider1.setSkewFactorFromMidPoint(0.1);
    gainSlider1.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    gainSlider1.onValueChange = [this]() { audioProcessor.synthAudioSource.setGainOsc1(gainSlider1.getValue()); };

    gainLabel1.setText("Gain", juce::NotificationType::dontSendNotification);

    gainSlider2.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    gainSlider2.setRange(0, 1);
    gainSlider2.setValue(1);
    gainSlider2.setSkewFactorFromMidPoint(0.1);
    gainSlider2.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    gainSlider2.onValueChange = [this]() { audioProcessor.synthAudioSource.setGainOsc2(gainSlider2.getValue()); };

    gainLabel2.setText("Gain", juce::NotificationType::dontSendNotification);

    gainSlider3.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    gainSlider3.setRange(0, 1);
    gainSlider3.setValue(1);
    gainSlider3.setSkewFactorFromMidPoint(0.1);
    gainSlider3.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    gainSlider3.onValueChange = [this]() { audioProcessor.synthAudioSource.setGainOsc3(gainSlider3.getValue()); };

    gainLabel3.setText("Gain", juce::NotificationType::dontSendNotification);

    attackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    attackSlider.setRange(5, 3000, 1);
    attackSlider.setValue(15);
    attackSlider.setTextValueSuffix("ms");
    attackSlider.setSkewFactorFromMidPoint(1000);
    attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    attackSlider.onValueChange = [this]() { audioProcessor.setAttack(attackSlider.getValue()); };

    attackLabel.setText("Attack", juce::NotificationType::dontSendNotification);

    decaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    decaySlider.setRange(5, 3000, 1);
    decaySlider.setValue(15);
    decaySlider.setTextValueSuffix("ms");
    decaySlider.setSkewFactorFromMidPoint(1000);
    decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    decaySlider.onValueChange = [this]() { audioProcessor.setDecay(decaySlider.getValue()); };

    decayLabel.setText("Decay", juce::NotificationType::dontSendNotification);

    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    sustainSlider.setRange(0, 1);
    sustainSlider.setValue(1);
    sustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    sustainSlider.onValueChange = [this]() { audioProcessor.setSustain(sustainSlider.getValue()); };

    sustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);

    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    releaseSlider.setRange(5, 3000, 1);
    releaseSlider.setValue(15);
    releaseSlider.setTextValueSuffix("ms");
    releaseSlider.setSkewFactorFromMidPoint(1000);
    releaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    releaseSlider.onValueChange = [this]() { audioProcessor.setRelease(releaseSlider.getValue()); };

    releaseLabel.setText("Release", juce::NotificationType::dontSendNotification);

    oscillatorLabel1.setText("Osc 1", juce::NotificationType::dontSendNotification);
    oscillatorLabel2.setText("Osc 2", juce::NotificationType::dontSendNotification);
    oscillatorLabel3.setText("Osc 3", juce::NotificationType::dontSendNotification);

    oscillator1Toggle.setToggleState(true, false);
    oscillator1Toggle.onClick =
        [this]()
    {
        audioProcessor.toggleOsc1();
    };

    oscillator2Toggle.onClick =
        [this]()
    {
        audioProcessor.toggleOsc2();
    };

    oscillator3Toggle.onClick =
        [this]()
    {
        audioProcessor.toggleOsc3();
    };

    gainLabel1.setText("Gain", juce::NotificationType::dontSendNotification);

    //Noise

    noiseToggle.onClick =
        [this]()
    {
        audioProcessor.synthAudioSource.setNoiseIsActive();
    };

    noiseLabel.setText("Noise", juce::NotificationType::dontSendNotification);

    noiseGainSlider.setRange(0, 1);
    noiseGainSlider.setValue(1);
    noiseGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    noiseGainSlider.onValueChange =
        [this]()
    {
        audioProcessor.synthAudioSource.setGainNoise(noiseGainSlider.getValue());
    };

    noiseGainLabel.setText("Gain", juce::NotificationType::dontSendNotification);

    setSize(1300, 400);
    startTimer(400);
}

MidiSynthesiserAudioProcessorEditor::~MidiSynthesiserAudioProcessorEditor()
{
}

//==============================================================================
void MidiSynthesiserAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.drawVerticalLine(400, 75, 175);
    g.drawVerticalLine(800, 75, 175);
    g.drawHorizontalLine(210, 10, 390);
}

void MidiSynthesiserAudioProcessorEditor::resized()
{
    masterVolumeSlider.setBounds(1200, 20, 20, 150);
    masterVolumeLabel.setBounds(1150, 190, 100, 10);
    attackSlider.setBounds(10, 20, 180, 20);
    attackLabel.setBounds(10, 5, 80, 10);
    decaySlider.setBounds(210, 20, 180, 20);
    decayLabel.setBounds(270, 5, 80, 10);
    sustainSlider.setBounds(410, 20, 180, 20);
    sustainLabel.setBounds(470, 5, 100, 20);
    releaseSlider.setBounds(610, 20, 180, 20);
    releaseLabel.setBounds(670, 5, 100, 20);

    waveTableSlider1.setBounds(10, 120, 180, 20);
    waveTableLabel1.setBounds(50, 105, 100, 10);
    waveTableSlider2.setBounds(410, 120, 180, 20);
    waveTableLabel2.setBounds(450, 105, 100, 10);
    waveTableSlider3.setBounds(810, 120, 180, 20);
    waveTableLabel3.setBounds(850, 105, 100, 10);

    gainSlider1.setBounds(210, 120, 180, 20);
    gainLabel1.setBounds(270, 105, 60, 10);
    gainSlider2.setBounds(610, 120, 180, 20);
    gainLabel2.setBounds(670, 105, 60, 10);
    gainSlider3.setBounds(1010, 120, 180, 20);
    gainLabel3.setBounds(1070, 105, 60, 10);
    noiseGainSlider.setBounds(10, 240, 180, 20);
    noiseGainLabel.setBounds(50, 225, 100, 10);

    oscillatorLabel1.setBounds(175, 170, 50, 10);
    oscillator1Toggle.setBounds(150, 170, 30, 30);
    oscillatorLabel2.setBounds(625, 170, 50, 10);
    oscillator2Toggle.setBounds(600, 170, 30, 30);
    oscillatorLabel3.setBounds(1025, 170, 50, 10);
    oscillator3Toggle.setBounds(1000, 170, 30, 30);
    noiseLabel.setBounds(175, 260, 50, 10);
    noiseToggle.setBounds(150, 260, 30, 30);

    keyboardComponent.setBounds(10, getHeight() / 4 * 3, getWidth() - 20, getHeight() / 4);
}

void MidiSynthesiserAudioProcessorEditor::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}