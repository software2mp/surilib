/*! \file Progress.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
