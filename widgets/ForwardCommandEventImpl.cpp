/*! \file ForwardCommandEventImpl.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "suri/CommandEventInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

const CommandEventInterface::CommandEventFlags CommandEventInterface::None = 0x00000000;
const CommandEventInterface::CommandEventFlags CommandEventInterface::MenuEvent =
      0x00000001;
const CommandEventInterface::CommandEventFlags CommandEventInterface::All =
      CommandEventInterface::MenuEvent;

/*!
 *
 */
class EmptyCommandEvent : public CommandEventInterface {
public:
   EmptyCommandEvent() :
         enabled_(true) {
   }
   virtual ~EmptyCommandEvent() {
   }

   virtual void Execute(wxCommandEvent&) {
   }

   virtual CommandEventInterface::CommandEventFlags GetCommandFlags() const {
      return CommandEventInterface::None;
   }

   virtual void CommandEnabled(bool Enable = true) {
      enabled_ = Enable;
   }
   virtual bool IsCommandEnabled() const {
      return enabled_;
   }

   virtual bool CanProcessEvent(wxCommandEvent&) const {
      return true;
   }

   virtual int GetCommandId() const {
      return -1;
   }
protected:
private:
   bool enabled_;
};

/*!
 *
 *
 */
ForwardCommandEventImpl::ForwardCommandEventImpl() :
      pForwardedClass_(new EmptyCommandEvent) {

}
/*!
 *
 *
 */
ForwardCommandEventImpl::ForwardCommandEventImpl(const std::string &ForwardedClass) :
      pForwardedClass_(NULL) {
}

/*!
 *
 *
 */
ForwardCommandEventImpl::ForwardCommandEventImpl(
      CommandEventInterface *pForwardedInstance) :
      pForwardedClass_(pForwardedInstance) {
}

ForwardCommandEventImpl::~ForwardCommandEventImpl() {

}

void ForwardCommandEventImpl::Execute(wxCommandEvent &Event) {
   pForwardedClass_->Execute(Event);
}

CommandEventInterface::CommandEventFlags ForwardCommandEventImpl::GetCommandFlags() const {
   return pForwardedClass_->GetCommandFlags();
}

void ForwardCommandEventImpl::CommandEnabled(bool Enable) {
   pForwardedClass_->CommandEnabled(Enable);
}

bool ForwardCommandEventImpl::IsCommandEnabled() const {
   return pForwardedClass_->IsCommandEnabled();
}

int ForwardCommandEventImpl::GetCommandId() const {
   return pForwardedClass_->GetCommandId();
}

bool ForwardCommandEventImpl::CanProcessEvent(wxCommandEvent &Event) const {
   return pForwardedClass_->CanProcessEvent(Event);
}

} /** namespace suri */
