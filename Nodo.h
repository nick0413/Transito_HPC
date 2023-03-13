class Nodo
{
	public:
		Nodo(bool,float,float,int); // Constructor
		
		bool obtenerInteraccionConCamion();
		void establecerInteraccionConCamion(bool);

		float obtenerCapacidadMaximaBasura();
		void establecerCapacidadMaximaBasura(float);
		
		float obtenerCantidadDeBasura();
		void establecerCantidadDeBasura(float);

		int obtenerTipo();
		void establecerTipo(int);

	private:
		bool interaccionConCamion;
		float capacidadMaximaBasura;
		float cantidadDeBasura;
		int tipo; // Tipo de contenedor según lo que almacene

};

