/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
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

#include "play_sd_raw.h"
#include "spi_interrupt.h"

#define B2M (uint32_t)((double)4294967296000.0 / AUDIO_SAMPLE_RATE_EXACT / 2.0) // 97352592


void AudioLoadSdRaw::begin(void)
{
	playing = false;
	speed = 1;
}

bool AudioLoadSdRaw::load(const char *filename)
{
	stop();
	__disable_irq();
	if (rawfile) {
		rawfile.close();
	}
	rawfile = SD.open(filename);
	__enable_irq();
	if (!rawfile) {
		//Serial.println("unable to open file");
		return false;
	}
	rewind();
}

bool AudioLoadSdRaw::setPos(uint64_t pos) /* closest block at this time */
{
	uint32_t file_offset;
	file_offset = (skip<<32) / B2M;

	file_offset /= 512;
	file_offset *= 512;
    
	rawfile.seek(file_offset);
}

void AudioLoadSdRaw::setSpeed(int speedIn)
{
	if (speedIn) {
		if (speed < 0) {
			goForward = 0;
			speed = -speedIn;
		} else {
			goForward = 1;
			speed = speedIn;
			}
		}
}

void AudioLoadSdRaw::rewind()
{
	if (goForward) {
	        rawfile.seek(0);
	} else {
		rawfile.seek((rawfile.size()/512)*512);
		}
}

void AudioLoadSdRaw::play()
{
	stop();
	AudioStartUsingSPI();
	if (!rawfile) {
		//Serial.println("No file loaded!\n");
		return false;
		}

	//Serial.println("able to open file");
	playing = true;
	return true;
}

void AudioLoadSdRaw::stop(void)
{
	__disable_irq();
	if (playing) {
		playing = false;
		__enable_irq();
		AudioStopUsingSPI();
	} else {
		__enable_irq();
	}
}

void AudioLoadSdRaw::update(void)
{
	unsigned int i, n;
	audio_block_t *block;

	// only update if we're playing
	if (!playing) return;

	// allocate the audio blocks to transmit
	block = allocate();
	if (block == NULL) return;

	if (goForward) {
		moreData = rawFile.available();
	} else {
		moreData = rawfile.position() > 0;
		rawfile.seek(rawfile.position()-AUDIO_BLOCK_SAMPLES*4); // *2 would take us back to where we just were
	}

	if (moredata) {
		// we can read more data from the file...
		n = rawfile.read(block->data, AUDIO_BLOCK_SAMPLES*2);
		for (i=n/2; i < AUDIO_BLOCK_SAMPLES; i++) {
			block->data[i] = 0;
		}
		if (! goForward) {
			reverseMem(block->data);
		}
		transmit(block);
	} else {
		rawfile.close();
		AudioStopUsingSPI();
		playing = false;
	}
	release(block);
}

void AudioLoadSdRaw::reverseMem(uint16_t *data)
{
	int i;
	uint16_t tmp;
	for (i=0; i<AUDIO_BLOCK_SAMPLES/2-1; i++) {
		tmp = data[i];
		data[i] = data[AUDIO_BLOCK_SAMPLES-i-1];
		data[AUDIO_BLOCK_SAMPLES-i-1] = tmp;
	}
}

uint32_t AudioLoadSdRaw::positionMillis(void)
{
	return ((uint64_t)rawfile.position() * B2M) >> 32;
}

uint32_t AudioLoadSdRaw::lengthMillis(void)
{
	return ((uint64_t)rawfile.size() * B2M) >> 32;
}


