/* eSpeak NG API.
 *
 * Copyright (C) 2018 James M. Lee
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#define HAVE_ENDIAN_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alsa/asoundlib.h>
#include "alsa.h"
#define BUFLEN 128
#define CHAN 1
short buf[BUFLEN * CHAN];
snd_pcm_t *playback_handle;
int buf_idx;
int err;
int alsa_init_done;
void alsa_write_buf(void){
    if ((err = snd_pcm_writei (playback_handle, buf, BUFLEN)) != BUFLEN) {
      fprintf (stderr, "write to audio interface failed (%s)\n",
	       snd_strerror (err));
      exit (1);
    }
}
void alsa_send_sample(int sample) {
  int i;
  for (i=0; i<CHAN; i++){
    buf[buf_idx++] = sample;
    if( buf_idx == BUFLEN) {
      alsa_write_buf();
      buf_idx=0;
    }
  }
}
void alsa_send_samples(short * ibuf, int samples) {
  int i;
  if(alsa_init_done) {
    //printf("sending %d samples\n", samples);
      for(i=0; i<samples; i++) {
	alsa_send_sample(ibuf[i]);
      }
  }
}

void alsa_open_device( char * alsaDeviceName) {
  unsigned int sampleRate = 22050;
  snd_pcm_hw_params_t *hw_params;
  buf_idx=0;
  if((alsaDeviceName == NULL) || (strlen(alsaDeviceName) == 0)) {
    //null device, so dont do anything
    fprintf(stderr, "bypassing alsa\n");
    alsa_init_done = 0;
    return;
  }
  //printf("trying to open %s\n", alsaDeviceName);
  if ((err = snd_pcm_open (&playback_handle, alsaDeviceName, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    fprintf (stderr, "cannot open audio device %s (%s)\n", 
	     alsaDeviceName,
	     snd_strerror (err));
    exit (1);
  }
  //printf("success\n");
		   
  //printf("trying to Malloc\n");
  if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
    fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
	     snd_strerror (err));
    exit (1);
  }
  //printf("success\n");
				 
  //printf("trying to initialize hardware params\n");				 
  if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
    fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
	     snd_strerror (err));
    exit (1);
  }
  //printf("success\n");
  
  //printf("trying to set access\n");	
  if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    fprintf (stderr, "cannot set access type (%s)\n",
	     snd_strerror (err));
    exit (1);
  }
  //printf("success\n");
	
  //printf("trying to set format\n");
  if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
    fprintf (stderr, "cannot set sample format (%s)\n",
	     snd_strerror (err));
    exit (1);
  }
	
  if ((err = snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, &sampleRate, 0)) < 0) {
    fprintf (stderr, "cannot set sample rate (%s)\n",
	     snd_strerror (err));
    exit (1);
  }
	
  if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, CHAN)) < 0) {
    fprintf (stderr, "cannot set channel count (%s)\n",
	     snd_strerror (err));
    exit (1);
  }
	
  if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
    fprintf (stderr, "cannot set parameters (%s)\n",
	     snd_strerror (err));
    exit (1);
  }
	
  snd_pcm_hw_params_free (hw_params);
	
  if ((err = snd_pcm_prepare (playback_handle)) < 0) {
    fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
	     snd_strerror (err));
    exit (1);
  }
  alsa_init_done=1;
}
 	
void alsa_terminate (void) {
  if(alsa_init_done) {
    snd_pcm_drain (playback_handle);
    snd_pcm_close (playback_handle);
  }
  alsa_init_done = 0;
}
