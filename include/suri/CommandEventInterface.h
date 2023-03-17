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
