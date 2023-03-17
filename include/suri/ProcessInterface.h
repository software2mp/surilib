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

#ifndef PROCESSINTERFACE_H_
#define PROCESSINTERFACE_H_

#include <vector>
#include <string>

#include "suri/ProcessAdaptLayer.h"

namespace suri {

class Part;

/*!
 * Interfaz que representa el comportamiento que posee un proceso dentro del framework.
 */
class ProcessInterface {
public:
   /** constructor **/
   ProcessInterface() : pAdaptLayer_(new ProcessAdaptLayer()) {}

   /** destructor **/
   virtual ~ProcessInterface() {delete pAdaptLayer_;}

   // ------ Metodos que administran la ejecucion del proceso ------
   /** Corre el proceso y genera la salida usando los metodos de configuracion */
   virtual bool RunProcess()=0;

   /** Permite configurar la salida del proceso antes de correrlo */
   virtual bool ConfigureProcess()=0;

   /** Una vez ejecutado el proceso hace visible la salida */
   virtual bool ConfigureOutput()=0;
  /** 
    * Metodo que a partir de un xml que contiene los parametros del proceso configura el mismo.
    * @param[in] XmStr string en formato xml que contiene los parametros necesarios para la 
    * correcta ejecucion del proceso
    * @return true en caso de poder configurar correctamente el proceso en funcion de los parametros
    * @return false en caso contrario
   **/
   virtual bool ConfigureProcessFromXmlString(const std::string& XmlStr)=0;

   /** Configura la capa de adaptacion de proceso **/
  void SetAdaptLayer(ProcessAdaptLayer* pAdaptLayer) {
     if (pAdaptLayer_ ){
        delete pAdaptLayer_;
     }
     pAdaptLayer_  = pAdaptLayer;
     pAdaptLayer_->SetProcessName(this->processName_);
     OnProcessAdaptLayerSet();
   }

   /** Configura el nombre del proceso **/   
   void SetProcessName(const std::string& ProcessName) {
      pAdaptLayer_->SetProcessName(ProcessName);
      processName_ = ProcessName;
   }

   /** Obtiene la capa de adaptacion del prcoeso **/
   ProcessAdaptLayer* GetProcessAdaptLayer() {
      return pAdaptLayer_;
   }

   /** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
   virtual void OnProcessAdaptLayerSet() {}

   /**
    * Obtiene el nombre del proceso desde el adaptLayer
    * Devuelve vacío en caso de que falle
    **/
   std::string GetProcessName() const{
      if (pAdaptLayer_){
         return pAdaptLayer_->GetProcessName();
      }
      return "";
   }

protected:  
   /** Metodo que obtiene los parametros asociados al proceso en formato XML. 
    * Esto permite que el codigo cliente pueda hacer eso del xml resultante 
    * (ej. para la exportacion del mismo)
    * @return string en formato xml con los parametros asociados al proceso
    * @return string vacio en caso contrario. 
   **/
   virtual std::string GetParametersAsXmlString()const=0;
 
   /** capa de provision de atributos **/
   std::string processName_;
   ProcessAdaptLayer* pAdaptLayer_;
};

} /** namespace suri */

#endif /* PROCESSINTERFACE_H_ */
