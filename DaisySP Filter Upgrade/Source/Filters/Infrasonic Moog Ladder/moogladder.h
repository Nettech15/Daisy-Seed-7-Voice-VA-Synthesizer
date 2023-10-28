/* Ported from Audio Library for Teensy, Ladder Filter
 * Copyright (c) 2021, Richard van Hoesel
 * Copyright (c) 2022, Infrasonic Audio LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

//-----------------------------------------------------------
// Huovilainen New Moog (HNM) model as per CMJ jun 2006
// Richard van Hoesel, v. 1.03, Feb. 14 2021
// v1.6 (Infrasonic) removes polyphase FIR, uses 2x linear
//      oversampling for performance reasons, port to DaisySP conventions
// v1.5 adds polyphase FIR or Linear interpolation
// v1.4 FC extended to 18.7kHz, max res to 1.8, 4x oversampling,
//      and a minor Q-tuning adjustment
// v.1.03 adds oversampling, extended resonance,
// and exposes parameters input_drive and passband_gain
// v.1.02 now includes both cutoff and resonance "CV" modulation inputs
// please retain this header if you use this code.
//-----------------------------------------------------------

#pragma once
#ifndef INFS_MOOGLADDER_H
#define INFS_MOOGLADDER_H

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus

namespace infrasonic
{

class MoogLadder
{
    public:
        MoogLadder() {}
        ~MoogLadder() {}

        /** 
            Initializes the MoogLadder module.
            @param sample_rate The sample rate of the audio engine being run. 
        */
        void Init(float sample_rate);

        /** Process and return one input sample. **/
        float Process(const float input);
    
        /** Process mono buffer in place */
        void ProcessInPlace(float *buf, size_t size);

        /** 
            Sets the cutoff frequency or half-way point of the filter.
            Arguments
            @param freq Frequency value in Hz. Range: Any positive value.
        */
        void SetFreq(float freq);

        /** 
            Sets the resonance of the filter.
            @param res Range from 0 - 1.8, self-oscillates at highest values.
        */
        void SetRes(float res);

    private:
        static const uint8_t kInterpolation = 2;
        static constexpr float kInterpolationRecip = 1.0f / kInterpolation;
        static constexpr float kMaxResonance = 1.8f;

        float sample_rate_;
        float alpha_;
        float beta_[4] = {0.0, 0.0, 0.0, 0.0};
        float z0_[4] = {0.0, 0.0, 0.0, 0.0};
        float z1_[4] = {0.0, 0.0, 0.0, 0.0};
        float K_;
        float Fbase_;
        float Qadjust_;
        float pbg_;
        float oldinput_;

        inline float LPF(float s, int i);
        void compute_coeffs(float fc);
};


} // namespace infrasonic
#endif
#endif