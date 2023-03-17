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

#ifndef QUERY_H_
#define QUERY_H_


// Includes Estandar
#include <string>

// Includes Suri
#include "suri/ViewportEvent.h"

// Includes Wx

// Defines

// forwards
class OGRFeature;

namespace suri {

// forwards
class VectorElement;

/*!
 * Clase encargada de realizar la consulta
 * de pixel sobre una geometria en base un punto clickeado
 */
class Query : public ViewportEvent {
public:
   /** Ctor */
   Query(ViewportManagerInterface *pViewportManager,
         ViewcontextManagerInterface *pViewcontextManager,
         LibraryManager *pLibraryManager = NULL);
   virtual ~Query();

   virtual void Reset();
   virtual void GetCursor(wxCursor &Cursor);
   virtual void OnEndDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate, wxMouseEvent &Event);
    virtual void OnDrag(const Coordinates & InitialCoordinate,
                          const Coordinates & CurrentCoordinate, wxMouseEvent &Event);
    virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);
    virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event);

protected:
    /** Metodo privado que genera un polinomio en base al punto
     * que se pasa por parametro transformado en el Sr de llegada (SrWktOut). **/
    OGRGeometry *GetQueryPoly(const Coordinates& Point, const std::string& SrWkt,
                              double Tolerance, const std::string& SrWktOut);
    /** Genera el vector de seleccion. **/
    VectorElement *CreateSelectionVector(OGRFeature *pModelFeature, const std::string &SrWkr,
                                         Vector::VectorType Type) const;
    /** Agrega la geometria que forma parte de resultado de la consulta. **/
    void AddFeatureToVector(VectorElement *pVector, OGRFeature *pFeature)const;
    /** Administrador de bibliotecas */
    LibraryManager *pLibraryManager_;
};

} /** namespace suri */

#endif /* QUERY_H_ */
