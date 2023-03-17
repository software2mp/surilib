/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#ifndef PAINTEVENTINTERFACE_H_
#define PAINTEVENTINTERFACE_H_

// Includes Estandar
#include <string>

// Includes Suri

// Includes Wx

// Defines

// forwards
class wxPaintEvent;
class wxEraseEvent;
class wxDC;

namespace suri {

class PaintEventInterface {
protected:
   PaintEventInterface() {
   }
   PaintEventInterface(const PaintEventInterface&);
public:
   typedef int PaintEventFlags;
   static const PaintEventFlags NoneFlag;
   static const PaintEventFlags Paint;
   static const PaintEventFlags Background;
   static const PaintEventFlags All;

   virtual ~PaintEventInterface() {
   }
   virtual void OnPaint(wxPaintEvent&, wxDC&)=0;
   virtual void OnBackgroundErase(wxEraseEvent&)=0;
   virtual PaintEventFlags GetPaintFlags() const=0;

   virtual void PaintEnabled(bool Enable = true)=0;
   virtual bool IsPaintEnabled() const=0;

   virtual bool CanProcessEvent(wxPaintEvent&) const=0;
protected:
private:
};

class ForwardPaintEventImpl : public PaintEventInterface {
   ForwardPaintEventImpl(const ForwardPaintEventImpl&);
public:
   ForwardPaintEventImpl();
   ForwardPaintEventImpl(const std::string &ForwardedClass);
   ForwardPaintEventImpl(PaintEventInterface *pForwardedInstance);

   virtual ~ForwardPaintEventImpl();
   virtual void OnPaint(wxPaintEvent&, wxDC&);
   virtual void OnBackgroundErase(wxEraseEvent&);
   virtual PaintEventFlags GetPaintFlags() const;

   virtual void PaintEnabled(bool Enable = true);
   virtual bool IsPaintEnabled() const;

   virtual bool CanProcessEvent(wxPaintEvent&) const;
protected:
private:
   PaintEventInterface *pForwardedClass_;
};

}  // namespace suri

#endif /* PAINTEVENTINTERFACE_H_ */
