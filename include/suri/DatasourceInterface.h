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

#ifndef DATASOURCEINTERFACE_H_
#define DATASOURCEINTERFACE_H_

// Includes Estandar
#include <string>
#include <list>

// Includes Suri
#include "SuriObject.h"
#include "FactoryHelpers.h"
#include "Image.h"
#include "Vector.h"
#include "Element.h"
#include "LayerInterface.h"
#include "DatasourceValidatorInterface.h"
// Includes Wx
#include "wx/string.h"
#include "wx/xml/xml.h"

// Defines

// forwards

namespace suri {

/**
 * Define la interfaz de las distintas fuentes de datos
 */
class DatasourceInterface : public SuriObject {

private:
   /** Declara metodos privados y publicos para Factory. */
   HAS_FACTORY(DatasourceInterface, std::string);
   DECLARE_CREATE(DatasourceInterface, const std::string& Value1,
                  const Option& Value2 = Option());
   DECLARE_CREATE(DatasourceInterface, DeclareParameters_1(const wxXmlNode*) );
   DECLARE_CREATE(DatasourceInterface, DeclareParameters_1(Element *) );
   DECLARE_CREATE(DatasourceInterface, DeclareParameters_1(DatasourceInterface *) );

public:

   /** Constructor */
   explicit DatasourceInterface(const wxString& DataType);
   /** Destructor */
   virtual ~DatasourceInterface();
   /** Devuelve el elemento de la fuente de datos \deprecated */
   virtual Element* GetElement()=0;
   /** Devuelve la interpretacion de la fuente de datos como una imagen */
   virtual Image* GetDatasourceAsImage()=0;
   /** Devuelve la interpretacion de la fuente de datos como un vector */
   virtual Vector* GetDatasourceAsVector()=0;
   /** Setea el siguiente Datasource si es que al crearse se detecta multiplicidad */
   virtual bool SetNextDatasource(DatasourceInterface* Datasource)=0;
   /** Devuelve el siguiente Datasource si es que al crearse se detecta multiplicidad */
   virtual DatasourceInterface* GetNextDatasource()=0;
   /** Compara dos tipos diferentes de Datasource */
   virtual bool Equals(DatasourceInterface &Datasource)=0;
   /** Compara un Datasource contra un Layer */
   virtual bool Equals(LayerInterface &Layer)=0;
   /** Compara un Datasource contra un Element \deprecated */
   virtual bool Equals(Element &Element)=0;
   /** Setea el nombre de la fuente de datos */
   virtual bool SetName(const std::string &Name)=0;
   /** Obtiene el nombre de la fuente de datos. Devuelve el string vacio si no tiene nombre */
   virtual std::string GetName() const=0;
   /** Cambia el nombre de la fuente de datos */
   virtual bool SetCopyRight(const std::string &CopyRight)=0;
   /** Cambia la descripcion de una fuente de datos */
   virtual bool SetDescription(const std::string &Description)=0;
   /** Cambia el url de la fuente de datos */
   virtual bool SetUrl(const std::string &Url)=0;
   /** Cambia el icono asociado a la fuente de datos \deprecated */
   virtual bool SetIcon(const std::string &Icon)=0;
   /** Cambia el nombre de la fuente de datos */
   virtual const std::string GetCopyRight() const=0;
   /** Cambia la descripcion de una fuente de datos */
   virtual const std::string GetDescription() const=0;
   /** Cambia el url de la fuente de datos */
   virtual const std::string GetUrl() const=0;
   /** Cambia el icono asociado a la fuente de datos \deprecated */
   virtual const std::string GetIcon() const=0;
   /** Devuelve una instancia del validador asociado a la fuente de datos**/
   virtual DatasourceValidatorInterface* GetAssociatedValidator()const=0;
   /** Obtiene el sistema de referencia que posee la fuente de datos*/
   virtual std::string GetSpatialReference() const=0;
   /** Indica si la fuente de datos se encuentra bloqueada **/
   bool IsBlocked() const;
   /** Bloquea la fuente de datos **/
   bool Block();
   /** Bloquea la fuente de datos **/
   bool Unblock();
   /** Retorna si la fuente de datos es de solo lectura **/
   bool IsReadOnly() const;
   /** Devuelve true si un elemento esta listo para ser utilizado **/
   virtual bool IsValid() const = 0;
   /** Bloquea la edicion de la fuente de datos **/
   bool SetReadOnly();
   /** Bloquea la edicion de la fuente de datos **/
   bool UnsetReadOnly();

   Option GetMetadataOption() const {
      return metadata_;
   }

protected:
   Option metadata_;

private:
   /** bool que indica si la fuente de datos se encuentra bloqueada */
   bool blocked_;
   /** indica si la fuente de datos es de solo lectura */
   bool readonly_;
};

} /** namespace suri */

#endif /* DATASOURCEINTERFACE_H_ */
