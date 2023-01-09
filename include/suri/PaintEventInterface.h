/*! \file PaintEventInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
