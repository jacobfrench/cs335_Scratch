/* Author: Brian Castaneda
 * Program Name: Sounds
 * Purpose: Implementing sounds with collison and game rendering.
*/
#ifndef _brianC_H_
#define _brianC_H_
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "brianC.h"
#include <stdlib.h>
#include <AL/alut.h>
ALuint alSource;
ALuint alBuffer;
void createSound(int trackNo)
{
		//Get started right here.
		alutInit(0, NULL);
		if (alGetError() != AL_NO_ERROR) {
				printf("ERROR: alutInit()\n");
		}
		 // Clear error state.
		alGetError();

		// // Setup the listener.
		// // Forward and up vectors are used.
		float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
		alListenerfv(AL_ORIENTATION, vec);
		alListenerf(AL_GAIN, 1.0f);

		// // Buffer holds the sound information.
		// ALuint alBuffer;
		//alBuffer = alutCreateBufferFromFile("./boing.wav");
	alGenSources(1, &alSource);
		alSourcei(alSource, AL_BUFFER, alBuffer);

	switch(trackNo) {
	case 1:
			alBuffer = alutCreateBufferFromFile("./paddle1.wav");
			alSourcef(alSource, AL_GAIN, 1.0f);
			alSourcef(alSource, AL_PITCH, 1.0f);    
			break;
	case 2:
			alBuffer = alutCreateBufferFromFile("./blopp_converted.wav");
			alSourcef(alSource, AL_GAIN, 1.0f);
			alSourcef(alSource, AL_PITCH, 1.0f);
			break;
	case 3:
			alBuffer = alutCreateBufferFromFile("./pad1a.wav");
			alSourcef(alSource, AL_GAIN, 1.0f);
			alSourcef(alSource, AL_PITCH, 1.0f);
			break;
	case 4:
			alBuffer = alutCreateBufferFromFile("./pad2.wav");
			alSourcef(alSource, AL_GAIN, 1.0f);
			alSourcef(alSource, AL_PITCH, 1.0f);
			break;
	case 5:
			alBuffer = alutCreateBufferFromFile("./win.wav");
			alSourcef(alSource, AL_GAIN, 1.0f);
			alSourcef(alSource, AL_PITCH, 1.0f);  
			break;
	case 6:
			alBuffer = alutCreateBufferFromFile("./block.wav");
			alSourcef(alSource, AL_GAIN, 1.0f);
			alSourcef(alSource, AL_PITCH, 1.0f);         
			break;
	case 8:
			alBuffer = alutCreateBufferFromFile("./explode.wav");
			alSourcef(alSource, AL_GAIN, 1.0f);
			alSourcef(alSource, AL_PITCH, 1.0f);         
			break;
	case 9:
			alBuffer = alutCreateBufferFromFile("./shrink.wav");
			alSourcef(alSource, AL_GAIN, 1.0f);
			alSourcef(alSource, AL_PITCH, 1.0f);         
			break;
		}
		// // Source refers to the sound.
		// // Generate a source, and store it in a buffer.
		// // Set volume and pitch to normal, no looping of sound.
		alSourcei(alSource, AL_LOOPING, AL_FALSE);

		if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		}
		alSourcePlay(alSource);
}

void createSound2()
{
		//Get started right here.
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		}
		 // Clear error state.
		alGetError();

		// // Setup the listener.
		// // Forward and up vectors are used.
		float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
		alListenerfv(AL_ORIENTATION, vec);
		alListenerf(AL_GAIN, 1.0f);
		// // Buffer holds the sound information.
		// ALuint alBuffer;
		//alBuffer = alutCreateBufferFromFile("./boing.wav");
		alGenSources(1, &alSource);
		alSourcei(alSource, AL_BUFFER, alBuffer);
		alBuffer = alutCreateBufferFromFile("./gameMusic.wav");
		alSourcef(alSource,AL_GAIN,1.0f);
		alSourcef(alSource, AL_PITCH,1.0f);
		alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
			printf("ERROR: setting source\n");
	}
		alSourcePlay(alSource);	
}

void cleanSound()
{

	// Cleanup.
 	//First delete the source.
	alDeleteSources(1, &alSource);
	//Delete the buffer.
	alDeleteBuffers(1, &alBuffer);
	// Close out OpenAL itself.
	// Get active context.
	ALCcontext *Context = alcGetCurrentContext();
	// Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	// Disable context.
	alcMakeContextCurrent(NULL);
	// Release context(s).
	alcDestroyContext(Context);
	// Close device.
	alcCloseDevice(Device);

}

#endif
