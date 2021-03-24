//
// Created by vfs on 5/23/2020.
//

//LPDIRECTSOUNDBUFFER second_buffer;
//game_sound_output sound_output;

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD *dwChunkSize, DWORD *dwChunkDataPosition) {
	HRESULT hr = S_OK;
	if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, 0, NULL, FILE_BEGIN ) )
		return HRESULT_FROM_WIN32( GetLastError() );

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if( 0 == ReadFile( hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL ) )
			hr = HRESULT_FROM_WIN32( GetLastError() );

		if( 0 == ReadFile( hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL ) )
			hr = HRESULT_FROM_WIN32( GetLastError() );

		switch (dwChunkType)
		{
			case fourccRIFF:
				dwRIFFDataSize = dwChunkDataSize;
				dwChunkDataSize = 4;
				if( 0 == ReadFile( hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL ) )
					hr = HRESULT_FROM_WIN32( GetLastError() );
				break;

			default:
				if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, dwChunkDataSize, NULL, FILE_CURRENT ) )
					return HRESULT_FROM_WIN32( GetLastError() );
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			*dwChunkSize = dwChunkDataSize;
			*dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;

}

HRESULT ReadChunkData(HANDLE hFile, void *buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if( INVALID_SET_FILE_POINTER == SetFilePointer( hFile, bufferoffset, NULL, FILE_BEGIN ) )
		return HRESULT_FROM_WIN32( GetLastError() );
	DWORD dwRead;
	if( 0 == ReadFile( hFile, buffer, buffersize, &dwRead, NULL ) )
		hr = HRESULT_FROM_WIN32( GetLastError() );
	return hr;
}

ZOMBIES_RESULT init_sound(HWND window, int32_t samplesPerSec, int32_t bufferSize) {
//	IXAudio2 *xaudio = NULL;
//
//	XAudio2Create(&xaudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
//
//	IXAudio2MasteringVoice *master_voice = NULL;
//	xaudio->CreateMasteringVoice(&master_voice);
//
//	HANDLE file = CreateFileA("music/thunder.wav",
//			GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	SetFilePointer(file, 0, NULL, FILE_BEGIN);
//
//	DWORD dwChunkSize;
//	DWORD dwChunkPosition;
////check the file type, should be fourccWAVE or 'XWMA'
//	FindChunk(file, fourccRIFF, &dwChunkSize, &dwChunkPosition);
//	DWORD filetype;
//	ReadChunkData(file, &filetype, sizeof(DWORD), dwChunkPosition);
//	if (filetype != fourccWAVE)
//		return ZOMBIES_ERROR;
//
//
//	WAVEFORMATEXTENSIBLE wfx = {0};
//	XAUDIO2_BUFFER buffer = {0};
//
//	FindChunk(file, fourccFMT, &dwChunkSize, &dwChunkPosition);
//	ReadChunkData(file, &wfx, dwChunkSize, dwChunkPosition);
//
//
//	//fill out the audio data buffer with the contents of the fourccDATA chunk
//	FindChunk(file, fourccDATA, &dwChunkSize, &dwChunkPosition);
//	BYTE * pDataBuffer = new BYTE[dwChunkSize];
//	ReadChunkData(file, pDataBuffer, dwChunkSize, dwChunkPosition);
//
//
//	buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
//	buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
//	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
//
//	IXAudio2SourceVoice *source_voice;
//	xaudio->CreateSourceVoice(&source_voice, (WAVEFORMATEX *) &wfx);
//	source_voice->SubmitSourceBuffer(&buffer);
	//source_voice->Start();

//	sound_output.samplesPerSecond = 48000;
//	sound_output.hz = 440;
//	sound_output.volume = 800;
//	sound_output.runningSampleIndex = 0;
//	sound_output.period = sound_output.samplesPerSecond / sound_output.hz;
//	sound_output.bytesPerSample = sizeof(int16_t) * 2;
//	sound_output.bufferSize = sound_output.samplesPerSecond * sound_output.bytesPerSample;
//
//
//	LPDIRECTSOUND directSound;
//
//	if (SUCCEEDED(DirectSoundCreate(0, &directSound, 0))) {
//		WAVEFORMATEX waveFormat = {};
//		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
//		waveFormat.nChannels = 2;
//		waveFormat.nSamplesPerSec = samplesPerSec;
//		waveFormat.wBitsPerSample = 16;
//		waveFormat.nBlockAlign = waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
//		waveFormat.nAvgBytesPerSec = waveFormat.nBlockAlign * waveFormat.nSamplesPerSec;
//		waveFormat.cbSize = 0;
//
//		if (SUCCEEDED(directSound->SetCooperativeLevel(window, DSSCL_PRIORITY))) {
//			DSBUFFERDESC bufferDesc = {};
//			bufferDesc.dwSize = sizeof(bufferDesc);
//			bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
//
//			LPDIRECTSOUNDBUFFER primaryBuffer;
//
//			if (SUCCEEDED(directSound->CreateSoundBuffer(&bufferDesc, &primaryBuffer, 0))) {
//
//				HRESULT error = primaryBuffer->SetFormat(&waveFormat);
//				if (SUCCEEDED(error)) {
//					LOG_INFO("The sound format was set");
//				} else {
//					return ZOMBIES_ERROR;
//				}
//			} else {
//				return ZOMBIES_ERROR;
//			}
//		} else {
//			return ZOMBIES_ERROR;
//		}
//
//		DSBUFFERDESC bufferDesc = {};
//		bufferDesc.dwSize = sizeof(bufferDesc);
//		bufferDesc.dwFlags = 0;
//		bufferDesc.dwBufferBytes = bufferSize;
//		bufferDesc.lpwfxFormat = &waveFormat;
//
//
//
//		HRESULT error = directSound->CreateSoundBuffer(&bufferDesc, &second_buffer, 0);
//		if (SUCCEEDED(error)) {
//			LOG_INFO("The second sound buffer was created");
//		}
//	} else {
//		return ZOMBIES_ERROR;
//	}

	return ZOMBIES_SUCCESS;
}

//void update_sound() {
//
//	DWORD playCursor;
//	DWORD writeCursor;
//
//	if (SUCCEEDED(second_buffer->GetCurrentPosition(&playCursor, &writeCursor))) {
//
//		DWORD byteToLock = (sound_output.runningSampleIndex * sound_output.bytesPerSample) % sound_output.bufferSize;
//		DWORD bytesToWrite = 0;
//
//		if (byteToLock > playCursor) {
//			bytesToWrite = sound_output.bufferSize - byteToLock;
//			bytesToWrite += playCursor;
//		} else {
//			bytesToWrite = playCursor - byteToLock;
//		}
//
//		fill_sound_buffer(&sound_output, byteToLock, bytesToWrite);
//	}
//}

//void fill_sound_buffer(game_sound_output *soundOutput, DWORD byteToLock, DWORD bytesToWrite) {
//	LPVOID region1;
//	DWORD region1Size;
//	LPVOID region2;
//	DWORD region2Size;
//
//	if (SUCCEEDED(second_buffer->Lock(byteToLock, bytesToWrite, &region1, &region1Size, &region2, &region2Size, 0))) {
//		int16_t *sampleOut = (int16_t *) region1;
//
//		DWORD region1SampleCount = region1Size / soundOutput->bytesPerSample;
//		for (DWORD sampleIndex = 0; sampleIndex < region1SampleCount; ++sampleIndex) {
//			float t = 2.0f * (float) M_PI * ((float) soundOutput->runningSampleIndex / (float) soundOutput->period);
//			float sineValue = sinf(t);
//			int16_t sampleValue = (int16_t) (sineValue * soundOutput->volume);
//
//			*sampleOut++ = sampleValue;
//			*sampleOut++ = sampleValue;
//			++soundOutput->runningSampleIndex;
//		}
//
//		sampleOut = (int16_t *) region2;
//		DWORD region2SampleCount = region2Size / soundOutput->bytesPerSample;
//		for (DWORD sampleIndex = 0; sampleIndex < region2SampleCount; ++sampleIndex) {
//
//			float t = 2.0f * (float) M_PI * ((float) soundOutput->runningSampleIndex / (float) soundOutput->period);
//			float sineValue = sinf(t);
//			int16_t sampleValue = (int16_t) (sineValue * soundOutput->volume);
//
//			*sampleOut++ = sampleValue;
//			*sampleOut++ = sampleValue;
//			++soundOutput->runningSampleIndex;
//		}
//
//		if (SUCCEEDED(second_buffer->Unlock(region1, region1Size, region2, region2Size))) {
//			printf("finished writing buffer\n");
//		}
//	}
//}
