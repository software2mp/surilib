/*! \file ForwardPaintEventImpl.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/PaintEventInterface.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

const PaintEventInterface::PaintEventFlags PaintEventInterface::NoneFlag = 0x00000000;
const PaintEventInterface::PaintEventFlags PaintEventInterface::Paint = 0x00000001;
const PaintEventInterface::PaintEventFlags PaintEventInterface::Background = Paint << 1;
const PaintEventInterface::PaintEventFlags PaintEventInterface::All = Paint
      | Background;

class EmptyPaintEvent : public PaintEventInterface {
   EmptyPaintEvent(const EmptyPaintEvent&);
public:
   EmptyPaintEvent() :
         enabled_(true) {
   }
   virtual ~EmptyPaintEvent() {
   }
   virtual void OnPaint(wxPaintEvent&, wxDC&) {
   }
   virtual void OnBackgroundErase(wxEraseEvent&) {
   }
   virtual PaintEventFlags GetPaintFlags() const {
      return PaintEventInterface::NoneFlag;
   }

   virtual void PaintEnabled(bool Enable = true) {
      enabled_ = Enable;
   }
   virtual bool IsPaintEnabled() const {
      return enabled_;
   }

   virtual bool CanProcessEvent(wxPaintEvent&) const {
      return true;
   }
protected:
private:
   bool enabled_;
};

ForwardPaintEventImpl::ForwardPaintEventImpl() :
      pForwardedClass_(new EmptyPaintEvent) {

}

/*!
 *
 *
 */
ForwardPaintEventImpl::ForwardPaintEventImpl(const std::string &ForwardedClass) :
      pForwardedClass_(NULL) {

}

/*!
 *
 *
 */
ForwardPaintEventImpl::ForwardPaintEventImpl(PaintEventInterface *pForwardedInstance) :
      pForwardedClass_(pForwardedInstance) {

}

/*!
 *
 *
 */
ForwardPaintEventImpl::~ForwardPaintEventImpl() {

}

/*!
 *
 *
 */
void ForwardPaintEventImpl::OnPaint(wxPaintEvent &Event, wxDC &DeviceContext) {
   pForwardedClass_->OnPaint(Event, DeviceContext);
}

/*!
 *
 *
 */
void ForwardPaintEventImpl::OnBackgroundErase(wxEraseEvent &Event) {
   pForwardedClass_->OnBackgroundErase(Event);
}

/*!
 *
 *
 */
PaintEventInterface::PaintEventFlags ForwardPaintEventImpl::GetPaintFlags() const {
   return pForwardedClass_->GetPaintFlags();
}

/*!
 *
 *
 */
void ForwardPaintEventImpl::PaintEnabled(bool Enable) {
   pForwardedClass_->PaintEnabled(Enable);
}

/*!
 *
 *
 */
bool ForwardPaintEventImpl::IsPaintEnabled() const {
   return pForwardedClass_->IsPaintEnabled();
}

bool ForwardPaintEventImpl::CanProcessEvent(wxPaintEvent &Event) const {
   return pForwardedClass_->CanProcessEvent(Event);
}

}  // namespace suri

