
#include "SoundPlayer.h"


bool CSoundPlayer::LoadAudio(string filename)
{
	string fname = "sounds/" + filename;
    cout << "loading.. " << fname;
	// delete current data if it exists
    if (pAudioChunk != NULL) { Mix_HaltChannel( Channel); Mix_FreeChunk( pAudioChunk); }
    // load new wav data from file
	pAudioChunk = Mix_LoadWAV( fname.c_str());

	if (pAudioChunk == NULL) { cout << "..ERROR" << endl; Channel=0; return false; } 
	else cout << "..ok" << endl;
	return true;
}

void CSoundPlayer::Play(int loops)
{
  if (loops < 0) loops=0;
  if (pAudioChunk != NULL)  Channel = Mix_PlayChannel(-1, pAudioChunk, loops-1);
  else return;
  //Mix_Volume( Channel, Volume); // set volume of current channel
  //cout << Channel << endl;
   
}


void CSoundPlayer::Play(string filename, int loops)
{
   if (pAudioChunk == NULL)
   {
	 if (!LoadAudio(filename)) return;
   }
   if (loops==-1) Play(0);
   else Play(loops); 
	
}

void CSoundPlayer::SetVolume( int volume)  
{ 
   if (volume < 0 || volume > 100) return;
   Volume = volume;
   Mix_Volume( Channel, Volume); // set volume of current channel
}


void CSoundPlayer::Clear()
{
     if (pAudioChunk != NULL) Mix_FreeChunk( pAudioChunk);
	 pAudioChunk = NULL; Channel = 0;
}


void CSoundPlayer::Pause()
{
  Mix_Pause( Channel);
}

void CSoundPlayer::Resume()
{
  Mix_Resume( Channel);
}



	
