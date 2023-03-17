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

#ifndef SHARPENINGPROCESS_H_
#define SHARPENINGPROCESS_H_

#include <vector>
#include <string>

#include "suri/ProcessInterface.h"
#include "suri/DataViewManager.h"
#include "suri/Element.h"

#include "gdal_priv.h"

namespace suri {

class BandInfo;

/** Clase que representa el proceso de fusion de bandas pancromaticas */
class SharpeningProcess : public ProcessInterface {
public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
   /**
    * Ctor.
    */
   SharpeningProcess(Element*& pOutput, DataViewManager* pDataViewManager);

   /**
    * Dtor.
    */
   virtual ~SharpeningProcess();

   /**
    * Corre el proceso y genera la salida usando los metodos de configuracion.
    */
   virtual bool RunProcess();

   /**
    * Permite configurar la salida del proceso antes de correrlo.
    */
   virtual bool ConfigureProcess();

   /**
    * Una vez ejecutado el proceso hace visible la salida.
    */
   virtual bool ConfigureOutput();
  /** 
    * Metodo que a partir de un xml que contiene los parametros del proceso configura el mismo.
    * @param[in] XmStr string en formato xml que contiene los parametros necesarios para la 
    * correcta ejecucion del proceso
    * @return true en caso de poder configurar correctamente el proceso en funcion de los parametros
    * @return false en caso contrario
   **/
   virtual bool ConfigureProcessFromXmlString(const std::string& XmlStr);

protected:
   /** Metodo que obtiene los parametros asociados al proceso en formato XML. 
    * Esto permite que el codigo cliente pueda hacer eso del xml resultante 
    * (ej. para la exportacion del mismo)
    * @return string en formato xml con los parametros asociados al proceso
    * @return string vacio en caso contrario. 
   **/
   virtual std::string GetParametersAsXmlString()const;

private:
   Element*& pOutputElement_;
   DataViewManager* pDataViewManager_;

   /**
    * Copia informacion de una banda.
    */
   void CopyBandInfo(GDALRasterBand* SrcBand, GDALRasterBand* DstBand,
                     int CanCopyStatsMetadata, int CopyScale, int CopyNoData);

   /**
    * Escala el raster especificado.
    */
   void Resize(const std::string& SrcFilename, const std::string& DestFilename,
               int OutXSize, int OutYSize);

   /**
    * Aplica un realce a la imagen de origen (lineal 2%)
    */
   void Enhance(const std::string& SrcFilename, const std::string& DestFilename,
                const std::vector<std::string>& Status, bool NDAvailable = false,
                double NDValue = 0.0);

   /**
    * Apila las bandas seleccionadas para RVA en una imagen.
    */
   void Stack(const std::string& DestFilename, const std::vector<BandInfo>& Bands);
};

}  // namespace suri

#endif  // SHARPENINGPROCESS_H_
