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

#ifndef DRAWVECTORBUTTON_H_
#define DRAWVECTORBUTTON_H_

// Includes standard
#include <vector>

// Includes Suri
#include "suri/Vector.h"
#include "suri/Coordinates.h"
#include "suri/VectorElement.h"
#include "suri/Subject.h"
#include "suri/VectorEditor.h"
// Includes Wx

// Includes App

// Defines
/** Utilizado para definir la categoria del vector. \todo se podria implementar con enum */
#define DRAW_TABLE_NAME "draw"
/** Utilizado para definir la categoria del vector. \todo se podria implementar con enum */
#define MEASSURE_TABLE_NAME "meassure"

/** Utiliza MemoryVector durante la edicion de vectores */
#define __USE_MEMORY_VECTOR_DURING_EDITION__

// Includes OGR
#include "ogrsf_frmts.h"

/** namespace suri */
namespace suri {
// Forwards
class ViewerWidget;
class DatasourceManagerInterface;
class DatasourceInterface;

/** Clase base para la creacion de elementos y el dibujo de sus Vectores. */
/**
 * Permite a las classes trabajar con vectores sin preocuparce por crear
 * los elementos.
 * Esta clase define el funcionamiento comun y las clases hijo solo deben
 * implementar los metodos SetupElement,AddPointToVector y CloseFeature
 * cuando sea necesario.
 * \todo pensar mejor la interfaz de la clase
 */
class VectorElementEditor : public Subject, public ObserverInterface  {
   /** Ctor. de Copia. */
   VectorElementEditor(const VectorElementEditor &VectorElementEditor);

public:
   /** Ctor. default */
   VectorElementEditor(ViewerWidget *pViewer, Vector::VectorType Type, std::string CategoryName,
         int ListGroup, VectorEditor* pVectorEditor,
         DataViewManager* pDataViewManager);
   /** Dtor. */
   virtual ~VectorElementEditor();
   /** Indica si se termino de editar el vector. */
   virtual bool IsVectorOpen();
   /** Salva el vector. */
   virtual bool CloseVector();
   /** Agrega puntos al vector. Si no hay vector lo crea */
   virtual bool AddPoint(const Coordinates &Point);
   /** Elimina el punto del vector. */
   virtual bool DeletePoint(int Position);
   /** Retorna la cantidad de puntos */
   virtual int GetPointCount();
   /** Metodo que retorna el elemento en edicion */
   VectorElement *GetCurrentElement() {
      return pCurrentVectorElement_;
   }
   /** Cierra el feature abierto */
   virtual void CloseFeature();
   /** Elimina el feature */
   bool DeleteFeature(long FeatureID);
   /** Salva el vector */
   virtual bool SaveVector(const std::string &FileName = "");
   /** Permite seleccionar un nombre archivo para salvar el vector */
   bool SelectOutputFileName(std::string &Filename);
   /** Analiza si se puede eliminar el shapefile y sus archivos asociados */
   bool CanDeleteShapefile(const std::string &Filename);
   /** Elimina el shapefile y sus archivos asociados */
   bool DeleteShapefile(const std::string &Filename);
   /** Devuelve el tipo de vector */
   Vector::VectorType GetType();

   /** Recibe notificaciones del control contenedor */
   virtual void Update(NotificationInterface* pNotification);

protected:
   /** Crea un vector en memoria. */
   virtual bool CreateVectorElement();
   /** Configura el elemento. */
   virtual void SetupElement() {
   }
   /** Setea flag de feature abierta a cerrada. */
   /** Agrega el vector a la lista de elementos. */
   virtual bool AddVectorElement();
   /** Agrega un punto al vector. */
   virtual bool AddPointToVector(const Coordinates &Point)=0;
   /** Permite configurar el elemento antes de agregarlo a la lista */
   virtual bool ConfigureSavedElement(VectorElement* pVectorElement);
   /** Carga los estilos del vector */
   virtual bool LoadStyles();
protected:
   ViewerWidget *pViewer_; /*! View */
   VectorElement *pCurrentVectorElement_; /*! vector en curso */
   Vector::VectorType type_; /*! tipo de vector */
   std::string categoryName_; /*! categoria de vector (Layer, Draw, Meassure, */
   /* etc) */
   int layers_; /*! cantidad de capas del vector */
   int listGroup_; /*! grupo al cual pertenece el vector */
   std::string elementUrl_; /*! URL que se pasa al elemento */
   std::string elementName_; /*! Nombre del elemento */
   VectorEditor* pVectorEditor_; /*! VectorEditor que se utilza para agregar */
   /* puntos a los vectores */
   std::vector<std::string> styles_; /*! estilos requeridos por el vector. */
#ifdef __USE_MEMORY_VECTOR_DURING_EDITION__
   std::string elementFileName_; /*! Nombre de archivo a salvar */
#endif
private:
   /** Permite asignar un nombre de archivo distinto a cada vector en memoria */
   static int vectorCounter_; /*! Contador para nombrar los vectores que se */
   DataViewManager* pDataViewManager_;
   DatasourceManagerInterface* pDatasourceManager_;
   /* editan en memoria */
};
}  // namespace suri

#endif /*DRAWVECTORBUTTON_H_*/
