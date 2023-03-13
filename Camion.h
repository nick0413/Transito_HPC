#include <vector>
using namespace std;


class Camion{
    public:
        //constructor
        Camion(int,vector<float>,float,float);
        
        //getters y setters
        int obtenerTipoBasura();
        void establecerTipoBasura(int);

        vector<float> obtenerPosicionCamion();
        void establecerPosicionCamion(vector<float>);

        float obtenerCapacidadMaxima();
        void establecerCapacidadMaxima(float);

        float obtenerCantidadBasura();
        void establecerCantidadBasura(float);

        //Indicadores
        bool sobrepasoCapacidadMaxima;
        
    private:
        int tipoBasura;
        vector<float> posicionCamion;
        float capacidadMaxima;
        float cantidadBasura;

};