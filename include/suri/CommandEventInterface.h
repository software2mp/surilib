/*! \file CommandEventInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COMMANDEVENTINTERFACE_H_
#define COMMANDEVENTINTERFACE_H_

// Includes Estandar
#include <string>

// Includes Suri

// Includes Wx

// Defines

// forwards
class wxCommandEvent;

namespace suri {

/*!
 *
 */
class CommandEventInterface {
public:
   typedef int CommandEventFlags;
   static const CommandEventFlags None;
   static const CommandEventFlags MenuEvent;
   static const CommandEventFlags All;

   CommandEventInterface() {
   }
   virtual ~CommandEventInterface() {
   }

   virtual void Execute(wxCommandEvent&)=0;

   virtual CommandEventFlags GetCommandFlags() const=0;

   virtual void CommandEnabled(bool Enable = true)=0;
   virtual bool IsCommandEnabled() const=0;

   virtual bool CanProcessEvent(wxCommandEvent&) const=0;

   virtual int GetCommandId() const=0;
protected:
private:
};

class ForwardCommandEventImpl : public CommandEventInterface {
public:
   ForwardCommandEventImpl();
   ForwardCommandEventImpl(const std::string &ForwardedClass);
   ForwardCommandEventImpl(CommandEventInterface *pForwardedInstance);
   virtual ~ForwardCommandEventImpl();

   virtual void Execute(wxCommandEvent&);

   virtual CommandEventFlags GetCommandFlags() const;

   virtual void CommandEnabled(bool Enable = true);
   virtual bool IsCommandEnabled() const;

   virtual bool CanProcessEvent(wxCommandEvent&) const;

   virtual int GetCommandId() const;
protected:
private:
   CommandEventInterface *pForwardedClass_;
};

} /* namespace suri */

#endif /* COMMANDEVENTINTERFACE_H_ */
