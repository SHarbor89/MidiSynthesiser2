#pragma once

#include <JuceHeader.h>

struct WaveTableSound : public juce::SynthesiserSound
{
    WaveTableSound() {}

    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
    bool appliesToChannel(int /*midiChannel*/) override { return true; }

};

//==============================================================================
struct WaveTableVoice : public juce::SynthesiserVoice
{
    WaveTableVoice()
    {
        createWavetable();
    }

    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<WaveTableSound*> (sound) != nullptr;
    }

    void startNote(int midiNoteNumber, float velocity,
        juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentIndex = 0.0;
        level = velocity;
        //release = 0.0;
        sampleIndex = 0;

        auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();
        samplesPerMilliSecond = getSampleRate() / 1000;

        auto tableSizeOverSampleRate = tableSize / getSampleRate();
        tableDelta = cyclesPerSecond * tableSizeOverSampleRate;
    }

    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        stopSample = sampleIndex;
        stopFlag = true;
        allowTailOff = true;
        /*if (allowTailOff)
        {
            if (release == 0.0)
                release = 1.0;
        }
        else
        {
            clearCurrentNote();
            tableDelta = 0.0;
        }*/
    }

    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}

    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (tableDelta != 0.0)
        {

            while (--numSamples >= 0)
            {
                auto currentSample = 0.0;
                if (isVoiceActive()) currentSample = (float)(getNextSample() * applyEnvelope()) * level;

                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);

                currentIndex += tableDelta;
                sampleIndex += tableDelta;
                currentIndex = currentIndex >= tableSize ? currentIndex - tableSize : currentIndex;
                ++startSample;
            }
        }
    }

    forcedinline float getNextSample() noexcept
    {
        double currentSample = 0.0;
        if (numOscillators > 0)
        {
            auto index0 = (unsigned int)currentIndex;
            auto index1 = index0 == (tableSize - 1) ? (unsigned int)0 : index0 + 1;

            auto frac = currentIndex - (float)index0;

            auto value0 = (double)waveTables[currentTableOsc1][index0] * osc1IsActive * gain1
                + (double) waveTables[currentTableOsc2][index0] * osc2IsActive * gain2
                + (double) waveTables[currentTableOsc3][index0] * osc3IsActive * gain3
                + (double) rand.nextFloat() * noiseIsActive * noiseGain / numOscillators;
            auto value1 = (double) waveTables[currentTableOsc1][index1] * osc1IsActive * gain1
                + (double) waveTables[currentTableOsc2][index1] * osc2IsActive * gain2
                + (double) waveTables[currentTableOsc3][index1] * osc3IsActive * gain3
                + (double) rand.nextFloat() * noiseIsActive * noiseGain / numOscillators;

            currentSample = value0 + frac * ((double)value1 - value0);

            if ((currentIndex += tableDelta) > (float)tableSize)
                currentIndex -= (float)tableSize;
        }
        return currentSample;
    }

    void createWavetable()
    {
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1);
        auto currentAngle = 0.0;

        for (unsigned int i = 0; i < tableSize; ++i)
        {
            //Sine Wave
            waveTables[sine][i] = (std::sin(currentAngle) + 1) / 2;

            //Square Wave
            waveTables[square][i] = i < tableSize / 2 ? 1.0f : -1.0f;

            //Saw Wave
            waveTables[saw][i] = (1 - ((float)i) / tableSize) * 2 - 1;

            //Triangle Wave
            waveTables[triangle][i] = ((std::abs(((double)tableSize / 2 - i))) / ((double)tableSize / 2)) * 2 - 1;

            currentAngle += angleDelta;
        }
    }

    void setWaveTableOsc1(double type) { currentTableOsc1 = (int)type; }

    void setWaveTableOsc2(double type) { currentTableOsc2 = (int)type; }

    void setWaveTableOsc3(double type) { currentTableOsc3 = (int)type; }

    void setAttack(double newAttack) { attack = newAttack; }

    void setDecay(double newDecay) { decay = newDecay; }

    void setSustain(double newSustain) { sustain = newSustain; }

    void setRelease(double newRelease) { release = newRelease; }

    forcedinline double applyEnvelope()
    {
        if (stopFlag)
        {
            double timeElapsed = (sampleIndex - stopSample) / samplesPerMilliSecond;
            if (timeElapsed > release)
            {
                clearCurrentNote();
                tableDelta = 0.0;
                stopFlag = false;
                return 0.0;
            }
            return ((release - timeElapsed) / release) * envelope;
        }
        else
        {
            double timeElapsed = sampleIndex / samplesPerMilliSecond;
            if (timeElapsed < attack)
            {
                envelope = timeElapsed / attack;
            }
            else if (timeElapsed - attack < decay)
            {
                envelope = 1.0 - (1.0 - sustain) * ((timeElapsed - attack) / decay);
            }
            else envelope = sustain;
        }
        return envelope;
    }

    void setOsc1IsActive()
    {
        osc1IsActive = !osc1IsActive;
        if (osc1IsActive) numOscillators++;
        else numOscillators--;
    }

    void setOsc2IsActive()
    {
        osc2IsActive = !osc2IsActive;
        if (osc2IsActive) numOscillators++;
        else numOscillators--;
    }

    void setOsc3IsActive()
    {
        osc3IsActive = !osc3IsActive;
        if (osc3IsActive) numOscillators++;
        else numOscillators--;
    }

    void setNoiseIsActive()
    {
        noiseIsActive = !noiseIsActive;
        if (noiseIsActive) numOscillators++;
        else numOscillators--;
    }

    void setGainOsc1(double newGain) { gain1 = newGain; }

    void setGainOsc2(double newGain) { gain2 = newGain; }

    void setGainOsc3(double newGain) { gain3 = newGain; }

    void setGainNoise(double newGain) { noiseGain = newGain; }

private:
    double gain1 = 0.0, gain2 = 0.0, gain3 = 0.0, noiseGain = 0.0, currentIndex = 0.0, tableDelta = 0.0, level = 0.0,
        attack = 0.0, decay = 0.0, sustain = 0.0, envelope = 0.0, release = 0.0;
    int currentTableOsc1 = sine, currentTableOsc2 = sine, currentTableOsc3 = sine, numOscillators = 1;
    double stopSample = 0, sampleIndex = 0;
    double samplesPerMilliSecond = 0;
    bool stopFlag = false, osc1IsActive = true, osc2IsActive = false, osc3IsActive = false, noiseIsActive = false;
    enum tableType {
        sine,
        square,
        saw,
        triangle
    };
    const unsigned int tableSize = 512;
    float waveTables[4][512];
    juce::Random rand;
};

//==============================================================================
class SynthAudioSource : public juce::AudioSource
{
public:
    SynthAudioSource(juce::MidiKeyboardState& keyState)
        : keyboardState(keyState)
    {
        for (auto i = 0; i < numVoices; ++i)
        {
            synth.addVoice(new WaveTableVoice());
        }

        synth.addSound(new WaveTableSound());
    }

    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }

    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate(sampleRate);
        midiCollector.reset(sampleRate);
    }

    void releaseResources() override {}

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();

        midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
        keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample,
            bufferToFill.numSamples, true);

        synth.renderNextBlock(*bufferToFill.buffer, incomingMidi,
            bufferToFill.startSample, bufferToFill.numSamples);
    }

    void setIncomingMidi(juce::MidiBuffer& midiMessages)
    {
        incomingMidi = midiMessages;
    }

    juce::MidiMessageCollector* getMidiCollector()
    {
        return &midiCollector;
    }

    void setWaveTableOsc1(double type)
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setWaveTableOsc1(type);
        }
    }

    void setWaveTableOsc2(double type)
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setWaveTableOsc2(type);
        }
    }

    void setWaveTableOsc3(double type)
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setWaveTableOsc3(type);
        }
    }

    void setAttack(double newAttack)
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setAttack(newAttack);
        }
    }

    void setDecay(double newDecay)
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setDecay(newDecay);
        }
    }

    void setSustain(double newSustain)
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setSustain(newSustain);
        }
    }

    void setRelease(double newRelease)
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setRelease(newRelease);
        }
    }

    void setOsc1IsActive()
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setOsc1IsActive();
        }
    }

    void setOsc2IsActive()
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setOsc2IsActive();
        }
    }

    void setOsc3IsActive()
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setOsc3IsActive();
        }
    }

    void setNoiseIsActive()
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setNoiseIsActive();
        }
    }

    void setGainOsc1(double newGain)
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setGainOsc1(newGain);
        }
    }

    void setGainOsc2(double newGain)
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setGainOsc2(newGain);
        }
    }

    void setGainOsc3(double newGain)
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setGainOsc3(newGain);
        }
    }

    void setGainNoise(double newGain)
    {
        for (int i = 0; i < numVoices; i++)
        {
            auto* voice = dynamic_cast<WaveTableVoice*>(synth.getVoice(i));
            if (voice == nullptr) break;
            voice->setGainNoise(newGain);
        }
    }

private:
    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;
    int numVoices = 8;
    juce::MidiMessageCollector midiCollector;
    juce::MidiBuffer incomingMidi;
};

//==============================================================================
/**
*/
class MidiSynthesiserAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    MidiSynthesiserAudioProcessor();
    ~MidiSynthesiserAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    float getMasterVolume();
    void setMasterVolume(double newVolume);

    void setAttack(double newAttack);
    void setDecay(double newDecay);
    void setSustain(double newSustain);
    void setRelease(double newRelease);

    juce::MidiKeyboardState keyboardState;
    SynthAudioSource synthAudioSource;

    void toggleOsc1();
    void toggleOsc2();
    void toggleOsc3();
    void toggleNoise();

private:
    float masterVolume = 0.15, oldMasterVolume = 0.15;
    double attack = 0.0, decay = 0.0, sustain = 0.0, release = 0.0;
    juce::AudioSourceChannelInfo bufferToFill;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiSynthesiserAudioProcessor)
};
