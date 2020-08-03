//Paula Sokolowicz, legajo 1679478.
//Micaela Pucciariello, legajo 1685466.

#include <iostream>
#include <fstream>

#include "rwstring.hpp"
#include "listasutil.hpp"
#include "listasord.hpp"
#include "listasordUni.hpp"
#include "listas.hpp"

using namespace std;

const int lbandera = 12;
const int lproducto = 10;

struct Registro
{
    string bandera;
    string producto;
    double toneladas;
    int contenedores;
};

struct Descarga
{
    double toneladas = 0;
    int contenedores = 0;
};

struct Producto
{
    string producto;
    int contenedores = 0;
    Nodo<Descarga>* listaDesc = nullptr;
};

struct Bandera
{
    string bandera;
    double toneladas = 0;
    Nodo<Producto>* listaProd = nullptr;
};

fstream& operator >> (fstream &fs, Registro& reg)
{
    reg.bandera = readstring(fs, lbandera);
    reg.producto = readstring(fs, lproducto);
    fs.read(reinterpret_cast<char *>(&reg.toneladas), sizeof(reg.toneladas));
    fs.read(reinterpret_cast<char *>(&reg.contenedores), sizeof(reg.contenedores));
    return fs;
}

ostream& operator << (ostream &os, const Descarga& desc)
{
    os << desc.toneladas << '\t' << '\t'<< desc.contenedores;
    return os;
}

int criterio_Bandera(Bandera a, Bandera b)
{
    return a.bandera.compare(b.bandera);
}

int criterio_Producto(Producto a, Producto b)
{
    return a.producto.compare(b.producto);
}

int main()
{
    fstream archivo;
    archivo.open("Descargas.bin", ios::binary | ios::in);
    if(!archivo)
    {
        cout << "ERROR. No se pudo abrir el archivo de lectura." << endl;
        return 1;
    }

    Nodo<Bandera> *listaBandera = nullptr;

    Registro auxRegistro;
    Bandera auxBandera;
    Producto auxProducto;
    Descarga auxDescarga;

    Nodo<Bandera> *Nbandera = nullptr;
    Nodo<Producto> *Nproducto = nullptr;

    while(archivo >> auxRegistro)
    {
        auxBandera.bandera = auxRegistro.bandera;
        Nbandera = insertar_unico(auxBandera, listaBandera, criterio_Bandera);
        Nbandera->dato.toneladas += auxRegistro.toneladas;

        auxProducto.producto = auxRegistro.producto;
        Nproducto = insertar_unico(auxProducto, Nbandera->dato.listaProd, criterio_Producto);
        Nproducto->dato.contenedores += auxRegistro.contenedores;

        auxDescarga.toneladas = auxRegistro.toneladas;
        auxDescarga.contenedores = auxRegistro.contenedores;

        agregar(Nproducto->dato.listaDesc, auxDescarga);
    }

    archivo.close();

    string bandera;
    string producto;

    cout<<"Ingrese bandera: " << endl;;
    cin >> bandera;

    Nbandera = nullptr;
    Nproducto = nullptr;

    while (cin)
    {
        auxBandera.bandera = bandera;

        Nbandera = buscar(auxBandera, listaBandera, criterio_Bandera);

        if(Nbandera != nullptr)
        {
            cout<<"Ingrese producto: " << endl;
            cin >> producto;

            auxProducto.producto = producto;

            Nproducto = buscar(auxProducto, Nbandera->dato.listaProd, criterio_Producto);

            if (Nproducto != nullptr)
            {
                cout << "El total de toneladas descargadas de la bandera ingresada es: " << Nbandera->dato.toneladas <<"\n" << endl;
                cout << "La cantidad de contenedores descargados del producto en la bandera es: " << Nproducto->dato.contenedores <<"\n"<< endl;
                cout << "Lista de descargas correspondiente a la combinacion bandera-producto: " << endl;
                cout << "Toneladas\tContenedores" << endl;
                mostrar(Nproducto->dato.listaDesc);
            }
            else
            {
                cout << "No existe el producto ingresado." << endl;
            }
        }
        else
        {
            cout << "No existe la bandera ingresada." << endl;
        }
        cout<<"Ingrese bandera: " << endl;
        cin >> bandera;
    }

    return 0;
}
