#ifndef test_audio_included
#define test_audio_included
//----------------------------------------------------------------------

// https://www.music.mcgill.ca/~gary/rtaudio/

/*
  libjack-jackd2-dev
  libpulse-dev
*/

#define __UNIX_JACK__
//#define __LINUX_ALSA__
//#define __LINUX_PULSE__

#include "extern/rtaudio-4.1.2/RtAudio.h"
#include "extern/rtaudio-4.1.2/RtAudio.cpp"

#include "skei.h"
//#include "skei_types.h"

//----------------------------------------------------------------------

// RtAudio callback function prototype.
/*
  All RtAudio clients must create a function of type RtAudioCallback
  to read and/or write data from/to the audio stream.  When the
  underlying audio system is ready for new input or output data, this
  function will be invoked.

  outputBuffer
    For output (or duplex) streams, the client should write nFrames of audio
    sample frames into this buffer. This argument should be recast to the
    datatype specified when the stream was opened. For input-only streams,
    this argument will be NULL.

  inputBuffer
    For input (or duplex) streams, this buffer will hold nFrames of input audio
    sample frames. This argument should be recast to the datatype specified
    when the stream was opened. For output-only streams, this argument will be
    NULL.

  nFrames
    The number of sample frames of input or output data in the buffers. The
    actual buffer size in bytes is dependent on the data type and number of
    channels in use.

  streamTime
    The number of seconds that have elapsed since the stream was started.

  status
    If non-zero, this argument indicates a data overflow or underflow condition
    for the stream. The particular condition can be determined by comparison
    with the RtAudioStreamStatus flags.

  userData
    A pointer to optional data provided by the client when opening the stream
    (default = NULL).

  To continue normal stream operation, the RtAudioCallback function
  should return a value of zero.  To stop the stream and drain the
  output buffer, the function should return a value of one.  To abort
  the stream immediately, the client should return a value of two.
 */

int audio_callback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data )
{
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if ( status ) std::cout << "Stream over/underflow detected." << std::endl;
  unsigned long *bytes = (unsigned long *) data;
  memcpy(outputBuffer,inputBuffer,*bytes);
  return 0;
}

//----------------------------------------------------------------------

int main(void) {

  RtAudio audio;
  RtAudio::DeviceInfo info;
  RtAudio::StreamParameters parameters;
  RtAudio::StreamOptions options;

  // Determine the number of devices available
  uint32 devices = audio.getDeviceCount();
  // Scan through devices for various capabilities
  for (uint32 i=0; i<devices; i++ ) {
    DTrace("device %i\n",i);

    info = audio.getDeviceInfo( i );
    if (info.probed == true) {

      //--- probe ---

      DTrace("  name: %s\n",info.name.c_str());
      DTrace("  max input channels: %i\n",info.inputChannels);
      DTrace("  max output channels: %i\n",info.outputChannels);
      DTrace("  max duplex channels: %i\n",info.duplexChannels);

      uint32 num = info.sampleRates.size();
      DTrace("  supported sample rates: ");
      for (uint32 j=0; j<num; j++ ) DTrace("%i ",info.sampleRates[j]);
      DTrace("\n");

      uint32 fmt = info.nativeFormats;
      DTrace("  native formats: ");
      if (fmt & RTAUDIO_SINT8)    DTrace( "int8 " );
      if (fmt & RTAUDIO_SINT16)   DTrace( "int16 " );
      if (fmt & RTAUDIO_SINT24)   DTrace( "int24 " );
      if (fmt & RTAUDIO_SINT32)   DTrace( "int32 " );
      if (fmt & RTAUDIO_FLOAT32)  DTrace( "float32 " );
      if (fmt & RTAUDIO_FLOAT64)  DTrace( "float64 " );
      DTrace("\n");

      //--- init ---

      parameters.deviceId = audio.getDefaultOutputDevice();
      parameters.nChannels = 2;
      unsigned int sampleRate = 44100;
      unsigned int bufferFrames = 256; // 256 sample frames
      options.flags = RTAUDIO_NONINTERLEAVED;

      //try {
        audio.openStream(
          &parameters,
          NULL,
          RTAUDIO_FLOAT32,
          sampleRate,
          &bufferFrames,
          &audio_callback,
          NULL,
          &options
        );
      //}
      //catch ( RtAudioError& e ) {
      //  std::cout << '\n' << e.getMessage() << '\n' << std::endl;
      //  exit(0);
      //}

      audio.startStream();

      if (audio.isStreamRunning()) audio.stopStream();
      if (audio.isStreamOpen()) audio.closeStream();

    }
  }

  return 0;
}

//----------------------------------------------------------------------
#endif
