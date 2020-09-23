#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================


//==============================================================================
MidiSynthesiserAudioProcessor::MidiSynthesiserAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), synthAudioSource(keyboardState)
#endif
{
    bufferToFill.startSample = 0;
}

MidiSynthesiserAudioProcessor::~MidiSynthesiserAudioProcessor()
{
}

//==============================================================================
const juce::String MidiSynthesiserAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiSynthesiserAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MidiSynthesiserAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MidiSynthesiserAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MidiSynthesiserAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiSynthesiserAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MidiSynthesiserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiSynthesiserAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String MidiSynthesiserAudioProcessor::getProgramName(int index)
{
    return {};
}

void MidiSynthesiserAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void MidiSynthesiserAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synthAudioSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void MidiSynthesiserAudioProcessor::releaseResources()
{
    synthAudioSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiSynthesiserAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void MidiSynthesiserAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    bufferToFill.buffer = &buffer;
    bufferToFill.numSamples = buffer.getNumSamples();

    synthAudioSource.setIncomingMidi(midiMessages);
    synthAudioSource.getNextAudioBlock(bufferToFill);
    buffer.applyGainRamp(0, buffer.getNumSamples(), oldMasterVolume, masterVolume);
    oldMasterVolume = masterVolume;
}

//==============================================================================
bool MidiSynthesiserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiSynthesiserAudioProcessor::createEditor()
{
    return new MidiSynthesiserAudioProcessorEditor(*this);
}

//==============================================================================
void MidiSynthesiserAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MidiSynthesiserAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void MidiSynthesiserAudioProcessor::setAttack(double newAttack)
{
    attack = newAttack;
    synthAudioSource.setAttack(newAttack);
}

void MidiSynthesiserAudioProcessor::setDecay(double newDecay)
{
    decay = newDecay;
    synthAudioSource.setDecay(newDecay);
}

void MidiSynthesiserAudioProcessor::setSustain(double newSustain)
{
    sustain = newSustain;
    synthAudioSource.setSustain(newSustain);
}

void MidiSynthesiserAudioProcessor::setRelease(double newRelease)
{
    release = newRelease;
    synthAudioSource.setRelease(newRelease);
}

float MidiSynthesiserAudioProcessor::getMasterVolume() { return masterVolume; }

void MidiSynthesiserAudioProcessor::setMasterVolume(double newVolume) { masterVolume = newVolume; }

void MidiSynthesiserAudioProcessor::toggleOsc1()
{
    synthAudioSource.setOsc1IsActive();
}

void MidiSynthesiserAudioProcessor::toggleOsc2()
{
    synthAudioSource.setOsc2IsActive();
}

void MidiSynthesiserAudioProcessor::toggleOsc3()
{
    synthAudioSource.setOsc3IsActive();
}

void MidiSynthesiserAudioProcessor::toggleNoise()
{
    synthAudioSource.setNoiseIsActive();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiSynthesiserAudioProcessor();
}