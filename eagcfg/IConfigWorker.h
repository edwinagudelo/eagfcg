#pragma once

using namespace System;

namespace eagcfg {

	/**
	* Configuration interface definition
	*/
	public interface class IConfigWorker {

	public:
		
		bool crear_parametro(String^ parametro, String^ valor, String^ comentario, String^ path, String^ bd);
		String^ leer_parametro(String^ parametro, String^ path, String^ bd);
		bool actualizar_parametro(String^ parametro, String^ valor, String^ path, String^ bd);
		bool borrar_parametro(String^ parametro, String^ path, String^ bd);
		bool crear_bd(String^ path, String^ bd);

	};
}
