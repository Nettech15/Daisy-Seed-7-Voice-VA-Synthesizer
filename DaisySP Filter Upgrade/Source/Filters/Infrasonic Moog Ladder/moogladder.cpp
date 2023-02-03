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
 
#include "moogladder.h"
#include <Utility/dsp.h>

using namespace infrasonic;

static inline float fast_tanh(float x)
{
    if (x > 3.0f) return 1.0f;
    if (x < -3.0f) return -1.0f;
    float x2 = x * x;
    return x * (27.0f + x2) / (27.0f + 9.0f * x2);
}

void MoogLadder::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    alpha_       = 1.0f;
    K_           = 1.0f;
    Fbase_       = 1000.0f;
    Qadjust_     = 1.0f;
    pbg_         = 0.5f;
    oldinput_    = 0.f;

    SetFreq(5000.f);
    SetRes(0.2f);
}

float MoogLadder::Process(const float input)
{
    float total = 0.0f;
    float interp = 0.0f;
    for (size_t os = 0; os < kInterpolation; os++)
    {
        float u = (interp * oldinput_ + (1.0f - interp) * input)
            - (z1_[3] - pbg_ * input) * K_ * Qadjust_;
        u = fast_tanh(u);
        float stage1 = LPF(u, 0);
        float stage2 = LPF(stage1, 1);
        float stage3 = LPF(stage2, 2);
        float stage4 = LPF(stage3, 3);
        total += stage4 * kInterpolationRecip;
        interp += kInterpolationRecip;
    }
    oldinput_ = input;
    return total;
}

void MoogLadder::ProcessInPlace(float *buf, size_t size)
{
    for (size_t i=0; i < size; i++) {
        float input = buf[i];
        float total = 0.0f;
        float interp = 0.0f;
        for (size_t os = 0; os < kInterpolation; os++) {
            float u = (interp * oldinput_ + (1.0f - interp) * input)
                - (z1_[3] - pbg_ * input) * K_ * Qadjust_;
            u = fast_tanh(u);
            float stage1 = LPF(u, 0);
            float stage2 = LPF(stage1, 1);
            float stage3 = LPF(stage2, 2);
            float stage4 = LPF(stage3, 3);
            total += stage4 * kInterpolationRecip;
            interp += kInterpolationRecip;
        }
        oldinput_ = input;
        buf[i] = total;
    }
}

void MoogLadder::SetFreq(float freq)
{
    Fbase_ = freq;
    compute_coeffs(freq);
}

void MoogLadder::SetRes(float res)
{
    // maps resonance = 0->1 to K = 0 -> 4
    res = daisysp::fclamp(res, 0.0f, kMaxResonance);
    K_ = 4.0f * res;
}

float MoogLadder::LPF(float s, int i)
{
    float ft = s * (1.0f/1.3f) + (0.3f/1.3f) * z0_[i] - z1_[i];
    ft = ft * alpha_ + z1_[i];
    z1_[i] = ft;
    z0_[i] = s;
    return ft;
}

void MoogLadder::compute_coeffs(float freq)
{
    freq = daisysp::fclamp(freq, 5.0f, sample_rate_ * 0.425f);
    float wc = freq * (float)(2.0f * PI_F / ((float)kInterpolation * sample_rate_));
    float wc2 = wc * wc;
    alpha_ = 0.9892f * wc - 0.4324f * wc2 + 0.1381f * wc * wc2 - 0.0202f * wc2 * wc2;
    Qadjust_ = 1.006f + 0.0536f * wc - 0.095f * wc2 - 0.05f * wc2 * wc2;
}