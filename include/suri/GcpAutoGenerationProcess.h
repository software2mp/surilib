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

#ifndef GCPAUTOGENERATIONPROCESS_H_
#define GCPAUTOGENERATIONPROCESS_H_

// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "ProcessInterface.h"
#include "GcpList.h"
#include "Subset.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class Mask;
class Canvas;
class MemoryCanvas;
class FileCanvas;
class DataViewManager;
class Element;
class RasterElement;

/**
 * Proceso de generacion automatica de GCPs
 */
class GcpAutoGenerationProcess : public ProcessInterface {

public:
   
   class Parameters;
   
   virtual ~GcpAutoGenerationProcess();
   /** Ctor. */
   GcpAutoGenerationProcess(const Parameters& params);
   // ------ Metodos que administran la ejecucion del proceso ------
   /** Configura los parametros para la busqueda de GCPs */
   bool ConfigureProcess();

   /** Cumple con la interfaz */
   bool ConfigureOutput();
   /** Corre el proceso y genera la salida usando los metodos de configuracion */
   virtual bool RunProcess();
   /**
    * Metodo que a partir de un xml que contiene los parametros del proceso configura el mismo.
    * @param[in] XmStr string en formato xml que contiene los parametros necesarios para la
    * correcta ejecucion del proceso
    * @return true en caso de poder configurar correctamente el proceso en funcion de los parametros
    * @return false en caso contrario
    */
   virtual bool ConfigureProcessFromXmlString(const std::string& XmlStr);
   /** Metodo que obtiene los parametros asociados al proceso en formato XML.
    * Esto permite que el codigo cliente pueda hacer eso del xml resultante
    * (ej. para la exportacion del mismo)
    * @return string en formato xml con los parametros asociados al proceso
    * @return string vacio en caso contrario.
    */
   std::string GetParametersAsXmlString() const;
   /**
    * Escribe los gcps encontrados a un archivo
    */
   void WriteGcpsToFile() const;
   /**
    * Parametros de  configuracion.
    */ 
   class Parameters{
   public:
      /**
       * Raster element de la imagen base
       */
      RasterElement* pBaseElement_;
      /**
       * Raster element de la imagen warp
       */
      RasterElement* pWarpElement_;
      /**
       * Path del archivo GCPs de salida.
       */
      std::string outputFile;
      /**
       * Bandas de la imagen base a utilizar en el proceso.
       */
      std::vector<int> baseBands_;
      /**
       * Bandas de la imagen warp a utilizar en el proceso.
       */
      std::vector<int> warpBands_;
      /**
       * Archivo con gcps semilla para encontrar una interseccion entre
       * las imagenes a corregistrar
       */
      std::string seedGcpsPath_;
      /**
       * Nivel de agresividad del algoritmo
       */
      int agressionLevel_;
    };
private:
   Parameters parameters_;
   GcpList gcpList_;
   static const std::string kProcessName;
};

} /** namespace suri */

#endif /* GCPAUTOGENERATIONPROCESS_H_ */
