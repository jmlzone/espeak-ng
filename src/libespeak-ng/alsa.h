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

#ifndef HAVE_DIRECT_ALSA_H
#define HAVE_DIRECT_ALSA_H

void alsa_write_buf(void);
void alsa_send_sample(int sample);
void alsa_send_samples(short * ibuf, int samples);
void alsa_open_device( char * alsaDeviceName);
void alsa_terminate (void);

#endif
