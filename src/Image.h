// -*- mode: c++; c-file-style: "linux"; c-basic-offset: 2; indent-tabs-mode: nil -*-
//
//  Copyright (C) 2004-2015 Andrej Vodopivec <andrej.vodopivec@gmail.com>
//            (C) 2014-2015 Gunter Königsmann <wxMaxima@physikbuch.de>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#ifndef IMAGE_H
#define IMAGE_H

#include "MathCell.h"
#include <wx/image.h>

#include <wx/filesys.h>
#include <wx/fs_arc.h>
#include <wx/buffer.h>

/*! Manages an auto-scaling image

  This class keeps two versions of an image:
    - The image in its original compressed format. This way the image can losslessly
      be exported lateron
    - A bitmap version of the image that is scaled down to a size that makes sense 
      with the current viewport.

  Storing images this way has many advantages:
    - It allows us to restrict scaling operations to only once on actually drawing 
      the image after a resize of the wxMaxima window.
    - We can keep (and therefore export and save) jpeg photos in their original 
      format instead of using the .png compression that is superior for line art - 
      but not for depicting real-live images.
    - We save time on saving the .wxmx file since image compression, if done good, 
      might need a good deal of time and this class never needs to compress images
      itself.
    - It allows images to keep their metadata, if needed
    - and if we have big images (big plots or for example photographs) we don't need
      to store them in their uncompressed form.
    - One could even delete the cached scaled images for all cells that currently 
      are off-screen in order to save memory.
 */
class Image
{
public:
  Image();
  /*! A constructor that loads a bitmap

    This constructor actually has to do some compression since we got
    the bitmap in an uncompressed form.
   */
  Image(const wxBitmap &bitmap);
  /*! A constructor that loads an image

    \param image The name of the file
    \param filesystem The filesystem to load it from
    \param viewport_xsize The width of the viewport the image is to be displayed in
    \param viewport_ysize The height of the viewport the image is to be displayed in
    \param remove true = Delete the file after loading it
   */
  Image(wxString image,bool remove = true, wxFileSystem *filesystem = NULL);
  /*! Temporarily forget the scaled image in order to save memory

    Will recreate the scaled image as soon as needed.
   */
  void ClearCache() {if(m_scaledBitmap.GetWidth()>0)m_scaledBitmap.Create(0,0);}
  //! Reads the compressed image into a memory buffer
  wxMemoryBuffer ReadCompressedImage(wxInputStream *data);
  //! Returns the file name extension of the current image
  wxString GetExtension() {return m_extension;};
  //! Loads an image from a file
  void LoadImage(wxString image,bool remove = true, wxFileSystem *filesystem = NULL);
  //! "Loads" an image from a bitmap
  void LoadImage(const wxBitmap &bitmap);
  //! Saves the image in its original form, or as .png if it originates in a bitmap
  wxSize ToImageFile(wxString filename);
  //! Returns the bitmap being displayed
  wxBitmap GetBitmap();
  //! Returns the image in its unscaled form
  wxBitmap GetUnscaledBitmap();
  //! Needs to be called on changing the viewport size 
  void ViewportSize(size_t viewPortWidth,size_t viewPortHeight,double scale);
  //! The width of the scaled image
  size_t m_width;
  //! The height of the scaled image
  size_t m_height;
  //! Returns the original image in its compressed form
  wxMemoryBuffer GetCompressedImage(){return m_compressedImage;}
  size_t GetOriginalWidth(){return m_originalWidth;}
  size_t GetOriginalHeight(){return m_originalHeight;}

protected:
  //! The width of the unscaled image
  size_t m_originalWidth;
  //! The height of the unscaled image
  size_t m_originalHeight;
  //! The current viewport scale
  double m_scale;
  //! The current viewport width
  size_t m_viewportWidth;
  //! The current viewport height
  size_t m_viewportHeight;
  //! The image in its original compressed form
  wxMemoryBuffer m_compressedImage;
  //! The bitmap, scaled down to the screen size
  wxBitmap m_scaledBitmap;
  //! The file extension for the current image type
  wxString m_extension;
};

#endif // IMAGE_H
