/*********************************************************************
GTEC - Games Technology Game Engine
Copyright (C) 2012-2020  Andreas Hoppe

This program is free software; you can redistribute it and /or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; If not, see < http://www.gnu.org/licenses/>.
*************************************************************************/
#include "GTEC.h"

class CSoundPlayer
{

public:

	// ----  Constructors ------	
  CSoundPlayer() { pAudioChunk = NULL; Channel = 0; Volume = 100;}

  // -----  Destructor --------
  ~CSoundPlayer() { if (pAudioChunk != NULL) { Mix_HaltChannel( Channel); Mix_FreeChunk( pAudioChunk); }}

private:
  Mix_Chunk *pAudioChunk;  // pointer to audio data
  int Channel;			   // audio channel
  int Volume;			   // volume
  
  
public:
 
 
  // Load audio from a .wav file
  bool LoadAudio( string filename);

  // Play a certain number of loops
  void Play(int loops);

  // play once
  void Play() 	{ Play(1); }

  // play continuously
  void PlayAlways() { Play(0); }
  
  // to be compatible with GFC
  void Play(string filename, int loops=1);

  // set volume in %
  void SetVolume( int volume);
  
  void Stop() { Mix_HaltChannel( Channel); }

  void Pause();

  void Resume();
  
  void Clear();
	
};
