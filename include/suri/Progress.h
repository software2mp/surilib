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

#ifndef PROGRESS_H_
#define PROGRESS_H_

// Includes standard
#include <string>

/** namespace suri */
namespace suri {
/** Clase que crea un progreso */
/**
 * Esta clase se utiliza para interactuar con el ProcessManager de forma
 * simplificada.
 * El codigo cliente crea una instancia con la cantidad de ciclos del progreso
 * que se ejecutaran y en cada ciclo ejecuta Update(). Progress interactua
 * con ProgressManager que muestra una interfaz de progreso y/o cancelacion
 * del mismo.
 */

/**
 * TODO (matiaslafroce): evaluar si se puede hacer que ProgressManager
 * no sea mas un singleton e instanciarlo en esta clase
 */
class Progress {
public:
   /** Ctor. Pide un progressId a ProgressManager */
   Progress(int Cycles, const std::string &Description = "");
   /** Dtor. Libera al ProgressManager */
   virtual ~Progress();
   /** realiza tick sobre ProgressManager */
   bool Update();
private:
   int progressId_; /*! Id en ProgressManager asociado a la tarea */
};
}

#endif /*PROGRESS_H_*/
