#ifndef skei_module_included
#define skei_module_included
//----------------------------------------------------------------------



//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#if 0

/*
  - parameter interpolation, start/end, linearly interpolate  inbetween,
    through block.. each block, calculate new values, interpolate from
    previous set..
  - oversampling
*/

//----------

#define BLOCK_SIZE 32

//----------

//class SModule;
//typedef SArray<SModule*> SModules;

typedef void(*SModuleProc)(void* MData, float* buffer, uint32 length);
typedef SArray<SModuleProc> SModuleProcs;

//----------

struct SModuleData {
};

class SModule {
  private:

  public:

    void preProcessSubBlock(void) {
      // midi..
      // sync
      // interpolated parameters

    }

    //----------

    void processSubBlock(int32 numSamples) {
    }

    //----------

    void processFullSubBlock(void) {
      processSubBlock(BLOCK_SIZE);
    }

    //----------

    void postProcessSubBlock(void) {
    }

    //----------

    void processBlock(int32 numSamples) {
      int blocksize = BLOCK_SIZE;
      while (numSamples>0) {
        preProcessSubBlock();
        numSamples -= blocksize;
        if (numSamples<0) {
          blocksize += numSamples;
          processSubBlock(blocksize);
        }
        else processFullSubBlock();
        postProcessSubBlock();
      }
    }

};

#undef BLOCK_SIZE

#endif // 0

//----------------------------------------------------------------------
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=1609178#p1609178

  block processing *IS* way to go ;)
  There are several "schools" for this. Some process each voice one by one,
  making sub-blocks for every so and so many samples until something
  significant happens (NoteOn, NoteOff, MidiCCs), some process every voice in
  slices of so and so many samples.

  The trick is typically finding a control rate for your modulations that's
  adequate for the kind of process. 32 samples typically fits them all, except
  for compressors maybe. So, you'd process every part of your plugin in slices
  of 32 samples for instance. First you update modulations, then you process
  oscillators for 32 samples, the filters for 32 samples, etc. When processing
  a filter for instance, you calculate the cutoff/res at the beginning and at
  the end of the block. While processing the samples in a tight loop, you
  simply interpolate coefficients and stuff linearly.

  The advantage of block processing comes from its cache friendliness, from
  having as many variables kept in cpu registers as possible (The fewer
  variables in your loop, the fewer loads and stores) and from avoiding
  branches. Never forget that the instructions have to be cached, too. A large
  one-synth-in-a-single-function approach might mean that your loop doesn't fit
  into one L1 cache line, which slows everything down big time!

  Cheers,
  ;) Urs

  Oh, I forgot one thing.

  If you process in blocks, the compiler can oftenly optimize loops by
  unrolling them and parallelizing instructions, thereby keeping the cpu
  pipeline busy. That's probably the most important part of it.

  In my engine for instance I "guarantee" each module a multiple of 16 samples
  for processing. Thus I can do this:

  for (int s=0; s<numSlicesOf16samples; s++) {
    for (int n=0; n<16; n++) {
      // do something
    }
  }

  You can easily imagine that the inner loop can be optimized like hell, which
  wouldn't as good if there was a variable instead of a 16.

  However, this also means that you have to do clever caching, as hosts don't
  always give you a multiple of 16 samples to process. Thus I have introduced a
  latency of 16 samples. But it's fast like shit..

  Here's a simplified example of blockprocessing:

  void processEnv ( int inEnv, int voice );
  void processLFO ( int inEnv, int voice );

  void processOSC( int inOSC, float* out, int numSamples, int voice ) { ... }
  void processVCF( int inOSC, float* out, int numSamples, int voice ) { ... }

  void whatever...

  float tempBuffer[ 32 ];

  void processStuff( float* out, int numSamples ) {
    int blocksize = 32;
    while ( numSamples > 0 ) {
      // do something here to check for Midi events, voice stealing etc.
      numSamples -= blocksize;
      if ( numSamples < 0 ) blocksize += numSamples;
      for ( int v = 0, v < numVoices; v++ ) {
         processEnv ( 0, v );
         processEnv ( 1, v );
         processEnv ( 2, v );
         processLFO ( 0, v );
         processLFO ( 1, v );
         clearBuffer( tempBuffer, blocksize );
         processOSC( 0, tempBuffer, blocksize, v );
         processOSC( 1, tempBuffer, blocksize, v );
         processVCF( 1, tempBuffer, blocksize, v );
         mix2output( out, tempBuffer, blocksize, v );
      }
    }
  }

  //----------

  http://www.kvraudio.com/forum/viewtopic.php?p=1610698#p1610698

  I have an internal voice-buffer where I work on my voice before I mix it into
  the "real"-buffer.

  This makes it easier to have a semi-modular configuration with varying number
  of oscillators, filters and such

  By the way, I don't even mix it to the real buffer but to another global
  buffer where global effects (delay, rev, comp/lim) is applied.

  Another good thing with having internal buffers is that you can make the
  "replace" or "accumulate" desicion outside the innerloop, instead of multiple
  versions of routines.

  My main point is that one should have neat looking innerloops.

  and by the way.. you will almost for sure end up with buffers for control
  data (from LFOs and EGs) if you incorporate modulation matrix.

  Jonas

  //----------

  One (unrelated) question: Do you avoid object orientation features for
  performance reasons or was it just this example?

  OOP is what you make it. C++ classes certainly provide a cool way to do it,
  but OOP isn't bound to that approach. I mostly work with callbacks that take
  void pointer structs as arguments to do pretty much the same as C++ classes,
  but I avoid having to include each and every header in central files. This
  brings my compile times down and forces me to write cleaner code. No more no
  less.

  (In fact I've almost forgotten about C++ stuff... the only keyword I still
  use is "namespace")

*/
