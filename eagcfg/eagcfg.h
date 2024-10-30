// eagcfg.h

#pragma once

#include "IConfigWorker.h"

using namespace System;
using namespace System::IO;
using namespace System::Data;

using namespace log4net;
using namespace System::Data::SQLite;


namespace eagcfg {
	
	public ref class CConfig : IConfigWorker
	{
	public:

		CConfig();
		CConfig(String^ path, String^ db);
		virtual ~CConfig();

		// Funcion que se encarga de crear los parametros de acuerdo a la BD
		virtual bool crear_parametro(String^ parametro, String^ valor, String^ comentario, String^ path, String^ bd);
		virtual String^ leer_parametro(String^ parametro, String^ path, String^ bd);
		virtual bool actualizar_parametro(String^ parametro, String^ valor, String^ path, String^ bd);
		virtual bool borrar_parametro(String^ parametro, String^ path, String^ bd);
		virtual bool crear_bd(String^ path, String^ bd);

	private:
		SQLiteConnection^ conexion;
		SQLiteCommand^ comando;
		SQLiteDataReader^ lector;
		String^ connectionString;
		String^ ruta;
		String^ bd;

		String^ buildConnectionString();
		bool connect();
		void prepareSQL(String^ strsql);

		static ILog^ _logger = LogManager::GetLogger("CConfig");

	};
}
